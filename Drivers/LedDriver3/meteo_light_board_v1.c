/*******************************************************************************
* Copyright (c) 2015 ittraining
*
* All rights reserved.
* This program is free to use, but the ban on selling behavior.
* Modify the program must keep all the original text description.
*
* Email: service@ittraining.com.tw
* Blog : http://blog.ittraining.com.tw
* Web:  http://www.ittraining.com.tw 
*******************************************************************************/


#include <linux/module.h>   
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/time.h>
//#include "../include/it_shield.h"

#define MY_MAJOR 201
#define MY_MINOR 0
#define MY_DEV_COUNT 1

#define MY_DEV_NAME "meteo_lightboard_v1_dev"

#define MY_GPIO_INT_NAME "autoclock_int"

#define MY_INTERRUPT_IN 21
#define MY_OUTPUT 20

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");


#include<linux/ktime.h>
ktime_t calltime, delta, rettime;

/*--------------------------------character device--------------------------------*/


#include <linux/cdev.h>
#include <linux/fs.h>    
#include <linux/uaccess.h>
#include <linux/version.h>

static int     light_board_open( struct inode *, struct file * );
static ssize_t light_board_write(struct file * , const  char *  , size_t, loff_t *);
static int     light_board_close(struct inode *, struct file * );

struct file_operations my_fops = {
        write   :       light_board_write,
        open    :       light_board_open,
        release :       light_board_close,
        owner   :       THIS_MODULE
};

struct cdev light_board_cdev;
static char *msg=NULL;

/*--------------------------------character device--------------------------------*/
/*--------------------------------SPI--------------------------------*/
#include <linux/slab.h> // kmalloc
#define DI_PIN 10 
#define CL_PIN 11
#define CE_PIN 8
#define OE_PIN 7

#define PA_PIN 17
#define PB_PIN 27
#define NA_PIN 22
#define NB_PIN 23

struct bitbang_spi_led {
	spinlock_t		map_lock;
	bool** map;// [16][48];
	int column;
};
struct bitbang_spi_led 	spi_led;

void switch_row_sequencely(int row);

/*--------------------------------SPI--------------------------------*/
/*--------------------------------IRQ--------------------------------*/

static short int test_irq = 0;
static unsigned long flags = 0;

static irqreturn_t row_print_isr(int irq, void *data)
{
	local_irq_save(flags);

	bool** map = spi_led.map; 

	gpio_set_value(CE_PIN, 0);
	gpio_set_value(DI_PIN, 0);
	int row; 
	for (row = 0; row < 48; row++){
		gpio_set_value(CL_PIN, 0);
		gpio_set_value(CL_PIN, 1);
	}
	gpio_set_value(CE_PIN, 1);
	switch_row_sequencely(spi_led.column);


	gpio_set_value(CE_PIN, 0);
	int col = spi_led.column;
	
	for (row = 0; row < 48; row++) {
		gpio_set_value(CL_PIN, 0);
		if (map[col][47 - row])
			gpio_set_value(DI_PIN, 1);
		else
			gpio_set_value(DI_PIN, 0);

		gpio_set_value(CL_PIN, 1);
	}
	
	
	gpio_set_value(CE_PIN, 1);

	spi_led.column++;
	if (spi_led.column == 16) {

		rettime = ktime_get();
		s64 actual_time = ktime_to_ns(ktime_sub(rettime, calltime));
		//printk("%lld\n", (long long)actual_time);
		spi_led.column = 0;
	}

	local_irq_restore(flags);
	return IRQ_HANDLED;
}

/*------------------------------------------IRQ--------------------------------------*/
/*--------------------------------------hrtimer--------------------------------------*/


#define MY_AUTO_OUTPUT 20

#include <linux/hrtimer.h>
enum hrtimer_restart enHRTimer = HRTIMER_NORESTART;
s64 i64TimeInNsec = 1000 * NSEC_PER_USEC;
static struct hrtimer hr_timer;
static int hrtimer_count = 0;

enum hrtimer_restart my_hrtimer_callback(struct hrtimer *hr_timer)
{
	gpio_set_value(MY_AUTO_OUTPUT, 1);
	calltime = ktime_get();
	hrtimer_count++;
	if (hrtimer_count == 1000) {
		//printk("hrtimer 1000\n");
		hrtimer_count = 0;
	}
	hrtimer_forward(hr_timer, hrtimer_cb_get_time(hr_timer), ktime_set(0, i64TimeInNsec));
	gpio_set_value(MY_AUTO_OUTPUT, 0);

