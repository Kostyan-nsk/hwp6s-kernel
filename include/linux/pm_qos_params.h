#ifndef _LINUX_PM_QOS_PARAMS_H
#define _LINUX_PM_QOS_PARAMS_H
/* interface for the pm_qos_power infrastructure of the linux kernel.
 *
 * Mark Gross <mgross@linux.intel.com>
 */
#include <linux/plist.h>
#include <linux/notifier.h>
#include <linux/miscdevice.h>

#define PM_QOS_RESERVED 0
#define PM_QOS_CPU_DMA_LATENCY 1
#define PM_QOS_NETWORK_LATENCY 2
#define PM_QOS_NETWORK_THROUGHPUT 3
#define PM_QOS_BUS_DMA_THROUGHPUT 4
#define PM_QOS_DISPLAY_FREQUENCY 5
#define PM_QOS_BUS_QOS 6
#define PM_QOS_DVFS_RESPONSE_LATENCY 7


#if defined (CONFIG_ARCH_HI6620) || defined (CONFIG_ARCH_K3V2)
#if defined (CONFIG_ARCH_HI6620) || defined (CONFIG_IPPS_SUPPORT)

/*sun add 2011-8-22 begin*/
#define PM_QOS_CPU_MAX_PROFILE    4
#define PM_QOS_CPU_MIN_PROFILE    5
#define PM_QOS_CPU_PROFILE_BLOCK  6
#define PM_QOS_CPU_PROFILE_SAFE   7

#define PM_QOS_GPU_MAX_PROFILE    8
#define PM_QOS_GPU_MIN_PROFILE    9
#define PM_QOS_GPU_PROFILE_BLOCK  10
#define PM_QOS_GPU_PROFILE_SAFE   11

#define PM_QOS_DDR_MAX_PROFILE    12
#define PM_QOS_DDR_MIN_PROFILE    13
#define PM_QOS_DDR_PROFILE_BLOCK  14
#define PM_QOS_DDR_PROFILE_SAFE   15

#define PM_QOS_CPU_NUMBER_LOCK    16
#define PM_QOS_CPU_NUMBER_MAX     17
#define PM_QOS_CPU_NUMBER_MIN     18
#define PM_QOS_CPU_NUMBER_SAFE    19
/*sun add 2011-8-22 end  */
#define PM_QOS_IPPS_POLICY        20
#if defined (CONFIG_ARCH_HI6620)
#define PM_QOS_BUS_MAX_PROFILE    21
#define PM_QOS_BUS_MIN_PROFILE    22

#define PM_QOS_CPU_INT_LATENCY    23

#define PM_QOS_NUM_CLASSES 24
#else
#define PM_QOS_NUM_CLASSES 21
#endif

#else

#define PM_QOS_NUM_CLASSES 4

#endif
#else
#define PM_QOS_DDR_MINFREQ          (4)
#define PM_QOS_DDR_MAXFREQ          (5)
#define PM_QOS_ACPU_MINFREQ         (6)
#define PM_QOS_ACPU_MAXFREQ         (7)
#define PM_QOS_GPU_MINFREQ          (8)
#define PM_QOS_GPU_MAXFREQ          (9)

#define PM_QOS_NUM_CLASSES 10
#endif

#define PM_QOS_DEFAULT_VALUE -1

#define PM_QOS_CPU_DMA_LAT_DEFAULT_VALUE	(2000 * USEC_PER_SEC)
#define PM_QOS_NETWORK_LAT_DEFAULT_VALUE	(2000 * USEC_PER_SEC)
#define PM_QOS_NETWORK_THROUGHPUT_DEFAULT_VALUE	0
#define PM_QOS_BUS_DMA_THROUGHPUT_DEFAULT_VALUE 0
#define PM_QOS_DISPLAY_FREQUENCY_DEFAULT_VALUE	0
#define PM_QOS_DVFS_RESPONSE_LAT_DEFAULT_VALUE	(2000 * USEC_PER_SEC)

#if defined (CONFIG_ARCH_HI6620) || defined (CONFIG_ARCH_K3V2)
#if defined (CONFIG_ARCH_HI6620) || defined (CONFIG_IPPS_SUPPORT)

/*sun add 2011-8-22 begin*/

/*cpu profile related*/
//#define PM_QOS_CPU_MAXPROFILE_DEFAULT_VALUE (0x7FFFFFFF)
#define PM_QOS_CPU_MAXPROFILE_DEFAULT_VALUE (0x1E74E0)
#define PM_QOS_CPU_MINPROFILE_DEFAULT_VALUE (0)
#define PM_QOS_CPU_BLKPROFILE_DEFAULT_VALUE (0)
#define PM_QOS_CPU_SAFEPROFILE_DEFAULT_VALUE (0)

