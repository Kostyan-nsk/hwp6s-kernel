/*
 *  drivers/cpufreq/cpufreq_optdemand.c
 *
 *  Copyright (C)  2014 Yu Wei <yuwei3@hisilicon.com>
 *                      Mo Haoju <mohaoju@hisilicon.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/percpu-defs.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/tick.h>
#include <linux/types.h>
#include <linux/cpu.h>
#include <linux/input.h>
#include <linux/workqueue.h>


/* optdemand governor macros */
#define DEF_SAMPLING_DOWN_FACTOR		(4)
#define MAX_SAMPLING_DOWN_FACTOR		(100000)
#define MIN_SAMPLE_RATE				(10000)
#define DEF_BOOST_PULSE_DURATION		(80000)
#define DEF_GO_HISPEED_LOAD			(95)
#define DEF_HISPEED_FREQ			(1196000)
#define DEF_UP_THRESHOLD			(80)
#define DEF_DOWN_THRESHOLD			(30)

#define MIN_SAMPLING_RATE_RATIO			(2)
#define LATENCY_MULTIPLIER			(1000)
#define MIN_LATENCY_MULTIPLIER			(20)
#define TRANSITION_LATENCY_LIMIT		(10 * 1000 * 1000)

#define UP_THRESHOLD				1
#define DOWN_THRESHOLD				2

static unsigned int operating_points[7][3] = {
	/* kHz   up_threshold   down_threshold */
	{208000,	70,	0},
	{416000,	75,	45},
	{624000,	80,	50},
	{798000,	85,	55},
	{1196000,	90,	60},
	{1596000,	95,	65},
	{1795000,	100,	70},
};

static int usage_count;

struct cpufreq_optdemand_cpuinfo {
	struct cpufreq_frequency_table *freq_table;
	int cpu;
	u64 prev_cpu_idle;
	u64 prev_cpu_wall;
	u64 prev_cpu_nice;
	struct cpufreq_policy *cur_policy;
	struct delayed_work work;
	/*
	 * percpu mutex that serializes governor limit change with gov_dbs_timer
	 * invocation. We do not want gov_dbs_timer to run when user is changing
	 * the governor or limits.
	 */
	struct mutex timer_mutex;
	unsigned int rate_mult;
};

static DEFINE_PER_CPU(struct cpufreq_optdemand_cpuinfo, cpuinfo);

static struct cpufreq_optdemand_tunables {
	unsigned int sampling_rate;
	unsigned int min_sampling_rate;
	unsigned int sampling_down_factor;
	unsigned int go_hispeed_load;
	unsigned int hispeed_freq;
	spinlock_t threshold_lock;
	int unsigned num_thresholds;
	/* Non-zero means indefinite speed boost active */
	int boost;
	/* Duration of a boot pulse in usecs */
	int boostpulse_duration;
	/* boost pulse of input event */
	bool input_event_boost;
	ktime_t time_stamp;
} tunables = {
	.sampling_rate = 50000,
	.hispeed_freq = DEF_HISPEED_FREQ,
	.sampling_down_factor = DEF_SAMPLING_DOWN_FACTOR,
	.go_hispeed_load = DEF_GO_HISPEED_LOAD,
	.boostpulse_duration = DEF_BOOST_PULSE_DURATION,
	.num_thresholds = 7,
};

static DEFINE_MUTEX(dbs_mutex);

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_OPTDEMAND
static struct cpufreq_governor cpufreq_gov_optdemand;
#endif

#ifdef CONFIG_INPUT_PULSE_SUPPORT
struct cpufreq_optdemand_inputopen {
	struct input_handle *handle;
	struct work_struct inputopen_work;
};

static struct cpufreq_optdemand_inputopen inputopen;
static struct workqueue_struct *down_wq;
#endif

