/*
 *  drivers/cpufreq/cpufreq_abyssplugV2.c
 *
 *
 *  Based on conservative governor
 *
 *  Copyright (C)  2001 Russell King
 *            (C)  2003 Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>.
 *                      Jun Nakajima <jun.nakajima@intel.com>
 *            (C)  2009 Alexander Clouter <alex@digriz.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>
#include <linux/jiffies.h>
#include <linux/kernel_stat.h>
#include <linux/mutex.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/earlysuspend.h>
#include <linux/io.h>

static struct workqueue_struct *abyssplugv2_wq;

#define TB_BOOST 1

// cpu load trigger
#define DEF_BOOST (85)

/*
 * dbs is used in this file as a shortform for demandbased switching
 * It helps to keep variable names smaller, simpler
 */

#define DEF_FREQUENCY_UP_THRESHOLD		(60)
#define DEF_FREQUENCY_UP_THRESHOLD_HOTPLUG	(75)
#define DEF_FREQUENCY_DOWN_THRESHOLD		(25)
#define DEF_FREQUENCY_DOWN_THRESHOLD_HOTPLUG	(15)
#define DEF_START_DELAY				(0)

/*
 * The polling frequency of this governor depends on the capability of
 * the processor. Default polling frequency is 1000 times the transition
 * latency of the processor. The governor will work on any processor with
 * transition latency <= 10mS, using appropriate sampling
 * rate.
 * For CPUs with transition latency > 10mS (mostly drivers with CPUFREQ_ETERNAL)
 * this governor will not work.
 * All times here are in uS.
 */
#define MIN_SAMPLING_RATE_RATIO			(2)
#define DEF_SAMPLING_RATE			(100000)
#define MIN_SAMPLING_RATE			(10000)

static unsigned int min_sampling_rate;

// raise sampling rate to SR*multiplier on blank screen
static unsigned int sampling_rate_awake;
static unsigned int up_threshold_awake;
static unsigned int down_threshold_awake;
static unsigned int boost_awake;

#define SAMPLING_RATE_SLEEP_MULTIPLIER (4)

#define LATENCY_MULTIPLIER			(1000)
#define MIN_LATENCY_MULTIPLIER			(100)
#define DEF_SAMPLING_DOWN_FACTOR		(4)
#define MAX_SAMPLING_DOWN_FACTOR		(10)
#define TRANSITION_LATENCY_LIMIT		(10 * 1000 * 1000)

static void do_dbs_timer(struct work_struct *work);

struct cpu_dbs_info_s {
	cputime64_t prev_cpu_idle;
	cputime64_t prev_cpu_wall;
	cputime64_t prev_cpu_nice;
	struct cpufreq_policy *cur_policy;
	struct delayed_work work;
	unsigned int down_skip;
	unsigned int requested_freq;
	int cpu;
	/*
	 * percpu mutex that serializes governor limit change with
	 * do_dbs_timer invocation. We do not want do_dbs_timer to run
	 * when user is changing the governor or limits.
	 */
	struct mutex timer_mutex;
};
static DEFINE_PER_CPU(struct cpu_dbs_info_s, cs_cpu_dbs_info);

static unsigned int dbs_enable;	/* number of CPUs using this policy */

/*
 * dbs_mutex protects dbs_enable in governor start/stop.
 */
static DEFINE_MUTEX(dbs_mutex);

static struct dbs_tuners {
	unsigned int sampling_rate;
	unsigned int sampling_down_factor;
	unsigned int up_threshold;
	unsigned int up_threshold_hotplug;
	unsigned int down_threshold;
	unsigned int down_threshold_hotplug;
	unsigned int ignore_nice;
	unsigned int boost;

} dbs_tuners_ins = {
	.up_threshold = DEF_FREQUENCY_UP_THRESHOLD,
	.up_threshold_hotplug = DEF_FREQUENCY_UP_THRESHOLD_HOTPLUG,
	.down_threshold = DEF_FREQUENCY_DOWN_THRESHOLD,
	.down_threshold_hotplug = DEF_FREQUENCY_DOWN_THRESHOLD_HOTPLUG,
	.sampling_down_factor = DEF_SAMPLING_DOWN_FACTOR,
	.sampling_rate = DEF_SAMPLING_RATE,
	.ignore_nice = 0,
	.boost = DEF_BOOST,
};

