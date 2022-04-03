/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"
DZ_WAITQ	pipe_reader_waitq;
DZ_WAITQ	pipe_writer_waitq;
SPINLOCK	pipe_space_spinlock;

struct semaphore pipe_space_semaphore;
struct semaphore pipe_reader_semaphore;
struct semaphore pipe_writer_semaphore;

INT			pipe_size	= PAGE_SIZE;
INT			pipe_incount	= -1;
INT			pipe_outcount	= -1;

#define		IF_PIPE_FULL()	if (((pipe_incount == (PAGE_SIZE - 1)) && pipe_outcount == -1 )|| (pipe_incount ==  (pipe_outcount - 1)))

#define		IF_PIPE_EMPTY()	if ((pipe_incount == -1 && pipe_outcount == -1) || (pipe_incount == pipe_outcount )) 

PCHAR dz_pipe_init(char *pipebuf, char *pipename)
{

	pipe_size	= PAGE_SIZE;
	pipe_incount	= -1;
	pipe_outcount	= -1;
	//TODO:memset(&pipebuf, 0, pipe_size);

	//Spinlocks Initializations
	spin_lock_init(&pipe_space_spinlock);

	spin_lock_init(&pipe_reader_waitq.waitq_lock);
	INIT_LIST_HEAD(&pipe_reader_waitq.waitq_head);

	spin_lock_init(&pipe_writer_waitq.waitq_lock);
	INIT_LIST_HEAD(&pipe_writer_waitq.waitq_head);

	//Semaphore Initializations
	sema_init(&pipe_space_semaphore, 1);
	sema_init(&pipe_writer_semaphore, 0);
	sema_init(&pipe_reader_semaphore, 0);

	return (pipebuf = dz_kmalloc(pipe_size, GFP_KERNEL));
}

INT dz_pipe_read_core_with_semaphore(char *pipebuf, char *databuf, INT datalen)
{
	INT data_read = 0;
	INT data_left = 0;
	INT	data_read_start = 0;

start_read_with_semaphore:
	down(&pipe_space_semaphore);

	IF_PIPE_EMPTY() {

		up(&pipe_space_semaphore);

		//Put this reader thread into sleepq
		down(&pipe_reader_semaphore);
		goto start_read_with_semaphore;

	} else {
		data_read_start = pipe_outcount;
		data_read = data_read_start + datalen;
		if (data_read > pipe_size - 1) {
			data_left = data_read - pipe_size;
		} else {
			pipe_outcount = data_read;
			data_left	 = datalen;
		}
	}
	up(&pipe_space_semaphore);
	memcpy(databuf, pipebuf + data_read_start, data_read);  

	//WakeUP any Writer threads
	up(&pipe_writer_semaphore);
	return data_left;
}


INT dz_pipe_read_core_with_spinlock(char *pipebuf, char *databuf, INT datalen)
{
	INT data_read = 0;
	INT data_left = 0;
	INT	data_read_start = 0;

	//Used for waking up writer threads present in writer sleeping q
	DZ_WAITQ_ITEM	*waiter_write = NULL;

	//Used for enqueing current thread in sleepq
	DZ_WAITQ_ITEM	*waiter_read = dz_kmalloc(sizeof(DZ_WAITQ_ITEM), GFP_KERNEL); 

start_read_with_spinlock:
	spin_lock(&pipe_space_spinlock);

	IF_PIPE_EMPTY() {

		spin_lock(&pipe_reader_waitq.waitq_lock);
		waiter_read->wait_task = current;
		list_add_tail(&waiter_read->wait_item, &pipe_reader_waitq.waitq_head);
		spin_unlock(&pipe_reader_waitq.waitq_lock);

		spin_unlock(&pipe_space_spinlock); //Release space_lock
		schedule(); //Put this thread to sleep in reader waitq
		goto start_read_with_spinlock;

	} else {
		data_read_start = pipe_outcount;
		data_read = data_read_start + datalen;
		if (data_read > pipe_size - 1) {
			data_left = data_read - pipe_size;
		} else {
			pipe_outcount = data_read;
			data_left	 = datalen;
		}
	}
	spin_unlock(&pipe_space_spinlock);
	memcpy(databuf, pipebuf + data_read_start, data_read);  

	//WakeUP any Writer threads
	spin_lock(&pipe_writer_waitq.waitq_lock);
	if (list_empty(&pipe_writer_waitq.waitq_head)) {
		spin_unlock(&pipe_writer_waitq.waitq_lock);
		return data_left;
	} else {
		for(;;) {
			waiter_write = list_first_entry(&pipe_writer_waitq.waitq_head, struct dz_waitq_item_s, wait_item);
			if (waiter_write) {
				list_del(&waiter_write->wait_item);
				wake_up_process(waiter_write->wait_task);
				waiter_write = NULL;
			} else {
				break;
			}
		}
	}
	spin_unlock(&pipe_writer_waitq.waitq_lock);

	return data_left;
}

