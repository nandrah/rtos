#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#include "functions.h"
#include "kernel.h"

extern int ticks;
char tick_flag = 0;


ISR_FTM1Ch0(void);

void main(void) {
	EnableInterrupts;
	/* include your code here */
	ticks = 0;
	timer_init();
	io_init();
	adc_init();
	pwm_init();
	
  for(;;) {
	  if(tick_flag == 1) {
		  scheduler();
		  dispatcher();
		  tick_flag = 0;
	  }
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

__interrupt 67 ISR_FTM1Ch0(){
	
	tick_flag = 1;
	FTM1C0SC_CH0F = 0;

	ticks++;
}