/**
 * Smooth scaling conservative governor
 *            
 * This modification makes the governor use two lookup tables holding
 * current, next and previous frequency to directly get a correct
 * target frequency instead of calculating target frequencies with
 * up_threshold and step_up %. The two scaling lookup tables used 
 * contain different scaling steps/frequencies to achieve faster upscaling 
 * on higher CPU load.
 * 
 * CPU load triggering faster upscaling can be adjusted via SYSFS, 
 * VALUE between 1 and 100 (% CPU load):
 * echo VALUE > /sys/devices/system/cpu/cpufreq/abyssplugV2/boost               
 */

#define TB_FREQ 0
#define TB_UP 1
#define TB_DOWN 2

static int tb_freqs[7][3]={
    {1795000,1795000,1596000},
    {1596000,1795000,1196000},
    {1196000,1596000, 798000},
    { 798000,1196000, 624000},
    { 624000, 798000, 416000},
    { 416000, 624000, 208000},
    { 208000, 416000, 208000},
};

/* Table for use with NX-Kernel for Samsung GT-N7000 */
static int tb_freqs_power[7][3]={
    {1795000,1795000,1596000},
    {1596000,1795000,1196000},
    {1196000,1795000, 798000},
    { 798000,1596000, 624000},
    { 624000,1196000, 416000},
    { 416000, 798000, 208000},
    { 208000, 624000, 208000},
};

static int tb_get_next_freq(unsigned int curfreq, unsigned int updown, unsigned int load) {
    int i=0;
    if (load < dbs_tuners_ins.boost)
    {
        for(i = 0; i < 7; i++)
        {
            if(curfreq == tb_freqs[i][TB_FREQ])
                return tb_freqs[i][updown];
        }
    }
    else
    {
        for(i = 0; i < 7; i++)
        {
            if(curfreq == tb_freqs_power[i][TB_FREQ])
                return tb_freqs_power[i][updown];
        }
    }
    return (curfreq);
    }

static inline cputime64_t get_cpu_idle_time_jiffy(unsigned int cpu,
							cputime64_t *wall)
{
	cputime64_t idle_time;
	cputime64_t cur_wall_time;
	cputime64_t busy_time;

	cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());
	busy_time = cputime64_add(kstat_cpu(cpu).cpustat.user,
			kstat_cpu(cpu).cpustat.system);

	busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.irq);
	busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.softirq);
	busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.steal);
	busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.nice);

	idle_time = cputime64_sub(cur_wall_time, busy_time);
	if (wall)
		*wall = (cputime64_t)jiffies_to_usecs(cur_wall_time);

	return (cputime64_t)jiffies_to_usecs(idle_time);
}

static inline cputime64_t get_cpu_idle_time(unsigned int cpu, cputime64_t *wall)
{
	u64 idle_time = get_cpu_idle_time_us(cpu, wall);

	if (idle_time == -1ULL)
		return get_cpu_idle_time_jiffy(cpu, wall);

	return idle_time;
}

/************************** sysfs interface ************************/
static ssize_t show_sampling_rate_min(struct kobject *kobj,
				      struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", min_sampling_rate);
}

define_one_global_ro(sampling_rate_min);

/* cpufreq_abyssplugV2 Governor Tunables */
#define show_one(file_name, object)					\
static ssize_t show_##file_name						\
(struct kobject *kobj, struct attribute *attr, char *buf)		\
{									\
	return sprintf(buf, "%u\n", dbs_tuners_ins.object);		\
}
show_one(sampling_rate, sampling_rate);
show_one(sampling_down_factor, sampling_down_factor);
show_one(up_threshold, up_threshold);
show_one(up_threshold_hotplug, up_threshold_hotplug);
show_one(down_threshold, down_threshold);
show_one(down_threshold_hotplug, down_threshold_hotplug);
show_one(ignore_nice_load, ignore_nice);
show_one(boost, boost);

static ssize_t store_sampling_down_factor(struct kobject *a,
					  struct attribute *b,
					  const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1 || input > MAX_SAMPLING_DOWN_FACTOR || input < 1)
		return -EINVAL;

	dbs_tuners_ins.sampling_down_factor = input;
	return count;
}

static ssize_t store_sampling_rate(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1)
		return -EINVAL;

	dbs_tuners_ins.sampling_rate = max(input, min_sampling_rate);
	return count;
}

static ssize_t store_up_threshold(struct kobject *a, struct attribute *b,
				  const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1 || input > 100 ||
			input <= dbs_tuners_ins.down_threshold)
		return -EINVAL;

	dbs_tuners_ins.up_threshold = input;
	return count;
}

static ssize_t store_up_threshold_hotplug(struct kobject *a, struct attribute *b,
				  const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1 || input > 100 ||
			input <= dbs_tuners_ins.down_threshold)
		return -EINVAL;

	dbs_tuners_ins.up_threshold_hotplug = input;
	return count;
}