static inline u64 get_cpu_idle_time_jiffy(unsigned int cpu, u64 *wall)
{
	u64 idle_time;
	u64 cur_wall_time;
	u64 busy_time;

	cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());

	busy_time = kstat_cpu(cpu).cpustat.user;
	busy_time += kstat_cpu(cpu).cpustat.system;
	busy_time += kstat_cpu(cpu).cpustat.irq;
	busy_time += kstat_cpu(cpu).cpustat.softirq;
	busy_time += kstat_cpu(cpu).cpustat.steal;
	busy_time += kstat_cpu(cpu).cpustat.nice;

	idle_time = cur_wall_time - busy_time;
	if (wall)
		*wall = cputime_to_usecs(cur_wall_time);

	return cputime_to_usecs(idle_time);
}

static inline u64 get_cpu_idle_time(unsigned int cpu, 
					    u64 *wall, int io_busy)
{
	u64 idle_time = get_cpu_idle_time_us(cpu, io_busy ? wall : NULL);

	if (idle_time == -1ULL)
		return get_cpu_idle_time_jiffy(cpu, wall);
	else if (!io_busy)
		idle_time += get_cpu_iowait_time_us(cpu, wall);

	return idle_time;
}

static unsigned int get_threshold(unsigned int freq, unsigned int threshold)
{
	unsigned int i = 0;

	while (i < tunables.num_thresholds && operating_points[i][0] < freq)
	    i++;

	return operating_points[i][threshold];
}

/*
 * Increase or decrease frequency according to the up threshold
 * and down threshold of the current freq.
 */
static inline unsigned int choose_freq(struct cpufreq_optdemand_cpuinfo *pcpu,
		unsigned int load_freq)
{
    struct cpufreq_policy *policy = pcpu->cur_policy;
    unsigned int freq = policy->cur, index;
    unsigned int up_threshold, down_threshold;

	up_threshold = get_threshold(freq, UP_THRESHOLD);
	/* frequency increase */
	if (load_freq > up_threshold * freq) {
		/* If load meet increase threshold, apply sampling_down_factor */
		pcpu->rate_mult = tunables.sampling_down_factor;
		if (policy->cur < policy->max)  {
			freq = load_freq / up_threshold;
			if (cpufreq_frequency_table_target(policy, pcpu->freq_table, freq,
							    CPUFREQ_RELATION_L, &index))
			    pr_err("%s: failed to get next frequency\n", __func__);
			else
			    freq = pcpu->freq_table[index].frequency;

		if (load_freq >= tunables.go_hispeed_load * policy->cur)
		    freq = max(freq, tunables.hispeed_freq);
		}
		return freq;
	}

	/* frequency decrease, need to wait N continuous freq decrease period
	 * since last freq increase.
	 */
	down_threshold = get_threshold(freq, DOWN_THRESHOLD);
	if (load_freq < down_threshold * freq) {
	    if (policy->cur > policy->min) {
		if(pcpu->rate_mult > 0)
		    pcpu->rate_mult -= 1;

		if(!pcpu->rate_mult) {
		    freq = load_freq / down_threshold;
		    if (cpufreq_frequency_table_target(policy, pcpu->freq_table, freq,
							CPUFREQ_RELATION_H, &index)) {
			pr_err("%s: failed to get next frequency\n", __func__);
			return freq;
		    }
		    freq = pcpu->freq_table[index].frequency;
		}
	    }
	}
	return freq;
}

static void optdemand_dbs_check_cpu(struct cpufreq_optdemand_cpuinfo *pcpu)
{
	struct cpufreq_policy *policy = pcpu->cur_policy;
	unsigned int max_load = 0;
	unsigned int freq_next, j;

	/* Get Absolute Load */
	for_each_online_cpu(j) {
		struct cpufreq_optdemand_cpuinfo *j_pcpu;
		u64 cur_wall_time, cur_idle_time;
		unsigned int idle_time, wall_time;
		unsigned int load;

		j_pcpu = &per_cpu(cpuinfo, j);

		/*
		 * For the purpose of ondemand, waiting for disk IO is
		 * an indication that you're performance critical, and
		 * not that the system is actually idle. So do not add
		 * the iowait time to the cpu idle time.
		 */
		cur_idle_time = get_cpu_idle_time(j, &cur_wall_time, 0);

		wall_time = (unsigned int)
			(cur_wall_time - j_pcpu->prev_cpu_wall);
		j_pcpu->prev_cpu_wall = cur_wall_time;

		idle_time = (unsigned int)
			(cur_idle_time - j_pcpu->prev_cpu_idle);
		j_pcpu->prev_cpu_idle = cur_idle_time;

		if (unlikely(!wall_time || wall_time < idle_time))
			continue;

		load = 100 * (wall_time - idle_time) / wall_time;

		load *= policy->cur;

		if (load > max_load)
			max_load = load;
	}
	freq_next = choose_freq(pcpu, max_load);
	if (freq_next != policy->cur)
	    __cpufreq_driver_target(policy, freq_next, CPUFREQ_RELATION_C);
}

