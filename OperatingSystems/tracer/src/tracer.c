#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/spinlock.h>
#include <linux/hashtable.h>
#include <linux/list.h>
#include <linux/kprobes.h>
#include <linux/miscdevice.h>
#include "tracer.h"

#define MAX_ACTIVE_NUM 32

struct process_stats {
    pid_t pid;
    int num_kmalloc;
    int num_kfree;
    int num_sched;
    int num_up;
    int num_down;
    int num_lock;
    int num_unlock;
    size_t kmalloc_mem;
    size_t kfree_mem;
    struct list_head mem_list;
    struct hlist_node hlist;
};

struct mem_info {
    size_t addr;
    size_t size;
    struct list_head list;
};

DEFINE_HASHTABLE(procs_table, 8);
DEFINE_SPINLOCK(table_lock);

static struct process_stats *get_proc(pid_t pid)
{
    struct process_stats *proc;

    hash_for_each_possible(procs_table, proc, hlist, pid) {
        if (proc->pid == pid) {
            return proc;
        }
	}

    return NULL;
}

static int tracer_print(struct seq_file *m, void *v)
{
    struct process_stats *proc;
    size_t bkt;

    seq_puts(m, "PID\tkmalloc\tkfree\tkmalloc_mem\tkfree_mem\tsched\tup\tdown\tlock\tunlock\n");

    spin_lock(&table_lock);
    hash_for_each(procs_table, bkt, proc, hlist)
        seq_printf(m, "%d\t%d\t%d\t%zd\t%zd\t%d\t%d\t%d\t%d\t%d\n",
            proc->pid, proc->num_kmalloc, proc->num_kfree, proc->kmalloc_mem, proc->kfree_mem,
            proc->num_sched, proc->num_up, proc->num_down, proc->num_lock, proc->num_unlock);
    spin_unlock(&table_lock);

    return 0;
}

static int tracer_read_open(struct inode *inode, struct file *file)
{
    return single_open(file, tracer_print, NULL);
}

static long tracer_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct process_stats *proc;
	struct mem_info *mem, *tmp_mem;

    switch (cmd) {
    case TRACER_ADD_PROCESS:
        proc = kmalloc(sizeof *proc, GFP_KERNEL);
        if (!proc) {
            return -ENOMEM;
		}
		memset(proc, 0, sizeof *proc);
    	
		proc->pid = arg;
		INIT_LIST_HEAD(&proc->mem_list);
    	
        spin_lock(&table_lock);
        hash_add(procs_table, &proc->hlist, arg);
        spin_unlock(&table_lock);

        break;

    case TRACER_REMOVE_PROCESS:
		proc = get_proc(arg);
		if (!proc) {
			return -EINVAL;
		}

		spin_lock(&table_lock);
		hash_del(&proc->hlist);
		list_for_each_entry_safe(mem, tmp_mem, &proc->mem_list, list) {
			list_del(&mem->list);
			kfree(mem);
		}
		kfree(proc);
        spin_unlock(&table_lock);

        break;

    default:
        pr_err("Undefined IOCTL command\n");
        return -EINVAL;
    }

    return 0;
}

static struct proc_dir_entry *tracer_read;

static const struct proc_ops pops = {
    .proc_open = tracer_read_open,
    .proc_read = seq_read,
    .proc_release = single_release
};


static const struct file_operations fops = {
    .unlocked_ioctl = tracer_ioctl
};

static struct miscdevice tracer_dev = {
    .minor = TRACER_DEV_MINOR,
    .name = TRACER_DEV_NAME,
    .fops = &fops
};

static int kmalloc_probe_entry_handler(struct kretprobe_instance *p, struct pt_regs *regs)
{
    *(size_t *)p->data = regs->ax;

    return 0;
}

