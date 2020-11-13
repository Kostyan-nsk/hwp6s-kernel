//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2010, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : diag_sd.c
//  �� �� ��   : V1.0
//  ��������   : 2012-08-13
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2012-08-13
//    �޸����� : �����ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#include "diag_port.h"
#include "diag_fs_proc.h"
#include "DrvInterface.h"
#include "msp_drx.h"

/*SD   */
#define DIAG_SD_ROOT_PATH         "/mnt/sd"
#define DIAG_SD_LTE_LOG_PATH      "/lte_log"
#define DIAG_SD_LTE_LOG_FILE_PRE  "ltelogfile_"
#define DIAG_SD_LTE_LOG_FILE_SUF  ".lpd"
#define DIAG_SD_FILE_MAX_SIZE     (3*1024*1024*1024)
#define DIAG_SD_LOG_DIR_LEN       2048


DIAG_SEND_LOG_CTRL_INFO_STRU stDiagSdLogCtrlInfo = {0,0,0 ,0,0,0,0,0,-1,0};

VOS_UINT32 g_SdFileFdErr = 0;
VOS_UINT32 g_SdFileSeekErr = 0;
VOS_UINT32 g_SdFileWriteErr = 0;
VOS_UINT32 g_SdFileWriteOK= 0;

/*****************************************************************************
 �� �� ��  : diag_CloseSdFile
 ��������  :
 �������  : fd:
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_UINT32 diag_CloseSdFile( VOS_INT32 fd)
{
    VOS_UINT32 ret;
    mm_segment_t sd_fs = 0;

    sd_fs = get_fs();
    set_fs(KERNEL_DS);

    if(fd < 0)
    {
        ret = ERR_MSP_SUCCESS;
    }
    else
    {
        ret = DIAG_FS_CLOSE(fd);
        DIAG_FS_SYNC();
    }
    set_fs(sd_fs);

    return ret;
}

/*****************************************************************************
 �� �� ��  : diag_CleanDir
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_UINT32 diag_CleanDir(VOS_UINT32 ulFileNum)
{
    VOS_CHAR dirName[128] = {0};
    VOS_INT32 ret;
    VOS_CHAR filedirName[128] = {0};
    VOS_CHAR CurrentFileName[128] = {0};
    VOS_UINT8 i;

    mm_segment_t sd_fs = 0;

    sd_fs = get_fs();
    set_fs(KERNEL_DS);

    if(stDiagSdLogCtrlInfo.CurrentFileFd != -1)
    {
        diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);
        stDiagSdLogCtrlInfo.CurrentFileFd = -1;
    }

    strcat(dirName, DIAG_SD_ROOT_PATH);
    strcat(dirName, DIAG_SD_LTE_LOG_PATH);

    strcat(filedirName, dirName);
    strcat(filedirName, "/");
    strcat(filedirName, DIAG_SD_LTE_LOG_FILE_PRE);

    for( i=0;i<ulFileNum;i++)
    {
        sprintf(CurrentFileName, "%s%d.lpd",filedirName,i);

        ret = DIAG_FS_RMFILE(CurrentFileName);

        MSP_MEMSET(CurrentFileName, 0, 128);
    }

    DIAG_FS_SYNC();

    set_fs(sd_fs);

    return  ret;
}

/*****************************************************************************
 �� �� ��  : diag_OpenSdFile
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_UINT32 diag_OpenSdFile( VOS_VOID)
{
    VOS_UINT32 ret;
    VOS_CHAR filedirName[128] = {0};
    VOS_CHAR CurrenfileName[128] = {0};
    VOS_UINT32  ulFileNameLen;
    mm_segment_t sd_fs = 0;


    sd_fs = get_fs();
    set_fs(KERNEL_DS);

    stDiagSdLogCtrlInfo.ulCurrenFileSize = 0;

    strcat(filedirName, DIAG_SD_ROOT_PATH);

    strcat(filedirName, DIAG_SD_LTE_LOG_PATH);
    ulFileNameLen = VOS_StrLen(filedirName);

    filedirName[ulFileNameLen] = '/';

    strcat(filedirName, DIAG_SD_LTE_LOG_FILE_PRE);

    /*ƴ���ļ����*/
    sprintf(CurrenfileName, "%s%d.lpd",filedirName,(VOS_UINT8)stDiagSdLogCtrlInfo.ulFileNum);

    stDiagSdLogCtrlInfo.CurrentFileFd = DIAG_FS_OPEN(CurrenfileName, O_RDWR |O_CREAT, 0666);

    if(stDiagSdLogCtrlInfo.CurrentFileFd < 0)
    {
        ret  = ERR_MSP_FAILURE;
    }
    else
    {
        stDiagSdLogCtrlInfo.ulFileNum =stDiagSdLogCtrlInfo.ulFileNum  + 1;
        stDiagSdLogCtrlInfo.ulCurrenFileSize = 0;
        ret  =  ERR_MSP_SUCCESS;
    }

    DIAG_FS_FILE_SYNC( stDiagSdLogCtrlInfo.CurrentFileFd);
    set_fs(sd_fs);

    return  ret;

}

