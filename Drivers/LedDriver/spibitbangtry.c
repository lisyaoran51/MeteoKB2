#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/fs.h>      
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/gpio.h>
#include <linux/version.h>
#include <linux/mutex.h>

/* test hrtimer */

#include <linux/slab.h>
#include <linux/time.h>
#include <asm/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/time.h>

//#include "bcm2835.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple Blocking IO device RaspPi");


static struct hrtimer hr_timer2;



enum hrtimer_restart enHRTimer=HRTIMER_NORESTART;
s64 i64TimeInNsec = 1000000 * NSEC_PER_USEC;
static int hrtimer_count = 0;
struct  timeval hrtimer_call_time[100];






/* test hrtimer */

/*-------------------------------------------------------------------------*/

#define MY_MAJOR 201
#define MY_MINOR 0
#define MY_DEV_COUNT 1

#define MY_DEV_NAME "my_blocking_io_dev"
#define MY_INT_NAME "my_blocking_io_int"

//data DS 
//#define DI_PIN RPI_GPIO_P1_18 
//#define DI_PIN RPI_BPLUS_GPIO_J8_37 
#define DI_PIN 10 

//clock SH_CP
//#define CL_PIN RPI_GPIO_P1_16
//#define CL_PIN RPI_BPLUS_GPIO_J8_33 
#define CL_PIN 11

//latch ST_CP
//#define CE_PIN RPI_GPIO_P1_22
//#define CE_PIN RPI_BPLUS_GPIO_J8_35
#define CE_PIN 8

static int     my_open(struct inode *, struct file *);
static ssize_t my_write(struct file *, const  char *, size_t, loff_t *);
static int     my_close(struct inode *, struct file *);

struct file_operations my_fops = {
		write : my_write,
		open : my_open,
		release : my_close,
		owner : THIS_MODULE
};

struct cdev my_cdev;
static char   *msg = NULL;
static unsigned char   **user_map = NULL;
static bool changed = false;
static unsigned char   user_map2[16][48];
static int col = 0;




struct bitbang_spi_led {
	struct hrtimer hr_timer;
	spinlock_t		map_lock;
	bool** map;// [16][48];
	int column;

};

enum hrtimer_restart my_hrtimer_callback(struct hrtimer *hr_timer)
{
	printk("my_hrtimer_callback!\n"); 
	struct bitbang_spi_led *spi_led = container_of(hr_timer, struct bitbang_spi_led, hr_timer);
	bool** map = spi_led->map;
	//switchRowSequencely(col);

	gpio_set_value(CE_PIN, 0);
	//bcm2835_gpio_write(CE_PIN, LOW);

	int row;

	for (row = 0; row < 48; row++) {
		gpio_set_value(CL_PIN, 0);
		//bcm2835_gpio_write(CL_PIN, LOW);
		if (map[col][row])
			gpio_set_value(DI_PIN, 0);
			//bcm2835_gpio_write(DI_PIN, LOW);
		else
			gpio_set_value(DI_PIN, 1);
			//bcm2835_gpio_write(DI_PIN, HIGH);


		gpio_set_value(CL_PIN, 1);
		//bcm2835_gpio_write(CL_PIN, HIGH);
	}
	gpio_set_value(CE_PIN, 1);
	//bcm2835_gpio_write(CE_PIN, HIGH);

	hrtimer_forward(hr_timer, hrtimer_cb_get_time(hr_timer), ktime_set(0, i64TimeInNsec));

	col++;

	if (col == 16) {

		col = 0;
		if (changed) {
			int i;
			for (i = 0; i < 16; i++) {
				int j;
				for (j = 0; j < 48; j++) {
					if (user_map[i][j] > 0)
						map[i][j] = true;
					else
						map[i][j] = false;
				}
			}

		}

	}


	return enHRTimer;
}

static ssize_t
my_write(struct file *filp, const char __user *buf,
	size_t count, loff_t *f_pos) {

	int len;

	memset(*user_map, 0, sizeof(unsigned char) * 16 * 48);
	len = copy_from_user((char*)*user_map, buf, count);
	changed = true;

	return len;
}

static int my_open(struct inode *inod, struct file *filp)
{
	int major;
	int minor;

	major = imajor(inod);
	minor = iminor(inod);
	printk("\n*****Some body is opening me at major %d  minor %d*****\n", major, minor);

	return 0;
}