static int kmalloc_probe_handler(struct kretprobe_instance *p, struct pt_regs *regs)
{
    size_t size = *(size_t *)p->data;
    size_t addr = regs_return_value(regs);
	struct mem_info *mem;

    struct process_stats *proc = get_proc(current->pid);
    if (!proc) {
        return -EINVAL;
    }

	mem = kmalloc(sizeof *mem, GFP_ATOMIC);
	if (!mem) {
        return -EINVAL;
	}
	mem->addr = addr;
    mem->size = size;

	spin_lock(&table_lock);
    list_add(&mem->list, &proc->mem_list);
	proc->num_kmalloc++;
    proc->kmalloc_mem += size;
	spin_unlock(&table_lock);
    
    return 0;
}

static int kfree_probe_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	size_t addr = regs->ax;
    struct mem_info *mem, *tmp_mem;

    struct process_stats *proc = get_proc(current->pid);
    if (!proc) {
        return -EINVAL;
    }

    spin_lock(&table_lock);
    list_for_each_entry_safe(mem, tmp_mem, &proc->mem_list, list) {
        if (addr == mem->addr) {
            proc->num_kfree++;
            proc->kfree_mem += mem->size;
	        list_del(&mem->list);
            kfree(mem);

            break;
        }
    }
    spin_unlock(&table_lock);

    return 0;
}

static int schedule_probe_handler(struct kretprobe_instance *p, struct pt_regs *regs)
{
    struct process_stats *proc = get_proc(current->pid);
    if (!proc) {
        return -EINVAL;
    } else {
        spin_lock(&table_lock);
        proc->num_sched++;
        spin_unlock(&table_lock);
    }

    return 0;
}

static int up_probe_handler(struct kretprobe_instance *p, struct pt_regs *regs)
{
    struct process_stats *proc = get_proc(current->pid);
    if (!proc) {
        return -EINVAL;
    } else {
        spin_lock(&table_lock);
        proc->num_up++;
        spin_unlock(&table_lock);
    }

    return 0;
}

static int down_probe_handler(struct kretprobe_instance *p, struct pt_regs *regs)
{
    struct process_stats *proc = get_proc(current->pid);
    if (!proc) {
        return -EINVAL;
    } else {
        spin_lock(&table_lock);
        proc->num_down++;
        spin_unlock(&table_lock);
    }

    return 0;
}

static int mutex_lock_probe_handler(struct kretprobe_instance *p, struct pt_regs *regs)
{
    struct process_stats *proc = get_proc(current->pid);
    if (!proc) {
        return -EINVAL;
    } else {
        spin_lock(&table_lock);
        proc->num_lock++;
        spin_unlock(&table_lock);
    }
    
	return 0;
}

static int mutex_unlock_probe_handler(struct kretprobe_instance *p, struct pt_regs *regs)
{
    struct process_stats *proc = get_proc(current->pid);
    if (!proc) {
        return -EINVAL;
    } else {
        spin_lock(&table_lock);
        proc->num_unlock++;
        spin_unlock(&table_lock);
    }

    return 0;
}

static struct kretprobe kmalloc_probe = {
    .kp = {.symbol_name = "__kmalloc"},
    .entry_handler = kmalloc_probe_entry_handler,
    .handler = kmalloc_probe_handler,
    .maxactive = MAX_ACTIVE_NUM
};

static struct kretprobe kfree_probe = {
    .kp = {.symbol_name = "kfree"},
    .entry_handler = kfree_probe_handler,
    .maxactive = MAX_ACTIVE_NUM
};

static struct kretprobe schedule_probe = {
    .kp = {.symbol_name = "schedule"},
    .entry_handler = schedule_probe_handler,
    .maxactive = MAX_ACTIVE_NUM
};

static struct kretprobe up_probe = {
    .kp = {.symbol_name = "up"},
    .entry_handler = up_probe_handler,
    .maxactive = MAX_ACTIVE_NUM
};

static struct kretprobe down_intr_probe = {
    .kp = {.symbol_name = "down_interruptible"},
    .entry_handler = down_probe_handler,
    .maxactive = MAX_ACTIVE_NUM
};

