/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ssiDrv.c
  �� �� ��   : ����
  ��    ��   :
  ��������   :
  ����޸�   :
  ��������   : SSI Module driver
  �����б�   :
              BSP_SSI_ReceiveData
              BSP_SSI_SendData
              SSI_test_RegGet
              SSI_test_RegSet
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   :
    �޸�����   :

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <mach/pmussi_drv.h>

#include "pmic_common.h"

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : BSP_PMU_RegSet
 ��������  : set v register data
 �������  : unsigned int regAddr --- �����üĴ�����ַ,PMU��ַλ��Ϊ8bit,32bit������չ
             unsigned int setData --- ���õ�ֵ,PMU����λ��Ϊ8bit,32bit������չ
 �������  : ��
 �� �� ֵ  : BSP_OK    - successful
             BSP_ERROR - unsuccessful
 ���ú���  : BSP_SSI_SendData
 ��������  : NA

*****************************************************************************/
BSP_S32 BSP_PMU_RegSet( BSP_U32 regAddr, BSP_U32 setData )
{
    pmussi_reg_write(regAddr,setData);
    return BSP_OK;
}

/*****************************************************************************
 �� �� ��  : DRV_PMU_REG_GET
 ��������  : get PMU register data
 �������  : BSP_U32  regAddr --- �����Ĵ�����ַ,PMU��ַλ��Ϊ8bit,32bit������չ
 �������  : BSP_U32  * getData --- ��żĴ���ֵ,PMU����λ��Ϊ8bit,32bit������չ
 �� �� ֵ  : BSP_OK    - successful
             BSP_ERROR - unsuccessful
 ���ú���  : BSP_SSI_ReceiveData
 ��������  : NA

*****************************************************************************/
BSP_S32 BSP_PMU_RegGet( BSP_U32 regAddr, BSP_U32 * getData )
{
  *getData = pmussi_reg_read(regAddr);
   return BSP_OK;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

