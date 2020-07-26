

/*****************************************************************************
  1 ͷ�ļ�����
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
  2 ȫ�ֱ�������
*****************************************************************************/
PNP_CALLBACK_STACK* pEnableStack = NULL;
PNP_CALLBACK_STACK* pDisableStack = NULL;

int g_usb_enum_complete = 0;


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
   ��NAS�Ľӿ�ʵ�֣�������׮ʵ��
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
    /* ģ��USB����֪ͨ */
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
    /* ģ��USB����֪ͨ */
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
   ��OAM�Ľӿ�ʵ�֣�������׮ʵ��
*****************************************************************************/






/*****************************************************************************
   ��TTF�Ľӿ�ʵ�֣�������׮ʵ��
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
 �� �� ��  : l2_notify_register
 ��������  : L2״̬�����˳�֪ͨ�ص�����ע��ӿ�
 �������  : FUNC_USB_LP_NOTIFY *pUSBLPFunc�ص�����ָ��
 �������  : ��
 �� �� ֵ  : 0:    ע��ɹ�
           ������ע��ʧ��
*****************************************************************************/
int l2_notify_register(FUNC_USB_LP_NOTIFY pUSBLPFunc)
{
    return ERROR;
}

/*�ṩ��NAS�Ľӿ�*/
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

/*�ṩ��OAM�Ľӿ�*/
EXPORT_SYMBOL(l2_notify_register);

/*�ṩ��TTF�Ľӿ�*/
EXPORT_SYMBOL(USB_ETH_DrvSetRxFlowCtrl);
EXPORT_SYMBOL(USB_ETH_DrvClearRxFlowCtrl);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

