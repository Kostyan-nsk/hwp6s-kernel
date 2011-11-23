#include <linux/mount.h>
#include <linux/seq_file.h>
#include <linux/poll.h>

struct mnt_namespace {
	atomic_t		count;
	struct vfsmount *	root;
	struct list_head	list;
	wait_queue_head_t poll;
	int event;
};

struct mount {
	struct vfsmount mnt;
};

static inline struct mount *real_mount(struct vfsmount *mnt)
{
	return container_of(mnt, struct mount, mnt);
}

static inline int mnt_has_parent(struct vfsmount *mnt)
{
	return mnt != mnt->mnt_parent;
}

extern struct mount *__lookup_mnt(struct vfsmount *, struct dentry *, int);

static inline void get_mnt_ns(struct mnt_namespace *ns)
{
	atomic_inc(&ns->count);
}

struct proc_mounts {
	struct seq_file m; /* must be the first element */
	struct mnt_namespace *ns;
	struct path root;
	int (*show)(struct seq_file *, struct vfsmount *);
};

extern const struct seq_operations mounts_op;