static ssize_t store_down_threshold(struct kobject *a, struct attribute *b,
				    const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1 || input < 1 || input > 100 ||
			input >= dbs_tuners_ins.up_threshold)
		return -EINVAL;

	dbs_tuners_ins.down_threshold = input;
	return count;
}

static ssize_t store_down_threshold_hotplug(struct kobject *a, struct attribute *b,
				    const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1 || input < 1 || input > 100 ||
			input >= dbs_tuners_ins.up_threshold)
		return -EINVAL;

	dbs_tuners_ins.down_threshold_hotplug = input;
	return count;
}

static ssize_t store_ignore_nice_load(struct kobject *a, struct attribute *b,
				      const char *buf, size_t count)
{
	unsigned int input;
	int ret;

	unsigned int j;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	if (input > 1)
		input = 1;

	if (input == dbs_tuners_ins.ignore_nice) /* nothing to do */
		return count;

	dbs_tuners_ins.ignore_nice = input;

	/* we need to re-evaluate prev_cpu_idle */
	for_each_online_cpu(j) {
		struct cpu_dbs_info_s *dbs_info;
		dbs_info = &per_cpu(cs_cpu_dbs_info, j);
		dbs_info->prev_cpu_idle = get_cpu_idle_time(j,
						&dbs_info->prev_cpu_wall);
		if (dbs_tuners_ins.ignore_nice)
			dbs_info->prev_cpu_nice = kstat_cpu(j).cpustat.nice;
	}
	return count;
}

static ssize_t store_boost(struct kobject *a,
					  struct attribute *b,
					  const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);

	if (ret != 1 || input > 100 || input < 1)
		return -EINVAL;

	dbs_tuners_ins.boost = input;
	return count;
}

define_one_global_rw(sampling_rate);
define_one_global_rw(sampling_down_factor);
define_one_global_rw(up_threshold);
define_one_global_rw(up_threshold_hotplug);
define_one_global_rw(down_threshold);
define_one_global_rw(down_threshold_hotplug);
define_one_global_rw(ignore_nice_load);
define_one_global_rw(boost);

static struct attribute *dbs_attributes[] = {
	&sampling_rate_min.attr,
	&sampling_rate.attr,
	&sampling_down_factor.attr,
	&up_threshold_hotplug.attr,
	&down_threshold.attr,
	&down_threshold_hotplug.attr,
	&ignore_nice_load.attr,
	&boost.attr,
	&up_threshold.attr,
	NULL
};

static struct attribute_group dbs_attr_group = {
	.attrs = dbs_attributes,
	.name = "abyssplugv2",
};

/************************** sysfs end ************************/

static void dbs_check_cpu(struct cpu_dbs_info_s *this_dbs_info)
{
	unsigned int load = 0, cpu;
	struct cpufreq_policy *policy;
	cputime64_t cur_wall_time, cur_idle_time;
	unsigned int idle_time, wall_time;

	policy = this_dbs_info->cur_policy;
	cpu = this_dbs_info->cpu;

	/*
	 * Every sampling_rate, we check, if current idle time is less
	 * than 20% (default), then we try to increase frequency
	 * Every sampling_rate*sampling_down_factor, we check, if current
	 * idle time is more than 80%, then we try to decrease frequency
	 *
	 * Any frequency increase takes it to the maximum frequency.
	 * Frequency reduction happens at minimum steps of
	 * 5% (default) of maximum frequency
	 */

	/* Get Absolute Load */

	cur_idle_time = get_cpu_idle_time(cpu, &cur_wall_time);

	wall_time = (unsigned int) cputime64_sub(cur_wall_time,
				this_dbs_info->prev_cpu_wall);
	this_dbs_info->prev_cpu_wall = cur_wall_time;

	idle_time = (unsigned int) cputime64_sub(cur_idle_time,
			this_dbs_info->prev_cpu_idle);
	this_dbs_info->prev_cpu_idle = cur_idle_time;

	if (dbs_tuners_ins.ignore_nice) {
	    cputime64_t cur_nice;
	    unsigned long cur_nice_jiffies;

	    cur_nice = cputime64_sub(kstat_cpu(cpu).cpustat.nice,
				this_dbs_info->prev_cpu_nice);
	    /*
	     * Assumption: nice time between sampling periods will
	     * be less than 2^32 jiffies for 32 bit sys
	     */
	    cur_nice_jiffies = (unsigned long)
				cputime64_to_jiffies64(cur_nice);

	    this_dbs_info->prev_cpu_nice = kstat_cpu(cpu).cpustat.nice;
	    idle_time += jiffies_to_usecs(cur_nice_jiffies);
	}

	if (wall_time && (wall_time > idle_time))
	    load = 100 * (wall_time - idle_time) / wall_time;
	else
	    load = 0;
	
	/* Check for frequency increase */
	if (load > dbs_tuners_ins.up_threshold) {
	    this_dbs_info->down_skip = 0;

	    /* if we are already at full speed then break out early */
	    if (this_dbs_info->requested_freq == policy->max)
		goto cpu_up;

	    this_dbs_info->requested_freq = tb_get_next_freq(policy->cur, TB_UP, load);

	    __cpufreq_driver_target(policy, this_dbs_info->requested_freq,
						CPUFREQ_RELATION_H);
	    return;
	}
cpu_up:
	if (num_online_cpus() < 4 && load > dbs_tuners_ins.up_threshold_hotplug) {
	    cpu_up(num_online_cpus());
	    return;
	}
	/*
	 * The optimal frequency is the frequency that is the lowest that
	 * can support the current CPU usage without triggering the up
	 * policy. To be safe, we focus 10 points under the threshold.
	 */
	if (load < (dbs_tuners_ins.down_threshold - 10)) {

	/* if we cannot reduce the frequency anymore, break out early */
	if (policy->cur == policy->min)
	    goto cpu_down;

	this_dbs_info->requested_freq = tb_get_next_freq(policy->cur, TB_DOWN, load);

	__cpufreq_driver_target(policy, this_dbs_info->requested_freq,
						CPUFREQ_RELATION_H);
	return;
	}
cpu_down:
	if (num_online_cpus() > 1 && load < dbs_tuners_ins.down_threshold_hotplug) {
	    cpu_down(num_online_cpus()-1);
	    return;
	}

}