/* Will return if we need to evaluate cpu load again or not */
static inline bool need_load_eval(unsigned int sampling_rate) {

	ktime_t time_now = ktime_get();
	s64 delta_us = ktime_us_delta(time_now, tunables.time_stamp);

	/* Do nothing if we recently have sampled */
	if (delta_us < (s64)(sampling_rate / 2))
		return false;
	else
		tunables.time_stamp = time_now;

	return true;
}

static void optdemand_dbs_timer(struct work_struct *work)
{
	struct cpufreq_optdemand_cpuinfo *pcpu =
		container_of(work, struct cpufreq_optdemand_cpuinfo, work.work);
	unsigned int cpu = pcpu->cpu, freq_cur;
	struct cpufreq_policy *policy = pcpu->cur_policy;
	bool boosted;

        freq_cur = policy->cur;

	if (!need_load_eval(tunables.sampling_rate))
	    goto max_delay;

	boosted = tunables.boost || tunables.input_event_boost;

	if (boosted) {
	    tunables.input_event_boost = false;
	    pcpu->rate_mult = tunables.boostpulse_duration / tunables.sampling_rate;
	    if(policy->cur < tunables.hispeed_freq)
		__cpufreq_driver_target(policy, tunables.hispeed_freq, CPUFREQ_RELATION_L);
	}
	else {
	    optdemand_dbs_check_cpu(pcpu);
	}

max_delay:
	queue_delayed_work_on(cpu, system_wq, &pcpu->work,
				usecs_to_jiffies(tunables.sampling_rate));
}

#ifdef CONFIG_INPUT_PULSE_SUPPORT

/*
 * Pulsed boost on input event raises CPUs to hispeed_freq.
 */
static void cpufreq_optdemand_input_event(struct input_handle *handle,
					    unsigned int type,
					    unsigned int code, int value)
{

	if (type == EV_SYN && code == SYN_REPORT)
	    tunables.input_event_boost = true;
}

static void cpufreq_optdemand_input_open(struct work_struct *w)
{
	struct cpufreq_optdemand_inputopen *io =
		container_of(w, struct cpufreq_optdemand_inputopen,
			     inputopen_work);
	int error;

	error = input_open_device(io->handle);
	if (error)
		input_unregister_handle(io->handle);
}

static int cpufreq_optdemand_input_connect(struct input_handler *handler,
					     struct input_dev *dev,
					     const struct input_device_id *id)
{
	struct input_handle *handle;
	int error;

	pr_info("%s: connect to %s\n", __func__, dev->name);
	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
		return -ENOMEM;

	handle->dev = dev;
	handle->handler = handler;
	handle->name = "cpufreq_optdemand";

	error = input_register_handle(handle);
	if (error)
		goto err;

	inputopen.handle = handle;
	queue_work(down_wq, &inputopen.inputopen_work);
	return 0;
err:
	kfree(handle);
	return error;
}

