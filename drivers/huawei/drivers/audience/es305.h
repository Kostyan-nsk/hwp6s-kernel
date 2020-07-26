#ifndef AUDIENCE_ES305_H
#define AUDIENCE_ES305_H

#include <linux/ioctl.h>

#define ES305_NAME  "audience_es305"

struct es305_platform_data {
    uint32_t gpio_es305_wakeup;
    uint32_t gpio_es305_reset;
};

struct es305_img {
    unsigned char *buf;
    unsigned img_size;
};

/* path ids */
enum ES305_PATHID {
    ES305_PATH_NO_INIT                    = 0,
    ES305_PATH_BYPASS                     = 1,
};

/* max size of firmware */
#define ES305_MAX_FW_SIZE ( 100 * 1024 ) /* 100K */

/* IOCTLs for Audience ES305 */
#define ES305_IOCTL_MAGIC 'u'

#define ES305_CMD_RESET    _IO(ES305_IOCTL_MAGIC,  0x00)
#define ES305_CMD_SYNC     _IO(ES305_IOCTL_MAGIC,  0x01)
#define ES305_CMD_SLEEP    _IO(ES305_IOCTL_MAGIC,  0x02)

#define ES305_I2C_DOWNLOAD _IOW(ES305_IOCTL_MAGIC, 0x03, struct es305_img*)
#define ES305_SET_PATHID   _IOW(ES305_IOCTL_MAGIC, 0x04, enum ES305_PATHID)
#define ES305_SET_NS       _IOW(ES305_IOCTL_MAGIC, 0x05, unsigned char)
#define ES305_SET_STATUS   _IOW(ES305_IOCTL_MAGIC, 0x06, unsigned char)

/* For Diag */
#define ES305_WRITE_MSG    _IOW(ES305_IOCTL_MAGIC, 0x10, unsigned)
#define ES305_READ_DATA    _IOR(ES305_IOCTL_MAGIC, 0x11, unsigned)

/* commands */
/* sync polling mode */
#define AUDIENCE_MSG_SYNC               ( 0x80000000 )

/* sleep mode */
#define AUDIENCE_MSG_SLEEP_MODE         ( 0x80100001 )

/* reset */
#define AUDIENCE_MSG_RESET_IMMEDIATE    ( 0x80020000 )
#define AUDIENCE_MSG_RESET_DELAYED      ( 0x80020001 )

/* bootload initiate */
#define AUDIENCE_MSG_BOOTLOADINITIATE   ( 0x80030000 )

#define AUDIENCE_MSG_BOOT_BYTE1         ( 0x00 )
#define AUDIENCE_MSG_BOOT_BYTE2         ( 0x01 )
#define AUDIENCE_MSG_BOOT_ACK           ( 0x01 )

/******************************************************************************/

/* general definitions */
#define AUDIENCE_RESET_STABLE_TIME      ( 4  )  /* ms / keep 4 cycles in clk */
#define AUDIENCE_SW_RESET_TIME          ( 20 )  /* ms */
#define AUDIENCE_I2C_ACCESS_DELAY_TIME  ( 50 )  /* ms */
#define AUDIENCE_BOOT_ACK_TIME          ( 1  )  /* ms */
#define AUDIENCE_SLEEP_TIME             ( 20 )  /* ms */
#define AUDIENCE_BOOT_SYNC_TIME         ( 120 ) /* ms */

#define AUDIENCE_SYNC_TIME              (  1000 )  /* us */
#define AUDIENCE_POLLING_TIME           ( 20000 )  /* us */
#define AUDIENCE_WAKEUP_H_TIME          (  1000 )  /* us */
#define AUDIENCE_WAKEUP_L_TIME          ( 30000 )  /* us */

#define AUDIENCE_ACK_ERROR              ( 0xFF )
#define AUDIENCE_ACK_NONE               ( 0x00 )

#define RETRY_TIMES                     ( 5 )
#define POLLING_RETRY_TIMES             ( 3 )
#define ES305_CMD_FIFO_DEPTH            ( 128 )

#define AUDIENCE_SW_RESET_OK            ( 0x81 )
#define AUDIENCE_SW_RESET_ERROR         ( 0x82 )

#endif //AUDIENCE_ES305_H