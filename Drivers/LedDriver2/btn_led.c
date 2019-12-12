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

#define MY_GPIO_INT_NAME "my_button_int"
#define MY_DEV_NAME "it_device"

#define MY_INTERRUPT_IN 3
#define MY_OUTPUT 5

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");

static short int button_irq = 0;
static unsigned long flags = 0;
static int led_trigger = 0;


struct timer_list my_timer;
static void timer_function(unsigned long data) {

	
	// modify the timer for next time
	//mod_timer(&my_timer, jiffies + HZ / 10);
	printk("timer_function! %d\n", HZ);
	gpio_set_value(MY_OUTPUT, 1);
}

static short int test_irq = 0;

static irqreturn_t my_test_isr(int irq, void *data)
{
	int val = 0;
	local_irq_save(flags);
	printk("test_isr !!!!\n");
	//gpio_set_value(BUZZER, led_trigger);
	//val = gpio_get_value(BUTTON1);
	//printk("get value : %d \n", val);
	//led_trigger = led_trigger ? (0) : (1);
	local_irq_restore(flags);
	return IRQ_HANDLED;
}

int init_module(void)
{

	printk("module: my interrupt test.\n");

	printk("module: set output.\n");
	if (!gpio_is_valid(MY_OUTPUT)) return -1;
	if (gpio_request(MY_OUTPUT, "MY_OUTPUT")) return -1;
	gpio_direction_output(MY_OUTPUT, 0);
	gpio_set_value(MY_OUTPUT, 0);


	printk("module: setting my irq.\n");
	if (!gpio_is_valid(MY_INTERRUPT_IN)) return -1;
	if (gpio_request(MY_INTERRUPT_IN, "MY_INTERRUPT_IN")) return -1;
	if ((test_irq = gpio_to_irq(MY_INTERRUPT_IN)) < 0)  return -1;
	if (request_irq(test_irq, my_test_isr, IRQF_TRIGGER_RISING, MY_GPIO_INT_NAME, MY_DEV_NAME)) return -1;
	
	//  -- initialize the timer 
	init_timer(&my_timer);
	my_timer.expires = jiffies + HZ;
	my_timer.function = timer_function;
	my_timer.data = spi_led;

	// -- TIMER START 
	add_timer(&my_timer);


	return 0;

}


void cleanup_module(void)
{
	del_timer(&my_timer);
	//gpio_set_value(BUZZER, 0);
	//gpio_free(BUZZER);
	//free_irq(button_irq, MY_DEV_NAME);
	free_irq(test_irq, MY_DEV_NAME);
	//gpio_free(BUTTON1);
	gpio_free(MY_OUTPUT);
	gpio_free(MY_INTERRUPT_IN);
}
