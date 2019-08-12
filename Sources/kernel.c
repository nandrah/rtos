/*
 * kernel.c
 *
 *  Created on: Aug 8, 2019
 *      Author: alevi
 */
#include "kernel.h"
#include "functions.h"

extern int ticks = 0;

ready_list_node ready[] =
{
		//	function,		flag,	period,		priority,	task_count
		{read_adc_button,	0,		5,				1,		0},
		{rota_bit, 			0,		10,				2,		0},
		{pwm_led,			0,		20,				3,		0},
		{pwm_servo,			0,		40,				4,		0},
		{detect_presence,	0,		80,				5,		0},
		{reflect_pb_state,	0,		100,			6,		0},
		{toggle_led_adc, 	0,		100,			7,		0}
};

void scheduler(void) {
	int i;
	for(i=0; i < tasks; i++){
		if(ticks >= ready[i].task_count){
			ready[i].flag = 1;
		}
	}
}

void dispatcher(void) {
	int i;
	for(i=0; i < tasks; i++){
		if(ready[i].flag == 1){
			ready[i].ptr_func();
			ready[i].flag = 0;
			ready[i].task_count += ready[i].period;
		}
	}
}


