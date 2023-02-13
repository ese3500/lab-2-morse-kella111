/*
 * GccApplication2.c
 *
 * Created: 06/02/2023 20:59:46
 * Author : DELL
 */ 

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL

void Initialize() 
{
	DDRB |= 0b011110;
}


int main(void)
{
	Initialize();
	PORTB |= 0b011110;
    while (1) 
    {
		//PORTB |= 0b011110;
    }
}