static void cpufreq_optdemand_input_disconnect(struct input_handle *handle)
{
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id cpufreq_optdemand_ids[] = {
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT |
			 INPUT_DEVICE_ID_MATCH_ABSBIT,
		.evbit = { BIT_MASK(EV_ABS) },
		.absbit = { [BIT_WORD(ABS_MT_POSITION_X)] =
			    BIT_MASK(ABS_MT_POSITION_X) |
			    BIT_MASK(ABS_MT_POSITION_Y) },
	}, /* multi-touch touchscreen */
	{
		.flags = INPUT_DEVICE_ID_MATCH_KEYBIT |
			 INPUT_DEVICE_ID_MATCH_ABSBIT,
		.keybit = { [BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH) },
		.absbit = { [BIT_WORD(ABS_X)] =
			    BIT_MASK(ABS_X) | BIT_MASK(ABS_Y) },
	}, /* touchpad */
	{ },
};

static struct input_handler cpufreq_optdemand_input_handler = {
	.event          = cpufreq_optdemand_input_event,
	.connect        = cpufreq_optdemand_input_connect,
	.disconnect     = cpufreq_optdemand_input_disconnect,
	.name           = "cpufreq_optdemand",
	.id_table       = cpufreq_optdemand_ids,
};

#endif /*CONFIG_INPUT_PULSE_SUPPORT*/


/************************** sysfs interface ************************/

static unsigned int *get_tokenized_data(const char *buf, int *num_tokens)
{
	const char *cp;
	int i;
	int ntokens = 0;
	unsigned int *tokenized_data;
	int err = -EINVAL;
	cp = buf;
	while ((cp = strpbrk(cp + 1, " ")))
		ntokens++;

	if (!ntokens)
		goto err;

	tokenized_data = kmalloc(ntokens * sizeof(unsigned int), GFP_KERNEL);
	if (!tokenized_data) {
		err = -ENOMEM;
		goto err;
	}

	cp = buf;
	for (i = 0; i < ntokens; i++) {
		if (sscanf(cp, "%u", &tokenized_data[i]) != 1)
			goto err_kfree;

		cp = strpbrk(cp, " ");
		if (!cp)
			break;
		cp++;
	    }

	if (i != ntokens)
		goto err_kfree;

	*num_tokens = ntokens;
	return tokenized_data;

err_kfree:
	kfree(tokenized_data);
err:
	return ERR_PTR(err);
}

/*
 * No matter what frequency is, its down threshold must be smaller
 * than its up threshold.
 */

static int is_threshold_invalid(unsigned int *new_thresholds, unsigned int ntokens, unsigned int updown)
{
    int i;

    if(!new_thresholds)
        return -EINVAL;

    for (i = 0; i < ntokens && i < tunables.num_thresholds; i++) {
	switch (updown) {
	    case UP_THRESHOLD:
		if (new_thresholds[i] < operating_points[i][DOWN_THRESHOLD])
		    return -EINVAL;
		break;
	    case DOWN_THRESHOLD:
		if (new_thresholds[i] > operating_points[i][UP_THRESHOLD])
		    return -EINVAL;
		break;
	}
    }
    return 0;
}

/**
 * update_sampling_rate - update sampling rate effective immediately if needed.
 * @new_rate: new sampling rate
 *
 * If new rate is smaller than the old, simply updating
 * dbs_tuners_int.sampling_rate might not be appropriate. For example, if the
 * original sampling_rate was 1 second and the requested new sampling rate is 10
 * ms because the user needs immediate reaction from ondemand governor, but not
 * sure if higher frequency will be required or not, then, the governor may
 * change the sampling rate too late; up to 1 second later. Thus, if we are
 * reducing the sampling rate, we need to make the new value effective
 * immediately.
 */
