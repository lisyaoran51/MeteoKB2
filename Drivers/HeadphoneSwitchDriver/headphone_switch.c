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


#define HEADPHONE_SWITCH 5
#define HEADPHONE_DETECTOR 16

static struct gpio detect_switch_gpios[] = {
    { HEADPHONE_SWITCH, GPIOF_OUT_INIT_LOW, "Headphone Switch" }, /* default to OFF */
	{ HEADPHONE_DETECTOR, GPIOF_IN, "Headphone Detector"  }, /* default to OFF */
};

static struct workqueue_struct *detectWorkQueue;
static struct delayed_work delayedDetectWorkQueue;


static void headphone_detect_work_handler(struct work_struct * work);

//DECLARE_WORK(my_work_queue, my_work_handler);
static void headphone_detect_work_handler(struct work_struct * work)
{
	int isPluggedIn = gpio_get_value(HEADPHONE_DETECTOR);	// 1:插入耳機 0:沒插入耳機
	
	if(isPluggedIn == 1)
		gpio_set_value(HEADPHONE_SWITCH, 1); // 切換至耳機
	else
		gpio_set_value(HEADPHONE_SWITCH, 0); // 切換至音響
	
    queue_delayed_work(detectWorkQueue, &delayedDetectWorkQueue, 50);
}


int init_module(void)
{
	printk("Init headphone detector.\n");
	
	// request gpio 使用
    if (gpio_request_array(detect_switch_gpios, ARRAY_SIZE(detect_switch_gpios)))  return -1;
	
	detectWorkQueue = create_workqueue("Detect Work Queue");
    if (!detectWorkQueue) {
        printk(KERN_ERR "No memory for headphone detect workqueue.\n");
        return 1;   
    }
	
    INIT_DELAYED_WORK(&delayedDetectWorkQueue, headphone_detect_work_handler);
	
    queue_delayed_work(detectWorkQueue, &delayedDetectWorkQueue, 50);


	return 0;
}

void cleanup_module(void)
{
	flush_scheduled_work();
    destroy_workqueue(detectWorkQueue);

	gpio_set_value(HEADPHONE_SWITCH, 0);
	gpio_free_array (detect_switch_gpios, ARRAY_SIZE(detect_switch_gpios));

}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");
