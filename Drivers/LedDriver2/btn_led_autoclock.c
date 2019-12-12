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
#define MY_OUTPUT 2

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");


#include<linux/ktime.h>
ktime_t calltime, delta, rettime;


struct timer_list my_timer;
static void timer_function(unsigned long data) {

	
	// modify the timer for next time
	mod_timer(&my_timer, jiffies + HZ);
	printk("timer_function! %d\n", HZ);
	gpio_set_value(MY_OUTPUT, 0);//led_trigger);
	//led_trigger = led_trigger == 0 ? 1 : 0;
	calltime = ktime_get();
}

/*--------------------------------SPI--------------------------------*/
#include <linux/slab.h> // kmalloc
#define DI_PIN 10 
#define CL_PIN 11
#define CE_PIN 8

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

static irqreturn_t my_test_isr(int irq, void *data)
{
	local_irq_save(flags);

	bool** map = spi_led.map; 

	gpio_set_value(CE_PIN, 0);
	int row; 
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
	switch_row_sequencely(spi_led.column);

	spi_led.column++;
	if (spi_led.column == 16) {

		//	rettime = ktime_get();
		//	s64 actual_time = ktime_to_ns(ktime_sub(rettime, calltime));
		//	printk("%lld\n", (long long)actual_time);
		spi_led.column = 0;
	}

	local_irq_restore(flags);
	return IRQ_HANDLED;
}

/*------------------------------------------IRQ--------------------------------------*/
/*--------------------------------------hrtimer--------------------------------------*/


#define MY_AUTO_OUTPUT 14

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
	/*bitbang*/
	printk("*****LED GPIO Init ******************\n");
	if (gpio_request(DI_PIN, "DI_PIN")) return -1;
	if (gpio_request(CL_PIN, "CL_PIN")) return -1;
	if (gpio_request(CE_PIN, "CE_PIN")) return -1;

	if (gpio_request(PA_PIN, "PA_PIN")) return -1;
	if (gpio_request(PB_PIN, "PB_PIN")) return -1;
	if (gpio_request(NA_PIN, "NA_PIN")) return -1;
	if (gpio_request(NB_PIN, "NB_PIN")) return -1;

	gpio_direction_output(DI_PIN, 0);
	gpio_direction_output(CL_PIN, 0);
	gpio_direction_output(CE_PIN, 0);

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
	my_timer.expires = jiffies + HZ*3;
	my_timer.function = timer_function;
	my_timer.data = 0;

	// -- TIMER START 
	//add_timer(&my_timer);


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
	del_timer(&my_timer);
	//gpio_set_value(BUZZER, 0);
	//gpio_free(BUZZER);
	//free_irq(button_irq, MY_DEV_NAME);
	free_irq(test_irq, MY_DEV_NAME);
	//gpio_free(BUTTON1);
	gpio_free(MY_OUTPUT);
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