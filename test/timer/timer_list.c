#include<linux/timer.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zyc <zyc737347123@gmail.com>");

void my_fun(unsigned long data);

static int __init mytimer_init(void)
{
	struct timer_list my_timer;
	init_timer(&my_timer);
	my_timer.expires=jiffies+5*HZ;
	my_timer.data=0;
	my_timer.function=my_fun;

	add_timer(&my_timer);
}

static void __exit mytimer_exit(void)
{
	printk(KERN_INFO "%s %d\n",__func__,__LINE__);
}

void my_fun(unsigned long data)
{
	printk(KERN_INFO "my timer try\n");
	printk(KERN_INFO "%s : %d\n",current->comm,current->pid);
}

module_init(mytimer_init);
module_exit(mytimer_exit);
