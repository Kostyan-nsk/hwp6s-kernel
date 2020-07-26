

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#ifndef __RFILESYSTEM_PROCESS__
#define __RFILESYSTEM_PROCESS__
#include <mtd/mtd-abi.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
/*#define LOG_TAG          "RFS_CORE"*/

#define NV_MAP_PATH      "/modem/nvm1"
#define CP_LOG_PATH      "/data/cp-log"
#define NV_BAK_PATH      "/modem/nvm2/nv_bak"
#define ZSP_BIN_PATH     "/modem/modem_image/balong/ZSP"

#define GU_NV_FILE       "Nvim.bin"
#define GU_NV_XML_FILE   "xnv.xml"
#define LTE_NV_FILE      "NV.HNV"
#define LTE_NV_XML_FILE  "lte_xnv.xml"
#define SC_DIR           "SC"
#define LTE_NV_DIR       "NV"
#define GU_NV_DIR        "Nvim"
#define NV_BAK_DIR       "nv_bak"

#define LOG_TAG          "NVM_SERVER_BALONG"

#define BLK_DEV          "/dev/block/mmcblk0p"

#define PATH_MAX         4096

#define ACK_VALUE        0xAAAA
#define ACK_WAIT_SEC     1
#define ACK_WAIT_USEC    0

#define DRV_NAME_MAX        255
#define DRV_BLK_MAX         50

#define DRV_SEEK_SET            0               /* absolute offset, was L_SET */
#define DRV_SEEK_CUR            1               /* relative to current offset, was L_INCR */
#define DRV_SEEK_END            2               /* relative to end of file, was L_XTND */

#define SEEK_SET       0

#define __SRD         0x0001
#define __SWR         0x0002
#define __SRW         0x0004

#define DRV_TIME           unsigned long

#define MNT_H_MAGIC       0x55AA

#define FSTAT_DIR		0040000		/* directory */
#define DEFAULT_DIR_PERM_KERNEL 	0000777 /* default directory permissions */

#define DIR_ENTRY_SIZE    1024
#define WR_BUFF_SIZE      1024

/*
 *ÿ�η��Ͱ�����󳤶���BUF_LEN, ���ݷ��Ͱ��ĳ��ȼ�����跢�ͼ���
 */
#define BUF_LEN       (7*1024)

#define RFILE_APPEND_AUTO     1
#define RFILE_APPEND_MENU     0

#define EXCH_DIR_ENTRY_SIZE         1024

#define RFILE_CLOSE_ALL       0

#define RFILE_DEBUGPL(lvl, x...) do { \
	if ((lvl)&g_rfile_dbg_lvl) { \
		printk(x); \
		printk("\n");\
	} \
} while (0)

/* ACPU���ѹ�����ʹ��FS����EMMCͬ��*/
#define RFILE_WAKE_SYNC_EMMC do { \
    while(false != g_ulmmc_suspend_flag){ \
        msleep(2); \
    } \
} while (0)

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
typedef	unsigned long       DRV_INO_T;


typedef enum
{
    FOPEN = 0,       /*1*/
	FOPEN_RESP,      /*2*/
	FSEEK,
	FSEEK_RESP,
	FREAD,
	FREAD_RESP,
	FWRITE,
	FWRITE_RESP,
	FCLOSE,
	FCLOSE_RESP,     /*10*/
	REMOVE,
	REMOVE_RESP,
	FTELL,
	FTELL_RESP,
	FMKDIR,         /*15*/
	FMKDIR_RESP,
	FRMDIR,
	FRMDIR_RESP,
	FOPENDIR,
	FOPENDIR_RESP,   /*20*/
	FREADDIR,
	FREADDIR_RESP,
	FCLOSEDIR,
	FCLOSEDIR_RESP,
	FSTAT,           /*25*/
	FSTAT_RESP,
	FXCOPY,
	FXCOPY_RESP,
	FXDELETE,
	FXDELETE_RESP,   /*30*/
	FACCESS,
	FACCESS_RESP,
	FRENAME,
	FRENAME_RESP,
	FMASS_READ,
	FMASS_READ_RESP,
	FMASS_WRITE,
	FMASS_WRITE_RESP,
	OPS_MAX,
}OPS_TYPE;