INT dz_pipe_read(char *pipebuf, char *databuf, INT datalen)
{
	INT data_read = 0;

	while(true) {
		data_read = dz_pipe_read_core_with_spinlock(pipebuf, databuf, datalen);
		if (data_read == datalen) {
			return data_read;
		} else {
			datalen = datalen - data_read;
		}
	}
	return 0;
}

INT dz_pipe_write_core_with_semaphore(char *pipebuf, char *databuf, INT datalen)
{
	INT data_written = 0;
	INT data_left = 0;
	INT	data_write_start = 0;

start_write_with_semaphore:
	down(&pipe_space_semaphore);

	IF_PIPE_FULL() {
		up(&pipe_space_semaphore);
		down(&pipe_writer_semaphore);
		goto start_write_with_semaphore;

	} else {

		data_write_start = pipe_incount;
		data_written = pipe_incount + datalen;	
		if (data_written > pipe_size - 1) {
			data_left = data_written - pipe_size;
		} else {
			pipe_incount = data_written;
			data_left	 = datalen;
		}
	}
	up(&pipe_space_semaphore);

	memcpy(pipebuf + data_write_start, databuf, data_written);  

	//WakeUP any reader threads
	up(&pipe_reader_semaphore);
	return data_left;
}

INT dz_pipe_write_core_with_spinlock(char *pipebuf, char *databuf, INT datalen)
{
	INT data_written = 0;
	INT data_left = 0;
	INT	data_write_start = 0;

	//Used for waking up reader threads present in reader sleeping q
	DZ_WAITQ_ITEM	*waiter_read = NULL;

	//Used for enqueing current thread in sleepq
	DZ_WAITQ_ITEM	*waiter_write = dz_kmalloc(sizeof(DZ_WAITQ_ITEM), GFP_KERNEL); 

start_write_with_spinlock:
	spin_lock(&pipe_space_spinlock);

	IF_PIPE_FULL() {

		spin_lock(&pipe_writer_waitq.waitq_lock);
		waiter_write->wait_task = current;
		list_add_tail(&waiter_write->wait_item, &pipe_writer_waitq.waitq_head);
		spin_unlock(&pipe_writer_waitq.waitq_lock);

		spin_unlock(&pipe_space_spinlock); //Release space_lock
		schedule(); //Put this thread to sleep in writer waitq
		goto start_write_with_spinlock;

	} else {

		data_write_start = pipe_incount;
		data_written = pipe_incount + datalen;	
		if (data_written > pipe_size - 1) {
			data_left = data_written - pipe_size;
		} else {
			pipe_incount = data_written;
			data_left	 = datalen;
		}
	}
	spin_unlock(&pipe_space_spinlock);

	memcpy(pipebuf + data_write_start, databuf, data_written);  

	//WakeUP any reader threads
	spin_lock(&pipe_reader_waitq.waitq_lock);
	if (list_empty(&pipe_reader_waitq.waitq_head)) {
		spin_unlock(&pipe_reader_waitq.waitq_lock);
		return data_left;
	} else {
		for(;;) {
			waiter_read = list_first_entry(&pipe_reader_waitq.waitq_head, struct dz_waitq_item_s, wait_item);
			if (waiter_read) {
				list_del(&waiter_read->wait_item);
				wake_up_process(waiter_read->wait_task);
				waiter_read = NULL;
			} else {
				break;
			}
		}
	}
	spin_unlock(&pipe_reader_waitq.waitq_lock);

	return data_left;
}

INT dz_pipe_write(char *pipebuf, char *databuf, INT datalen)
{
	INT data_written = 0;

	while(true) {
		data_written = dz_pipe_write_core_with_spinlock(pipebuf, databuf, datalen);
		if (data_written == datalen) {
			return data_written;
		} else {
			datalen = datalen - data_written;
		}
	}
}

BOOL dz_process_lab_engine_pipe(LBA offset, UINT iosize)
{
	

	return true;
}

EXPORT_SYMBOL(dz_process_lab_engine_pipe);