static void update_sampling_rate(unsigned int new_rate)
{
	int cpu;

	tunables.sampling_rate = new_rate = max(new_rate,
			tunables.min_sampling_rate);

	for_each_online_cpu(cpu) {
		struct cpufreq_policy *policy;
		struct cpufreq_optdemand_cpuinfo *pcpu;
		unsigned long next_sampling, appointed_at;

		policy = cpufreq_cpu_get(cpu);
		if (!policy)
			continue;
		if (policy->governor != &cpufreq_gov_optdemand) {
			cpufreq_cpu_put(policy);
			continue;
		}
		pcpu = &per_cpu(cpuinfo, cpu);
		cpufreq_cpu_put(policy);

		mutex_lock(&pcpu->timer_mutex);

		if (!delayed_work_pending(&pcpu->work)) {
			mutex_unlock(&pcpu->timer_mutex);
			continue;
		}

		next_sampling = jiffies + usecs_to_jiffies(new_rate);
		appointed_at = pcpu->work.timer.expires;

		if (time_before(next_sampling, appointed_at)) {

			mutex_unlock(&pcpu->timer_mutex);
			cancel_delayed_work_sync(&pcpu->work);
			mutex_lock(&pcpu->timer_mutex);

			queue_delayed_work_on(pcpu->cpu, system_wq, &pcpu->work,
						usecs_to_jiffies(new_rate));
		}
		mutex_unlock(&pcpu->timer_mutex);
	}
}

static ssize_t store_sampling_rate(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	update_sampling_rate(input);
	return count;
}

static ssize_t show_sampling_rate(struct kobject *kobj, struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", tunables.sampling_rate);
}

static struct global_attr sampling_rate_attr = __ATTR(sampling_rate, 0644,
		show_sampling_rate, store_sampling_rate);

static ssize_t show_sampling_rate_min(struct kobject *kobj, struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", MIN_SAMPLE_RATE);
}

static struct global_attr sampling_rate_min_attr = __ATTR(sampling_rate_min, 0644,
		show_sampling_rate_min, NULL);

static ssize_t store_go_hispeed_load(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1 || input > 100)
		return -EINVAL;

	tunables.go_hispeed_load = input;
	return count;
}

static ssize_t show_go_hispeed_load(struct kobject *kobj, struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", tunables.go_hispeed_load);
}

static struct global_attr go_hispeed_load_attr = __ATTR(go_hispeed_load, 0644,
		show_go_hispeed_load, store_go_hispeed_load);

static ssize_t store_sampling_down_factor(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	unsigned int input, j;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1 || input > MAX_SAMPLING_DOWN_FACTOR || input < 1)
		return -EINVAL;

	tunables.sampling_down_factor = input;

	// Reset down sampling multiplier in case it was active
	for_each_online_cpu(j) {
		struct cpufreq_optdemand_cpuinfo *pcpu = &per_cpu(cpuinfo, j);
		pcpu->rate_mult = 1;
	}
	return count;
}

static ssize_t show_sampling_down_factor(struct kobject *kobj, struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", tunables.sampling_down_factor);
}

static struct global_attr sampling_down_factor_attr = __ATTR(sampling_down_factor, 0644,
		show_sampling_down_factor, store_sampling_down_factor);

static ssize_t store_boost(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	tunables.boost = input;

	if (tunables.boost)
	    tunables.input_event_boost = true;

	return count;
}

static ssize_t show_boost(struct kobject *kobj, struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", tunables.boost);
}

static struct global_attr boost_attr = __ATTR(boost, 0644,
		show_boost, store_boost);

static ssize_t store_boostpulse(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;
		
	tunables.input_event_boost = true;
	return count;
}

static struct global_attr boostpulse_attr = __ATTR(boostpulse, 0644,
		NULL, store_boostpulse);

static ssize_t store_boostpulse_duration(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	tunables.boostpulse_duration = input;
	return count;
}

static ssize_t show_boostpulse_duration(struct kobject *kobj,
					struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", tunables.boostpulse_duration);
}

static struct global_attr boostpulse_duration_attr = __ATTR(boostpulse_duration, 0644,
		show_boostpulse_duration, store_boostpulse_duration);

static ssize_t store_hispeed_freq(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	tunables.hispeed_freq = input;
	return count;
}

static ssize_t show_hispeed_freq(struct kobject *kobj, struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", tunables.hispeed_freq);
}

static struct global_attr hispeed_freq_attr = __ATTR(hispeed_freq, 0644,
		show_hispeed_freq, store_hispeed_freq);