/* dirent */
typedef struct dirent
{
    DRV_INO_T           d_ino;                      /* file serial number */
    char                d_name [DRV_NAME_MAX + 1];	/* file name, null-terminated */
}DRV_DIRENT_STRU;

typedef struct _stat
{
    unsigned long       st_dev;     /* Device ID number */
    unsigned long       st_ino;     /* File serial number */
    int                 st_mode;    /* Mode of file */
    unsigned long       st_nlink;   /* Number of hard links to file */
    unsigned short      st_uid;     /* User ID of file */
    unsigned short      st_gid;     /* Group ID of file */
    unsigned long       st_rdev;    /* Device ID if special file */
    signed   long       st_size;    /* File size in bytes */
    DRV_TIME            st_atime;   /* Time of last access */
    DRV_TIME            st_mtime;   /* Time of last modification */
    DRV_TIME            st_ctime;   /* Time of last status change */
    long                st_blksize; /* File system block size */
    unsigned long       st_blocks;  /* Number of blocks containing file */
    unsigned char       st_attrib;  /* DOSFS only - file attributes */
    int                 st_reserved1;  /* reserved for future use */
    int                 st_reserved2;  /* reserved for future use */
    int                 st_reserved3;  /* reserved for future use */
    int                 st_reserved4;
}DRV_STAT_STRU;


typedef struct
{
    int                 dd_fd;                      /* file descriptor for open directory */
    int                 dd_cookie;                  /* filesys-specific marker within dir */
    int                 dd_eof;                     /* readdir EOF flag */
    DRV_DIRENT_STRU     dd_dirent;                  /* obtained directory entry */
}DRV_DIR_STRU;


//Frame
typedef struct
{
   unsigned int opsPid;
   unsigned int totalFrame;
   unsigned int curFrame;
   unsigned int curlen;
   int reserve;
}RFILE_MNTN_HEADER;

typedef int (*fileOps)(RFILE_MNTN_HEADER *header, char * data);

typedef struct
{
    int blk_begin;           /*�߼������ʼ��*/
    int blk_end;             /*�߼����ĩβ��*/
    int last_blk_offset;   /*���һ������Ҫ��ȡ�����ݳ���*/
    int first_blk_offset;  /*��һ���в���Ҫ��ȡ�����ݳ���*/
    long long physical_offset;   /*�ļ�ָ�������ƫ����*/
    int reserv[4];
}RFILE_MASS_RW_PARA;


//OPEN
typedef struct
{
    unsigned int opsType;
    unsigned int pathLen;
    unsigned int modeLen;
}RFILE_OPEN_HEAD;

typedef struct
{
    unsigned int opsType;
    int fp;
}RFILE_RESP_OPEN_MSG;

//CLOSE
typedef struct
{
    unsigned int opsType;
    int fp;
}RFILE_CLOSE_HEAD;
typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_CLOSE_MSG;

//TELL
typedef struct
{
    unsigned int opsType;
    int fp;
}RFILE_TELL_HEAD;
typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_TELL_MSG;

//SEEK
typedef struct
{
    unsigned int opsType;
    int fp;
    long offset;
    int whence;
}RFILE_SEEK_HEAD;
typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_SEEK_MSG;

//REMOVE
typedef struct
{
    unsigned int opsType;
    int pathLen;
}RFILE_REMOVE_HEAD;
typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_REMOVE_MSG;

//READ
typedef struct
{
    unsigned int opsType;
    int fp;
    unsigned int size;
}RFILE_READ_HEAD;
typedef struct
{
    unsigned int opsType;
    int fp;
    int size;
}RFILE_RESP_READ_MSG;

//WRITE
typedef struct
{
    unsigned int opsType;
    int fp;
    unsigned int size;
}RFILE_WRITE_HEAD;

typedef struct
{
    unsigned int opsType;
    int fp;
    unsigned int ret;
}RFILE_RESP_WRITE_MSG;

