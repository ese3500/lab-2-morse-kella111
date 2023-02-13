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
	DDRB |= 0b011110; // set Pins 9-12 as outputs
}


int main(void)
{
	Initialize();
	int x = 0;
	int y = 0b000010;
	int flag = 1; 
	
    while (1) 
    {
		
		if (PIND & (1<<PIND7)) // button pressed
		{
			 PORTB = y << x; // shift y x bits to the left
			 flag = 0; 
			 _delay_ms(30);	// unstable button	
		}
		else // not pressed
		{
			PORTB = 0x00;
				
			if (flag == 0) // make changes happen once per button press
			{
				flag = 1;
				x += 1;	
				
				if (x > 3) 
					{
						x = 0;
						y = 0b000010;
					}					
			}				
		}
	}

}