/*
 * lab2_D.c
 *
 * Created: 10/02/2023 16:55:01
 * Author : DELL
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
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

int pind5_1 = 0;


char morse_code[10];


// letter table 

char A[] = ".-";
char B[] = "-...";
char C[] = "-.-.";
char D[] = "-..";
char E[] = ".";
char F[] = "..-."; 
char G[] = "--.";
char H[] = "....";
char I[] = "..";
char J[] = ".---";
char K[] = "-.-";
char L[] = ".-..";
char M[] = "--";
char N[] = "-.";
char O[] = "---";
char P[] = ".--.";
char Q[] = "--.-";
char R[] = ".-.";
char S[] = "...";
char T[] = "-";
char U[] = "..-";
char V[] = "...-";
char W[] = ".--";
char X[] = "-..-"; 
char Y[] = "-.--";
char Z[] = "--..";

char one[] = ".----";
char two[] = "..---";
char three[] = "...--";
char four[] = "....-";
char five[] = ".....";
char six[] = "-....";
char seven[] = "--...";
char eight[] = "---..";
char nine[] = "----.";
char zero[] = "-----";

void Initialize()
{
	
	cli();
	
	DDRB &= ~(1 << DDB0); // set PB0 = ICP1 to input (pin 0).

	// divide internal clock by 8
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

int decode_press(int time_diff)
{
	
	if (time_diff >= 20*freq && time_diff <= 200*freq)
	{
		/*
		sprintf(String, "dot = %u \n", time_diff);
		sprintf(String, "dot \n");
		UART_putstring(String);
		_delay_ms(1000);
		*/
		
		return (int) ".";			
	}
	else if (time_diff > 200*freq && time_diff <= 400*freq)
	{
		/*
		sprintf(String, "dash \n");
		UART_putstring(String);
		_delay_ms(1000);
		*/
		
		/*
		PORTD |= (1<<PORTD6); //turn on red LED
		_delay_ms(100);
		PORTD &= ~(1<<PORTD6); // turn off red LED
		*/
		
		return (int) "-";
	}
	
	else
	{
		/*
		sprintf(String, "out of range = %u \n", t_decoded);
		UART_putstring(String);
		_delay_ms(1000);
		*/
		
		return 0;
	}
}


int code_capture(char* code) 
{
	int captured = 0;
	if (!strcmp(A, code)) captured = (int)"A";
	else if (!strcmp(B, code)) captured = (int)"B";
	else if (!strcmp(C, code)) captured = (int)"C";
	else if (!strcmp(D, code)) captured = (int)"D";
	else if (!strcmp(E, code)) captured = (int)"E";
	else if (!strcmp(F, code)) captured = (int)"F";
	else if (!strcmp(G, code)) captured = (int)"G";
	else if (!strcmp(H, code)) captured = (int)"H";
	else if (!strcmp(I, code)) captured = (int)"I";
	else if (!strcmp(J, code)) captured = (int)"J";
	else if (!strcmp(K, code)) captured = (int)"K";
	else if (!strcmp(L, code)) captured = (int)"L";
	else if (!strcmp(M, code)) captured = (int)"M";
	else if (!strcmp(N, code)) captured = (int)"N";
	else if (!strcmp(O, code)) captured = (int)"O";
	else if (!strcmp(P, code)) captured = (int)"P";
	else if (!strcmp(Q, code)) captured = (int)"Q";
	else if (!strcmp(R, code)) captured = (int)"R";
	else if (!strcmp(S, code)) captured = (int)"S";
	else if (!strcmp(T, code)) captured = (int)"T";
	else if (!strcmp(U, code)) captured = (int)"U";
	else if (!strcmp(V, code)) captured = (int)"V";
	else if (!strcmp(W, code)) captured = (int)"W";
	else if (!strcmp(X, code)) captured = (int)"X";
	else if (!strcmp(Y, code)) captured = (int)"W";
	else if (!strcmp(Z, code)) captured = (int)"Z";
	
	else if (!strcmp(one, code)) captured = (int)"1";
	else if (!strcmp(two, code)) captured = (int)"2";
	else if (!strcmp(three, code)) captured = (int)"3";
	else if (!strcmp(four, code)) captured = (int)"4";
	else if (!strcmp(five, code)) captured = (int)"5";
	else if (!strcmp(six, code)) captured = (int)"6";
	else if (!strcmp(seven, code)) captured = (int)"7";
	else if (!strcmp(eight, code)) captured = (int)"8";
	else if (!strcmp(nine, code)) captured = (int)"9";
	else if (!strcmp(zero, code)) captured = (int)"0";
	
	return captured;
}


int main(void)
{
	UART_init(BAUD_PRESCALER);
	
	
	Initialize();
	
	while (1)
	{
		
		if (t_diff != t_diff_prev)
		{
			
			int press = decode_press(t_diff);
			t_diff_prev = t_diff;
			
			if (press == (int) ".") {
			PORTD |= (1<<PORTD6); //turn on dot LED
			_delay_ms(50);
			PORTD &= ~(1<<PORTD6); // turn off dot LED
			}
			
			else if (press == (int) "-") {
				PORTD |= (1<<PORTD5); //turn on dash LED
				_delay_ms(50);
				PORTD &= ~(1<<PORTD5); // turn off dash LED
			}
			
			strcat(morse_code, (char*)press);
			
		}
		
		// release time
		if (TCNT1 - t_fall > 800*freq && releaseFlag == 0)
		{
			
			//sprintf(String, "space = %u \n", t_release);
		
			sprintf(String, " ");
			UART_putstring(String);
			_delay_ms(1000);
			
			int letter = code_capture(morse_code);
			
			sprintf(String, "%s", (char*) letter);
			UART_putstring(String);
			_delay_ms(1000);
			
			strcpy(morse_code, "");
			
			/*
			sprintf(String, "morse_code empty = %s \n", morse_code);
			UART_putstring(String);
			_delay_ms(1000);
			*/
			
			releaseFlag = 1;
		}
		
	}
	
}