//MKDIR
typedef struct
{
    unsigned int opsType;
    int pathlen;
}RFILE_MKDIR_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_MKDIR_MSG;

//RMDIR
typedef struct
{
    unsigned int opsType;
    int pathlen;
}RFILE_RMDIR_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_RMDIR_MSG;

//OPENDIR
typedef struct
{
    unsigned int opsType;
    int pathlen;
}RFILE_OPENDIR_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_OPENDIR_MSG;

//READDIR
typedef struct
{
    unsigned int opsType;
    int dir;
}RFILE_READDIR_HEAD;

typedef struct
{
    unsigned int opsType;
    int          retval;
    DRV_DIRENT_STRU ret;
}RFILE_RESP_READDIR_MSG;

//CLOSEDIR
typedef struct
{
    unsigned int opsType;
    int dir;
}RFILE_CLOSEDIR_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_CLOSEDIR_MSG;

//STATDIR
typedef struct
{
    unsigned int opsType;
    int pathlen;
}RFILE_STAT_HEAD;

typedef struct
{
    unsigned int opsType;
    int           retval;
    DRV_STAT_STRU ret;
}RFILE_RESP_STAT_MSG;

//ACCESS
typedef struct
{
    unsigned int opsType;
    unsigned int pathLen;
    unsigned int modeLen;
}RFILE_ACCESS_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_ACCESS_MSG;


//XCOPYDIR
typedef struct
{
    unsigned int opsType;
    int source_path_len;
    int dest_path_len;
}RFILE_XCOPY_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_XCOPY_MSG;

//XDELETEDIR
typedef struct
{
    unsigned int opsType;
    int source_path_len;
}RFILE_XDELETE_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_XDELETE_MSG;

//rename
typedef struct
{
    unsigned int opsType;
    int old_path_len;
    int new_path_len;
}RFILE_RENAME_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
}RFILE_RESP_RENAME_MSG;


//massread
typedef struct
{
    unsigned int opsType;
    unsigned int partition_name_len;
    unsigned int offset;
    unsigned int length;
}RFILE_MASSREAD_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
    int size;
}RFILE_RESP_MASSREAD_MSG;

/*
|_____________|__________|______|_________|____________|__________________|
  mntn header  partionLen offset  datasize  partionName      data
*/
//masswrite
typedef struct
{
    unsigned int opsType;
    unsigned partition_name_len;
    unsigned offset;
    unsigned size;
}RFILE_MASSWRITE_HEAD;

typedef struct
{
    unsigned int opsType;
    int ret;
    int size;
}RFILE_RESP_MASSWRITE_MSG;


struct linux_dirent {
    unsigned long   d_ino;
    unsigned long   d_off;
    unsigned short  d_reclen;
    char            d_name[1];
};

struct linux_dirent_rfile {
    unsigned long   d_ino;
    unsigned long   d_off;
    unsigned short  d_reclen;
    char            d_name[DRV_NAME_MAX + 1];
};

typedef struct {
	int		_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	unsigned short	_flag;
	unsigned short  reserve;
	int     		_file;
	int		_fcount;
	void		*_flock;
    unsigned long  fs_set;
	unsigned char	_ubuf[4];
} FILE;

typedef struct
{
    int fp_v;
    struct list_head list;
}fp_list_t;
struct dir_list
{
    struct linux_dirent *dirent;
    struct list_head    list;
};