static void do_dbs_timer(struct work_struct *work)
{
	struct cpu_dbs_info_s *dbs_info =
		container_of(work, struct cpu_dbs_info_s, work.work);

	/* We want all CPUs to do sampling nearly on same jiffy */
	int delay = usecs_to_jiffies(dbs_tuners_ins.sampling_rate);

	delay -= jiffies % delay;

	dbs_check_cpu(dbs_info);

	queue_delayed_work_on(dbs_info->cpu, abyssplugv2_wq, &dbs_info->work, delay);
}

static inline void dbs_timer_init(struct cpu_dbs_info_s *dbs_info)
{
	/* We want all CPUs to do sampling nearly on same jiffy */
	int delay = usecs_to_jiffies(DEF_START_DELAY * 1000 * 1000 + dbs_tuners_ins.sampling_rate);
	delay -= jiffies % delay;

	INIT_DELAYED_WORK_DEFERRABLE(&dbs_info->work, do_dbs_timer);
	queue_delayed_work_on(dbs_info->cpu, abyssplugv2_wq, &dbs_info->work, delay);
}

static inline void dbs_timer_exit(struct cpu_dbs_info_s *dbs_info)
{
	cancel_delayed_work_sync(&dbs_info->work);
}

static void powersave_early_suspend(struct early_suspend *handler)
{
  mutex_lock(&dbs_mutex);
  sampling_rate_awake = dbs_tuners_ins.sampling_rate;
  up_threshold_awake = dbs_tuners_ins.up_threshold;
  down_threshold_awake = dbs_tuners_ins.down_threshold;
  boost_awake = dbs_tuners_ins.boost;
  dbs_tuners_ins.sampling_rate *= SAMPLING_RATE_SLEEP_MULTIPLIER;
  dbs_tuners_ins.up_threshold = 85;
  dbs_tuners_ins.down_threshold = 30;
  dbs_tuners_ins.boost = 95;
  mutex_unlock(&dbs_mutex);
}

static void powersave_late_resume(struct early_suspend *handler)
{
  mutex_lock(&dbs_mutex);
  dbs_tuners_ins.sampling_rate = sampling_rate_awake;
  dbs_tuners_ins.up_threshold = up_threshold_awake;
  dbs_tuners_ins.down_threshold = down_threshold_awake;
  dbs_tuners_ins.boost = boost_awake;
  mutex_unlock(&dbs_mutex);
}

static struct early_suspend _powersave_early_suspend = {
  .suspend = powersave_early_suspend,
  .resume = powersave_late_resume,
  .level = EARLY_SUSPEND_LEVEL_DISABLE_FB,
};

