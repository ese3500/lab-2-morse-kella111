/*
 * lab2_C.c
 *
 * Created: 09/02/2023 18:24:13
 * Author : DELL
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#define ticks 65536
#define freq 62.5 // (ticks/ms)

#include "uart.h"

char String[25];
int t_rise; 
int t_fall; 
int t_diff;
int t_decoded;
int t_diff_prev;
int t_release;
int t_release_prev;

int releaseFlag = 0;

void Initialize()
{
	
	cli();
	
	DDRB &= ~(1 << DDB0); // set PB0 = ICP1 to input (pin 0).
	//DDRB |= (1 << DDB5); // set PB5 to output
	//PORTB |= (1 << PORTB5); // enable pull up resistor
	
	// divide internal clock by 256 
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	// set timer 1 to normal 
	TCCR1A &= ~(1 << WGM10);
	TCCR1A &= ~(1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	TCCR1B &= ~(1 << WGM13);
	
	TCCR1B |= (1 << ICES1); // a rising (positive) edge will trigger the capture.
	TIMSK1 |= (1 << ICIE1); // enable input capture interrupt
	TIFR1 |= (1 << ICF1); // clear interrupt flag
	
	sei();	
}

ISR(TIMER1_CAPT_vect)
{	
	if ((TCCR1B & (1<<ICES1)))
	{ // if positive trigger 
		t_rise = TCNT1;
		//releaseFlag = 0;
	}
	else // if negative trigger
	{
		releaseFlag = 0;
		t_fall = TCNT1;
		
		if (t_fall < t_rise) // check for overflow
		{
			t_diff =  (ticks - t_rise + t_fall);
		}
		
		else // no overflow
		{
			t_diff = t_fall - t_rise;
		}
		
		//releaseFlag = 0;
	}
		
	TCCR1B ^= (1 << ICES1); // toggle trigger.	
	TIFR1 |= (1 << ICF1); // clear interrupt flag.
	
}

void decode_press(int time_diff) 
{
	
	if (time_diff >= 20*freq && time_diff <= 200*freq)
	{
		sprintf(String, "dot = %u \n", time_diff);
		sprintf(String, "dot \n");
		UART_putstring(String);
		_delay_ms(1000);
	}
	else if (time_diff > 200*freq && time_diff <= 400*freq)
	{
		//sprintf(String, "dash = %u \n", time_diff);
		sprintf(String, "dash \n"); 
		UART_putstring(String);
		_delay_ms(1000);
	}
	
	else
	{
		sprintf(String, "out of range = %u \n", t_decoded);
		UART_putstring(String);
		_delay_ms(1000);
	}
}

int main(void)
{
	UART_init(BAUD_PRESCALER); 
	
	Initialize();
	
	while (1)
	{
		
		if (t_diff != t_diff_prev)
		{
			decode_press(t_diff);
			t_diff_prev = t_diff;
		} 
		
		// release time
		if (TCNT1 - t_fall > 800*freq && releaseFlag == 0) 
		{
			//releaseFlag = 1;
			//sprintf(String, "space = %u \n", t_release);
			//sprintf(String, "space\n");
			sprintf(String, "space\n");
			UART_putstring(String);
			_delay_ms(1000);
			
			releaseFlag = 1;
		}
		
	}
}
