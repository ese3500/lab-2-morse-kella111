//*
 * lab2_a2.c
 *
 * Created: 06/02/2023 22:18:08
 * Author : DELL
 */ 

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#define F_CPU 16000000UL


// set PD7 as input
void Initialize() 
{
	DDRD &= ~(1 << DDD7); // set PD7 to input
	DDRD |= (1 << DDD6); // set PD6 to output
	
}

int main(void)
{
	Initialize();
	int x = 0;
	int y = 0b000010;
	int flag = 1; 
	
    while (1) 
    {	
		---a1---
		
		// if pressed --> PIND7 == HIGH
		if (PIND & (1<<PIND7)) 
			PORTD |= (1<<PORTD6);
			
		// if released --> PIND7 == LOW
		else 
			PORTD &= ~(1<<PORTD6);
			
			
	}

}