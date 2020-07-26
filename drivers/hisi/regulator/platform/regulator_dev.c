/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : regulator_dev.c
  �� �� ��   : ����
  ��    ��   : ����ΰ 176398
  ��������   : 2012��12��20��
  ����޸�   :
  ��������   : regulator�豸ע��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��12��20��
    ��    ��   : ����ΰ 176398
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/platform_device.h>
#include <mach/platform.h>
#include <linux/regulator/machine.h>
#include <hsad/config_mgr.h>
#include <regulator_map.h>


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
struct regulator_init_data* platform_regulators = NULL;

#ifdef _DRV_LLT_
struct platform_device regulator_platform_device =
#else
static struct platform_device regulator_platform_device =
#endif
{
    .name =  "platform-regulator",
    .id = 0,
    .dev = {
        .platform_data = NULL,
    },
};

#if 0
/* please add all pmu related platform device in the struct.*/
static struct platform_device *regulator_platform_dev[] __initdata = {
    &regulator_platform_device
       /*add other pmu device here*/
};
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

unsigned int get_boardid(void);

int __init regulator_platform_dev_init(void)
{
    int ret = 0;
    unsigned int board_id = get_boardid();
    struct regulator_config_platform_table *table_item = NULL;

    table_item = &(platform_regulators_total[0]);
    while (REGULATOR_CONFIG_PLAT_TBL_END_SYMBOL != table_item->board_id) {
        if (board_id == table_item->board_id) {
            break;
        }
        table_item++;
    }
    if (REGULATOR_CONFIG_PLAT_TBL_END_SYMBOL == table_item->board_id){
        printk("Get regulator_type fail!, not match with %d.\n", table_item->board_id);
        return -EINVAL;
    }

    platform_regulators = table_item->table;
    regulator_platform_device.dev.platform_data = platform_regulators;

    ret = platform_device_register(&regulator_platform_device);
    //ret = platform_add_devices(regulator_platform_dev, ARRAY_SIZE(regulator_platform_dev));

    printk("\n*******************regulator device init(%d)**********************\n",ret);

    return ret;
};

arch_initcall(regulator_platform_dev_init);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