static ssize_t show_down_thresholds(struct kobject *kobj, struct attribute *attr, char *buf)
{
	int i;
	ssize_t ret = 0;

	for (i = 0; i < tunables.num_thresholds; i++)
	    ret += sprintf(buf + ret, "%u ", operating_points[i][DOWN_THRESHOLD]);

	sprintf(buf + ret - 1, "\n");

	return ret;
}

static ssize_t store_down_thresholds(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	int i, ntokens;
	unsigned int *new_thresholds = NULL;

	new_thresholds = get_tokenized_data(buf, &ntokens);
	if (IS_ERR(new_thresholds))
		return PTR_RET(new_thresholds);

    if (is_threshold_invalid(new_thresholds, ntokens, DOWN_THRESHOLD)) {
        return -EINVAL;
    }

	for (i = 0; i < ntokens && i< tunables.num_thresholds; i++)
	    operating_points[i][DOWN_THRESHOLD] = new_thresholds[i];

	return count;
}

static struct global_attr down_thresholds_attr = __ATTR(down_thresholds, 0644,
		show_down_thresholds, store_down_thresholds);

static ssize_t show_up_thresholds(struct kobject *kobj, struct attribute *attr, char *buf)
{
	int i;
	ssize_t ret = 0;

	for (i = 0; i < tunables.num_thresholds; i++)
	    ret += sprintf(buf + ret, "%u ", operating_points[i][UP_THRESHOLD]);

	sprintf(buf + ret - 1, "\n");

	return ret;
}

static ssize_t store_up_thresholds(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	int ntokens, i;
	unsigned int *new_thresholds = NULL;

	new_thresholds = get_tokenized_data(buf, &ntokens);
	if (IS_ERR(new_thresholds))
		return PTR_RET(new_thresholds);

	if (is_threshold_invalid(new_thresholds, ntokens, UP_THRESHOLD))
	    return -EINVAL;

	for (i = 0; i < ntokens && i < tunables.num_thresholds; i++)
	    operating_points[i][UP_THRESHOLD] = new_thresholds[i];

	return count;
}

static struct global_attr up_thresholds_attr = __ATTR(up_thresholds, 0644,
		show_up_thresholds, store_up_thresholds);

static struct attribute *dbs_attributes[] = {
	&sampling_rate_min_attr.attr,
	&sampling_rate_attr.attr,
	&go_hispeed_load_attr.attr,
	&hispeed_freq_attr.attr,
	&sampling_down_factor_attr.attr,
	&boost_attr.attr,
	&boostpulse_attr.attr,
	&boostpulse_duration_attr.attr,
	&up_thresholds_attr.attr,
	&down_thresholds_attr.attr,
	NULL
};

static struct attribute_group optdemand_attr_group = {
	.attrs = dbs_attributes,
	.name = "optdemand",
};
/************************** sysfs end ************************/

