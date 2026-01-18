/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *           View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#define PROX_THRESHOLD 310

void channel_adc(uint8_t n){
	ADMUX = n;
}

uint16_t fingerdetection() {
	
	channel_adc(1);

	return read_adc();
}

uint16_t heartbeat() {
	
	channel_adc(2);
	
	return read_adc();
}

void init_adc(uint16_t n)
{
	/* TODO: Initialisation code */	
	
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
	uint8_t count = 0;
	double bpm;
	uint8_t counted = 0;
	uint8_t Va = 93;
	uint8_t Vp = 62;
	uint8_t pulse = 0;
	uint8_t edge = 0;
	uint8_t time = 0;
	uint8_t t_start = 0;
	uint8_t t_end;
	uint16_t result;
	
	
	init_debug_uart0();
	init_adc();
	
	for (;;) 
	{
		/* TODO: Measurement code */
		//result = read_adc();
		
		//voltage = V_MAX / 1023 * result;
		
		result = fingerdetection();
		
		
		if(result < PROX_THRESHOLD) {
			
			
		    heartvalue = heartbeat();
			
			
			//no pulse detected
			if (heartvalue < Vp ){
			
				pulse = 0;
				
			}
			
			if (heartvalue > Va){
				
				if(pulse == 0){
					
					t_end = t_start;
					t_start = time;
					
					if (edge){
						bpm =  60 * 100 / (t_end - t_start);
						printf("Bpm is %lf",bpm);
					}
					
					
					edge = 1;
					pulse = 1;
					PORTB |= _BV(PB7); //light LED
						
				}
			}
			
			
			
		//when finger is moved away
		else{
			
			PORTB &= ~_BV(PB7);
			
		}
		
		time ++;

		//printf("%4d : %6.5fV\n", result, voltage);
		_delay_ms(1000);
	}
}
