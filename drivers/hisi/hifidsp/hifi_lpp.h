

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __HIFI_MISC_H__
#define __HIFI_MISC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 宏定义
*****************************************************************************/
/*#define HIFI_MISC_NAME "hifi"*/

/* Debug info */
#define INFO_LEVEL   0
#define DEBUG_LEVEL  0
#define ERROR_LEVEL  1
#define PAGE_MAX_SIZE        0x1000

#ifndef OK
#define OK           0
#endif
#define ERROR       (-1)
#define BUSY        (-2)
#define NOMEM       (-3)
#define INVAILD     (-4)

/* IOCTL入参和出参的SIZE限制 */
#define SIZE_LIMIT_PARAM        (256)

/* HIFI专用区 */
#define OFFSET_HIFI_PRIV        (0x00300000)
#define HIFI_MUSIC_DATA_SIZE	(0x100000) /* LowPowerPlayer.cpp:MEM_BUFF_SIZE */
#define HIFI_PRIV_ADDR          (HIFI_SYS_MEM_ADDR + OFFSET_HIFI_PRIV)
#define SIZE_PARAM_PRIV         (100 * 1024)

/* 接收HIFI消息，前部cmd_id占用的字节数 */

#define SIZE_CMD_ID        (8)

/* notice主动上报一次缓冲 */
#define REV_MSG_NOTICE_ID_MAX       2

#define ACPU_TO_HIFI_ASYNC_CMD    0xFFFFFFFF

#define BUFFER_NUM  8

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
typedef enum{
    HIFI_CLOSE,
    HIFI_OPENED,
}HIFI_STATUS;


/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/

typedef enum HIFI_MSG_ID_ {

	/*DTS command id from ap*/
	ID_AP_AUDIO_SET_DTS_ENABLE_CMD		= 0xDD36,
	ID_AP_AUDIO_SET_DTS_DEV_CMD			= 0xDD38,
	ID_AP_AUDIO_SET_DTS_GEQ_CMD			= 0xDD39,
	ID_AP_AUDIO_SET_DTS_GEQ_ENABLE_CMD	= 0xDD3B,

	/* Voice Record */
	ID_AP_HIFI_VOICE_RECORD_START_CMD	= 0xDD40,
	ID_AP_HIFI_VOICE_RECORD_STOP_CMD	= 0xDD41,

	/* voicePP MSG_ID */
	ID_AP_VOICEPP_START_REQ				= 0xDD42,						/* 启动VOICEPP通话 */
	ID_VOICEPP_MSG_START				= ID_AP_VOICEPP_START_REQ,
	ID_VOICEPP_AP_START_CNF				= 0xDD43,
	ID_AP_VOICEPP_STOP_REQ				= 0xDD44,						/* 终止VOICEPP通话 */
	ID_VOICEPP_AP_STOP_CNF				= 0xDD45,
	ID_AP_VOICEPP_SET_DEVICE_REQ		= 0xDD46,						/* 配置输入输出设备 */
	ID_VOICEPP_AP_SET_DEVICE_CNF		= 0xDD47,
	ID_AP_VOICEPP_SET_WB_REQ			= 0xDD48,						/* 设置宽窄带 */
	ID_VOICEPP_AP_SET_WB_CNF			= 0xDD49,
	ID_VOICEPP_MSG_END					= 0xDD4A,

	ID_AP_AUDIO_PLAY_START_REQ			= 0xDD51,/* AP启动Hifi audio player request命令 */
	ID_AUDIO_AP_PLAY_START_CNF			= 0xDD52,/* Hifi启动audio player后回复AP confirm命令 */
	ID_AP_AUDIO_PLAY_PAUSE_REQ			= 0xDD53,/* AP停止Hifi audio player request命令 */
	ID_AUDIO_AP_PLAY_PAUSE_CNF			= 0xDD54,/* Hifi停止audio player后回复AP confirm命令 */
	ID_AUDIO_AP_PLAY_DONE_IND			= 0xDD56,/* Hifi通知AP audio player一块数据播放完毕或者播放中断indication */
	ID_AP_AUDIO_PLAY_UPDATE_BUF_CMD 	= 0xDD57,/* AP通知Hifi新数据块更新command */
	ID_AP_AUDIO_PLAY_QUERY_TIME_REQ 	= 0xDD59,/* AP查询Hifi audio player播放进度request命令 */
	ID_AP_AUDIO_PLAY_WAKEUPTHREAD_REQ	= 0xDD5A,
	ID_AUDIO_AP_PLAY_QUERY_TIME_CNF 	= 0xDD60,/* Hifi回复AP audio player播放进度confirm命令 */
	ID_AP_AUDIO_PLAY_QUERY_STATUS_REQ	= 0xDD61,/* AP查询Hifi audio player播放状态request命令 */
	ID_AUDIO_AP_PLAY_QUERY_STATUS_CNF	= 0xDD62,/* Hifi回复AP audio player播放状态confirm命令 */
	ID_AP_AUDIO_PLAY_SEEK_REQ			= 0xDD63,/* AP seek Hifi audio player到某一位置request命令 */
	ID_AUDIO_AP_PLAY_SEEK_CNF			= 0xDD64,/* Hifi回复AP seek结果confirm命令 */
	ID_AP_AUDIO_PLAY_SET_VOL_CMD		= 0xDD70,/* AP设置音量命令 */
	ID_AP_AUDIO_RECORD_PCM_HOOK_CMD		= 0xDD7A,/* AP 通知HIFI开始抓取PCM数据 */
	/* enhance msgid between ap and hifi */
	ID_AP_HIFI_ENHANCE_START_REQ		= 0xDD81,
	ID_HIFI_AP_ENHANCE_START_CNF		= 0xDD82,
	ID_AP_HIFI_ENHANCE_STOP_REQ 		= 0xDD83,
	ID_HIFI_AP_ENHANCE_STOP_CNF 		= 0xDD84,
	ID_AP_HIFI_ENHANCE_SET_DEVICE_REQ	= 0xDD85,
	ID_HIFI_AP_ENHANCE_SET_DEVICE_CNF	= 0xDD86,

	/* audio enhance msgid between ap and hifi */
	ID_AP_AUDIO_ENHANCE_SET_DEVICE_IND	= 0xDD91,
	ID_AP_AUDIO_MLIB_SET_PARA_IND		= 0xDD92,
} HIFI_MSG_ID;

