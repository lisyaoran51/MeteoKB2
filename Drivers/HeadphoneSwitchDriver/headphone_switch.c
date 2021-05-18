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

// 艾鍗的work queue範例

#include <linux/module.h>   
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>


#define MY_DEV_NAME "HeadphoneSwitcher"


#define HEADPHONE_SWITCH 5
#define HEADPHONE_DETECTOR 16

static struct gpio leds_gpios[] = {
    { HEADPHONE_SWITCH, GPIOF_OUT_INIT_LOW, "Headphone Switch" }, /* default to OFF */
	{ HEADPHONE_DETECTOR, GPIOF_IN, "Headphone Detector"  }, /* default to OFF */
};


struct mydev{
	struct gpio *tbl;
	int irq;
	struct work_struct workq;
	dev_t dev;
	char dev_name[32];
	
};

struct mydev *p_dev;

static void my_work_handler(struct work_struct * work);

//DECLARE_WORK(my_work_queue, my_work_handler);
static void my_work_handler(struct work_struct * work)
{
	int isPluggedIn = gpio_get_value(HEADPHONE_DETECTOR);	// 1:插入耳機 0:沒插入耳機
	
	if(isPluggedIn == 1)
		gpio_set_value(HEADPHONE_SWITCH, 1); // 切換至耳機
	else
		gpio_set_value(HEADPHONE_SWITCH, 0); // 切換至音響
	static int led_trigger = 0; 
	struct mydev	*ptr;
}


static irqreturn_t r_irq_handler(int irq, void *data) 
{
   unsigned long flags;
   local_irq_save(flags);
   schedule_work(&p_dev->workq);
   local_irq_restore(flags);
   return IRQ_HANDLED;
}


int init_module(void)
{
	int err;
	
	printk("*****LED GPIO Init ******************\n");
	
	// request gpio 使用
    if (gpio_request_array(leds_gpios, ARRAY_SIZE(leds_gpios)))  return -1;

	
	p_dev->irq = gpio_to_irq(p_dev->tbl[2].gpio);
	strncpy(p_dev->dev_name,p_dev->tbl[2].label,32);
	INIT_WORK(&p_dev->workq, my_work_handler);

	
	if(p_dev->irq < 0) return -1;
	err = request_irq( p_dev->irq, 
			r_irq_handler, 
			IRQF_TRIGGER_RISING, 
			MY_INT_NAME, 
			MY_DEV_NAME);
	if(err ) return -1;

	return 0;
}

void cleanup_module(void)
{
	free_irq(p_dev->irq, MY_DEV_NAME);

	 gpio_set_value(p_dev->tbl[0].gpio, 0);
     gpio_set_value(p_dev->tbl[1].gpio, 0);
	 gpio_free_array (leds_gpios, ARRAY_SIZE(leds_gpios));

	flush_scheduled_work();
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");
