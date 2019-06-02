/*
 * ServoMotor.c
 *
 * Created: 11/5/2018 4:51:12 PM
 * Author : braso
 */ 


#ifndef F_CPU
#define F_CPU 7372800UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h>


#include "avr_timers.h"
#include "servo_defines.h"

bool zeroDegrees = false;
bool twentyDegrees = false;
bool fortyFiveDegrees = false;
bool seventyDegrees = false;
bool ninetyDegrees = false;


//PWM 16-bit Timer (50hz)
// void PWM_init() 
// {
// 	TCCR3A = (1 << COM3A1) | (0 << COM3A0) | (1 << WGM11) | (0 >> WGM10); //inverted mode Fast-PWM
// 	TCCR3B = (1 << WGM33) | (1 << WGM32) | (0 << CS32) | (1 << CS31) | (1 << CS30);
// 	TCNT3 = 0;
// 	ICR3 = 2303;  // Fast PWM with 50hz PWM frequency
// 	// Servo pin direction
// 	DDRE |= (1 << PINE3);
// 	
// 	
// 	
// }

void delayFunc()
{	
	TCCR2 = (1 << WGM21) | (1 << CS22) | (1 << CS20);	// Prescaler 1024
	TCNT2 = 0;											// Init counter 0
	OCR2 = 72;											// Valoare de comparat (10 ms)
	TIMSK |= (1 << OCIE2);								// Activare intrerupere timer
	
	//Init 16-bit Timer for delay
	//TCCR1B = (1 << WGM12) | (1 << CS12);	// Prescalar 256
	//TCNT1 = 0;								// Initializare counter 0
	//OCR1A = 244;							// Valoarea de comparat 10ms .. pentru 5ms este 144, 1ms, este 28.8 =~ 29 iar 0.5ms 14.4
	//TIMSK |= (1 << OCIE1A);					// Activare intrerupere timer
}


int main(void)
{	
	//OCR3A = 1152;
	PORTB = 0x00;
	// Pin Direction 
	DDRE = (0 << DDRE4) | (0 << DDRE5) | (0 << DDRE6) | (0 << DDRE7);
	DDRD = (0 << DDRD0) | (0 << DDRD1) | (0 << DDRD2) | (0 << DDRD3);
	// Pin Write (pull-up -> high)
	PORTE = (1 << PORTE4) | (1 << PORTE5) | (1 << PORTE6) | (1 << PORTE7);
	PORTD = (1 << PORTD0) | (1 << PORTD1) | (1 << PORTD2) | (1 << PORTD3);
	// External interrupt
	EICRA = (1 << ISC31) | (0 << ISC30) | (1 << ISC21) | (0 << ISC20) | (1 << ISC11) | (0 << ISC10) | (1 << ISC01) | (0 << ISC00);
    EICRB = (1 << ISC71) | (0 << ISC70) | (1 << ISC61) | (0 << ISC60) | (1 << ISC51) | (0 << ISC50) | (1 << ISC41) | (0 << ISC40);
	// External interrupt 
	EIMSK = (1 << INT7) | (1 << INT6) | (1 << INT5) | (1 << INT4) | (1 << INT3) | (1 << INT2) | (1 << INT1) | (1 << INT0);
	
	// Init 8-bit Timer for delay (Timer 2)
	delayFunc();
	
	// Pwm function for Servo		
	pwm_InitServoMotor(&PORTE,PORTE3);
	//pwm_InitServoMotor(&PORTB,PORTB5);
	// PWM_init();
	
	sei();
	
    do 
    {		
	
    } while (1);
}

ISR(TIMER2_COMP_vect)
{

	if(zeroDegrees == true){		
		   
		//OCR3A = 57;		// 0 Degree (0.5 ms) K7
		set_ServoPosition(TIMER3,57);
		zeroDegrees = false;
	}
	
	 if(twentyDegrees == true){		
		
		//OCR3A = 93;		// 20 Degree (0.8 ms) K6
		set_ServoPosition(TIMER3,93);
		twentyDegrees = false;
	}
	
	 if (fortyFiveDegrees == true){	
	
		//OCR3A = 115;	// 45 Degree (1 ms) K5
		set_ServoPosition(TIMER3,115);
		fortyFiveDegrees = false;
	}
	
	if(seventyDegrees == true)
	{
		//OCR3A = 143;	// 70 Degree (1.2 ms) K5
		set_ServoPosition(TIMER3,143);
		seventyDegrees = false;
	}
	
	if(ninetyDegrees == true) 
	{
		//OCR3A = 170;	// 90 Degree (1.5 ms) K4
		set_ServoPosition(TIMER3,170);
		ninetyDegrees = false;
	}
}

ISR(INT7_vect) // 0 Degree, button -> K7
{
	zeroDegrees = true;
}

ISR(INT6_vect) // 20 Degree, button ->  K6
{

	twentyDegrees = true;
}

ISR(INT5_vect) // 45 Degree, button -> K5
{
	fortyFiveDegrees = true;
}

ISR(INT4_vect) // 70 Degree, button -> K4
{
	seventyDegrees = true;
}

ISR(INT3_vect) // 90 Degree, button -> K3
{
	ninetyDegrees = true;
}