/*****************************************************************************
 �� �� ��  : diag_ReCreatLogDir
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_UINT32 diag_ReCreatLogDir(VOS_VOID)
{
    VOS_INT32 Fd;
    VOS_INT32 nRead;
    VOS_CHAR dirName[128] = {0};
    VOS_UINT8 *pBuf =NULL;
    VOS_INT32 RsRet;
#if (VOS_LINUX== VOS_OS_VER)
    pBuf = kmalloc(DIAG_SD_LOG_DIR_LEN, GFP_KERNEL|__GFP_DMA);
#endif
    if(pBuf == NULL)
    {
        return ERR_MSP_MALLOC_FAILUE;
    }

    mm_segment_t sd_fs;

    sd_fs = get_fs();
    set_fs(KERNEL_DS);

    diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);

    stDiagSdLogCtrlInfo.CurrentFileFd = -1;

    strcat(dirName, DIAG_SD_ROOT_PATH);
    strcat(dirName, DIAG_SD_LTE_LOG_PATH);

    Fd = DIAG_FS_OPEN(dirName, O_DIRECTORY, 0);

    if(Fd <0)
    {
        /* Ŀ¼�����ڣ�����ɾ���ɹ�*/
#if (VOS_LINUX== VOS_OS_VER)
        kfree(pBuf);
#endif
    }
    else
    {
        nRead = DIAG_FS_GETDENTS(Fd, pBuf, DIAG_SD_LOG_DIR_LEN);
#if (VOS_LINUX== VOS_OS_VER)
        kfree(pBuf);
#endif

        if(-1 == nRead)
        {
            DIAG_FS_CLOSE(Fd);
            set_fs(sd_fs);
            return ERR_MSP_FAILURE;
        }

        /*Ŀ¼�ǿյģ�����Ҫɾ��*/
        if(0 == nRead)
        {
            DIAG_FS_CLOSE(Fd);
            set_fs(sd_fs);
            return ERR_MSP_SUCCESS;
        }
        else
        {
            DIAG_FS_CLOSE(Fd);
            diag_CleanDir(nRead);
            return ERR_MSP_SUCCESS;
        }
    }

    RsRet =  DIAG_FS_MKDIR(dirName,0700);

    DIAG_FS_SYNC();

    set_fs(sd_fs);
    if(RsRet <0)
    {
        return MSP_SD_CREAT_DIR_ERROR;
    }
    else
    {
        return ERR_MSP_SUCCESS;
    }

}

