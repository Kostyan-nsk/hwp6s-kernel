

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include  <linux/slab.h>
#include  <linux/kernel.h>
#include  <linux/module.h>
#include  "drv_usb.h"
#include  "drv_usb_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
PNP_CALLBACK_STACK* pEnableStack = NULL;
PNP_CALLBACK_STACK* pDisableStack = NULL;

int g_usb_enum_complete = 0;


/*****************************************************************************
  3 函数实现
*****************************************************************************/

/*****************************************************************************
   与NAS的接口实现，包含打桩实现
*****************************************************************************/


unsigned int BSP_USB_RegUdiEnableCB(USB_UDI_ENABLE_CB_T pFunc)
{
    PNP_CALLBACK_STACK* tmp = pEnableStack;

    USB_API_DBG("enter\n");

    if (NULL == pFunc)
    {
        USB_API_DBG("pFunc NULL\n");
        return 1;
    }

    pEnableStack = (PNP_CALLBACK_STACK*) kmalloc(sizeof(PNP_CALLBACK_STACK), M_ZERO);

    pEnableStack->pPre = tmp;
    pEnableStack->pCallback = pFunc;

    if(g_usb_enum_complete)
    {
        pEnableStack->pCallback();
    }

    return 0;
}


unsigned int BSP_USB_RegUdiDisableCB(USB_UDI_DISABLE_CB_T pFunc)
{
    PNP_CALLBACK_STACK* tmp = pDisableStack;

    USB_API_DBG("enter\n");

    if (NULL == pFunc)
    {
        USB_API_DBG("pFunc NULL\n");
        return 1;
    }

    pDisableStack = (PNP_CALLBACK_STACK*) kmalloc(sizeof(PNP_CALLBACK_STACK), M_ZERO);

    pDisableStack->pPre = tmp;
    pDisableStack->pCallback = pFunc;

    return 0;
}


void BSP_USB_UdiEnableNotify (void)
{
    PNP_CALLBACK_STACK* pEnableFunc = pEnableStack;
    /* 模拟USB插入通知 */
    while (pEnableFunc)
    {
        pEnableFunc->pCallback();
        pEnableFunc = pEnableFunc->pPre;
    }
    g_usb_enum_complete = 1;
}
void BSP_USB_UdiDisableNotify (void)
{
    PNP_CALLBACK_STACK* pDisableFunc = pDisableStack;
    /* 模拟USB插入通知 */
    while (pDisableFunc)
    {
        pDisableFunc->pCallback();
        pDisableFunc = pDisableFunc->pPre;
    }
    g_usb_enum_complete = 0;
}
int BSP_USB_GetDiagModeValue(unsigned char *dial_mode, unsigned char *cdc_spec)
{
    return 1;
}


int BSP_USB_GetLinuxSysType(void)
{
    return -1;
}


unsigned char BSP_USB_GetPortMode(char* PsBuffer, unsigned long* Length )
{
    return 1;
}


unsigned int BSP_USB_GetU2diagDefaultValue(void)
{
    return 0;
}


int BSP_USB_NASSwitchGatewayRegFunc(USB_NET_DEV_SWITCH_GATEWAY switchGwMode)
{
    return 0;
}


int BSP_USB_SetPid(unsigned char u2diagValue)
{
    return 1;
}

/*****************************************************************************
   与OAM的接口实现，包含打桩实现
*****************************************************************************/






/*****************************************************************************
   与TTF的接口实现，包含打桩实现
*****************************************************************************/


unsigned long USB_ETH_DrvSetRxFlowCtrl(unsigned long ulParam1, unsigned long ulParam2)
{
    return 0;
}


unsigned long USB_ETH_DrvClearRxFlowCtrl(unsigned long ulParam1, unsigned long ulParam2)
{
    return 0;
}


unsigned int BSP_USB_PortTypeValidCheck(unsigned char *pucPortType, unsigned long ulPortNum)
{
    return 0;
}


unsigned int BSP_USB_GetAvailabePortType(unsigned char *pucPortType, unsigned long *pulPortNum, unsigned long ulPortMax)
{
    return 0;
}


unsigned int BSP_USB_PortTypeQuery(DRV_DYNAMIC_PID_TYPE_STRU *pstDynamicPidType)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : l2_notify_register
 功能描述  : L2状态进入退出通知回调函数注册接口
 输入参数  : FUNC_USB_LP_NOTIFY *pUSBLPFunc回调函数指针
 输出参数  : 无
 返 回 值  : 0:    注册成功
           其他：注册失败
*****************************************************************************/
int l2_notify_register(FUNC_USB_LP_NOTIFY pUSBLPFunc)
{
    return ERROR;
}

/*提供给NAS的接口*/
EXPORT_SYMBOL(BSP_USB_RegUdiEnableCB);
EXPORT_SYMBOL(BSP_USB_RegUdiDisableCB);
EXPORT_SYMBOL(BSP_USB_UdiEnableNotify);
EXPORT_SYMBOL(BSP_USB_UdiDisableNotify);
EXPORT_SYMBOL(BSP_USB_GetDiagModeValue);
EXPORT_SYMBOL(BSP_USB_GetU2diagDefaultValue);
EXPORT_SYMBOL(BSP_USB_SetPid);
EXPORT_SYMBOL(BSP_USB_GetPortMode);
EXPORT_SYMBOL(BSP_USB_GetLinuxSysType);
EXPORT_SYMBOL(BSP_USB_NASSwitchGatewayRegFunc);
EXPORT_SYMBOL(BSP_USB_PortTypeValidCheck);
EXPORT_SYMBOL(BSP_USB_GetAvailabePortType);
EXPORT_SYMBOL(BSP_USB_PortTypeQuery);

/*提供给OAM的接口*/
EXPORT_SYMBOL(l2_notify_register);

/*提供给TTF的接口*/
EXPORT_SYMBOL(USB_ETH_DrvSetRxFlowCtrl);
EXPORT_SYMBOL(USB_ETH_DrvClearRxFlowCtrl);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

