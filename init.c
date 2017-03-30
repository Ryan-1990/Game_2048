#include "game_console.h"
#include "LCD.h"
#include "FM25L16.h"


void LED_init(void)
{
	DDRD |= (1<<5);
	LED_OFF;
}

void KEY_init(void)
{
	DDRA &= 0x0F;
	PORTA |= 0xF0;
	DDRC &= 0x3F;
	PORTC |= 0xC0;
	MCUCR=0x0C; //INT1 rising edge
	GICR=0x80; //enable INT1
}

void PWM_init(void)
{
	DDRD |= (1<<7);
	PORTD &= ~(1<<7);
	TCCR2 = 0x69; 
	OCR2 = 0;	TCCR0|=(1<<COM01)|(1<<COM00);//???????OC0A??,???TOP?OC0??	TCCR0|=(1<<CS02)|(0<<CS01)|(1<<CS00); //???1024??	OCR0=0; 	sei();
}

void timer0_init(void)
{
	TCNT0 = 0;
	TCCR0 = 0x05;
	TIMSK = 0x01; //enable Timer/Counter0 Overflow Interrupt
}

unsigned int adc(void)   
{   
	unsigned int ad_value;   
	DDRA &= ~(1<<3);   
	ADMUX = 0xC3;   
	ADCSRA = 0x80;   
	ADCSRA |= (1<<6); //start convert
	while(!(ADCSRA&0x10));   
	ad_value = ADCL;   
	ad_value += (ADCH<<8);    

	return ad_value; 
}

void Init(void)
{
	cli(); //disable all interrupts
	//SREG=0x00; 
	LCD_init();
	FRAM_init();
	LED_init();
	KEY_init();
	PWM_init();
	timer0_init();
	adc();
	//SREG=0x80; 
	sei();  //enable all interrupts
}