/*****************************************************************************
 �� �� ��  : diag_WriteDataToSDFile
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_UINT32 diag_WriteDataToSDFile(VOS_UINT8* ucBuf , VOS_UINT32 ulBufLen)
{
    VOS_UINT32 ret;
    VOS_INT32 ulWriteLen = 0;
    mm_segment_t sd_fs = 0;
    sd_fs = get_fs();
    set_fs(KERNEL_DS);


    if(stDiagSdLogCtrlInfo.CurrentFileFd == -1)
    {
        g_SdFileFdErr++;
        ret  = ERR_MSP_NOT_FOUND;
    }
    else
    {
        if( -1 == DIAG_FS_LSEEK(stDiagSdLogCtrlInfo.CurrentFileFd, (int)stDiagSdLogCtrlInfo.ulCurrenFileSize, SEEK_SET) )
        {
            g_SdFileSeekErr++;
            ret  = ERR_MSP_FAILURE;
        }
        else
        {
            ulWriteLen = DIAG_FS_WRITE(stDiagSdLogCtrlInfo.CurrentFileFd, ucBuf, ulBufLen);

            if(ulWriteLen == ulBufLen)
            {
                ret  = ERR_MSP_SUCCESS;
                stDiagSdLogCtrlInfo.StorageTotalSize += ulWriteLen;
                stDiagSdLogCtrlInfo.ulCurrenFileSize += ulWriteLen;
                g_SdFileWriteOK++;
            }
            else
            {
                g_SdFileWriteErr++;
                ret  = ERR_MSP_UNKNOWN;
            }
        }
    }


    DIAG_FS_FILE_SYNC(stDiagSdLogCtrlInfo.CurrentFileFd);

    set_fs(sd_fs);

    return ret;
}


/*****************************************************************************
 �� �� ��  : diag_SetLogCtrlInfo
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_UINT32 diag_SetLogCtrlInfo(VOS_UINT32 ulSendType,VOS_UINT32 ulStorageLogSize,VOS_UINT32 ulPeriod)
{
    VOS_UINT32 ret;

    if(ulSendType >= EN_DIAG_SEND_LOG_BUTT )
    {
        return ERR_MSP_PHYNUM_INVALID;
    }

    (VOS_VOID)BSP_BBPGetCurTime(&(stDiagSdLogCtrlInfo.StartTime));
    stDiagSdLogCtrlInfo.MaxFileSize =DIAG_SD_FILE_MAX_SIZE;
    stDiagSdLogCtrlInfo.ulCurrenFileSize = 0;
    stDiagSdLogCtrlInfo.ulFileNum = 0;
    stDiagSdLogCtrlInfo.StorageTotalSize = 0;

    if(ulSendType == EN_DIAG_SEND_LOG_TO_SD)
    {
        if(!DRV_SDMMC_GET_STATUS())
        {
            printk(" DRV_SDMMC_GET_STATUS FALSE\n");

            return MSP_SD_GET_STATE_ERROR;
        }

        /*ɾ��SD�������е�log�ļ�*/
        ret  = diag_ReCreatLogDir();

        if(ret == ERR_MSP_SUCCESS)
        {
            ret = diag_OpenSdFile();

            if(ret == ERR_MSP_SUCCESS)
            {
                stDiagSdLogCtrlInfo.enSendLogType = ulSendType;
                stDiagSdLogCtrlInfo.ulPeriod = ulPeriod *60; /*����ʱ�䣬HSO�·��ĵ�λΪ���ӣ�����ת��������� */
                stDiagSdLogCtrlInfo.ulStorageLogSize = ulStorageLogSize *1024*1024 ;/*HSO�·�����MBΪ��λ������ת�����ֽڼ���*/
                stDiagSdLogCtrlInfo.StorageTotalSize = 0;
                stDiagSdLogCtrlInfo.ulCurrenFileSize = 0;
                stDiagSdLogCtrlInfo.ulStartFlag = 1;
            }
        }
    }
    else
    {
         stDiagSdLogCtrlInfo.enSendLogType = ulSendType;

        /*�ر�SD�����Ѿ��򿪵�log�ļ�*/
        diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);
        stDiagSdLogCtrlInfo.CurrentFileFd = -1;
        stDiagSdLogCtrlInfo.ulStartFlag = 0;

        return ERR_MSP_SUCCESS;
    }

    return ret;
}

