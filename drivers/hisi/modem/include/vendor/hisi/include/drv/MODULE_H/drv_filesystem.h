

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "drv_global.h"


#ifndef __DRV_FILESYSTEM_H__
#define __DRV_FILESYSTEM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/


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
/********************************************************************************************************
 �� �� ��  : BSP_FS_GetDskspc
 ��������  : �õ��ļ�ϵͳ�ռ䡣
 �������  : path���ļ�ϵͳ·����
 �������  : DskSpc���ܿռ䡣
                           UsdSpc���ù��ռ䡣
                           VldSpc�����пռ䡣
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
********************************************************************************************************/
extern int BSP_FS_GetDskspc(const char *path,unsigned int *DskSpc,unsigned int  *UsdSpc,  unsigned int *VldSpc);
#define DRV_FILE_GET_DISKSPACE(path,DskSpc,UsdSpc,VldSpc)    BSP_FS_GetDskspc(path,DskSpc,UsdSpc,VldSpc)

extern long bsp_sys_creat(const char  *pathname, int mode);
#define DRV_SYS_CREAT(pathname,mode) bsp_sys_creat(pathname,mode)

extern long bsp_sys_open(const char  *filename,int flags, int mode);
#define DRV_SYS_OPEN(filename,flags,mode) bsp_sys_open(filename,flags,mode)

extern long bsp_sys_close(unsigned int fd);
#define DRV_SYS_CLOSE(fd) bsp_sys_close(fd)

extern long bsp_sys_lseek(unsigned int fd, unsigned int offset,unsigned int origin);
#define DRV_SYS_LSEEK(fd,offset,origin) bsp_sys_lseek(fd,offset,origin)

extern long bsp_sys_read(unsigned int fd, char  *buf, unsigned int count);
#define DRV_SYS_READ(fd, buf, count) bsp_sys_read(fd, buf, count)

extern long bsp_sys_write(unsigned int fd, const char  *buf,unsigned int count);
#define DRV_SYS_WRITE(fd, buf, count) bsp_sys_write(fd, buf, count)

extern long bsp_sys_mkdir(const char  *pathname, int mode);
#define DRV_SYS_MKDIR(pathname, mode) bsp_sys_mkdir(pathname, mode)

extern long bsp_sys_rmdir(const char  *pathname);
#define DRV_SYS_RMDIR(pathname) bsp_sys_rmdir(pathname)

extern long bsp_sys_unlink(const char  *pathname);
#define DRV_SYS_UNLINK(pathname) bsp_sys_unlink(pathname)

extern long bsp_sys_getdents(unsigned int fd,
          void  *dirent,
          unsigned int count);
#define DRV_SYS_GETDENTS(fd,dirent,count) bsp_sys_getdents(fd,dirent,count)

extern long bsp_sys_sync(void);
#define DRV_SYS_SYNC() bsp_sys_sync()

extern long bsp_sys_sync_file(unsigned int fd);
#define DRV_SYS_SYNC_FILE(fd) bsp_sys_sync_file(fd)



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of drv_filesystem.h */

