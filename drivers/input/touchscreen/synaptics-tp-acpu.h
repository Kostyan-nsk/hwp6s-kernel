

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef __SYNAPTICS_ACPU_H__
#define __SYNAPTICS_ACPU_H__


#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/slab.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define NULL (void*)0
#define LOG_PRINT(lev,msg) printk msg
#define __func__ __FUNCTION__
#define OK      0

#define ACPU_ID 0
#define MCU_ID  1
#define MODULE_TP 0
#define PRI_URGENT 0
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

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
	int				irq;
	unsigned int	irq_flag;      /*IRQ type*/
	unsigned int	flip_flags;		/*Need to flip x/y axis or not*/
	int				x_res;			/*LCD x axis display resolution without Virtual key area.*/
	int				y_res;			/*LCD y axis display resolution without Virtual key area.*/
	int				y_all;          /*All LCD y axis resolution include Virtual key area.*/
	unsigned int	fuzz_x;			/*Fuzz of x axis*/
	unsigned int	fuzz_y;			/*Fuzz of y axis*/
	int				fuzz_p;			/*Fuzz of pressure*/
	int				fuzz_w;			/*Fuzz of touch width*/

	int	            reset_pin;
	int             points_supported;
	int             is_support_multi_touch;
}STRU_TP_PLATFORM_DATA;

#define RMI4_NUMBER_OF_MAX_FINGERS    10

typedef struct
{
    int touch_count;
    int x[RMI4_NUMBER_OF_MAX_FINGERS];
    int y[RMI4_NUMBER_OF_MAX_FINGERS];
    int z[RMI4_NUMBER_OF_MAX_FINGERS];
    int wx[RMI4_NUMBER_OF_MAX_FINGERS];
    int wy[RMI4_NUMBER_OF_MAX_FINGERS];
}STRU_TP_DATA;


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

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of synaptics-tp-acpu.h */
