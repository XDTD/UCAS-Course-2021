/*
 *  linux/kernel/blk_dev/ll_rw.c
 *
 * (C) 1991 Linus Torvalds
 */

/*
 * This handles all read/write requests to block devices
 */
#include <errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/system.h>

#include "blk.h"

/*
 * The request-struct contains all necessary data
 * to load a nr of sectors into memory
 */
struct request request[NR_REQUEST];

/*
 * used to wait on when there are no free requests
 */
struct task_struct * wait_for_request = NULL;

/* blk_dev_struct is:
 *	do_request-address
 *	next-request
 */
// 和盘打交道就需要和这个打交道
struct blk_dev_struct blk_dev[NR_BLK_DEV] = {
	{ NULL, NULL },		/* no_dev */
	{ NULL, NULL },		/* dev mem */
	{ NULL, NULL },		/* dev fd */
	{ NULL, NULL },		/* dev hd */
	{ NULL, NULL },		/* dev ttyx */
	{ NULL, NULL },		/* dev tty */
	{ NULL, NULL }		/* dev lp */
};

static inline void lock_buffer(struct buffer_head * bh)
{
	cli();  //这个中断只对当前进程有效
	while (bh->b_lock)  //第一次没加锁，往下走
		sleep_on(&bh->b_wait);  // 如果有锁就挂起等调度
	bh->b_lock=1;
	sti();
}

static inline void unlock_buffer(struct buffer_head * bh)
{
	if (!bh->b_lock)
		printk("ll_rw_block.c: buffer not locked\n\r");
	bh->b_lock = 0;
	wake_up(&bh->b_wait);
}

/*
 * add-request adds a request to the linked list.
 * It disables interrupts so that it can muck with the
 * request-lists in peace.
 */
static void add_request(struct blk_dev_struct * dev, struct request * req)
{
	struct request * tmp;

	req->next = NULL;
	cli();
	// 如果是设备的第一个，那就走下面一段
	if (req->bh)
		req->bh->b_dirt = 0;
	if (!(tmp = dev->current_request)) {
		dev->current_request = req;
		sti();
		(dev->request_fn)(); //指针函数，调用do_hd_request
		return;
	}
	// 之前有人弄过，就走下面一段
	// 电梯算法,尽量固定磁头
	for ( ; tmp->next ; tmp=tmp->next)
		if ((IN_ORDER(tmp,req) ||
		    !IN_ORDER(tmp,tmp->next)) &&
		    IN_ORDER(req,tmp->next))
			break;
	req->next=tmp->next;
	tmp->next=req;
	sti();
}

// 3 read bh
static void make_request(int major,int rw, struct buffer_head * bh)
{
	struct request * req; // 数组链
	int rw_ahead;

/* WRITEA/READA is special case - it is not really needed, so if the */
/* buffer is locked, we just forget about it, else it's a normal read */
	// 第一次跳过下面一段
	if (rw_ahead = (rw == READA || rw == WRITEA)) {
		if (bh->b_lock)
			return;
		if (rw == READA)
			rw = READ;
		else
			rw = WRITE;
	}
	// 从这开始
	if (rw!=READ && rw!=WRITE)
		panic("Bad block dev command, must be R/W/RA/WA"); // 如果既不是read也不是write出问题了
	lock_buffer(bh);  //b-lock = 1
	// 不脏并且要写，读过了而且又要读->别加锁了
	if ((rw == WRITE && !bh->b_dirt) || (rw == READ && bh->b_uptodate)) {
		unlock_buffer(bh);
		return;
	}
repeat:
/* we don't allow the write-requests to fill up the queue completely:
 * we want some room for reads: they take precedence. The last third
 * of the requests are only for reads.
 */
	// 读比写更着急
	if (rw == READ)
		req = request+NR_REQUEST;  // 32,读，数组地址偏移到最后，从后往前找
	else
		req = request+((NR_REQUEST*2)/3); // 写的时候要查找的地址只有2/3
	
/* find an empty request */
	while (--req >= request)
		if (req->dev<0)
			break;
	//没找到才走下面一段，第一次不走
/* if none found, sleep on new requests: check for rw_ahead */
	if (req < request) {
		if (rw_ahead) {
			unlock_buffer(bh);
			return;
		}
		sleep_on(&wait_for_request);
		goto repeat;
	}
	// 给请求项赋值
/* fill up the request-info, and add it to the queue */
	req->dev = bh->b_dev;
	req->cmd = rw;
	req->errors=0;
	req->sector = bh->b_blocknr<<1;
	req->nr_sectors = 2;
	req->buffer = bh->b_data;
	req->waiting = NULL;
	req->bh = bh;
	req->next = NULL;
	add_request(major+blk_dev,req);
}

void ll_rw_block(int rw, struct buffer_head * bh)
{
	unsigned int major;

	// Major是3
	// 前半段防止数组越界，一共6个设备不能超过这个数量
	// request_fn即已经挂载好的 do_hd_request
	// 第一次过来不进去
	if ((major=MAJOR(bh->b_dev)) >= NR_BLK_DEV || 
	!(blk_dev[major].request_fn)) {
		printk("Trying to read nonexistent block-device\n\r");
		return;
	}
	make_request(major,rw,bh);  // 3 read bh
}

void blk_dev_init(void)
{
	int i;
	// 数组组成的链表
	for (i=0 ; i<NR_REQUEST ; i++) {
		request[i].dev = -1;  // 初始化-1，没挂上设备
		request[i].next = NULL;  // 还没挂上
	}
}