struct rfile_ops {
    int   (*bsp_fopen)(const char* path, const char* mode);
    int   (*bsp_fclose)(void* stream);
    int   (*bsp_fread)( void *ptr, unsigned count, void *stream );
    int   (*bsp_fwrite)( void *ptr, unsigned count, void *stream );
    int   (*bsp_fseek)( void *stream, long offset, int whence );
    int   (*bsp_remove)( const char *pathname );
    int   (*bsp_ftell)( void *stream );
    int   (*bsp_mkdir)( const char *path);
    int   (*bsp_rmdir)( const char *path);
    int   (*bsp_opendir)( const char *dirname );
    int   (*bsp_readdir)( DRV_DIR_STRU *dirp,DRV_DIRENT_STRU* buf );
    int   (*bsp_closedir)( DRV_DIR_STRU *dirp );
    int   (*bsp_stat)( const char *path, DRV_STAT_STRU *buf );
    int   (*bsp_access)( const char *path, int amode);
    int   (*bsp_xcopy)( const char * source, const char * dest );
    int   (*bsp_xdelete)( const char *source);
    int   (*bsp_rename)( const char * oldname, const char * newname );
    int   (*bsp_mass_read)(const char *partionname,unsigned int offset,unsigned length,char *buffer);
    int   (*bsp_mass_write)(const char *partionname,unsigned int offset,unsigned length,char *buffer);
};

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
int fopenProxy(RFILE_MNTN_HEADER *mnt_h, char * data);
int fseekProxy(RFILE_MNTN_HEADER *mnt_h, char * data);
int freadProxy(RFILE_MNTN_HEADER *mnt_h, char * data);
int fwriteProxy(RFILE_MNTN_HEADER *mnt_h, char * data);
int fcloseProxy(RFILE_MNTN_HEADER *mnt_h, char * data);
int fremoveProxy(RFILE_MNTN_HEADER *mnt_h, char * data);
int ftellProxy(RFILE_MNTN_HEADER *mnt_h, char * data);
int fmkdirProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int frmdirProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int fopendirProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int freaddirProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int fclosedirProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int fstatdirProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int faccessProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int fxcopydirProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int faccessProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int fxdeletedirProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int frenameProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int fmassreadProxy( RFILE_MNTN_HEADER *mnt_h, char * data );
int fmasswriteProxy( RFILE_MNTN_HEADER *mnt_h, char * data );


FILE* bsp_linux_fopen( const char *filename, const char *mode );
int bsp_linux_fclose(FILE* stream);
int bsp_linux_fread( void *ptr, unsigned int count, FILE *stream );
int bsp_linux_fwrite( void *buf, unsigned int count, FILE *fp );
int bsp_linux_fseek( FILE *stream, long offset, int whence );
int bsp_linux_remove( const char *pathname );
int bsp_linux_ftell( FILE *stream );
int bsp_linux_mkdir( const char *pathname);
int bsp_linux_rmdir( const char *pathname);
int bsp_linux_opendir( const char *dirname );
int bsp_linux_readdir( DRV_DIR_STRU *dirp,DRV_DIRENT_STRU* buf );
int bsp_linux_closedir( DRV_DIR_STRU *dirp );
int bsp_linux_stat( const char *pathname, DRV_STAT_STRU *buf );
int bsp_linux_access( const char *filename, int amode);
int bsp_linux_xcopy( const char * source, const char * dest );
int bsp_linux_xdelete( const char *pathname);
int bsp_linux_rename( const char * oldname, const char * newname );
int bsp_mass_read(const char *partionname,unsigned int offset,unsigned length,char *buffer);
int bsp_mass_write(const char *partionname,unsigned int offset,unsigned length,char *buffer);


void rfile_regProxyFunc(void);
int bsp_linux_readdir( DRV_DIR_STRU *dirp,DRV_DIRENT_STRU* buf );
int rfile_read_ccpu_pkt( RFILE_MNTN_HEADER *mntn_header );
int rfile_get_effectblk_addr(int fd,int usr_logic_blk,int blksize);
int rfile_mass_write(int fd,char* write_buf,unsigned int blksize,unsigned int erase_addr);
int rfile_mass_read_all(int                  fd,
                           int                  length,
                           char                *buffer_temp,
                           char                *buffer,
                           RFILE_MASS_RW_PARA   read_blk_para,
                           struct mtd_info_user mtd_info);

int rfile_mass_write_all(int                 fd,
                           int                  length,
                           char                *buffer,
                           char                *buffer_temp,
                           RFILE_MASS_RW_PARA   write_blk_para,
                           struct mtd_info_user mtd_info);
/*Modify_for_c_reset, l00212112,20130511, starts*/
int bsp_ccpu_rest_rfile_handle(void);
/*Modify_for_c_reset, l00212112,20130511, ends*/



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of rfilesystem_process_new.h */