/*****************************************************************************
 �� �� ��  : diag_GetLogSendType
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

ENUM_DMS_LOG_SEND_TYPE diag_GetLogSendType(VOS_VOID)
{
    if(stDiagSdLogCtrlInfo.enSendLogType  == EN_DIAG_SEND_LOG_TO_SD)
    {
        return EN_DIAG_SEND_LOG_TO_SD;
    }
    else
    {
        return EN_DIAG_SEND_LOG_TO_USB;
    }
}

/*****************************************************************************
 �� �� ��  : diag_IsLogSendTimeOver
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_BOOL diag_IsLogSendTimeOver(VOS_VOID)
{
    VOS_UINT64 NewTime;
    VOS_UINT64 PeriodTime;
    VOS_UINT64 TempTime;

    if(stDiagSdLogCtrlInfo.ulPeriod == 0)
    {
        return FALSE;
    }

    (VOS_VOID)BSP_BBPGetCurTime(&NewTime);
     /*������Ϊ��λ��ʱ��ת����32KƵ�ʵļ���*/
    PeriodTime = stDiagSdLogCtrlInfo.ulPeriod *32*1024 ;

    /*����Ҫ ���Ƿ�ת*/
    TempTime = NewTime - stDiagSdLogCtrlInfo.StartTime ;

    if(TempTime > PeriodTime)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************
 �� �� ��  : diag_IsLogSendSizeFull
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_BOOL diag_IsLogSendSizeFull(VOS_UINT32 ulBufLen)
{
    if(stDiagSdLogCtrlInfo.ulStorageLogSize == 0)
    {
        return FALSE;
    }

    if((stDiagSdLogCtrlInfo.StorageTotalSize+ulBufLen )  < stDiagSdLogCtrlInfo.ulStorageLogSize)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/*****************************************************************************
 �� �� ��  : diag_IsCurrentFileFull
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_BOOL diag_IsCurrentFileFull(VOS_UINT32 ulBufLen)
{
    if((stDiagSdLogCtrlInfo.ulStorageLogSize != 0)&&( stDiagSdLogCtrlInfo.ulStorageLogSize <=stDiagSdLogCtrlInfo.MaxFileSize))
    {
        return FALSE;
    }
    else
    {
        if((stDiagSdLogCtrlInfo.ulCurrenFileSize+ulBufLen )  < stDiagSdLogCtrlInfo.MaxFileSize)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
}

/*****************************************************************************
 �� �� ��  : diag_StorDataToSD
 ��������  :
 �������  : :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��9��3��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_UINT32 diag_StorDataToSD(VOS_UINT8* ucBuf , VOS_UINT32 ulBufLen)
{
    VOS_UINT32 ulRet = ERR_MSP_SUCCESS;

    if(stDiagSdLogCtrlInfo.ulStartFlag == 0)
    {
        return ulRet;
    }

    if((ucBuf == NULL) ||(ulBufLen == 0 ))
    {
        diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);
        stDiagSdLogCtrlInfo.CurrentFileFd = -1;
        stDiagSdLogCtrlInfo.ulStartFlag =0;
        return ERR_MSP_INVALID_PARAMETER;
    }

    if(!DRV_SDMMC_GET_STATUS())
    {
        diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);
        stDiagSdLogCtrlInfo.CurrentFileFd = -1;
        stDiagSdLogCtrlInfo.ulStartFlag =0;
        return MSP_SD_GET_STATE_ERROR;
    }

    //�������ݵ��ܴ�С�Ƿ񳬹��趨ֵ,  ���سɹ�
    if(diag_IsLogSendSizeFull(ulBufLen))
    {
        diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);
        stDiagSdLogCtrlInfo.CurrentFileFd = -1;
        stDiagSdLogCtrlInfo.ulStartFlag =0;
        return ERR_MSP_SUCCESS;
    }

    //����ʱ�䳬���趨�����سɹ�
    if(diag_IsLogSendTimeOver())
    {
        diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);
        stDiagSdLogCtrlInfo.CurrentFileFd = -1;
        stDiagSdLogCtrlInfo.ulStartFlag =0;
        return ERR_MSP_SUCCESS;
    }

    /*��ǰ�ļ��Ƿ�д��*/
    if( !diag_IsCurrentFileFull(ulBufLen))
    {
        ulRet = diag_WriteDataToSDFile(ucBuf ,ulBufLen);
    }
    else
    {
        /*�ȹر��Ѿ�д�����ļ�*/
        diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);

        stDiagSdLogCtrlInfo.CurrentFileFd = -1;

        /*���´��ļ�*/
        ulRet = diag_OpenSdFile();

        if(ulRet != ERR_MSP_SUCCESS)
        {
            stDiagSdLogCtrlInfo.ulStartFlag =0;
            return ulRet;
        }

        ulRet = diag_WriteDataToSDFile(ucBuf ,ulBufLen);
    }

    if(ulRet != ERR_MSP_SUCCESS)
    {
        if(stDiagSdLogCtrlInfo.CurrentFileFd != -1)
        {
            diag_CloseSdFile(stDiagSdLogCtrlInfo.CurrentFileFd);

            stDiagSdLogCtrlInfo.CurrentFileFd = -1;
        }
        stDiagSdLogCtrlInfo.ulStartFlag =0;
    }

    return ulRet;

}

