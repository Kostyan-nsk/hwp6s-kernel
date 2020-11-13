/******************************************************************************

                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : cpu_view_adapter.c
  �� �� ��   : ����
  ��    ��   : g00178567
  ��������   : 2013��10��30��
  ����޸�   :
  ��������   : ����ϵͳ����
  �����б�   :
                OS_GetTaskIdList
                OS_GetTaskName
  �޸���ʷ   :
  1.��    ��   : 2013��10��30��
    ��    ��   : g00178567
    �޸�����   : �����ļ�

******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif

/******************************************************************************
   1 ͷ�ļ�����
******************************************************************************/
#include "vos.h"
#include "cpu_view_adapter.h"


/******************************************************************************
   1 �궨��
******************************************************************************/

/******************************************************************************
   6 ����ʵ��
******************************************************************************/

/******************************************************************************
 Prototype      : OS_GetTaskIdList
 Description    : ��ȡ����ID��ʵ���������
 Input          :
 Output         :
 Return Value   :
 Calls          :
 Called By      :

 History        : ---
  1.Date        : 2013-11-06
    Author      : g00178567
    Modification: Created function
******************************************************************************/
VOS_UINT16 OS_GetTaskIdList(int iTaskIdList[], int iTaskMaxNum)
{
    VOS_UINT16              usTaskNum   = iTaskMaxNum;

    if (VOS_NULL_PTR == iTaskIdList)
    {
        return VOS_NULL;
    }

#if ((VOS_VXWORKS == VOS_OS_VER) || (VOS_NUCLEUS == VOS_OS_VER) || (VOS_WIN32 == VOS_OS_VER))
    usTaskNum = (VOS_UINT16)taskIdListGet(iTaskIdList, usTaskNum);
#elif (VOS_RTOSCK == VOS_OS_VER)
    if (SRE_OK != SRE_TaskIDList((UINT16 *)&usTaskNum, (UINT32 *)iTaskIdList))
    {
        return VOS_NULL;
    }
#else
    return VOS_NULL;
#endif

    return usTaskNum;
}

/******************************************************************************
 Prototype      : OS_GetTaskName
 Description    : ��ȡ��������
 Input          :
 Output         :
 Return Value   :
 Calls          :
 Called By      :

 History        : ---
  1.Date        : 2013-11-06
    Author      : g00178567
    Modification: Created function
******************************************************************************/
char * OS_GetTaskName(VOS_UINT32 ulTaskId)
{
    char                   *pucTaskName = VOS_NULL_PTR;

#if ((VOS_VXWORKS == VOS_OS_VER) || (VOS_NUCLEUS == VOS_OS_VER) || (VOS_WIN32 == VOS_OS_VER))
    pucTaskName = taskName((int)ulTaskId);
#elif (VOS_RTOSCK == VOS_OS_VER)
    if (SRE_OK != SRE_TaskNameGet((TSK_HANDLE_T)ulTaskId, &pucTaskName))
    {
        return VOS_NULL_PTR;
    }
#else
    return VOS_NULL_PTR;
#endif

    return pucTaskName;
}

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

