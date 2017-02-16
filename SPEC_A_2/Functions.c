/** Functions.c
***
*** Alle übrig gebliebenen Funktionen, die in keine andere Datei gepasst
*** haben. 
***
*** Moritz Hauff, 17.02.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "Functions.h"

///////////////////////////////////////////////////////////////////////////
///Functions

void init_adc(void) 
{
	//16MHz/128 = 125kHz the ADC reference clock
	ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
	ADMUX |= (1 << REFS0);       //Set Voltage reference to Avcc (5v)
	ADCSRA |= (1 << ADEN);       //Turn on ADC
	ADCSRA |= (1 << ADSC);       //Do an initial conversion
}     

int16_t get_adc(uint8_t channel)
{
	ADMUX &= 0xE0;                 //Clear bits MUX0-4
	ADMUX |= channel & 0x07;       //Defines the new ADC channel to be read by setting bits MUX0-2
	ADCSRB = channel&(1 << 3);     //Set MUX5
	ADCSRA |= (1 << ADSC);         //Starts a new conversion
	while (ADCSRA & (1 << ADSC));  //Wait until the conversion is done
	return ADCW;
}

