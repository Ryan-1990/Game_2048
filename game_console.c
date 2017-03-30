/*************************************************************************
Title:    game_console Template
Initial Author:   David Jahshan
Extended by : (Run CAO) (614233)
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Main and functions for Game Console basic sample code

*************************************************************************/

//#include <util/delay.h>
#include "game_console.h" 
#include "init.h"
#include "LCD.h"
#include "game_2048.h"
#include "FM25L16.h"


int brightness = 0;
unsigned int battery = 0;
unsigned char function = 0;
unsigned char game_status = 0;


int main(void)
{
	Init();
	LCD_CLS();
	LCD_Print(5,2,"slection funtion",0);
	LCD_Print(0,4,"1. enter game",0);
	LCD_Print(0,5,"2. reseverd",0);

	brightness = FRAM_ReadMemory(0,22);
	OCR2 = brightness;


	for(;;)
	{
		//
	}
	return (0);
}


//INT1
SIGNAL(SIG_INTERRUPT1)
{
	static char item = 0;
	
	if(!(UP))         
	{
		if(function==0)
		{
			item--;
			if(item<0) item = 1;
			
			if(item==1)
			{
				LCD_Print(0,4,"1. enter game",1);
				LCD_Print(0,5,"2. reseverd",0);
			}
			else
			{
				LCD_Print(0,4,"1. enter game",0);
				LCD_Print(0,5,"2. reseverd",1);
			}
		}
		else 
		{
			move(4);
			LCD_OUT();
			game_status = check();
			if(game_status==1) //game over
			{
				LCD_GameOver();
			}
			else if(game_status==2) //win
			{
				LCD_GameWin();
			}
		}
	}
	else if(!(DOWN))
	{
		if(function==0)
		{
			item++;
			if(item>2) item = 1;
			if(item==1)
			{
				LCD_Print(0,4,"1. enter game",1);
				LCD_Print(0,5,"2. reseverd",0);
			}
			else if(item==2)
			{
				LCD_Print(0,4,"1. enter game",0);
				LCD_Print(0,5,"2. reseverd",1);
			}
		}
		else 
		{
			move(2);
			LCD_OUT();
			game_status = check();
			if(game_status==1) //game over
			{
				LCD_GameOver();
			}
			else if(game_status==2) //win
			{
				LCD_GameWin();
			}
		}
	}
	else if(!(LEFT)) 
	{
		if(function==0)
		{
			if(item == 1) 
			{
				function = 1;
				LCD_clear();
				LCD_game_init();
			    LCD_OUT();
			}
		}
		else 
		{
			move(3);
			LCD_OUT();
			game_status = check();
			if(game_status==1) //game over
			{
				LCD_GameOver();
			}
			else if(game_status==2) //win
			{
				LCD_GameWin();
			}
		}
	} 
	else if(!(RIGHT)) 
	{
		if(function==0)
		{
			if(item == 1) 
			{
				function = 1;
				LCD_clear();
				LCD_game_init();
			    LCD_OUT();
			}
		}
		else 
		{
			move(1);
			LCD_OUT();
			game_status = check();
			if(game_status==1) //game over
			{
				LCD_GameOver();
			}
			else if(game_status==2) //win
			{
				LCD_GameWin();
			}
		}
	}
	else if(!(ACT1))
	{
		brightness += 10;
		if(brightness>255) brightness = 255;
		OCR2 = brightness;
		FRAM_WriteMemory(0,22,brightness);
	}  
	else
	{
		brightness -= 10;
		if(brightness<10) brightness = 0;
		OCR2 = brightness;
		FRAM_WriteMemory(0,22,brightness);
	}
}


SIGNAL(SIG_OVERFLOW0)
{
	static unsigned char time_cnt=0;
	static unsigned char pre_per=6;
	int per=0;

	time_cnt++;
	if(time_cnt==10) 
	{
		time_cnt = 0;
		battery = adc();
		per = battery - 340;
		if(per<0) per = 0;
		else per = per / 50;
		if(per>5) per = 5;
		if(per>pre_per) per=pre_per;
		if(per<pre_per) LCD_Draw_Battery(per);
		pre_per = per;
		if(per<2) LED_ON;
	}
}
