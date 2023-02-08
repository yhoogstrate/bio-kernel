#include <linux/init.h>
#include <linux/module.h>

static int __init biokernel_init(void)
{
	pr_debug("biokernel module loaded\n");
	
	return 0;
}

static void __exit biokernel_exit(void)
{
	pr_debug("biokernel module unloaded\n");
}



module_init(biokernel_init);
module_exit(biokernel_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("0.01");
MODULE_AUTHOR("Youri Hoogstate");