static int my_close(struct inode *inod, struct file *filp)
{
	int major, minor;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
	major = MAJOR(filp->f_inode->i_rdev);
	minor = MINOR(filp->f_inode->i_rdev);
#else
	major = MAJOR(filp->f_dentry->d_inode->i_rdev);
	minor = MINOR(filp->f_dentry->d_inode->i_rdev);
#endif
	printk("*****Some body is closing me at major %d*****\n", minor);

	return 0;
}

int init_module(void)
{

	printk("*****LED GPIO Init ******************\n");
	if (gpio_request(DI_PIN, "DI_PIN")) return -1;
	if (gpio_request(CL_PIN, "CL_PIN")) return -1;
	if (gpio_request(CE_PIN, "CE_PIN")) return -1;

	gpio_direction_output(DI_PIN, 0);
	gpio_direction_output(CL_PIN, 0);
	gpio_direction_output(CE_PIN, 0);

	int err;
	dev_t devno;
	unsigned int count = MY_DEV_COUNT;

	printk("setup char device \n");
	// -- register device number  
	devno = MKDEV(MY_MAJOR, MY_MINOR);
	register_chrdev_region(devno, MY_DEV_COUNT, MY_DEV_NAME);

	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;
	err = cdev_add(&my_cdev, devno, count);

	struct bitbang_spi_led 	*spi_led;
	/* Allocate timer data */
	spi_led = kzalloc(sizeof(struct bitbang_spi_led*), GFP_KERNEL);

	printk("setup user map \n");

	user_map = (unsigned char **)kmalloc(sizeof(unsigned char*) * 16, GFP_KERNEL);
	unsigned char * p_data = (unsigned char *)kmalloc(sizeof(unsigned char) * 48, GFP_KERNEL);
	int i;
	for (i = 0; i < 16; i++, p_data += 48)
		user_map[i] = p_data;

	for (i = 0; i < 16; i++) {
		int j;
		for (j = 0; j < 48; j++) {
			user_map[i][j] = 0x00;
		}
	}

	if (user_map != NULL)
		printk("malloc allocator address: 0x%p\n", user_map);

	printk("setup spi map \n");

	spi_led->map = (bool **)kmalloc(sizeof(bool*) * 16, GFP_KERNEL);
	bool * p_data2 = (bool *)kmalloc(sizeof(bool) * 48, GFP_KERNEL);
	
	for (i = 0; i < 16; i++, p_data2 += 48)
		spi_led->map[i] = p_data2;

	for (i = 0; i < 16; i++) {
		int j;
		for (j = 0; j < 48; j++) {
			spi_led->map[i][j] = false;
		}
	}

	/* hrtimer test */

	ktime_t kt;

	enHRTimer = HRTIMER_RESTART;

	printk("setup hr timer \n");


	//HRT init  
	kt = ktime_set(0, i64TimeInNsec * 1000L); // 等spidev初始化
	hrtimer_init(&spi_led->hr_timer, CLOCK_REALTIME, HRTIMER_MODE_ABS);
	hrtimer_set_expires(&spi_led->hr_timer, kt);
	spi_led->hr_timer.function = &my_hrtimer_callback;


	hrtimer_start(&spi_led->hr_timer, kt, HRTIMER_MODE_ABS);
	/* hrtimer test */


	/*
	// -- setup GPIO for button
	msg = (char *)kmalloc(32, GFP_KERNEL);
	if (msg != NULL)
		printk("malloc allocator address: 0x%p\n", msg);

	// -- initialize the WAIT QUEUE head
	init_waitqueue_head(&my_wait_queue);

	// -- setup the INTERRUPT
	my_interrupt_config();
	*/

	// -- other handle
	if (err < 0)
	{
		printk("Device Add Error\n");
		return -1;
	}

	printk("'mknod /dev/myBR_file c %d 0'.\n", MY_MAJOR);
	return 0;
}

// -- MODULE END
void cleanup_module(void)
{
	dev_t devno;
	gpio_free(DI_PIN);
	gpio_free(CL_PIN);
	gpio_free(CE_PIN);


	devno = MKDEV(MY_MAJOR, MY_MINOR);
	unregister_chrdev_region(devno, MY_DEV_COUNT);
	cdev_del(&my_cdev);
}




