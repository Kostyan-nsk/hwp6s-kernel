

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <stddef.h>

#include "test_acore.h"
#include "regulator/test_regulator_drv.h"
#include "mmc/test_emmc_drv.h"
#include "ioc/test_iomux_drv.h"
#include "gpio/test_gpio_drv.h"
#include "clock/test_clock_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
int test_ok(void);
int test_error(void);
typedef struct
{
    unsigned int idx;           /* �������� */
    int (*func)(void);          /* �����ӿ� */
    unsigned char *info;        /* ������Ϣ */
    int result;                 /* ������� */
} utat_cases;
utat_cases g_acore_utat_cases[] =
{
    //{1, test_ok, "test return ok", OK},
    //{2, test_error, "test return error", OK},
    //{3, test_tp_remove, "test tp remove", OK},
    //{4, test_tp_probe, "test tp probe", OK},
    //{5, test_tp_suspend, "test tp suspend", OK},
    //{6, test_tp_resume, "test tp resume", OK},
    {1, test_emmc_file_create, "test_emmc_file_create", OK},
    {2, test_emmc_file_WR, "test_emmc_file_WR", OK},
    {3, test_clock_get, "test_clock_get", OK},
    {4, test_clock_enable, "test_clock_enable", OK},
    {5, test_iomux_get_config, "test_iomux_get_config", OK},
    {6, test_regulator_get, "test_regulator_get", OK},
    {7, test_regulator_enable, "test_regulator_enable", OK},
    {8, test_regulator_disable, "test_regulator_disable", OK},
    {9, test_gpio_to_irq, "test_gpio_to_irq", OK},
	//{10, test_mb_send2hifi_vos, "test mb send2hifi vos", OK},
    //{11, test_mb_send2hifi_dsp, "test mb send2hifi dsp", OK},
    //{12, test_mb_send2hifi_audio, "test mb send2hifi audio", OK},
    //{13, test_mb_send2hifi_fail, "test mb send2hifi fail", OK},
    //{14, test_mb_send2mcu_tp, "test mb send2mcu tp", OK},
	//{15, test_mb_send2mcu_fail, "test mb send2mcu fail", OK},
	//{16, test_mb_rev_hifi, "test mb receive from hifi", OK},
	//{17, test_mb_rev_mcu, "test mb receive from mcu", OK},
};


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

int test_ok(void)
{
    return OK;
}

int test_error(void)
{
    return ERROR;
}


int parse_arg(unsigned char *cases, int maxNum)
{
    unsigned char *p;
    unsigned int idx;

    p = strsep((char **)(&cases), ".");
    while(NULL != p)
    {
        idx = simple_strtoul(p, NULL, 0);
        if (idx > 0 && idx <= maxNum)
        {
            g_acore_utat_cases[idx-1].result = ERROR;
        }
        p = strsep((char **)(&cases), ".");
    }
    return OK;
}


int test_acore(unsigned char *cases)
{
    int caseNum;
    int i;
    int flag;

    if (NULL == cases)
    {
        return ERROR;
    }

    printk("\n%s begin\n", __func__);
    printk("test_acore arg:%s\n", cases);

    flag = 0;
    caseNum = sizeof(g_acore_utat_cases)/sizeof(utat_cases);
    if (!strcmp(cases, "all"))  /* ������������ */
    {
        printk("all test!\n");
        for(i=0; i<caseNum; i++)
        {
            g_acore_utat_cases[i].result = g_acore_utat_cases[i].func();
        }
    }
    else
    {
        parse_arg(cases ,caseNum);
        for(i=0; i<caseNum; i++)
        {
            if (OK != g_acore_utat_cases[i].result)
            {
                printk("%d :%s test!\n", i,g_acore_utat_cases[i].info);
                g_acore_utat_cases[i].result = g_acore_utat_cases[i].func();
            }
        }
    }

    /* ���ͳ����Ϣ */
    for(i=0; i<caseNum; i++)
    {
        if (OK != g_acore_utat_cases[i].result)
        {
            printk("%s failed!\n", g_acore_utat_cases[i].info);
            g_acore_utat_cases[i].result = OK;
            flag ++;
        }
    }

    printk("%s over\n", __func__);
    if (flag)
    {
        return ERROR;
    }
    else
    {
        return OK;
    }

}


int __init test_acore_init(void)
{
    printk("%s\n", __func__);
    return OK;

}


void  __exit test_acore_exit(void)
{
    printk("%s/n", __func__);
}


module_init(test_acore_init);
module_exit(test_acore_exit);

MODULE_AUTHOR("lubing <jean.lubing@huawei.com>");
MODULE_DESCRIPTION("test acore for v9r1");
MODULE_LICENSE("GPL");



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