static struct kretprobe mutex_lock_probe = {
    .kp = {.symbol_name = "mutex_lock_nested"},
    .entry_handler = mutex_lock_probe_handler,
    .maxactive = MAX_ACTIVE_NUM
};

static struct kretprobe mutex_unlock_probe = {
    .kp = {.symbol_name = "mutex_unlock"},
    .entry_handler = mutex_unlock_probe_handler,
    .maxactive = MAX_ACTIVE_NUM
};

static int __init kretprobe_init(void)
{
    int ret = register_kretprobe(&kmalloc_probe);
    if (ret < 0) {
        pr_err("Failed to register probe for kmalloc");
        goto err_kmalloc_register;
    }

    ret = register_kretprobe(&kfree_probe);
    if (ret < 0) {
        pr_err("Failed to register probe for kfree");
        goto err_kfree_register;
    }

    ret = register_kretprobe(&schedule_probe);
    if (ret < 0) {
        pr_err("Failed to register probe for schedule");
        goto err_schedule_register;
    }

    ret = register_kretprobe(&up_probe);
    if (ret < 0) {
        pr_err("Failed to register probe for up");
        goto err_up_register;
    }

    ret = register_kretprobe(&down_intr_probe);
    if (ret < 0) {
        pr_err("Failed to register probe for down_interruptible");
        goto err_down_register;
    }

    ret = register_kretprobe(&mutex_lock_probe);
    if (ret < 0) {
        pr_err("Failed to register probe for mutex_lock_nested");
        goto err_lock_register;
    }

    ret = register_kretprobe(&mutex_unlock_probe);
    if (ret < 0) {
        pr_err("Failed to register probe for mutex_unlock");
        goto err_unlock_register;
    }

    ret = misc_register(&tracer_dev);
    if (ret) {
        pr_err("Failed to register misc device\n");
		goto err_misc_register;
    }

    tracer_read = proc_create(TRACER_DEV_NAME, 0000, NULL, &pops);
    if (!tracer_read) {
        pr_err("Failed to create proc\n");
		goto err_proc;
	}

    return 0;

err_proc:
    misc_deregister(&tracer_dev);

err_misc_register:
err_unlock_register:
    unregister_kretprobe(&mutex_unlock_probe);

err_lock_register:
    unregister_kretprobe(&mutex_lock_probe);

err_down_register:
    unregister_kretprobe(&down_intr_probe);

err_up_register:
    unregister_kretprobe(&up_probe);

err_schedule_register:
    unregister_kretprobe(&schedule_probe);

err_kfree_register:
    unregister_kretprobe(&kfree_probe);

err_kmalloc_register:
    unregister_kretprobe(&kmalloc_probe);

    return ret;
}

static void free_procs_table(void)
{
    int bkt;
    struct process_stats *proc;
    struct mem_info *mem, *tmp_mem;

    hash_for_each(procs_table, bkt, proc, hlist) {
        list_for_each_entry_safe(mem, tmp_mem, &proc->mem_list, list) {
            list_del(&mem->list);
            kfree(mem);
        }
        kfree(proc);
    }
}

static void __exit kretprobe_exit(void)
{
    proc_remove(tracer_read);
    misc_deregister(&tracer_dev);
    unregister_kretprobe(&kmalloc_probe);
    unregister_kretprobe(&kfree_probe);
    unregister_kretprobe(&schedule_probe);
    unregister_kretprobe(&up_probe);
    unregister_kretprobe(&down_intr_probe);
    unregister_kretprobe(&mutex_lock_probe);
    unregister_kretprobe(&mutex_unlock_probe);
    free_procs_table();
}

module_init(kretprobe_init);
module_exit(kretprobe_exit);

MODULE_DESCRIPTION("Linux kprobe based tracer");
MODULE_AUTHOR("Mihai-Alexandru Filisanu <mihaifilisanu@gmail.com>");
MODULE_LICENSE("GPL v2");
