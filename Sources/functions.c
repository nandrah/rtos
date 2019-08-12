#include "derivative.h" /* include peripheral declarations */
#include "functions.h"

void read_adc_button(void) {
	adc_convert();
	read_pb();
}

void detect_presence(void) {
	unsigned long distance_count = 0;
	unsigned int x = 0;
	
	PTAD_PTAD2 = 1;
	
	// delay
	while(x < 1000){
		x = x+1;
	}
	PTAD_PTAD2 = 0;
	
	while(PTAD_PTAD3 == 1){
	}
	
	while(PTAD_PTAD3 == 0){
		distance_count++;
		if(distance_count > 900) {
			break;
		}
	}
	if(distance_count < 800) {
		PTFD_PTFD0 = 0; // Turn on LED 1
		PTFD_PTFD5 = 1;
	} else {
		PTFD_PTFD0 = 1;
		PTFD_PTFD5 = 0; // Turn on LED 4
	}
}

void adc_init(void){
	ADCCFG = 0b00000000;		// MODE = 8-bit conversion
	ADCSC1 = 0b01011111;		// Module disabled, One conversion each time, Interrupts disabled
	ADCSC2 = 0b00000000;		// SW trigger selected, trigger when a write to ADSC1
}

void adc_convert(){
	ADCSC1_ADCH = 0x08;			// Trigger conversion from Channel 8 - Potentiometer RV1
	while(ADCSC1_COCO != 1) {
		
	}
	adc_value = ADCRL;		// Get the conversion result
	ADCSC1_ADCH = 0x1F;		// Disable module
}

void rota_bit(void){
	static char time_500ms = 0;
	static char rotate = 1;
	static char rotate_count = 0;
	time_500ms++;
	
	if (time_500ms == 50){
		PTJD = rotate;
		rotate = rotate << 1;
		time_500ms = 0;
		rotate_count++;
		if(rotate_count == 8){
			PTJD_PTJD7 = 1; 
			rotate = 1;
			rotate_count = 0;
	}
	}else{}
}

void read_pb(void){
	button_state = PTGD_PTGD0; 
}

void reflect_pb_state(void){
	if (button_state == 0){ // Push button is pressed (active-low) SW1
		PTFD_PTFD4 = 0; // Turn on LED 3
	} else {
		PTFD_PTFD4 = 1; // Turn off LED 3
	} 
}

void pwm_init(void){
	TPM3SC = 0x0D;		//Configura el Timer 3 (TPM) para funcionar como PWM Edge Aligned
	TPM3MOD = 0x0899;	//Asignamos el valor del Periodo del PWM del canal 0 (Este valor es igual a 2,200 en decimal) 60Hz
	//Configura el canal 0 del TPM que es el PTB0 (PIN 22) para el pwm_led()
	TPM3C0SC = 0x28;	//Configura el Canal 0 del Timer 3 para funcionar como PWM Edge Aligned
	TPM3C0V = 0x0002;	//Asignamos el valor del Duty Cycle del PWM  del canal 0
	//Configura el canal 1 del TPM que es el PTB1 (PIN 24) para el PWM_Servo()
	TPM3C1SC = 0x28;	//Configura el Canal 1 del Timer 3 para funcionar como PWM Edge Aligned
	TPM3C1V = 0;	//Asignamos el valor del Duty Cycle del PWM  del canal 1
}

void pwm_led(void){
	static unsigned short pwm_led_duty = 0;
	static unsigned char pwm_cycles_count = 0;
	// 100 20ms_cycles = 2s period
	//Se tendrá que dividir el valor del periodo entre 100 para saber el incremento que se debe dar al Duty Cycle (PWM_Incre)
	pwm_cycles_count++;
	if ( pwm_cycles_count < 100){
		pwm_led_duty = pwm_led_duty + PWM_Led_Incre;
	}else{
		pwm_cycles_count = 0;
		pwm_led_duty = 0;
	}
	TPM3C0V = pwm_led_duty;
}

void pwm_servo(void){
	float pwm_servo_duty = 0;
	unsigned short min_duty = 97;	//   0 grados
	unsigned short max_duty = 261;	// 135 grados
	unsigned short duty_diff = 164;
	unsigned short max_adc = 264;
	float multiplier = 0.0;
	
	multiplier = (float)adc_value / max_adc;
	
	pwm_servo_duty = min_duty + (multiplier * duty_diff);
	TPM3C1V = (unsigned short)pwm_servo_duty;	
}

void toggle_led_adc(void) {
	if(adc_value > 200) {
		PTFD_PTFD1 = ~PTFD_PTFD1;
	}
}

void timer_init(void){

	FTM1MODE=0x04;
	FTM1SC=0x00;
	FTM1C0SC=0x50;
	
	//FTM1C0V = 0x3FFF;
	FTM1MOD = 0x420; // Counter value

	FTM1CNTH=0x00;
	//FTM1SC=0x0E;
	FTM1SC=0x0A;
}

void io_init(void){
	PTFDD_PTFDD0 = 1; // LEDS - Configure as outputs
	PTFDD_PTFDD1 = 1;
	PTFDD_PTFDD4 = 1;
	PTFDD_PTFDD5 = 1;
	
	PTFD_PTFD0 = 1; // Turn off leds (Active-low)
	PTFD_PTFD1 = 1;
	PTFD_PTFD4 = 1;
	PTFD_PTFD5 = 1;
	
	//PTDDD_PTDDD0 = 0; // Bit from potentiometer RV1 as input
	//PTDPE_PTDPE0 = 1;
	
	PTGD_PTGD0 = 0;			// Configure port G bit 0 as input for push button SW1
	PTGPE_PTGPE0 = 1;		// Enable pull-up resistor in port G bit 0
	
	PTJDD = 0xFF;			// Configure port J as output - rotabit
	PTJD = 0x0;				// Initialise port J value 
	
	PTADD_PTADD2 = 1;		// Trigger for ultrasonic sensor (output)
	PTADD_PTADD3 = 0;		// Echo for ultrasonic sensor (input)
	PTAD_PTAD2 = 0;
	
	PTHDD = 0xFF;
	PTHD = 0x0;
}
