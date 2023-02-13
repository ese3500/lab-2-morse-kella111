/*
 * lab2_B.c
 *
 * Created: 08/02/2023 19:48:16
 * Author : DELL
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#define F_CPU 16000000UL


void Initialize() 
{
	cli();
	
	DDRB &= ~(1 << DDB0); // set PB0 = ICP1 to input (pin 0).
	DDRB |= (1 << DDB5); // set PB5 to output
	PORTB |= (1 << PORTB5); // enable pull up resistor
	
	TCCR1B &= ~(1 << ICES1); // a falling (negative) edge will trigger the capture.
	TIMSK1 |= (1 << ICIE1); // enable input capture 
	TIFR1 |= (1 << ICF1); // clear interrupt flag
	
	sei();
	
}

ISR(TIMER1_CAPT_vect)
{
	PORTB ^= (1 << PORTB5); // toggle LED 
	TCCR1B ^= (1 << ICES1); // toggle trigger.
	TIFR1 |= (1 << ICF1); // clear interrupt flag
}


int main(void)
{
    Initialize();
    while (1);
}