/*gpu profile related*/
#define PM_QOS_GPU_MAXPROFILE_DEFAULT_VALUE (0x7FFFFFFF)
#define PM_QOS_GPU_MINPROFILE_DEFAULT_VALUE (0)
#define PM_QOS_GPU_BLKPROFILE_DEFAULT_VALUE (0)
#define PM_QOS_GPU_SAFEPROFILE_DEFAULT_VALUE (0)

/*ddr profile related*/
#define PM_QOS_DDR_MAXPROFILE_DEFAULT_VALUE (0x7FFFFFFF)
#define PM_QOS_DDR_MINPROFILE_DEFAULT_VALUE (0)
#define PM_QOS_DDR_BLKPROFILE_DEFAULT_VALUE (0)
#define PM_QOS_DDR_SAFEPROFILE_DEFAULT_VALUE (0)

/*cpu number related*/
#define PM_QOS_CPU_NUMBER_LOCK_DEFAULT_VALUE (0)
#define PM_QOS_CPU_NUMBER_MAX_DEFAULT_VALUE (NR_CPUS)
#define PM_QOS_CPU_NUMBER_MIN_DEFAULT_VALUE (1)
#define PM_QOS_CPU_NUMBER_SAFE_DEFAULT_VALUE (NR_CPUS)

/*ipps policy related define*/
#define PM_QOS_IPPS_POLICY_DEFAULT_VALUE    (-1)
#define PM_QOS_IPPS_POLICY_POWERSAVE        (0x0)
#define PM_QOS_IPPS_POLICY_NORMAL           (0x1)
#define PM_QOS_IPPS_POLICY_PERFORMANCE      (0x2)
#define PM_QOS_IPPS_POLICY_SPECIAL01        (0x3)
#define PM_QOS_IPPS_POLICY_SPECIAL02        (0x4)
#define PM_QOS_IPPS_POLICY_SPECIAL03        (0x5)
#define PM_QOS_IPPS_POLICY_SPECIAL04        (0x6)
#define PM_QOS_IPPS_POLICY_SPECIAL05        (0x7)
#define PM_QOS_IPPS_POLICY_SPECIAL06        (0x8)
#define PM_QOS_IPPS_POLICY_SPECIAL07        (0x9)
#define PM_QOS_IPPS_POLICY_SPECIAL08        (0xA)
#define PM_QOS_IPPS_POLICY_SPECIAL09        (0xB)
#define PM_QOS_IPPS_POLICY_SPECIAL0A        (0xC)
#define PM_QOS_IPPS_POLICY_SPECIAL0B        (0xD)
#define PM_QOS_IPPS_POLICY_MAX              (0xD)

#ifdef CONFIG_ARCH_HI6620
/*bus profile related*/
#define PM_QOS_BUS_MAXPROFILE_DEFAULT_VALUE (0x7FFFFFFF)
#define PM_QOS_BUS_MINPROFILE_DEFAULT_VALUE (0)

/*int vote latancy*/
#define PM_QOS_CPUIDLE_INT_DEFAULT_VALUE (2000 * USEC_PER_SEC)
#endif

/*sun add 2011-8-22 end*/
#endif
#define PM_QOS_PWRCTRL_DFS_DEFAULT_VALUE    (0)
#else
#define PM_QOS_PWRCTRL_DFS_DEFAULT_VALUE    (0)
#endif

#if defined (CONFIG_ARCH_HI6620)
#define PM_QOS_CPU_NUMBER_MIN_VALUE_FOR_DISPLAY    (2)
#define PM_QOS_DDR_MINPROFILE_VALUE_FOR_DISPLAY    (150000)
#endif
struct pm_qos_request_list {
    struct plist_node list;
    int pm_qos_class;
};


enum  {
    IPC_QOS = 1,   /*IPC�ź�����32Ʊ��ռ��32��ͶƱֵ, 1-32*/
    I2C_QOS  = 40, /*I2C 0-5, 40-45*/
    MMC_QOS = 50, /*MMC 0-2, 50-52*/
    USB_QOS = 60,
    IDLE_QOS,
};


void pm_qos_add_request(struct pm_qos_request_list *l, int pm_qos_class, s32 value);
void pm_qos_update_request(struct pm_qos_request_list *pm_qos_req,
        s32 new_value);
void pm_qos_remove_request(struct pm_qos_request_list *pm_qos_req);

int pm_qos_request(int pm_qos_class);
int pm_qos_add_notifier(int pm_qos_class, struct notifier_block *notifier);
int pm_qos_remove_notifier(int pm_qos_class, struct notifier_block *notifier);
int pm_qos_request_active(struct pm_qos_request_list *req);

#endif
