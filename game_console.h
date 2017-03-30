/*************************************************************************
Title:    Game Console Template Header File
Inital Author:   David Jahshan
Extended by : (Run CAO) (614233) 
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console

*************************************************************************/

#include <avr/io.h> 
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>

 
#define LED_ON PORTD &= ~(1<<5)
#define LED_OFF PORTD |= (1<<5)

//#define LCD_LIGHT_ON PORTD |= (1<<7)
//#define LCD_LIGHT_OFF PORTD &= ~(1<<7)

#define CS_1 PORTA|=(1<<0)
#define CS_0 PORTA&=~(1<<0)

#define EROM_CS_1 PORTD|=(1<<4)
#define EROM_CS_0 PORTD&=~(1<<4)

#define SCLK_1 PORTB|=(1<<7)
#define SCLK_0 PORTB&=~(1<<7)

#define MO_1 PORTB|=(1<<5)
#define MO_0 PORTB&=~(1<<5)

#define RST_1 PORTA|=(1<<1)
#define RST_0 PORTA&=~(1<<1)

#define CD_1 PORTA|=(1<<2)
#define CD_0 PORTA&=~(1<<2)

#define UP PINA&(1<<6)
#define DOWN PINA&(1<<7)
#define LEFT PINA&(1<<4)
#define RIGHT PINA&(1<<5)
#define ACT1 PINC&(1<<7)
#define ACT2 PINC&(1<<6)

#define FRAM_SO PINB&(1<<6)
