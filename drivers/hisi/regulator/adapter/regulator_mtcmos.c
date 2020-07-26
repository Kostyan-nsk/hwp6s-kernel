/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : regulator_mtcmos.c
  �� �� ��   : ����
  ��    ��   : ����ΰ 176398
  ��������   : 2012��12��19��
  ����޸�   :
  ��������   : mtcmos��Դ��������㣬���ڶԽ�regulatorƽ̨����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��12��19��
    ��    ��   : ����ΰ 176398
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include "regulator_mtcmos.h"
#include "regulator_mntn.h"
#include "mtcmos_driver.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

int regulator_mtcmos_is_enabled(struct regulator_dev *dev)
{
    int  regulator_id;
    int ret;

    rdev_info(dev, "%s func is called\n", __func__);
    regulator_id =  rdev_get_id(dev);

    ret = mtcmos_power_status(regulator_id);
    if (MTCMOS_MODULE_STATUS_ON == ret) {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

int regulator_mtcmos_enable(struct regulator_dev *dev)
{
    int  regulator_id;
    int ret;

    rdev_info(dev, "%s func is called\n", __func__);
    regulator_id =  rdev_get_id(dev);

    ret = mtcmos_power_on(regulator_id);
    if (MTCMOS_MODULE_OK == ret) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}
int regulator_mtcmos_disable(struct regulator_dev *dev)
{
    int  regulator_id;
    int ret;

    rdev_info(dev, "%s func is called\n", __func__);
    regulator_id =  rdev_get_id(dev);

    ret = mtcmos_power_off(regulator_id);
    if (MTCMOS_MODULE_OK == ret) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

struct regulator_ops regulator_mtcmos_ops = {
    .is_enabled = regulator_mtcmos_is_enabled,
    .enable = regulator_mtcmos_enable,
    .disable = regulator_mtcmos_disable,
};

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

