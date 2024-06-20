// SPDX-License-Identifier: GPL-2.0+

/*
 * list.c - Linux kernel list API
 *
 * Author: Mihai-Alexandru Filisanu <mihaifilisanu@gmail.com>
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>

#define PROCFS_MAX_SIZE		512
#define COMMAND_SIZE		5
#define NAME_SIZE		    128

#define procfs_dir_name		"list"
#define procfs_file_read	"preview"
#define procfs_file_write	"management"

#define ADD_TOP_COMMAND 	"addf"
#define ADD_END_COMMAND 	"adde"
#define DEL_FIRST_COMMAND 	"delf"
#define DEL_ALL_COMMAND 	"dela"

struct proc_dir_entry *proc_list;
struct proc_dir_entry *proc_list_read;
struct proc_dir_entry *proc_list_write;

struct name_list {
	char *name;
	struct list_head list;
};

LIST_HEAD(name_head);
DEFINE_SPINLOCK(lock);

static int list_proc_show(struct seq_file *m, void *v)
{
	struct name_list *pos;

	spin_lock(&lock);
	list_for_each_entry(pos, &name_head, list) {
        seq_printf(m, "%s\n", pos->name);
    }
    spin_unlock(&lock);

	return 0;
}

static int list_read_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static int list_write_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

/*
 * add - add a new element to the list
 * @name: the name of the new element
 * @top: if 1, add the element to the top of the list, else add it to the end
 *
 * Return: 0 on success, -ENOMEM on failure
 */
static int add(const char *name, int top)
{
	size_t len = strlen(name);
    struct name_list *new_name = kmalloc(sizeof *new_name, GFP_KERNEL);
    if (!new_name) {
        return -ENOMEM;
	}

	new_name->name = kmalloc(len, GFP_KERNEL);
    if (!new_name->name) {
        kfree(new_name);
        return -ENOMEM;
    }

	strcpy(new_name->name, name);
	
	spin_lock(&lock);
	top ? list_add(&new_name->list, &name_head) : list_add_tail(&new_name->list, &name_head);
	spin_unlock(&lock);

	return 0;
}

/*
 * delete - delete an element from the list
 * @name: the name of the element to be deleted
 * @delete_all: if 1, delete all occurrences of the element, else delete only the first occurrence
 */
void delete(const char *name, int delete_all) {
    struct name_list *itr, *tmp;

    spin_lock(&lock);
    list_for_each_entry_safe(itr, tmp, &name_head, list) {
        if (strcmp(itr->name, name) == 0) {
            list_del(&itr->list);
            kfree(itr);
            if (!delete_all)
                break;
        }
    }
    spin_unlock(&lock);
}

static ssize_t list_write(struct file *file, const char __user *buffer,
			  size_t count, loff_t *offs)
{
	char local_buffer[PROCFS_MAX_SIZE];
	unsigned long local_buffer_size = 0;
	char command[COMMAND_SIZE];
	char name[NAME_SIZE];

	local_buffer_size = count;
	if (local_buffer_size > PROCFS_MAX_SIZE)
		local_buffer_size = PROCFS_MAX_SIZE;

	memset(local_buffer, 0, PROCFS_MAX_SIZE);
	if (copy_from_user(local_buffer, buffer, local_buffer_size))
		return -EFAULT;

	sscanf(local_buffer, "%s %s", command, name);

	if (strcmp(command, ADD_TOP_COMMAND) == 0) {
		add(name, 1);
	} else if (strcmp(command, ADD_END_COMMAND) == 0) {
		add(name, 0);
	} else if (strcmp(command, DEL_FIRST_COMMAND) == 0) {
		delete(name, 0);
	} else if (strcmp(command, DEL_ALL_COMMAND) == 0) {
		delete(name, 1);
	} else {
		return -EINVAL;
	}

	return local_buffer_size;
}

static const struct proc_ops r_pops = {
	.proc_open		= list_read_open,
	.proc_read		= seq_read,
	.proc_release	= single_release,
};

static const struct proc_ops w_pops = {
	.proc_open		= list_write_open,
	.proc_write		= list_write,
	.proc_release	= single_release,
};

static int list_init(void)
{
	proc_list = proc_mkdir(procfs_dir_name, NULL);
	if (!proc_list)
		return -ENOMEM;

	proc_list_read = proc_create(procfs_file_read, 0000, proc_list,
				     &r_pops);
	if (!proc_list_read)
		goto proc_list_cleanup;

	proc_list_write = proc_create(procfs_file_write, 0000, proc_list,
				      &w_pops);
	if (!proc_list_write)
		goto proc_list_read_cleanup;

	return 0;

proc_list_read_cleanup:
	proc_remove(proc_list_read);
proc_list_cleanup:
	proc_remove(proc_list);
	return -ENOMEM;
}

static void list_exit(void)
{
	proc_remove(proc_list);
}

module_init(list_init);
module_exit(list_exit);

MODULE_DESCRIPTION("Linux kernel list API");
MODULE_AUTHOR("Mihai-Alexandru Filisanu <mihaifilisanu@gmail.com>");
MODULE_LICENSE("GPL v2");