struct misc_recmsg_param {
	unsigned short	msgID;
	unsigned short	playStatus;
};

/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/

/*处理hifi回复消息，记录cmd_id和数据*/
typedef struct
{
    unsigned char *mail_buff;       /* 邮箱数据接收的buff */
    unsigned int mail_buff_len;
    unsigned int cmd_id;            /* 邮箱接收数据前4个字节是cmd_id */
    unsigned char *out_buff_ptr;    /* 指向mail_buff cmd_id后的位置 */
    unsigned int out_buff_len;
} rev_msg_buff;

/* hifi可维可测工作结构 */
typedef struct
{
    unsigned int        work_id;
    struct work_struct  work_stru;
} hifi_mntn_work_stru;


struct recv_request{
    struct list_head recv_node;
    rev_msg_buff rev_msg;
};



/*****************************************************************************
  7 UNION定义
*****************************************************************************/


/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/
#ifndef LOG_TAG
#define LOG_TAG "hifi_misc "
#endif

#if INFO_LEVEL
#define logi(fmt, ...) pr_info(LOG_TAG"[I]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define logi(fmt, ...)
#endif

#if DEBUG_LEVEL
#define logd(fmt, ...) pr_info(LOG_TAG"[D]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define logd(fmt, ...)
#endif

#if ERROR_LEVEL
#define loge(fmt, ...) pr_info(LOG_TAG"[E]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define loge(fmt, ...)
#endif

#define IN_FUNCTION   logd("Begin\n");
#define OUT_FUNCTION  logd("End\n");

/*****************************************************************************
  9 全局变量声明
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/
int hifi_misc_async_write(unsigned char *arg, unsigned int len);
void __exit hifi_misc_exit( void );
void hifi_misc_handle_mail( void *usr_para, void *mail_handle, unsigned int mail_len );
int __init hifi_misc_init( void );
long hifi_misc_ioctl(struct file *fd,
                                    unsigned int cmd,
                                    unsigned long arg);
int hifi_misc_open(struct inode *finode, struct file *fd);
int hifi_misc_probe (struct platform_device *pdev);
void hifi_misc_proc_init( void );
int hifi_misc_proc_read(char *pg,
                                   char**start,
                                   off_t off,
                                   int count,
                                   int *eof,
                                   void *data);
void hifi_misc_receive_task_sync(void);
void hifi_misc_receive_task_proc(void);
int hifi_misc_release(struct inode *finode, struct file *fd);
int hifi_misc_remove(struct platform_device *pdev);
int hifi_misc_sync_write(unsigned char  *arg, unsigned int len);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hifi_misc.h */
