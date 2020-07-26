

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include <linux/kernel.h>

#ifndef __MNTN_DEAL_WITH_MCU_MSG_H__
#define __MNTN_DEAL_WITH_MCU_MSG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
/*#define MNTN_DEBUG*/

#define PRINT_LEV KERN_INFO

#define DATA_SIZE_IN_MSG_HEAD   4   /* ��ϢͷΪ���ݷ���4�ֽڣ�����4�ֽڶ��� */

#define DATA_MAX_LEN  (50)
#define MNTN_BUFF_SIZE   (0x200)    /* ƹ��buffer��ÿ��buff��С��Ϊ512�ֽ� */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

typedef enum
{
    TP_MODULE_PRINT_ID,                //touchpad
    LS_MODULE_PRINT_ID,
    GS_MODULE_PRINT_ID,
    SHL_MODULE_PRINT_ID,                //shell
    PRT_MODULE_PRINT_ID,               //print
    URT_MODULE_PRINT_ID,               //uart
    I2C_MODULE_PRINT_ID,              //i2c
    MB_MODULE_PRINT_ID,               //mailbox
    SOFTIMER_MODULE_PRINT_ID,         //softtimer
    EXC_MODULE_PRINT_ID,              //exc
    HUTAF_MODULE_PRINT_ID,            //hutaf
    PWC_MODULE_PRINT_ID,              //pwrctrl
    PMUSSI_MODULE_PRINT_ID,
    GPIO_MODULE_PRINT_ID,
    MNTN_MODULE_PRINT_ID,
    MCU_MAX_PRINT_ID
}MCU_MODULE_PRINT_ID;

typedef enum
{
    ID_MCU_DATA_EVENT = 0x0,     /* �¼��ϱ� */
    ID_MCU_DATA_TRACE,           /* ��ͨTrace */
    ID_MCU_DATA_DEBUG,           /* Ϊʵ���ڹ涨ʱ�������ٷ���һ�ε�������� */
    ID_MCU_DATA_TYPE_BUTT
} DATA_TYPE;

typedef enum
{
    /* ̧��Ƶ�� */
    ID_MCU_EVENT_DFS_UP     =   0,

    /* ����Ƶ�� */
    ID_MCU_EVENT_DFS_DOWN   =   1,

    /* �ϵ� */
    ID_MCU_EVENT_POWER_ON   =   2,

    /* �µ� */
    ID_MCU_EVENT_POWER_OFF  =   3,

    ID_MCU_EVENT_TYPE_BUTT
} EVENT_TYPE;


typedef enum
{
    /* DFS��Core0���µ���MCU���� */
    ID_MCU_ADJUST_ACPU0     =   0,

    /* DFS�����µ���MCU���� */
    ID_MCU_ADJUST_CCPU      =   1,

    /* DFS��MCU���� */
    ID_MCU_ADJUST_GPU       =   2,

    /* �µ���MCU���� */
    ID_MCU_ADJUST_HIFI      =   3,

    /* DFS��MCU���� */
    ID_MCU_ADJUST_DDR       =   4,

    ID_MCU_ADJUST_MODULE_BUTT
} MODULE_TYPE;


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
typedef struct
{
    u32              ulTimeStamp;    /* ʱ�������д��ǰ��Counterֵ */
    u16              enEventType;    /* MCU�����¼����� EVENT_TYPEö�� */
    u16              enModule;       /* MCU����ģ������ MODULE_TYPEö�� */
    u32              ulPreVAl;       /* ��DFS��Ч��ָʾ����ǰ��Ƶ�� */
    u32              ulCurVal;       /* ��DFS��Ч��ָʾ���ں�ĵ�ǰƵ�� */
} MCU_EVENT;

/* ��Ԫ���ݸ�ʽ������A�� */
typedef struct
{
    u32                 ulTimeStemp;    /* ʱ��� */
    u16                 enDataType;     /* �������� DATA_TYPEö�� */
    u16                 enPrintModule;  /* ��ӡģ�� MCU_MODULE_PRINT_IDö�� */
    u32                 ulLength;       /* �������ݵĳ��� ��λ:�ֽ� */
    u8                  data[DATA_SIZE_IN_MSG_HEAD];        /* data������usLength���� */
} MCU_TO_ACPU_MSG;


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

extern s32 mntn_init(void);
extern void mntn_deal_with_mcu_mail(void * UserHandle, void * MailHandle, u32 ulMailLen);
extern s32 mntn_deal_with_msg(u8* data, u32 enDataType, u32 ulTimeStemp);
extern void RunMcuCmd(char* cmd);
extern long aprintk(char* str, int len);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of mntn_deal_with_mcu_msg.h */
