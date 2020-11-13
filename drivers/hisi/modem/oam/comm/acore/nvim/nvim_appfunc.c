#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "nvim_internalfunc.h"
#include "DrvInterface.h"
#include "NvIdList.h"

#if (VOS_OS_VER == VOS_LINUX)
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kallsyms.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#endif

/* Macro of log */
/*lint -e767 �޸���:������ 00105005;������:���� 51137;ԭ��:Log��ӡ */
#define    THIS_FILE_ID        PS_FILE_ID_NVIM_AFUNC_C
/*lint +e767 �޸���:������ 00105005;������:���� 51137; */

/*NV Memory ADDR*/
VOS_UINT32                      g_ulNVMemAddr       = 0;

/* ���NV���ݵ��ڴ�� */
NV_CONTROL_FILE_INFO_STRU       *g_pstNVDataBuf = VOS_NULL_PTR;

/* NV's ID coms from 1 to 20 */
NV_FILE_HANDLE_STRU             g_astNvFileHandle[NV_BACKUP_FILE_MAX_NUM+1];

VOS_UINT32                      g_ulNVInitStartSlice = 0;

VOS_UINT32                      g_ulNVInitEndSlice   = 0;

VOS_UINT32                      g_ulNVInitSem        = 0;

/* NV д�����Ƿ���Ҫ��¼slice��� */
extern  VOS_UINT32              g_ulRecordNvFlag;

/* ����NVд�����Ļ����ź��� */
VOS_UINT32                      g_ulNVWriteSem;

#if( (VOS_WIN32 == VOS_OS_VER) ||  (defined __PS_WIN32_RECUR__) )

VOS_CHAR *g_aucNvFolderPath = ".\\NvimUse\\"; /* NV File path */
/*FlashLess�ļ��ӿڶ���*/
const VOS_CHAR g_aucNvFilePath[]   = "Nvim.bin";  /* NV Ctrl File */

#else

#if (FEATURE_ON == FEATURE_MULTI_FS_PARTITION) /* SFT board*/

/* Absolute Path for Operating NV file */
const VOS_CHAR g_aucNvFolderPath[]   = "/mnvm2:0/NvimUse/"; /* NV File path */

/*FlashLess�ļ��ӿڶ���*/ /* V9R1 SFT���ļ�ϵͳ�������flashbess����ʱû�ж����·�� */
const VOS_CHAR g_aucNvFilePath[]     = "/yaffs0/Nvim/Nvim.bin";  /* NV Ctrl File */

#else /* BBIT board*/

/* Absolute Path for Operating NV file */
const VOS_CHAR g_aucNvFolderPath[]   = "/yaffs0/NvimUse/"; /* NV File path */

/*FlashLess�ļ��ӿڶ���*/
const VOS_CHAR g_aucNvFilePath[]     = "/yaffs0/Nvim/Nvim.bin";  /* NV Ctrl File */

#endif

#endif

#if (VOS_LINUX == VOS_OS_VER)

/* A C ��ͬ���ȴ��ź��� */
struct semaphore               g_stCoreNvSem;

/* NV д�����ź��� */
struct semaphore               g_stNvWriteSem;
VOS_UINT32 _VOS_SmMCreate(VOS_CHAR   acSmName[4],
                          VOS_UINT32 ulSmInit,
                          VOS_UINT32 ulFlags,
                          VOS_UINT32 *pulSmID)
{
    sema_init(&g_stNvWriteSem, 1);

    *pulSmID = (VOS_UINT32)(&g_stNvWriteSem);

    return VOS_OK;
}


VOS_UINT32 _VOS_SmCCreate(VOS_CHAR   acSmName[4],
                          VOS_UINT32 ulSmInit,
                          VOS_UINT32 ulFlags,
                          VOS_UINT32 *pulSmID)
{
    sema_init(&g_stCoreNvSem, 0);

    *pulSmID = (VOS_UINT32)(&g_stCoreNvSem);

    return VOS_OK;
}


VOS_UINT32 _VOS_SmP(VOS_UINT32 ulSemId, VOS_UINT32 ulTimeOutInMillSec)
{
    down((struct semaphore *)ulSemId);

    return VOS_OK;
}


VOS_UINT32 _VOS_SmV(VOS_UINT32 ulSemId)
{
    up((struct semaphore *)ulSemId);

    return VOS_OK;
}
#endif


