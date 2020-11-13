/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_ipcm.c
  �� �� ��   : ����
  ��    ��   : c61362
  ��������   : 2012��3��2��
  ����޸�   :
  ��������   : ������ϲ������װ�Ľӿڲ�
  �޸���ʷ   :
  1.��    ��   : 2012��3��2��
    ��    ��   : c61362
    �޸�����   : �½�Drvinterface.c
    
  2.��    ��   : 2013��2��19��
    ��    ��   : ��ϲ 220237
    �޸�����   : ��Drvinterface.c�������

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "BSP.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_S32 BSP_IPC_SemDelete(BSP_U32 u32SignalNum);
#endif

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM) || defined(BSP_CORE_APP)
extern BSP_S32 BSP_IPC_SemCreate(BSP_U32 u32SignalNum);
extern BSP_S32 BSP_IPC_IntEnable (IPC_INT_LEV_E ulLvl);
extern BSP_S32 BSP_IPC_IntDisable (IPC_INT_LEV_E ulLvl);
extern BSP_S32 BSP_IPC_IntConnect  (IPC_INT_LEV_E ulLvl, VOIDFUNCPTR routine, BSP_U32 parameter);
extern  BSP_S32 BSP_IPC_IntDisonnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);
extern BSP_S32 BSP_IPC_IntSend(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl);
extern BSP_S32 BSP_IPC_SemTake(BSP_U32 u32SignalNum,BSP_S32 s32timeout);
extern BSP_VOID BSP_IPC_SemGive(BSP_U32 u32SignalNum);
extern BSP_VOID BSP_IPC_SpinLock (BSP_U32 u32SignalNum);
extern BSP_VOID BSP_IPC_SpinUnLock (BSP_U32 u32SignalNum);
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 * �� �� ��  : DRV_IPC_SEMCREATE
 *
 * ��������  : �ź�����������
 *
 * �������  : ��
 * �������  : ��
 *
 * �� �� ֵ  : ��
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
BSP_S32 DRV_IPC_SEMCREATE(BSP_U32 u32SignalNum)
{
    return BSP_IPC_SemCreate(u32SignalNum);
}

/*****************************************************************************
* �� �� ��  : DRV_IPC_SEMDELETE
*
* ��������  : ɾ���ź���
*
* �������  :   BSP_U32 u32SignalNum Ҫɾ�����ź������

* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_S32 DRV_IPC_SEMDELETE(BSP_U32 u32SignalNum)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_IPC_SemDelete(u32SignalNum);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
* �� �� ��  : DRV_IPC_INTENABLE
*
* ��������  : ʹ��ĳ���ж�
*
* �������  :
                BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_S32 DRV_IPC_INTENABLE (IPC_INT_LEV_E ulLvl)
{
    return BSP_IPC_IntEnable(ulLvl);
}

/*****************************************************************************
 * �� �� ��  : DRV_IPC_INTDISABLE
 *
 * ��������  : ȥʹ��ĳ���ж�
 *
 * �������  :
                BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
BSP_S32 DRV_IPC_INTDISABLE (IPC_INT_LEV_E ulLvl)
{
    return BSP_IPC_IntDisable(ulLvl);
}

/*****************************************************************************
 * �� �� ��  : DRV_IPC_INTCONNECT
 *
 * ��������  : ע��ĳ���ж�
 *
 * �������  :
               BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
               VOIDFUNCPTR routine �жϷ������
 *             BSP_U32 parameter      �жϷ���������
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
BSP_S32 DRV_IPC_INTCONNECT  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter)
{
    return BSP_IPC_IntConnect(ulLvl, routine, parameter);
}

/*****************************************************************************
 * �� �� ��  : DRV_IPC_INTDISCONNECT
 *
 * ��������  : ȥע��ĳ���ж�
 *
 * �������  :
               BSP_U32 ulLvl Ҫȥע����жϺţ�ȡֵ��Χ0��31
               VOIDFUNCPTR routine �жϷ������
 *             BSP_U32 parameter      �жϷ���������
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
BSP_S32 DRV_IPC_INTDISCONNECT  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter)
{
    return BSP_IPC_IntDisonnect(ulLvl, routine, parameter);
}

/*****************************************************************************
* �� �� ��  : DRV_IPC_INTSEND
*
* ��������  : �����ж�
*
* �������  :
                IPC_INT_CORE_E enDstore Ҫ�����жϵ�core
                BSP_U32 ulLvl Ҫ���͵��жϺţ�ȡֵ��Χ0��31
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_S32 DRV_IPC_INTSEND(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl)
{
    return BSP_IPC_IntSend(enDstCore, ulLvl);
}

/*****************************************************************************
* �� �� ��  : DRV_IPC_SEMTAKE
*
* ��������  : ��ȡ�ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_S32  DRV_IPC_SEMTAKE(BSP_U32 u32SignalNum, BSP_S32 s32timeout)
{
    return BSP_IPC_SemTake(u32SignalNum, s32timeout);
}

/*****************************************************************************
* �� �� ��  : DRV_IPC_SEMGIVE
*
* ��������  : �ͷ��ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_VOID DRV_IPC_SEMGIVE(BSP_U32 u32SignalNum)
{
    BSP_IPC_SemGive(u32SignalNum);
}

/*****************************************************************************
* �� �� ��  : DRV_SPIN_LOCK
*
* ��������  : ��ȡ�ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_VOID DRV_SPIN_LOCK (BSP_U32 u32SignalNum)
{
    BSP_IPC_SpinLock (u32SignalNum);
}

/*****************************************************************************
* �� �� ��  : DRV_SPIN_UNLOCK
*
* ��������  : �ͷ��ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_VOID DRV_SPIN_UNLOCK(BSP_U32 u32SignalNum)
{
    BSP_IPC_SpinUnLock(u32SignalNum);
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

