/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : BSP_TIMER.h
  �� �� ��   : ����
  ��    ��   : ��ϲ 220237
  ��������   : 2013��3��30��
  ����޸�   :
  ��������   : BSP_TIMER.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��3��30��
    ��    ��   : ��ϲ 220237
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "soc_timer_interface.h"
#include "DrvInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define TIMER_32K_US_BOUNDARY           31
#define TIMER_DEFAULT_TICKS             100

#define TIMER_LOAD_TICKS                1

#define TIMER_CLK_RATE_MAX              10000
#define TIMER_CLK_RATE_MIN              1

#define TIMER_BIT_NUM_IN_A_DUAL_TIMER   2       /* ÿ��Dual Timer�к�Timer�ĸ��� */
#define Second_To_Millisecond           1000    /* ��ת���� */



#define CLK_REGOFF_LOAD         SOC_TIMER_TIMERN_LOAD_ADDR(0, 0)    /* Load (R/W) */
#define CLK_REGOFF_VALUE        SOC_TIMER_TIMERN_VALUE_ADDR(0, 0)   /* Value (R/O) */
#define CLK_REGOFF_CTRL         SOC_TIMER_TIMERN_CONTROL_ADDR(0, 0) /* Control (R/W) */
#define CLK_REGOFF_CLEAR        SOC_TIMER_TIMERN_INTCLR_ADDR(0, 0)  /* Clear (W/O) */
#define CLK_REGOFF_INTSTATUS    SOC_TIMER_TIMERN_RIS_ADDR(0, 0)     /* INT STATUS (R/O) */

/* TimerXRIS�ж�����λ*/
#define CLK_DEF_TC_INTENABLE    (1<<5)      /* 1�������θ��ж� */
#define CLK_DEF_TC_INTDISABLE   0           /* 0�����θ��ж� */

/* ��ʱ���ļ���ģʽλ*/
#define CLK_DEF_TC_PERIODIC     (1<<6)      /* 1������ģʽ */
#define CLK_DEF_TC_FREERUN      0           /* 0������ģʽ */

/* ��ʱ��ʹ��λ */
#define CLK_DEF_TC_ENABLE       (1<<7)      /* 1��Timerʹ�� */
#define	CLK_DEF_TC_DISABLE      0           /* 0��Timer��ֹ */

/* �û�ʱ��ʹ�� */
#define CLK_DEF_ENABLE   ( CLK_DEF_TC_PERIODIC | CLK_DEF_TC_INTENABLE | CLK_DEF_TC_ENABLE)
/* �û�ʱ��ȥʹ�� */
#define CLK_DEF_DISABLE  ( CLK_DEF_TC_PERIODIC | CLK_DEF_TC_INTDISABLE | CLK_DEF_TC_DISABLE)

#define PWRCTRL_REG_READ32(reg, result)	    (result = * ((volatile unsigned int*)(reg)))
#define PWRCTRL_REG_WRITE32(reg, data)	    (*((volatile unsigned int*)(reg)) = data)

/* �ж������궨�� */
#define INUM_TO_IVEC(intNum)    ((VOIDFUNCPTR *) (intNum))


#define INT_IRQ_TIMER0     IVEC_TO_INUM(IRQ_TIMER0)
#define INT_IRQ_TIMER1     IVEC_TO_INUM(IRQ_TIMER1)
#define INT_IRQ_TIMER2     IVEC_TO_INUM(IRQ_TIMER2)
#define INT_IRQ_TIMER3     IVEC_TO_INUM(IRQ_TIMER3)
#define INT_IRQ_TIMER10     IVEC_TO_INUM(IRQ_TIMER10)
#define INT_IRQ_TIMER11     IVEC_TO_INUM(IRQ_TIMER11)
#define INT_IRQ_TIMER12     IVEC_TO_INUM(IRQ_TIMER12)
#define INT_IRQ_TIMER13     IVEC_TO_INUM(IRQ_TIMER13)
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
typedef enum
{
    DTIMER_0    = 0,
    DTIMER_1    = 1,
    DTIMER_2    = 2,
    DTIMER_3    = 3,
    DTIMER_4    = 4,
    DTIMER_5    = 5,
    DTIMER_6    = 6,
    DTIMER_7    = 7,
    DTIMER_8    = 8,
    DTIMER_MAX  = 9
} BSP_DTIMER_ID;

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
    FUNCPTR_1  routine;     /* �жϻص����� */
    BSP_S32 arg;            /* �жϲ��� */
    BSP_U32 running;        /* ��ʱ���������еı�־ */
    BSP_U32 ticksPerSecond; /* ��Ŷ�ʱ��ÿ��tick�� */
    BSP_U32 inputClock;     /* ��ʱ������Ƶ�� */
} TIMER_PARA;

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
extern BSP_U32 TIMER_BASE(BSP_U32 u32ClkNum);
extern BSP_S32 BSP_TIMER_Init(BSP_VOID);
extern irqreturn_t BSP_TIMER_IntHandle(int irq, void* para);
extern BSP_S32 BSP_TIMER_UsrClkConnect(FUNCPTR_1 routine, BSP_S32 arg, BSP_U32 usrClkId);
extern BSP_S32 BSP_TIMER_UsrClkDisable(BSP_U32 usrClkId);
extern BSP_S32 BSP_TIMER_UsrClkEnableMs(BSP_U32 usrClkId, BSP_U32 value, BSP_U32 mode);
extern BSP_S32 BSP_TIMER_UsrClkEnableUs(BSP_U32 usrClkId, BSP_U32 value, BSP_U32 mode);
extern BSP_S32 BSP_TIMER_UsrClkEnableNone(BSP_U32 usrClkId, BSP_U32 value, BSP_U32 mode);
extern BSP_S32 BSP_TIMER_UsrClkStart(BSP_U32 usrClkId, FUNCPTR_1 routine, BSP_S32 arg, BSP_U32 timerValue, BSP_U32 mode, BSP_U32 unitType);
extern BSP_S32 BSP_TIMER_GetRestTime(BSP_U32 usrClkId, BSP_U32 unitType, BSP_U32 * pRestTime);
extern BSP_S32 BSP_TIMER_Show(BSP_VOID);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of BSP_TIMER.h */