	return enHRTimer;
}

/*--------------------------------------hrtimer--------------------------------------*/

void makemap(void);

int init_module(void)
{
	int err;
	dev_t devno;
	unsigned int count = MY_DEV_COUNT;

	printk("setup char device \n");
	// -- register device number  
	devno = MKDEV(MY_MAJOR, MY_MINOR);
	register_chrdev_region(devno, MY_DEV_COUNT , MY_DEV_NAME);

    cdev_init(&light_board_cdev, &my_fops);
	light_board_cdev.owner = THIS_MODULE;
	err = cdev_add(&light_board_cdev, devno, count);
	
	// -- setup GPIO for button
	msg = (char *)kmalloc(96, GFP_KERNEL);
	if (msg !=NULL)
		printk("malloc allocator address: 0x%p\n", msg);
	
	// -- other handle
	if (err < 0)
	{
		printk("Device Add Error\n");
		return -1;
	}

	printk("'mknod /dev/meteo_lightboard_v1 c %d 0'.\n", MY_MAJOR);
	
	
	/*bitbang*/
	printk("*****LED GPIO Init ******************\n");
	if (gpio_request(DI_PIN, "DI_PIN")) return -1;
	if (gpio_request(CL_PIN, "CL_PIN")) return -1;
	if (gpio_request(CE_PIN, "CE_PIN")) return -1;
	if (gpio_request(OE_PIN, "OE_PIN")) return -1;

	if (gpio_request(PA_PIN, "PA_PIN")) return -1;
	if (gpio_request(PB_PIN, "PB_PIN")) return -1;
	if (gpio_request(NA_PIN, "NA_PIN")) return -1;
	if (gpio_request(NB_PIN, "NB_PIN")) return -1;

	gpio_direction_output(DI_PIN, 0);
	gpio_direction_output(CL_PIN, 0);
	gpio_direction_output(CE_PIN, 0);
	gpio_direction_output(OE_PIN, 0);

	gpio_direction_output(PA_PIN, 0);
	gpio_direction_output(PB_PIN, 0);
	gpio_direction_output(NA_PIN, 0);
	gpio_direction_output(NB_PIN, 0);

	spi_led.map = (bool **)kmalloc(sizeof(bool*) * 16, GFP_KERNEL);
	bool * p_data = (bool *)kmalloc(sizeof(bool) * 16 * 48, GFP_KERNEL);
	
	int i;
	for (i = 0; i < 16; i++, p_data += 48)
		spi_led.map[i] = p_data;

	spi_led.column = 0;



	///!!!!!!!!!!!!!!!!!!
	makemap();///!!!!!!!!
	///!!!!!!!!!!!!!!!!!!

	/*bitbang end*/

	printk("module: setting my irq.\n");
	if (!gpio_is_valid(MY_INTERRUPT_IN)) return -1;
	if (gpio_request(MY_INTERRUPT_IN, "MY_INTERRUPT_IN")) return -1;
	if ((test_irq = gpio_to_irq(MY_INTERRUPT_IN)) < 0)  return -1;
	if (request_irq(test_irq, row_print_isr, IRQF_TRIGGER_RISING, MY_GPIO_INT_NAME, MY_DEV_NAME)) return -1;


	printk("module: set auto output.\n");
	if (!gpio_is_valid(MY_AUTO_OUTPUT)) return -1;
	if (gpio_request(MY_AUTO_OUTPUT, "MY_AUTO_OUTPUT")) return -1;
	gpio_direction_output(MY_AUTO_OUTPUT, 0);
	gpio_set_value(MY_AUTO_OUTPUT, 0);

	/* hrtimer test */

	ktime_t kt;
	enHRTimer = HRTIMER_RESTART;
	printk("setup hr timer \n");

	//HRT init  
	kt = ktime_set(0, i64TimeInNsec * 1000L); // µ¥spidevªì©l¤Æ
	hrtimer_init(&hr_timer, CLOCK_REALTIME, HRTIMER_MODE_ABS);
	hrtimer_set_expires(&hr_timer, kt);
	hr_timer.function = &my_hrtimer_callback;

	hrtimer_start(&hr_timer, kt, HRTIMER_MODE_ABS);
	/* hrtimer test */

	return 0;

}


