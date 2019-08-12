/*
 * kernel.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alevi
 */
#include "derivative.h" /* include peripheral declarations */
#include <stdbool.h> 
#include "functions.h"

#ifndef KERNEL_H_
#define KERNEL_H_


void scheduler(void);
void dispatcher(void);
void idle_task(void);

//static char tick_flag = 0;
static int tasks = 7;



/* Definition of task states */
typedef enum {
	OS_TASK_STATE_READY,
	OS_TASK_STATE_RUNNING,
	OS_TASK_STATE_BLOCKED
}OS_TASK_STATE;

typedef struct ready_list_node {
	void (*ptr_func)(void);
	char flag;
	char period;
	char priority;
	unsigned int task_count;
} ready_list_node;



#endif /* KERNEL_H_ */
