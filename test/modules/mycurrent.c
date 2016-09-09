#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zyc <zyc737347123@gmail.com>");

static int __init mycurrent_init(void)
{
	struct thread_info *ti;
	struct task_struct *mycurrent;

	ti=(struct thread_info *)(((unsigned long)&ti) & ~(0x1fff));// if stack is 8k
	mycurrent = ti->task;

	printk(KERN_INFO "%s %d\n",__func__,__LINE__);
	printk(KERN_INFO "mycurrent %s : %d\n",mycurrent->comm,mycurrent->pid);
	printk(KERN_INFO "current %s : %d\n",current->comm,current->pid);
}

static void __exit mycurrent_exit(void)
{
	printk(KERN_INFO "%s %d\n",__func__,__LINE__);
	printk(KERN_INFO "%s : %d\n",current->comm,current->pid);
		
}

module_init(mycurrent_init);
module_exit(mycurrent_exit);