static int cpufreq_governor_optdemand(struct cpufreq_policy *policy,
		unsigned int event)
{
	struct cpufreq_optdemand_cpuinfo *pcpu;
	unsigned int cpu = policy->cpu;
	u64 idle_time;
	int rc;

	pcpu = &per_cpu(cpuinfo, cpu);

	switch (event) {
	case CPUFREQ_GOV_START:
	    if (!policy->cur)
		return -EINVAL;

	    mutex_lock(&dbs_mutex);

	    usage_count++;
	    if (usage_count == 1) {
		idle_time = get_cpu_idle_time_us(cpu, NULL);
		if (idle_time != -1ULL) {
			/*
			 * In nohz/micro accounting case we set the minimum frequency
			 * not depending on HZ, but fixed (very low). The deferred
			 * timer might skip some samples if idle/sleeping as needed.
			*/
			tunables.min_sampling_rate = MIN_SAMPLE_RATE;
		} else {
			/* For correct statistics, we need 10 ticks for each measure */
			tunables.min_sampling_rate = MIN_SAMPLING_RATE_RATIO *
							jiffies_to_usecs(10);
		}
		rc = sysfs_create_group(cpufreq_global_kobject, &optdemand_attr_group);
		if (rc) {
		    mutex_unlock(&dbs_mutex);
		    return rc;
		}
		tunables.time_stamp = ktime_get();
	    }
	    mutex_unlock(&dbs_mutex);

#ifdef CONFIG_INPUT_PULSE_SUPPORT
	    if (policy->cpu == 0) {
		rc = input_register_handler(&cpufreq_optdemand_input_handler);
		if (rc)
		    pr_warn("%s: failed to register input handler\n", __func__);
	    }
#endif
	    pcpu->cpu = cpu;
	    pcpu->cur_policy = policy;
	    pcpu->freq_table = cpufreq_frequency_get_table(cpu);
	    pcpu->prev_cpu_idle = get_cpu_idle_time(cpu, &pcpu->prev_cpu_wall, 0);
	    pcpu->rate_mult = 0;

	    mutex_init(&pcpu->timer_mutex);
	    INIT_DELAYED_WORK_DEFERRABLE(&pcpu->work, optdemand_dbs_timer);

	    queue_delayed_work_on(cpu, system_wq, &pcpu->work,
				    usecs_to_jiffies(tunables.sampling_rate));
	    break;

	case CPUFREQ_GOV_STOP:
	    cancel_delayed_work(&pcpu->work);

	    mutex_lock(&dbs_mutex);
	    usage_count--;
	    mutex_destroy(&pcpu->timer_mutex);
	    if (!usage_count)
		sysfs_remove_group(cpufreq_global_kobject, &optdemand_attr_group);

	    mutex_unlock(&dbs_mutex);
#ifdef CONFIG_INPUT_PULSE_SUPPORT
	    if (policy->cpu == 0)
		input_unregister_handler(&cpufreq_optdemand_input_handler);
#endif
	    break;

	case CPUFREQ_GOV_LIMITS:
	    mutex_lock(&pcpu->timer_mutex);
	    if (policy->max < pcpu->cur_policy->cur)
		__cpufreq_driver_target(pcpu->cur_policy,
					policy->max, CPUFREQ_RELATION_H);
	    else if (policy->min > pcpu->cur_policy->cur)
		__cpufreq_driver_target(pcpu->cur_policy,
					policy->min, CPUFREQ_RELATION_L);
	    optdemand_dbs_check_cpu(pcpu);
	    mutex_unlock(&pcpu->timer_mutex);
	    break;
	}
	return 0;
}

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_OPTDEMAND
static
#endif
struct cpufreq_governor cpufreq_gov_optdemand = {
	.name			= "optdemand",
	.governor		= cpufreq_governor_optdemand,
	.max_transition_latency	= TRANSITION_LATENCY_LIMIT,
	.owner			= THIS_MODULE,
};

static int __init cpufreq_optdemand_init(void)
{
#ifdef CONFIG_INPUT_PULSE_SUPPORT
	/* No rescuer thread, bind to CPU queuing the work for possibly
	   warm cache (probably doesn't matter much). */
	down_wq = alloc_workqueue("optdemand_down", 0, 1);

	if (!down_wq)
    	return -ENOMEM;

	INIT_WORK(&inputopen.inputopen_work, cpufreq_optdemand_input_open);
#endif /*#ifdef CONFIG_INPUT_PULSE_SUPPORT*/

	return cpufreq_register_governor(&cpufreq_gov_optdemand);
}

static void __exit cpufreq_optdemand_exit(void)
{
	cpufreq_unregister_governor(&cpufreq_gov_optdemand);
}

MODULE_AUTHOR("Yu Wei <yuwei3@hisilicon.com>");
MODULE_AUTHOR("Mo Haoju <mohaoju@hisilicon.com>");
MODULE_DESCRIPTION("'cpufreq_optdemand' - A dynamic cpufreq governor for "
	"Low Latency Frequency Transition capable processors with boost support."
	"Backport from Honor 4X for Ascend P6S-U06 by Kostyan_nsk");
MODULE_LICENSE("GPL");

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_OPTDEMAND
fs_initcall(cpufreq_optdemand_init);
#else
module_init(cpufreq_optdemand_init);
#endif
module_exit(cpufreq_optdemand_exit);