VOS_UINT32 diag_SdInit(VOS_VOID)
{
	g_diagPort[EN_DIAG_SD_BEARER_DIAG].ucChanStat        = ACM_EVT_DEV_READY;
	g_diagPort[EN_DIAG_SD_BEARER_DIAG].ucHdlcFlag        = EN_HDLC_DATA;
	g_diagPort[EN_DIAG_SD_BEARER_DIAG].ulCodeDesChanId   = SOCP_CODER_DST_LOM_CNF;
	g_diagPort[EN_DIAG_SD_BEARER_DIAG].ulDecodeSrcChanId = SOCP_DECODER_SRC_LOM;
	g_diagPort[EN_DIAG_SD_BEARER_DIAG].ulRecivBufLen     = 0;
    g_diagPort[EN_DIAG_SD_BEARER_DIAG].slPortHandle      = UDI_INVALID_HANDLE;
    g_diagPort[EN_DIAG_SD_BEARER_DIAG].ulInitStat        = EN_PORT_INIT_SUCC;

    return ERR_MSP_SUCCESS;
}


 VOS_UINT32 diag_sdhelp()
 {
    diag_printf(" ��ǰ�ļ���FD = %d\n",stDiagSdLogCtrlInfo.CurrentFileFd);
    diag_printf(" ��ǰ�ļ���SIZE = 0x%x\n",stDiagSdLogCtrlInfo.ulCurrenFileSize);
    diag_printf(" ��ǰ�ļ���Ŀ = %d\n",stDiagSdLogCtrlInfo.ulFileNum);
    diag_printf(" ����ʱ�� = %d\n",stDiagSdLogCtrlInfo.ulPeriod);
    diag_printf(" ��ʼʱ�� = %d\n",(VOS_UINT32)(stDiagSdLogCtrlInfo.StartTime));
    diag_printf(" ���ô洢SIZE = 0x%x\n",(VOS_UINT32)(stDiagSdLogCtrlInfo.ulStorageLogSize));
    diag_printf(" �Ѵ洢SIZE = 0x%x\n",(VOS_UINT32)(stDiagSdLogCtrlInfo.StorageTotalSize));
    diag_printf(" FD����g_SdFileFdErr = %d\n",g_SdFileFdErr);
    diag_printf(" SEEK����g_SdFileFdErr = %d\n",g_SdFileSeekErr);
    diag_printf(" WRITE����g_SdFileFdErr = %d\n",g_SdFileWriteErr);
    diag_printf(" WRITE�ɹ�g_SdFileFdOK = %d\n",g_SdFileWriteOK);
    diag_printf(" SD���洢��ʼ��־ = %d\n",stDiagSdLogCtrlInfo.ulStartFlag);

 }




