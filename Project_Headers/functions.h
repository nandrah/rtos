/*
 * functions.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alevi
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define PWM_Led_Incre	22

static char button_state; 
static char adc_value;

void adc_init(void);
void adc_convert(void);
void read_adc_button(void);
void rota_bit(void);
void read_pb(void);
void reflect_pb_state(void);
void pwm_init(void);
void pwm_led(void);
void pwm_servo(void);
void detect_presence(void);
void toggle_led_adc(void);
void timer_init(void);
void io_init(void);


#endif /* FUNCTIONS_H_ */
