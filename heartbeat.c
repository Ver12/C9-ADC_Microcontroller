/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *           View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

#define PROX_THRESHOLD 
#define HEART_THRESHOLD 

uint16_t fingerdetection() {
	
	init_adc(0);

	return read_adc();
}

uint16_t heartbeat() {
	
	init_adc(1);
	
	return read_adc();
}

void init_adc(uint16_t n)
{
	/* TODO: Initialisation code */	
	
	ADMUX = n;
	
	// ADC Enable and prescaler of 64
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADEN); 
	
}

uint16_t read_adc(void)
{
	/* TODO: Acquisition code */
	
	// start single conversion
    // write '1' to ADSC
	ADCSRA |= _BV(ADSC); 
	
	// wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
	while (ADCSRA & _BV(ADSC)); 
	
	return ADC;
}


int main(void)
{
	uint16_t result;
	double voltage;
	
	uint16_t heartvalue;
	uint16_t time = 0;
	uint16_t peak = 0;
	
	init_debug_uart0();
	init_adc();
	
	for (;;) 
	{
		/* TODO: Measurement code */
		//result = read_adc();
		
		//voltage = V_MAX / 1023 * result;
		
		if(fingerdetection() < PROX_THRESHOLD) {
			
			PORTB |= _BV(PB7);	
			uint16_t heartvalue = heartbeat();
			
			if((peak == 0) && (heartvalue > HEART_THRESHOLD)){
				peak = 1;
				// calculate bpm
				time = 0;
				
			}
			
			else{
				time ++ 
			}
			
			if ((peak != 0 ) && (heartvalue < HEART_THRESHOLD)){
				peak = 0;
			}
		}
		
		else{
			
			PORTB &= ~_BV(PB7);
			
		}

		//printf("%4d : %6.5fV\n", result, voltage);
		_delay_ms(1000);
	}
}
