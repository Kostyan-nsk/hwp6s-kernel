
#ifndef __DRV_NV_H__
#define __DRV_NV_H__
#include "drv_global.h"
#if (defined(FEATURE_E5) &&  defined(FEATURE_HILINK) && ((FEATURE_E5 == FEATURE_ON) || (FEATURE_HILINK == FEATURE_ON)))
/*Ӧ�����ô˺������ʱ,��λ��10ms*/
#define  APP_SLEEP(tick)   drv_sleep((tick), (unsigned int )__FILE__, __LINE__)
#endif

/***************************************************************
*  ������        :   BSP_MMI_TestResultSet
*  ��������      :   ���ӿ��ǽ�MMI�Ĳ��Ա�־д��NV
                     ��ulFlag == 1 ��д�� ��ST P��
                     ��ulFlag == 0 ��д�� ��ST F��
*  �������      :   BSP_U32 ulFlag
*  �������      :   ��
*  ����ֵ        :   �ɹ�(0)  ʧ��(-1)
***************************************************************/
int BSP_MMI_TestResultSet(unsigned int ulFlag);
#define DVR_MMI_TEST_RESULT_SET(ulFlag) BSP_MMI_TestResultSet(ulFlag)

/***************************************************************
*  ������        :   BSP_MMI_TestResultGet
*  ��������      :   ���ӿڱȽ�MMI NV�е��Ƿ��ǡ�ST P��
                                  �Ƿ���1   ���Ƿ���0
*  �������      :   ��
*  �������      :   ��
*  ����ֵ        :   �ɹ�(1)  ʧ��(0)
***************************************************************/
unsigned int BSP_MMI_TestResultGet(void);
#define DVR_MMI_TEST_RESULT_GET() BSP_MMI_TestResultGet()

/*****************************************************************************
 �� �� ��  : BSP_PROD_GetRecoverNvInfo
 ��������  : �ӵ����õ�ǰ��Ҫ���ǵ�NV��Ϣ
 �������  : N/A

 �������  : ppNvInfo  - ָ��NV��Ϣ�ṹ�������
             pulNvNum  - �������

 �� �� ֵ  : 0����ȷ����0: ʧ��
*****************************************************************************/
extern  int BSP_PROD_GetRecoverNvInfo(void **ppNvInfo, unsigned long *pulNvNum);
#define  DRV_GET_RECOVER_NV_INFO(ppNvInfo, pulNvNum)  \
                BSP_PROD_GetRecoverNvInfo(ppNvInfo, pulNvNum)


extern int DRV_COPY_NVUSE_TO_NVBACKUP ( void );



extern int DRV_COPY_NVUPGRADE_TO_NVUSE( void );

#endif