VOS_VOID NV_ACoreInitIpcIsr(VOS_VOID)
{
    DRV_IPC_INTDISABLE((IPC_INT_LEV_E)IPC_ACPU_INT_SRC_CCPU_NVIM);

    /*�յ�C�˳�ʼ������жϣ��ͷ��ź���*/
    VOS_SmV(g_ulNVInitSem);

    DRV_IPC_INTENABLE((IPC_INT_LEV_E)IPC_ACPU_INT_SRC_CCPU_NVIM);
}
VOS_VOID NV_ACoreInitSync(VOS_VOID)
{
#if (VOS_LINUX == VOS_OS_VER)
    /* �����ź��� */
    VOS_SmCCreate("NVOK", 0, 0, &g_ulNVInitSem);
#endif
    /* ע���C���ж� */
    DRV_IPC_INTCONNECT((IPC_INT_LEV_E)IPC_ACPU_INT_SRC_CCPU_NVIM, (VOIDFUNCPTR)NV_ACoreInitIpcIsr, 0);

    DRV_IPC_INTENABLE((IPC_INT_LEV_E)IPC_ACPU_INT_SRC_CCPU_NVIM);
}


VOS_UINT32 NV_Init(VOS_VOID)
{
    VOS_UINT8           aucMagicNum[NV_MAGIC_NUM_LEN];
    VOS_UINT8           aucTemp[NV_MAGIC_NUM_LEN];
    BSP_DDR_SECT_INFO   stNVMemInfo;
    BSP_DDR_SECT_QUERY  stNVMemQuery;

    g_ulNVInitStartSlice = DRV_GET_SLICE();

    VOS_MemSet(g_astNvFileHandle, 0, sizeof(g_astNvFileHandle));

    vos_printf("\r\n---------------------NV_Init Start-----------------------------\r\n");

    VOS_MemSet(aucMagicNum, NV_MAGIC_NUM, NV_MAGIC_NUM_LEN);

    stNVMemQuery.enSectType = BSP_DDR_SECT_TYPE_NV;
    stNVMemQuery.ulSectSize = NV_BACKUP_BUFFER_SIZE;
    stNVMemQuery.enSectAttr = BSP_DDR_SECT_ATTR_NONCACHEABLE;

    if(BSP_OK != DRV_GET_FIX_DDR_ADDR(&stNVMemQuery, &stNVMemInfo))
    {
        vos_printf("\r\nNV_Init: Acpu DRV_GET_FIX_DDR_ADDR Failed\r\n");

        DRV_SYSTEM_ERROR(NV_INIT_ERROR, NV_GET_DRV_FIX_DDR_ADDR_FAIL,
                            0, (VOS_CHAR *)VOS_NULL_PTR, 0);

        return NV_INIT_FAIL;
    }

    g_ulNVMemAddr = stNVMemInfo.ulSectVirtAddr;     /*�ϲ�ʹ�����ַ*/

    /* ����NVд���������ź��� */
    if(VOS_OK != VOS_SmMCreate("NV", VOS_SEMA4_PRIOR | VOS_SEMA4_INVERSION_SAFE,
                                &g_ulNVWriteSem))
    {
        return VOS_ERR;
    }

    if(BSP_OK != DRV_IPC_SEMCREATE(IPC_SEM_NVIM))
    {
        NV_Printf("NV_Init: Acpu DRV_IPC_SEMCREATE Failed\r\n");

        return NV_INIT_FAIL;
    }

    /* �ȴ�C ��NV��ʼ����� */
    (VOS_VOID)VOS_SmP(g_ulNVInitSem, 0);

    NV_MEMCPY(aucTemp, (VOS_VOID*)NV_MAGICNUM_ADDR, NV_MAGIC_NUM_LEN);  /*��share memory��ȡ��־�ֽ�*/

    if(VOS_OK != VOS_MemCmp(aucMagicNum, aucTemp, NV_MAGIC_NUM_LEN))    /*Ccpu��Nv���Ѿ���ʼ���ɹ�*/
    {
        /* ���帴λ */
        DRV_SYSTEM_ERROR(NV_INIT_ERROR, NV_GET_CCORE_MAGIC_NUM_FAIL,
                            0, (VOS_CHAR *)VOS_NULL_PTR, 0);
    }

    NV_BuildGlobalVar();

    vos_printf("\r\n---------------------NV_Init End-----------------------------\r\n");

    g_ulNVInitEndSlice = DRV_GET_SLICE();

    DRV_MSP_PROC_REG(OM_READ_NV_PROC, (BSP_MspProc)NV_ReadEx);

    /* ����Ƿ���NVд������¼���� */
    if (NV_OK != NV_ReadEx(MODEM_ID_0, en_NV_Item_Write_Slice_Record_Flag, (VOS_VOID*)&g_ulRecordNvFlag, sizeof(g_ulRecordNvFlag)))
    {
        g_ulRecordNvFlag = 0;
    }

    return NV_OK;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



