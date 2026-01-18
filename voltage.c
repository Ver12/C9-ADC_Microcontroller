/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *           View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

void init_adc(void)
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
	
	init_debug_uart0();
	init_adc();
	
	for (;;) 
	{
		/* TODO: Measurement code */
		result = read_adc();  //10 bit raw-value
		
		voltage = (result * 3.3)/1024.0 ;

		printf("%4d : %6.5fV\n", result, voltage);
		
		_delay_ms(1000);
	}
}