void cleanup_module(void)
{
	free_irq(test_irq, MY_DEV_NAME);
	gpio_free(MY_INTERRUPT_IN);

	gpio_free(MY_AUTO_OUTPUT);

	gpio_free(DI_PIN);
	gpio_free(CL_PIN);
	gpio_free(CE_PIN);
	gpio_free(PA_PIN);
	gpio_free(PB_PIN);
	gpio_free(NA_PIN);
	gpio_free(NB_PIN);
}


void makemap() {
	int i;
	for (i = 0; i < 16; i++) {
		int j;
		for (j = 0; j < 48; j++) {
			spi_led.map[i][j] = false;
		}
	}

	for (i = 0; i < 16; i++) {
		spi_led.map[i][i] = true;
	}


}

void switch_row_sequencely(int row)
{
	switch (row) {
	case 0:
		gpio_set_value(PB_PIN, 0);
		gpio_set_value(NB_PIN, 0);
		break;

	case 1:
		gpio_set_value(NA_PIN, 1);
		break;

	case 2:
		gpio_set_value(NB_PIN, 1);
		break;

	case 3:
		gpio_set_value(NA_PIN, 0);
		break;

	case 4:
		gpio_set_value(PA_PIN, 1);
		gpio_set_value(NB_PIN, 0);
		break;

	case 5:
		gpio_set_value(NA_PIN, 1);
		break;

	case 6:
		gpio_set_value(NB_PIN, 1);
		break;

	case 7:
		gpio_set_value(NA_PIN, 0);
		break;

	case 8:
		gpio_set_value(PB_PIN, 1);
		gpio_set_value(NB_PIN, 0);
		break;

	case 9:
		gpio_set_value(NA_PIN, 1);
		break;

	case 10:
		gpio_set_value(NB_PIN, 1);
		break;

	case 11:
		gpio_set_value(NA_PIN, 0);
		break;

	case 12:
		gpio_set_value(PA_PIN, 0);
		gpio_set_value(NB_PIN, 0);
		break;

	case 13:
		gpio_set_value(NA_PIN, 1);
		break;

	case 14:
		gpio_set_value(NB_PIN, 1);
		break;

	case 15:
		gpio_set_value(NA_PIN, 0);
		break;
	}
	return 0;
}

/*--------------------------------character device--------------------------------*/

static int light_board_open(struct inode *inod, struct file *filp)
{
	int major;
	int minor;

	major = imajor(inod);
	minor = iminor(inod);
	printk("\n*****Some body is opening me at major %d  minor %d*****\n",major, minor);

	return 0;
}


static int light_board_close(struct inode *inod, struct file *filp)
{
	int major, minor;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
	major = MAJOR(filp->f_inode->i_rdev);
	minor = MINOR(filp->f_inode->i_rdev);
#else
	major = MAJOR(filp->f_dentry->d_inode->i_rdev);
	minor = MINOR(filp->f_dentry->d_inode->i_rdev);
#endif
	printk("*****Some body is closing me at major %d*****\n",minor);


	return 0;
}

static ssize_t light_board_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	short count;
	memset(msg, 0, 96);
	//printk("%d", len);
	count = copy_from_user( msg, buff, len );
	
	//printk("copy");
	
	int i;
	//for(i = 0; i < 96; i+=8)
	//	printk("%d %d %d %d %d %d %d %d \n", 
	//		msg[i], msg[i+1], msg[i+2], msg[i+3], msg[i+4], msg[i+5], msg[i+6], msg[i+7]);
	
	//printk("ok");
	for (i = 0; i < 16; i++) {
		int j;
		for (j = 0; j < 48; j++) {
			spi_led.map[i][j] = false;
		}
	}
	
	printk("map");
	for(i = 0; i < 16; i++){
		int j;
		for(j = 0; j < 6;j++){
			int j2;
			for(j2 = 0; j2 < 8; j2++){
				if(msg[j + i * 6] >> j2 & 0x1 == 0x1)
					spi_led.map[i][j2 + j * 8] = true;
				//printk("%d %d %d \n", i, j, j2);
			}
		}
	}
		
	
	
	return len;
}

