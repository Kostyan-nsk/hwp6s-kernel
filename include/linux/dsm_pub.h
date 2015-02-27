/**********************************************************
 * Filename:	dsm_pub.h
 *
 * Discription: Huawei device state monitor public head file
 *
 * Copyright: (C) 2014 huawei.
 *
 * Author: w00140597
 *
**********************************************************/

#ifndef _DSM_PUB_H
#define _DSM_PUB_H
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/wait.h>

extern int debug_output;

#define DSM_LOG_DEBUG(format, ...)				\
	do {						\
		if (debug_output)			\
			 printk("[DSM] "format,## __VA_ARGS__);\
	} while (0)


#define DSM_LOG_INFO(format, ...)		printk("[DSM] "format,## __VA_ARGS__)
#define DSM_LOG_ERR(format, ...)		printk("[DSM] "format,## __VA_ARGS__)

#define CLIENT_NAME_LEN		(32)

/*dsm error no define*/
#define DSM_ERR_NO_ERROR		(0)
#define DSM_ERR_I2C_TIMEOUT		(1)
#define DSM_ERR_SENSORHUB_IPC_TIMEOUT	(20400)

struct dsm_client_ops{
	int (*poll_state) (void);
	int (*dump_func) (int type, void *buff, int size);
};

struct dsm_dev{
	const char *name;
	struct dsm_client_ops *fops;
	size_t buff_size;
};

struct dsm_client{
	char client_name[CLIENT_NAME_LEN];
	int client_id;
	int error_no;
	unsigned long buff_flag;
	struct dsm_client_ops *cops;
	wait_queue_head_t waitq;
	size_t read_size;
	size_t used_size;
	size_t buff_size;
	u8 dump_buff[];
};

#ifdef CONFIG_HUAWEI_DSM
struct dsm_client *dsm_register_client (struct dsm_dev *dev);
int dsm_client_ocuppy(struct dsm_client *client);
int dsm_client_record(struct dsm_client *client, const char *fmt, ...);
int dsm_client_copy(struct dsm_client *client, void *src, int sz);
void dsm_client_notify(struct dsm_client *client, int error_no);
#else
static inline struct dsm_client *dsm_register_client (struct dsm_dev *dev)
{
	return NULL;
}
static inline int dsm_client_ocuppy(struct dsm_client *client)
{
	return 1;
}
static inline int dsm_client_record(struct dsm_client *client, const char *fmt, ...)
{
	return 0;
}
static inline int dsm_client_copy(struct dsm_client *client, void *src, int sz)
{
	return 0;
}
static inline void dsm_client_notify(struct dsm_client *client, int error_no)
{
	return;
}
#endif

#endif
