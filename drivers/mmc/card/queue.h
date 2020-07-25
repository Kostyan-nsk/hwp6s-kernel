#ifndef MMC_QUEUE_H
#define MMC_QUEUE_H

struct request;
struct task_struct;

struct mmc_blk_request {
	struct mmc_request	mrq;
	struct mmc_command	sbc;
	struct mmc_command	cmd;
	struct mmc_command	stop;
	struct mmc_data		data;
};

enum mmc_packed_cmd {
	MMC_PACKED_NONE = 0,
	MMC_PACKED_WR_HDR,
	MMC_PACKED_WRITE,
	MMC_PACKED_READ,
};

struct mmc_queue_req {
	struct request		*req;
	struct mmc_blk_request	brq;
	struct scatterlist	*sg;
	char			*bounce_buf;
	struct scatterlist	*bounce_sg;
	unsigned int		bounce_sg_len;
	struct mmc_async_req	mmc_active;
	struct list_head	packed_list;
	u32			packed_cmd_hdr[128];
	unsigned int		packed_blocks;
	enum mmc_packed_cmd	packed_cmd;
	int		packed_fail_idx;
	u8		packed_num;
};


#define CMD23_ARG_PACKED (1<<30)

#define MIN_PACKED_CMD_WRITES 3  
#define MIN_PACKED_CMD_READS 5

#define MIN_PACKED_CMD_THRESHOLD 20

#define PACKED_CMD_HDR_READ 1
#define PACKED_CMD_HDR_WRITE 2
#define PACKED_CMD_HDR_SIZE 512

#define RET_PACKED_UNHANDLED 0
#define RET_PACKED_HANDLED 1


#define rq_packed_read(req) \
	((rq_data_dir(req) == READ)  && !(req->cmd_flags & (REQ_FLUSH | REQ_DISCARD)))
	
#define rq_packed_write(req) \
	((rq_data_dir(req) == WRITE)  && !(req->cmd_flags & (REQ_FLUSH | REQ_DISCARD | REQ_FUA | REQ_META)))

#define rq_is_packed(req) \
	(req->cmd_flags & REQ_PACKED)

#define start_packed_request(info, rw) \
	(rw == READ ? info->packed_read_requests[0] : info->packed_write_requests[0])
	
#define end_packed_request(info,  rw) \
	(rw == READ ? info->packed_read_requests[info->packed_read_nr-1] : info->packed_write_requests[info->packed_write_nr-1])
	
#define for_each_packed_rw_request(info, req, rw) \
	for (info->tmp_index = 0; info->tmp_index < (rw == READ ? info->packed_read_nr : info->packed_write_nr); info->tmp_index++) \
		if ((req = (rw == READ ? info->packed_read_requests[info->tmp_index] : info->packed_write_requests[info->tmp_index])))
	
	
/*
 * eMMC 4.5 PACKED COMMAND HEADER
 */
struct packed_cmd_hdr {
	u8 version;
	u8 rw;
	u8 num_entries;
	char pad[5];
	u32 args[63][2];
};

struct packed_cmd_info {
	struct packed_cmd_hdr *hdr;

	struct scatterlist *cur_sg;
	struct request **packed_read_requests;
	struct request **packed_write_requests;
	struct request **unpacked_requests;

	int packed_read_capacity;
	int packed_write_capacity;
	
	int packed_read_nr;
	int packed_write_nr;
	int unpacked_nr;

	int packed_read_sectors;
	int packed_write_sectors;

	int sg_len;

	int tmp_index;
};

struct mmc_queue {
	struct mmc_card		*card;
	struct task_struct	*thread;
	struct semaphore	thread_sem;
	unsigned int		flags;
#define MMC_QUEUE_PACKED_CMD (1 << 0)   /* Can queue support packed command */
#define MMC_QUEUE_SUSPENDED  (1 << 1)

	int			(*issue_fn)(struct mmc_queue *, struct request *);
	void			*data;
	struct request_queue	*queue;
	struct mmc_queue_req	mqrq[2];
	struct mmc_queue_req	*mqrq_cur;
	struct mmc_queue_req	*mqrq_prev;
	/* Jiffies until which disable packed command. */
	unsigned long		nopacked_period;
};

extern int mmc_init_queue(struct mmc_queue *, struct mmc_card *, spinlock_t *,
			  const char *);
extern void mmc_cleanup_queue(struct mmc_queue *);
extern void mmc_queue_suspend(struct mmc_queue *);
extern void mmc_queue_resume(struct mmc_queue *);

extern unsigned int mmc_queue_map_sg(struct mmc_queue *,
				     struct mmc_queue_req *);
extern void mmc_queue_bounce_pre(struct mmc_queue_req *);
extern void mmc_queue_bounce_post(struct mmc_queue_req *);

#define IS_RT_CLASS_REQ(x)     \
	(IOPRIO_PRIO_CLASS(req_get_ioprio(x)) == IOPRIO_CLASS_RT)

static inline void mmc_set_nopacked_period(struct mmc_queue *mq,
					unsigned long nopacked_jiffies)
{
	mq->nopacked_period = jiffies + nopacked_jiffies;
	smp_wmb();
}

static inline int mmc_is_nopacked_period(struct mmc_queue *mq)
{
	smp_rmb();
	return (int)time_is_after_jiffies(mq->nopacked_period);
}

#endif
