
#ifdef __cplusplus
extern "C"
{
#endif

#include <linux/module.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <asm/io.h>
//#include <mach/common/bsp_version.h>
//#include <mach/common/bsp_memory.h>

//#include <mach/BSP_OM.h>
#include "bsp_om.h"
#include "BSP.h"
#include <linux/slab.h>

#include "soc_baseaddr_interface.h"
#include "soc_sctrl_interface.h"
#include <soc_ao_sctrl_interface.h>

#if defined BSP_FE
#include "BSP_DRV_END.h"
#endif

#define logMsg printk

#define BSP_OM_CHECK_RET(ret) \
do{\
    if (OK != ret)\
    {\
        logMsg("OM Check Error, line:%d, return:%d\n", __LINE__, ret,0,0,0,0);\
        return ret;\
    }\
}while(0)

extern void systemError(int modId, int arg1, int arg2, char * arg3, int arg3Length);
extern unsigned int exchMemMalloc(UINT32 ulSize);

/*2009-06-18 wangxuesong Add for BJ9D01071 end*/

#if 0
static char* g_OM_shellTaskName = "tShell0";
static UE_SW_BUILD_VER_INFO_STRU g_BspBuildVersion =
{
    1,      // V ����
    1,      // R ����
    1,      // C ����
    0,      // B ����
    0,      // SPC ����
    0,      // �Զ���

    // ��Ʒ���ͱ�ţ�����ͬ������ϵ�Ӳ��ƽ̨
    (BSP_U32)UE_SW_BUILD_VER_PRODUCT_UNKNOWN,
    __DATE__,
    __TIME__
};

static UE_SW_BUILD_VER_INFO_STRU g_BootLoaderVersion =
{
    1,      // V ����
    1,      // R ����
    1,      // C ����
    0,      // B ����
    0,      // SPC ����
    0,      // �Զ���

    // ��Ʒ���ͱ�ţ�����ͬ������ϵ�Ӳ��ƽ̨
    (BSP_U32)UE_SW_BUILD_VER_PRODUCT_UNKNOWN,
    __DATE__,
    __TIME__
};
#endif

/* ��������������*/
const BSP_CHAR BoardMainName[8][BOARD_MAINNAME_LEN] =
{
    "H69DUSA",
    "H69DUSB",
    "H69DUSC",
    "H69DUSD",
    "RSV",
    "RSV",
    "RSV",
    "RSV"
};

/* �������������� */
const BSP_CHAR BoardSubName[4][BOARD_SUBNAME_LEN] =
{
    "VA",
    "VB",
    "VC",
    "VD"
};
/* LTEЭ��ջMAC��ַ */
BSP_U8  g_mac_address_pstable[] =
{
    //0x00,0x1a,0x2b,0x3c,0x4d,0x5f
    0x4c,0x54,0x99,0x45,0xe5,0xd5
};
/*
ֻ����RAM���
*/
BSP_S32 OM_RAMCheckVolid(BSP_U32 u32CheckedAddr)
{
    return OK;
}

BSP_S32 OM_GetVersionInfo(OM_VERSION_INFO_S *pstVersionInfo)
{
    if(!pstVersionInfo)
    {
        return ERROR;
    }

    return ERROR;
}

/*****************************************************************\
5.2.1 BSP_OM_RegRead
��������
  ���Ĵ���ֵ
��ԭ�͡�
   INT32 BSP_OM_RegRead(UINT32 ulRegAddr, ENADDRTYPE enAddrType, UINT32* pulValue)

����	����	����/���
ulRegAddr	����ѯ�ļĴ�����ַ	�������
enAddrType	��ȡ�ļĴ�������(8��16��32λ)	�������
pulValue	���صļĴ���ֵ	�������

������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��
\****************************************************************/
BSP_S32 BSP_OM_RegRead(BSP_U32 u32RegAddr,
    ENADDRTYPE enAddrType, BSP_U32 *pu32Value)
{
    BSP_S32 s32Ret;
    s32Ret = OK;
    if(OK != s32Ret)
    {
        return ERROR;
    }

    switch(enAddrType)
    {
        case ADDRTYPE8BIT:
            *pu32Value = *((volatile BSP_U8 *)u32RegAddr);
            return OK;

        case ADDRTYPE16BIT:
            *pu32Value = *((volatile BSP_U16 *)u32RegAddr);
            return OK;

        case ADDRTYPE32BIT:
            *pu32Value = *((volatile BSP_U32 *)u32RegAddr);
            return OK;

        default:
            return ERROR;
    }
}

/*****************************************************************\
5.2.1 BSP_OM_RegWrite
��������
  д�Ĵ���ֵ
��ԭ�͡�
   INT32 BSP_OM_RegWrite(UINT32 u32RegAddr, ENADDRTYPE enAddrType, UINT32 u32Value)

����	����	����/���
u32RegAddr	��д��ļĴ�����ַ	�������
enAddrType	д��ļĴ�������(8��16��32λ)	�������
u32Value	���صļĴ���ֵ	�������

������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��
\****************************************************************/
BSP_S32 BSP_OM_RegWrite(BSP_U32 u32RegAddr,
    ENADDRTYPE enAddrType, BSP_U32 u32Value)
{
    BSP_S32 s32Ret;
    s32Ret = OK;
    if(OK != s32Ret)
    {
        return ERROR;
    }

    switch(enAddrType)
    {
        case ADDRTYPE8BIT:
            *((volatile BSP_U8 *)u32RegAddr) = (BSP_U8)u32Value;
            return OK;

        case ADDRTYPE16BIT:
            *((volatile BSP_U16 *)u32RegAddr) = (BSP_U16)u32Value;
            return OK;

        case ADDRTYPE32BIT:
            *((volatile BSP_U32 *)u32RegAddr) = u32Value;
            return OK;

        default:
            return ERROR;
    }
}


/*****************************************************************\
5.2.2 BSP_OM_MemRead
��������
��ȡָ�����ڴ�ռ�
��ԭ�͡�
  INT32 BSP_OM_MemRead(UINT32 ulMemAddr, ENADDRTYPE enAddrType, UINT32* pulValue)
����	����	����/���
ulMemAddr	����ѯ���ڴ��ַ	�������
enAddrType	��ȡ����������(8��16��32λ)	�������
pulValue	���ص�����	�������

������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��
\****************************************************************/
BSP_S32 BSP_OM_MemRead(BSP_U32 u32MemAddr, ENADDRTYPE enAddrType, BSP_U32 *pu32Value)
{
    BSP_S32 s32Ret;
    s32Ret = OM_RAMCheckVolid(u32MemAddr);
    if(OK != s32Ret)
    {
        return ERROR;
    }

    switch(enAddrType)
    {
        case ADDRTYPE8BIT:
            *pu32Value = *((volatile BSP_U8 *)u32MemAddr);
            return OK;

        case ADDRTYPE16BIT:
            *pu32Value = *((volatile BSP_U16 *)u32MemAddr);
            return OK;

        case ADDRTYPE32BIT:
            *pu32Value = *((volatile BSP_U32 *)u32MemAddr);
            return OK;

        default:
            return ERROR;
    }
}

/*****************************************************************\
5.2.3 BSP_OM_MemWrite
��������
  ��ָ���ĵ�ַ��д������
��ԭ�͡�
   INT32 BSP_OM_MemWrite(UINT32 ulMemAddr, ENADDRTYPE enAddrType, UINT32 ulValue)

����	����	����/���
ulMemAddr	��д������ݵ�ַ	�������
enAddrType	д�����������(8��16��32λ)	�������
Value	��д�������	�������

������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��
\****************************************************************/
BSP_S32 BSP_OM_MemWrite(BSP_U32 u32MemAddr,
    ENADDRTYPE enAddrType, BSP_U32 u32Value)
{
    BSP_S32 s32Ret;
    s32Ret = OM_RAMCheckVolid(u32MemAddr);
    if(OK != s32Ret)
    {
        return ERROR;
    }

    switch(enAddrType)
    {
        case ADDRTYPE8BIT:
            *((volatile BSP_U8 *)u32MemAddr) = (BSP_U8)u32Value;
            return OK;

        case ADDRTYPE16BIT:
            *((volatile BSP_U16 *)u32MemAddr) = (BSP_U16)u32Value;
            return OK;

        case ADDRTYPE32BIT:
            *((volatile BSP_U32 *)u32MemAddr) = u32Value;
            return OK;

        default:
            return ERROR;
    }
}

/*****************************************************************\
5.2.4 BSP_OM_MemNWrite
��������
  ��ָ���ĵ�ַ�ϰ�32λ����д������
��ԭ�͡�
   INT32 BSP_OM_MemNWrite(UINT32 ulMemAddr, UINT32 ulValue,UINT32 ulCount)

����	����	����/���
ulMemAddr	��д������ݵ�ַ	�������
Value	��д�������	�������
ulCount	��д������ݸ���	�������

������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��

\****************************************************************/
BSP_S32 BSP_OM_MemNWrite(BSP_U32 u32MemAddr,
    BSP_U32 u32Value, BSP_U32 u32Count)
{
    BSP_U32 inum;
    BSP_S32 s32Ret1,s32Ret2;
    BSP_U32 *pu32Addr = (BSP_U32 *)u32MemAddr;

    s32Ret1 = OM_RAMCheckVolid(u32MemAddr);
    s32Ret2 = OM_RAMCheckVolid(u32MemAddr + u32Count);

    if((OK != s32Ret1) || (OK != s32Ret2))
    {
        return ERROR;
    }

    for(inum=0; inum<u32Count; inum++)
    {
        *pu32Addr = u32Value;
        pu32Addr++;
    }

    return OK;
}


/*****************************************************************\
5.2.6 BSP_OM_MemChk
��������
  RAM���
��ԭ�͡�
   Int BSP_OM_MemChk(UINT32 ulStartAddr, UINT32 ulEndAddr, ENMEMCHKTYPE enChkType)

����	����	����/���
ulStartAddr	������RAM��ʼ��ַ	�������
ulEndAddr	������RAM������ַ	�������
enChkType	RAM�������(16��32λ)	�������

������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��
\****************************************************************/
BSP_S32 BSP_OM_MemChk(BSP_U32 u32StartAddr,
            BSP_U32 u32EndAddr, ENMEMCHKTYPE enChkType)
{
    BSP_U16 *pu16Addr = BSP_NULL;
    BSP_U32 *pu32Addr = BSP_NULL;
    volatile BSP_U32 u32Temp = 0;
    volatile BSP_U16 u16Temp = 0;
    BSP_U32 inum;
    if(OK != OM_RAMCheckVolid(u32StartAddr))
    {
        return ERROR;
    }

    if(OK != OM_RAMCheckVolid(u32EndAddr))
    {
        return ERROR;
    }

    if(enChkType == MEMCHKTYPE32BIT)
    {
        pu16Addr = (BSP_U16 *)u32StartAddr;
        for(inum=0; inum<((u32EndAddr-u32StartAddr)/2); inum++)
        {
            u16Temp = *pu16Addr; //ȡ������
            *pu16Addr = 0;      //����0
            if(*pu16Addr != 0)
            {
                *pu16Addr = u16Temp;
                return ERROR;
            }
            *pu16Addr = 0xFFFF;  //����0xFFFFFFFF
            if(*pu16Addr != 0xFFFF)
            {
                *pu16Addr = u16Temp;
                return ERROR;
            }
            *pu16Addr = u16Temp;
            pu16Addr++;
        }
        return OK;
    }
    else if(enChkType == MEMCHKTYPE16BIT)
    {
        pu32Addr = (BSP_U32 *)u32StartAddr;
        for(inum=0; inum<((u32EndAddr-u32StartAddr)/4); inum++)
        {
            u32Temp = *pu32Addr; //ȡ������
            *pu32Addr = 0;      //����0
            if(*pu32Addr != 0)
            {
                *pu32Addr = u32Temp;
                return ERROR;
            }
            *pu32Addr = 0xFFFFFFFF;  //����0xFFFFFFFF
            if(*pu32Addr != 0xFFFFFFFF)
            {
                *pu32Addr = u32Temp;
                return ERROR;
            }
            *pu32Addr = u32Temp;
            pu32Addr++;
        }
        return OK;
    }
    else
        return ERROR;
}

/*****************************************************************\
5.2.7 BSP_OM_GetVerTime
��������
  ��ð汾��������ʱ��

��ԭ�͡�
   void BSP_OM_GetVerTime(char* strVerTime)

����	����	����/���
strVerTime	�汾����ʱ���ַ�����ַ�������buffer������64�ֽ�	�������

������ֵ��
��

\****************************************************************/
BSP_VOID BSP_OM_GetVerTime(BSP_S8* strVerTime)
{
    return;
}

/*****************************************************************\
5.2.8 BSP_OM_SoftReboot
��������
 ���帴λ
��ԭ�͡�
   void BSP_OM_SoftReboot()
������ֵ��
��

\****************************************************************/
BSP_VOID BSP_OM_GetVersion()
{
    return;
}

/*****************************************************************\
5.2.8 BSP_OM_SoftReboot
��������
 ���帴λ
��ԭ�͡�
   void BSP_OM_SoftReboot()
������ֵ��
��

\****************************************************************/
BSP_VOID BSP_OM_SoftReboot()
{
    //BSP_WDT_reboot();
    return;
}

/*****************************************************************\
5.2.9 BSP_OM_Shutdown
��������
 ����ر�
��ԭ�͡�
   void BSP_OM_ShutDown()
������ֵ��
��

\****************************************************************/

BSP_VOID BSP_OM_ShutDown(void)
{
}

/*****************************************************************\
5.2.10 BSP_OM_SetBootFlag
��������
 ����BootLoader������־
��ԭ�͡�
   INT32 BSP_OM_SetBootFlag(ENBOOTFLAG enBootFlag)
����	����	����/���
ulFlag	BOOT_FAST ��������
BOOT_SLOW ��������	�������

������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��

\****************************************************************/
/*BSP_S32 BSP_OM_SetBootFlag(ENBOOTFLAG enBootFlag)
{
    if((enBootFlag == BOOT_FAST) || (enBootFlag == BOOT_SLOW))
    {
        g_enBootFlag = enBootFlag;
        return OK;
    }
    return ERROR;
}
*/
/*****************************************************************\
5.2.11 BSP_OM_ComVersionCtrl
��������
 ����汾��д�ӿ�
��ԭ�͡�
INT32 BSP_OM_ComVersionCtrl(INT32* pulValue, UINT32 ulLen,
    UINT32 ulType,UINT32 ulMode)

����	����	����/���
pulValue	��ģʽʱ��Ϊ�����ߴ����淵�ص�����汾��Ϣ���ڴ��ַ��
дģʽʱ��Ϊ������׼��д�������汾��Ϣ�ĵ�ַ��	����/�������
ulLen	��ģʽʱ��Ϊ�����ߴ����淵�ص�����汾��Ϣ���ڴ��С��
дģʽʱ��Ϊ������׼��д�������汾��Ϣ���ַ�����������"\0"ʱ���30���ַ���	�������
ulType	�汾��ϢID
ulMode	0����ȡָ��ID������汾��Ϣ��
1��д��ָ��ID������汾��Ϣ��	�������

������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��

\****************************************************************/
/*BSP_S32 BSP_OM_ComVersionCtrl(BSP_S32 *ps32Value,
    BSP_U32 u32Len, BSP_U32 u32Type, BSP_U32 u32Mode)
{
    return ERROR;
}*/

/*****************************************************************\
5.2.12 BSP_OM_ComVersionQuery
��������
��ѯ��������İ汾�š�
��ԭ�͡�
INT32 BSP_OM_ComVersionQuer(void ** ppVersionInfo,UINT32 * ulLen)
����	����	����/���
ppVersionInfo	������İ汾��Ϣ��ַ	�������
ulLen	���������ݵ��ֽ�����	�������
������ֵ��
����ֵ	����
OK	����ɹ�
ERROR	����ʧ��

\****************************************************************/
/*BSP_S32 BSP_OM_ComVersionQuery(BSP_VOID **ppVersionInfo,
    BSP_U32 u32Len)
{
    return ERROR;
}*/

/*****************************************************************\
5.2.13 BSP_OM_GetPaPower
��������
��ȡPA���书�ʡ�
��ԭ�͡�
int BSP_OM_GetPaPower(INT32 *pulData, INT32 *pulTable)
����	����	����/���
pulData	��ŷ��书�ʵ����ֵ��	�������
pulTable	��ѹ����ת����	�������

������ֵ��
����ֵ	����
OK	����ɹ���
ERROR	����ʧ�ܡ�

\****************************************************************/
/*BSP_S32 BSP_OM_GetPaPower(BSP_S32 *pu32Data, BSP_U32 *ps32Table)
{
    return ERROR;
}
*/
/*****************************************************************\
5.2.14 BSP_OM_GetPaTemperature
��������
��ȡPA��ǰ�¶ȡ�
��ԭ�͡�
int BSP_OM_GetPaTemperature (INT32 *pulData, INT32 *pulTable)
����	����	����/���
pulData	��ŷ��书�ʵ����ֵ��	�������
pulTable	��ѹ�¶�ת����	�������

������ֵ��
����ֵ	����
OK	����ɹ���
ERROR	����ʧ�ܡ�

\****************************************************************/
/*BSP_S32 BSP_OM_GetPaTemperature(BSP_S32 *ps32Data, BSP_S32 *ps32Table)
{
    return ERROR;
}*/

/*2009-09-17 wangxuesong added for BJ9D01877 begin*/
/*
*������: BSP_OM_GetFPGAVer
*
*����:  ��
*
*���:  ��
*/
BSP_S32 BSP_OM_GetFPGAVer()
{
    return OK;
}
/*2009-09-17 wangxuesong added for BJ9D01877 end*/

BSP_S32 BSP_OM_ShellLock(BSP_BOOL bRequest)
{
    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_OM_GetFlashSpec
*
* ��������  : ��ȡFLASH���
*
* �������  : ��
*
* �������  : BSP_U32 *pu32FlashTotalSize  : FLASH�ܴ�С
*             BSP_U32 *pu32FlashBlkSize     : FLASH���С
*             BSP_U32 *pu32FlashPageSize    : FLASHҳ��С
*
* �� �� ֵ  : BSP_ERROR:��ȡʧ��
*             BSP_OK:��ȡ�ɹ�
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_S32 BSP_OM_GetFlashSpec(BSP_U32 *pu32FlashTotalSize, BSP_U32 *pu32FlashBlkSize, BSP_U32 *pu32FlashPageSize)
{
    if(!pu32FlashTotalSize)
    {
        logMsg("null pointer(pu32FlashTotalSize)\n");
        return BSP_ERROR;
    }

    if(!pu32FlashBlkSize)
    {
        logMsg("null pointer(pu32FlashBlkSize)\n");
        return BSP_ERROR;
    }

    if(!pu32FlashPageSize)
    {
        logMsg("null pointer(pu32FlashPageSize)\n");
        return BSP_ERROR;
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_OM_NET
*
* ��������  : ���ڿ�ά�ɲ�ӿ�
*
* �������  : BSP_OM_NET_S *pstNetOm
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
BSP_VOID BSP_OM_NET(BSP_OM_NET_S *pstNetOm)
{

}/*lint !e715*/

/*****************************************************************************
* �� �� ��  : BSP_InitBootVersion
*
* ��������  : ��ʼ��BOOT�汾ȫ�ֱ���
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_VOID BSP_InitBootVersion(VOID)
{
    return;
}

/*****************************************************************************
* �� �� ��  : BSP_AtoX
*
* ��������  : ���ַ���ת��Ϊ����(16����)
*
* �������  : pStr:��ת���ַ���
*
* �������  : ת���������
*
* �� �� ֵ  : ��
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_U32 BSP_AtoX(BSP_U8* pStr)
{
	BSP_U8* p = pStr;
    BSP_U32 n;

	n = 0;
	for(;;p++)
    {
		switch(*p)
        {
		case ' ':
		case '\t':
		case '0':
        case 'x':
        case 'X':
			p++;
            continue;
        default:
            break;
		}
		break;
	}

	while( (*p >= '0' && *p <= '9') ||
           (*p >= 'a' && *p <= 'f') ||
           (*p >= 'A' && *p <= 'F') )
    {
        if (*p >= '0' && *p <= '9')
        {
            n = n*16 + *p++ - '0';
        }
        else if (*p >= 'a' && *p <= 'f')
        {
            n = n*16 + (*p++ - 'a' + 10);
        }
        else /* A ~ F */
        {
            n = n*16 + (*p++ - 'A' + 10);
        }
    }
	return (n);
}


/*****************************************************************************
* �� �� ��  : BSP_InitBspVersion
*
* ��������  : ��ʼ��BSP�汾ȫ�ֱ���
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_S32 BSP_InitBspVersion(VOID)
{
    return 0;
}

/*****************************************************************************
* �� �� ��  : BSP_GetBootBuildVersion
*
* ��������  : ��ȡBoot�汾
*
* �������  : ��
*
* �������  : BOOT�汾�ṹ��ָ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ��
*
*****************************************************************************/
const UE_SW_BUILD_VER_INFO_STRU* BSP_GetBootBuildVersion(VOID)
{
    return NULL;
}

/*****************************************************************************
* �� �� ��  : BSP_GetBuildVersion
*
* ��������  : ��ȡBSP�汾
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : BSP�汾�ṹ��ָ��
*
* ����˵��  : ��
*
*****************************************************************************/
const UE_SW_BUILD_VER_INFO_STRU* BSP_GetBuildVersion(VOID)
{
    return NULL;
}

/*****************************************************************************
* �� �� ��  : BSP_GetBuildVersion
*
* ��������  : ��ȡBSP�汾
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : BSP�汾�ṹ��ָ��
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_S32 BSP_OM_GetHwVersion(BSP_VOID)
{
    return OK;
}


/*****************************************************************************
* �� �� ��  : BSP_OM_GetBoardType
*
* ��������  : ��ȡ��������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : BSP��������ö��
*
* ����˵��  : ��
*
*****************************************************************************/
BOARD_TYPE_E BSP_OM_GetBoardType()
{
    return BOARD_TYPE_GUL;
}


/*****************************************************************************
* �� �� ��  : BBPGetCurTime
*
* ��������  : ��PS���ã�������ȡϵͳ��ȷʱ��
*
* �������  : ��
* �������  : BSP_U32 u32CurTime:��ǰʱ��
*
* �� �� ֵ  : BSP_OK : ��ȡ�ɹ�
*                        BSP_ERR_INVALID_PARA  :��ȡʧ��
*
* �޸ļ�¼  : 2011��3��7��  wangjing  creat
*****************************************************************************/
BSP_U32 BBPGetCurTime(BSP_U64 *pCurTime)
{
    BSP_U64 u64TempTime[4];
    unsigned long sctrl_addr = (unsigned long)IO_ADDRESS(REG_BASE_SC_ON);

    if (NULL == pCurTime)
    {
        printk("BSP_BBPGetCurTime Wrong Para. line:%d\n", __LINE__);
        return BSP_ERR_INVALID_PARA;
    }

    u64TempTime[0] = readl(SOC_AO_SCTRL_SC_ALWAYSON_SYS_STAT6_ADDR(sctrl_addr));
    u64TempTime[1] = readl(SOC_AO_SCTRL_SC_ALWAYSON_SYS_STAT7_ADDR(sctrl_addr));
    u64TempTime[2] = readl(SOC_AO_SCTRL_SC_ALWAYSON_SYS_STAT6_ADDR(sctrl_addr));
    u64TempTime[3] = readl(SOC_AO_SCTRL_SC_ALWAYSON_SYS_STAT7_ADDR(sctrl_addr));

    if (u64TempTime[2] < u64TempTime[0])
    {
        *pCurTime = ((u64TempTime[3] - 1) << 32) | u64TempTime[0];
    }
    else
    {
        *pCurTime = (u64TempTime[1] << 32) | u64TempTime[0];
    }

    return 0;
}
#if 0
/*****************************************************************************
 �� �� ��  : DRV_GET_BBP_TIMER_VALUE
 ��������  : ��ȡBBP��ʱ����ֵ������OAM ʱ��
 �������  : �ޡ�
 �������  : pulHigh32bitValue:BBP��ʱ����ֵ��ȡ��32bit,��������Ϊ�գ�
                                                      ��ֻ���ص�32bit��ֵ
                            pulLow32bitValue:BBP��ʱ����ֵ��ȡ�� 32bit,����Ϊ�գ�����᷵��ʧ�ܡ�
 �� �� ֵ  : 0-�ɹ�
                       -1 -ʧ��

*****************************************************************************/
 int DRV_GET_BBP_TIMER_VALUE(unsigned int  *pulHigh32bitValue,  unsigned int  *pulLow32bitValue)
 {
    BSP_U64 u64TempTime[4];
    unsigned long sctrl_addr = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);

    if (NULL == pulLow32bitValue)
    {
        printk("DRV_GET_BBP_TIMER_VALUE Wrong Para. line:%d\n", __LINE__);
        return BSP_ERR_INVALID_PARA;
    }

    u64TempTime[0] = readl(SOC_SCTRL_SC_BBPABB_SUBSYS_STAT3_ADDR(sctrl_addr));
    u64TempTime[1] = readl(SOC_SCTRL_SC_BBPABB_SUBSYS_STAT4_ADDR(sctrl_addr));
    u64TempTime[2] = readl(SOC_SCTRL_SC_BBPABB_SUBSYS_STAT3_ADDR(sctrl_addr));
    u64TempTime[3] = readl(SOC_SCTRL_SC_BBPABB_SUBSYS_STAT4_ADDR(sctrl_addr));

    if (u64TempTime[2] < u64TempTime[0])
    {
        if (pulHigh32bitValue != NULL) {
            *pulHigh32bitValue = (u64TempTime[3] - 1);
        }

        *pulLow32bitValue = u64TempTime[0];
    }
    else
    {
        if (pulHigh32bitValue != NULL) {
            *pulHigh32bitValue = u64TempTime[1];
        }

        *pulLow32bitValue = u64TempTime[0];
    }

    return 0;
 }
#endif

unsigned int BSP_MNTN_ExchMemMalloc(unsigned int ulSize)
{
    return exchMemMalloc(ulSize);
}

void BSP_MNTN_SystemError(int modId, int arg1, int arg2, char * arg3, int arg3Length)
{
    systemError( modId, arg1, arg2, arg3, arg3Length);
}
/*****************************************************************************
* �� �� ��  : BSP_GetMacAddr
*
* ��������  : ��ȡps mac��ַ
*
* �������  :
* �������  : ��
*
* �� �� ֵ  : VOID
*
* �޸ļ�¼  : 2009��9��2��   liumengcun  creat
*****************************************************************************/
UINT8* BSP_GetMacAddr()
{
    return g_mac_address_pstable;
}

/*****************************************************************************
* �� �� ��  : BSP_SetPsMacAddr
*
* ��������  : �޸�PS mac��ַ
*
* �������  :
* �������  : ��
*
* �� �� ֵ  : VOID
*
* �޸ļ�¼  : 2009��9��2��   liumengcun  creat
*****************************************************************************/
BSP_S32 BSP_SetPsMacAddr(BSP_U8 u8MacAddr0,BSP_U8 u8MacAddr1,BSP_U8 u8MacAddr2,
		BSP_U8 u8MacAddr3,BSP_U8 u8MacAddr4,BSP_U8 u8MacAddr5)
{
	g_mac_address_pstable[0] = u8MacAddr0;
    g_mac_address_pstable[1] = u8MacAddr1;
    g_mac_address_pstable[2] = u8MacAddr2;
    g_mac_address_pstable[3] = u8MacAddr3;
    g_mac_address_pstable[4] = u8MacAddr4;
    g_mac_address_pstable[5] = u8MacAddr5;

	return OK;
}

#if 0
const UE_SW_BUILD_VER_INFO_STRU* BSP_GetBootBuildVersion(VOID)
{
    return NULL;
}
#endif


unsigned int   * MemAddr32 = 0x00000000;

/*******************************************************************************
*
* writeM - write a unsigned int value to perihical address
*
*
* RETURNS: N/A
*/
void writeM(unsigned int pAddr, unsigned int value)
{
    *(volatile unsigned int *)(pAddr)=value;
}


/***********************************************************
 Function: memRead32--read memory and display the value
 Input:    the start address
 return:  void
 see also:memRead16/memRead8
 History:
 1.    2004.9.30   Creat
 2.    2007.3.20   Code stardand
************************************************************/
void memRead32 (unsigned int * uiMemAddr32)
{
    if (0 != uiMemAddr32)
    {
         MemAddr32 = (unsigned int *)((unsigned int)uiMemAddr32 & 0xFFFFFFFC);
    }

    printk("0x%.8X: 0x%.8X   0x%.8X   0x%.8X   0x%.8X\n",
            (unsigned int)(MemAddr32+0x0), *(MemAddr32+0x0), *(MemAddr32+0x1), *(MemAddr32+0x2), *(MemAddr32+0x3));
    printk("0x%.8X: 0x%.8X   0x%.8X   0x%.8X   0x%.8X\n",
            (unsigned int)(MemAddr32+0x4), *(MemAddr32+0x4), *(MemAddr32+0x5), *(MemAddr32+0x6), *(MemAddr32+0x7));
    printk("0x%.8X: 0x%.8X   0x%.8X   0x%.8X   0x%.8X\n",
            (unsigned int)(MemAddr32+0x8), *(MemAddr32+0x8), *(MemAddr32+0x9), *(MemAddr32+0xa), *(MemAddr32+0xb));
    printk("0x%.8X: 0x%.8X   0x%.8X   0x%.8X   0x%.8X\n",
            (unsigned int)(MemAddr32+0xc), *(MemAddr32+0xc), *(MemAddr32+0xd), *(MemAddr32+0xe), *(MemAddr32+0xf));
    MemAddr32 += 0x10;
}

/***********************************************************
 Function: r--simple read memory command
 Input:    the start address
 return:  void
 see also:memRead16/memRead8/memRead32
 History:
 1.    2004.9.30   Creat
 2.    2007.3.20   Code stardand
************************************************************/
void r(unsigned int * newMemAddr32)
{
    memRead32(newMemAddr32);
}

extern int get_efuse_chipid_value(unsigned char * pu8Buffer, unsigned int u32Length, int timeout);
extern int get_efuse_dieid_value(unsigned char * pu8Buffer, unsigned int u32Length, int timeout);


int DRV_GET_DIEID(unsigned char* buf,int lenth)
{
#ifdef AT_DBG
	int index = 0;
#endif
	int tmplen = 0;
	
    if(NULL == buf)
    {
        printk("the chip buf is null\n");
        return BUF_ERROR;
    }
	
    if( 0 >= lenth )
    {
        printk("the lenth is litter :%d\n",lenth);
        return LEN_ERROR;
    }

    tmplen = (lenth > DIEID_BYTE_LEN)?DIEID_BYTE_LEN:lenth;

    if (0 != get_efuse_dieid_value((unsigned char*)buf,tmplen,1000))
    {
        printk("read_efuse dieid is error\n");
        return READ_EFUSE_ERROR;
    }

#ifdef AT_DBG	
    printk(KERN_DEBUG "\n==========in DRV_GET_DIEID============\n");
	for(; index < tmplen; index++)
	{
		printk(KERN_DEBUG "%#x ",buf[index]);
	}
    printk(KERN_DEBUG "\n==========in DRV_GET_DIEID============\n");
#endif
    return OK;
}


int DRV_GET_CHIPID(unsigned char* buf,int lenth)
{
#ifdef AT_DBG
	int index = 0;
#endif
	int tmplen = 0;
	
    if(NULL == buf)
    {
        printk("the die buf is null\n");
        return BUF_ERROR;
    }

    if( 0 >= lenth )
    {
        printk("the lenth is litter :%d\n",lenth);
        return LEN_ERROR;
    }

    tmplen = (lenth > CHIPID_BYTE_LEN)?CHIPID_BYTE_LEN:lenth;

    if (0 != get_efuse_chipid_value((unsigned char*)buf,tmplen,1000))
    {
        printk("read_efuse chipid is error\n");
        return READ_EFUSE_ERROR;
    }

#ifdef AT_DBG		
    printk(KERN_DEBUG "\n==========in DRV_GET_CHIPID============\n");

	for(; index < tmplen; index++)
	{
		printk(KERN_DEBUG "%#x ",buf[index]);
	}
    printk(KERN_DEBUG "\n==========in DRV_GET_CHIPID============\n");
#endif
    return OK;
}



void Test_DIEID(void)
{
	unsigned char buf[20] = {0};
	int index;
	int result;
    printk("Test_DIEID\n");
	
	result = DRV_GET_DIEID(buf, DIEID_BYTE_LEN);
	
    printk("\n==========CALL DRV_GET_DIEID============\n");
	if(OK == result)
    {
		for(index = 0; index < DIEID_BYTE_LEN; index++)
		{
			printk("%#x ", buf[index]);
		}		
	}
	printk("\n==========CALL DRV_GET_DIEID============\n");
	return ;
}


void Test_CHIPID(void)
{
	unsigned char buf[8] = {0};
	int index;
	int result;
    printk("Test_CHIPID\n");
	
	result = DRV_GET_CHIPID(buf, CHIPID_BYTE_LEN);
	
    printk("\n==========CALL DRV_GET_CHIPID============\n");
	if(OK == result)
	{
		for(index = 0; index < CHIPID_BYTE_LEN; index++)
		{
			printk("%#x ", buf[index]);
		}		
	}
	printk("\n==========CALL DRV_GET_CHIPID============\n");
	return ;
}


EXPORT_SYMBOL(Test_CHIPID);
EXPORT_SYMBOL(Test_DIEID);

EXPORT_SYMBOL(DRV_GET_DIEID);
EXPORT_SYMBOL(DRV_GET_CHIPID);
EXPORT_SYMBOL(BSP_GetBootBuildVersion);
EXPORT_SYMBOL(BSP_OM_RegRead);
EXPORT_SYMBOL(BBPGetCurTime);
EXPORT_SYMBOL(BSP_OM_ShellLock);
EXPORT_SYMBOL(BSP_OM_GetBoardType);
EXPORT_SYMBOL(BSP_GetBuildVersion);
EXPORT_SYMBOL(BSP_OM_RegWrite);
EXPORT_SYMBOL(BSP_OM_SoftReboot);
EXPORT_SYMBOL(BSP_MNTN_ExchMemMalloc);
EXPORT_SYMBOL(BSP_MNTN_SystemError);
EXPORT_SYMBOL(BSP_GetMacAddr);
EXPORT_SYMBOL(BSP_SetPsMacAddr);
#ifdef __cplusplus
}
#endif