static int cpufreq_governor_dbs(struct cpufreq_policy *policy,
				   unsigned int event)
{
	unsigned int cpu = policy->cpu;
	struct cpu_dbs_info_s *this_dbs_info;
	int rc;

	this_dbs_info = &per_cpu(cs_cpu_dbs_info, cpu);

	switch (event) {
	case CPUFREQ_GOV_START:
		if ((!cpu_online(cpu)) || (!policy->cur))
		    return -EINVAL;

		this_dbs_info->cur_policy = policy;
		this_dbs_info->cpu = cpu;

		this_dbs_info->prev_cpu_idle = get_cpu_idle_time(cpu,
					&this_dbs_info->prev_cpu_wall);
		if (dbs_tuners_ins.ignore_nice)
		    this_dbs_info->prev_cpu_nice = kstat_cpu(cpu).cpustat.nice;

		this_dbs_info->down_skip = 0;
		this_dbs_info->requested_freq = policy->cur;

		mutex_lock(&dbs_mutex);

		dbs_enable++;
		/*
		 * Start the timerschedule work, when this governor
		 * is used for first time
		 */
		if (dbs_enable == 1) {
		    rc = sysfs_create_group(cpufreq_global_kobject,
						&dbs_attr_group);
		    if (rc) {
			mutex_unlock(&dbs_mutex);
			return rc;
		    }

		    /*
		     * conservative does not implement micro like ondemand
		     * governor, thus we are bound to jiffes/HZ
		     */
		    min_sampling_rate = 10000;
		    sampling_rate_awake = dbs_tuners_ins.sampling_rate;
		    up_threshold_awake = dbs_tuners_ins.up_threshold;
		    down_threshold_awake = dbs_tuners_ins.down_threshold;
		    boost_awake = dbs_tuners_ins.boost;

		    register_early_suspend(&_powersave_early_suspend);
		}
		mutex_unlock(&dbs_mutex);
		mutex_init(&this_dbs_info->timer_mutex);
		dbs_timer_init(this_dbs_info);

		break;

	case CPUFREQ_GOV_STOP:
		cancel_delayed_work(&this_dbs_info->work);
		printk(KERN_INFO"[%s] GOV STOP CPU:%u\n", __func__, cpu);
		mutex_lock(&dbs_mutex);

		dbs_enable--;
		/*
		 * Stop the timerschedule work, when this governor
		 * is used for first time
		 */
		if (!dbs_enable) {
		    sysfs_remove_group(cpufreq_global_kobject,
					&dbs_attr_group);

		    unregister_early_suspend(&_powersave_early_suspend);
		}
		mutex_unlock(&dbs_mutex);
		break;

	case CPUFREQ_GOV_LIMITS:
		mutex_lock(&this_dbs_info->timer_mutex);
		if (policy->max < this_dbs_info->cur_policy->cur)
			__cpufreq_driver_target(
					this_dbs_info->cur_policy,
					policy->max, CPUFREQ_RELATION_H);
		else if (policy->min > this_dbs_info->cur_policy->cur)
			__cpufreq_driver_target(
					this_dbs_info->cur_policy,
					policy->min, CPUFREQ_RELATION_L);
		mutex_unlock(&this_dbs_info->timer_mutex);
		break;
	}
	return 0;
}

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_ABYSSPLUG2
static
#endif
struct cpufreq_governor cpufreq_gov_abyssplug = {
	.name			= "abyssplugv2",
	.governor		= cpufreq_governor_dbs,
	.max_transition_latency	= TRANSITION_LATENCY_LIMIT,
	.owner			= THIS_MODULE,
};

static int __init cpufreq_gov_dbs_init(void)
{

    abyssplugv2_wq = alloc_workqueue("abyssplugv2", WQ_HIGHPRI, 1);

	return cpufreq_register_governor(&cpufreq_gov_abyssplug);
}

static void __exit cpufreq_gov_dbs_exit(void)
{
    destroy_workqueue(abyssplugv2_wq);
    cpufreq_unregister_governor(&cpufreq_gov_abyssplug);
}

// abyssplugV2 is based on the modified conservative (original author)
// Alexander Clouter <alex@digriz.org.uk>) smoove governor from Michael Weingaertner <mialwe@googlemail.com>
// (source: https://github.com/mialwe/mngb/)

MODULE_DESCRIPTION("'cpufreq_abyssplugV2' - A dynamic cpufreq governor based on cpufreq_conservative from Alexander Clouter."
"Uses frequency lookup tables and CPU hotplug by neobuddy89."
"Ported/modified for Huawei Ascend P6S-U06 by Kostyan_nsk");
MODULE_LICENSE("GPL");

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ABYSSPLUG
fs_initcall(cpufreq_gov_dbs_init);
#else
module_init(cpufreq_gov_dbs_init);
#endif
module_exit(cpufreq_gov_dbs_exit);
