#include "game_console.h"
//#include <util/delay.h>
//#include <avr\pgmspace.h>
#include "game_2048.h"
#include "FM25L16.h"


//======================================
unsigned char F6x8[39][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // space
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
 	
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
};

unsigned char NUM_1024[19]={0x42,0x7F,0x40,0x00,0x3E,0x41,0x41,0x3E,0x00,0x42,0x61,0x51,0x4E,0x00,0x18,0x14,0x12,0x7F,0x10};
unsigned char NUM_2048[9]={0x0F,0x11,0x21,0x42,0x84,0x42,0x21,0x11,0x0F};

void DLY_ms(unsigned int ms)
{
	unsigned int ii=0,jj=0;

	for(ii=0;ii<ms;ii++)
	{
		for(jj=0;jj<300;jj++);
	}
}

unsigned char itostr(unsigned int iNum,unsigned char *str)
{
  unsigned char iSize=0;
  char *p=str;
  char *s=str;

  if(iNum==0)
  str[iSize++]='0';
  while(iNum>0)
  {
    str[iSize++]=iNum%10+'0';
    iNum=iNum/10;
  }
  
  str[iSize]='\0';
  p=str+iSize-1;
  
  for(;p-s>0;p--,s++)   
  {   
    *s^=*p;   
    *p^=*s;   
    *s^=*p;   
  }   
  return (iSize);  
}

void SPI_MasterInit(void)
{
	
	DDRB |= (1<<5)|(1<<7); //MOSI & SCK (output)
	DDRA |= (1<<0)|(1<<1)|(1<<2);
	PORTA |= 0x07;
}

void SPI_MasterTransmit(unsigned char abyte)
{
	unsigned char i;
	           
	//EROM_CS_1; 
	SCLK_1;   
	CS_0;     
	for(i=0;i<8;i++)
	{
		SCLK_0;         
		if(0x80&abyte)
			MO_1;         
		else
			MO_0;
		abyte<<=1;
		SCLK_1; 
	}
	SCLK_1;
	CS_1;
	//EROM_CS_0;
}



void LCD_command_tx(unsigned char a)
{
	CD_0;
	SPI_MasterTransmit(a);//PDR = a;
}

void LCD_data_tx(unsigned char a)
{
	CD_1;
	SPI_MasterTransmit(a);//PDR = a;
}



void LCD_init(void)
{
    SPI_MasterInit();
	//LCD_LIGHT_ON;
	RST_1;
    DLY_ms(1);
    RST_0;              // RESET routine for the LCD
    DLY_ms(10);
    RST_1;
    DLY_ms(10);
 
    LCD_command_tx(0x40);       //Display start line 0
    LCD_command_tx(0xA0);       //SEG reverse
    LCD_command_tx(0xC8);       //Normal COM0~COM63
    LCD_command_tx(0xA4);       //Disable -> Set All Pixel to ON
    LCD_command_tx(0xA6);       //Display inverse off
    LCD_command_tx(0xA2);       //Set LCD Bias Ratio A2/A3
    LCD_command_tx(0x2F);       //Set Power Control 28...2F
    LCD_command_tx(0x27);       //Set VLCD Resistor Ratio 20...27
    LCD_command_tx(0x81);       //Set Electronic Volume
    LCD_command_tx(0x10);       //Set Electronic Volume 00...3F
    LCD_command_tx(0xFA);       //Set Adv. Program Control
    LCD_command_tx(0x90);       //Set Adv. Program Control x00100yz yz column wrap x Temp Comp
    LCD_command_tx(0xAF);       //Display on
}

void LCD_CLS(void)
{
	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char col_h = 0;
	unsigned char col_l = 0;

	for(row=0;row<8;row++)
	{
		LCD_command_tx(0xB0+row);
		for(col=0;col<102;col++)
		{
			col_h = (((col+30)&0xF0)>>4)|0x10;
			col_l = (col+30)&0x0F;

			LCD_command_tx(col_h);
			LCD_command_tx(col_l);
			LCD_data_tx(0x00);
		}
	}
}

void LCD_clear(void)
{
	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char col_h = 0;
	unsigned char col_l = 0;

	for(row=1;row<8;row++)
	{
		LCD_command_tx(0xB0+row);
		for(col=0;col<102;col++)
		{
			col_h = (((col+30)&0xF0)>>4)|0x10;
			col_l = (col+30)&0x0F;

			LCD_command_tx(col_h);
			LCD_command_tx(col_l);
			LCD_data_tx(0x00);
		}
	}
}

//x:0~101  y:0~7
void LCD_Set_Pos(unsigned char x, unsigned char y)
{ 
	LCD_command_tx(0xB0+y);
	LCD_command_tx((((x+30)&0xF0)>>4)|0x10);
	LCD_command_tx((x+30)&0x0F); 
} 

void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch,unsigned char flag)
{
	unsigned char c=0,i=0;
	
	if(ch==32) c=0;
	else if(ch==46) c=1;
	else if(ch>47&&ch<59) c=ch-46;
	else if(ch>96&&ch<123) c=ch-84;
	
	if(x>101)
	{
		x=0;
		y++;
	}
	LCD_Set_Pos(x,y);  
	if(!flag)
	{  
		for(i=0;i<6;i++)     
		{
			LCD_data_tx(F6x8[c][i]);
		}
	}
	else
	{  
		for(i=0;i<6;i++)     
		{
			LCD_data_tx(0xFF-F6x8[c][i]);
		}
	}
}

void LCD_Print(unsigned char x,unsigned char y,unsigned char * ch,unsigned char flag)
{
	unsigned char i=0;

	while(ch[i]!=0)
	{
		LCD_P6x8Str(x,y,ch[i],flag);
		i++;
		x+=6;
	}
}

void LCD_Draw_Battery(unsigned char percent)
{
	LCD_Set_Pos(95,0);
	LCD_data_tx(0x06);
	if(percent==0)
	{
		LCD_Set_Pos(96,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(97,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(98,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(99,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(100,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(101,0);
		LCD_data_tx(0x0F);
	}
	else if(percent==1)
	{
		LCD_Set_Pos(96,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(97,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(98,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(99,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(100,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(101,0);
		LCD_data_tx(0x0F);
	}
	else if(percent==2)
	{
		LCD_Set_Pos(96,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(97,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(98,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(99,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(100,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(101,0);
		LCD_data_tx(0x0F);
	}
	else if(percent==3)
	{
		LCD_Set_Pos(96,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(97,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(98,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(99,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(100,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(101,0);
		LCD_data_tx(0x0F);
	}
	else if(percent==4)
	{
		LCD_Set_Pos(96,0);
		LCD_data_tx(0x09);
		LCD_Set_Pos(97,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(98,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(99,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(100,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(101,0);
		LCD_data_tx(0x0F);
	}
	else if(percent==5)
	{
		LCD_Set_Pos(96,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(97,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(98,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(99,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(100,0);
		LCD_data_tx(0x0F);
		LCD_Set_Pos(101,0);
		LCD_data_tx(0x0F);
	}
}

void LCD_game_init(void)
{
	unsigned char i=0,j=0;

	GICR=0x00; //disable INT1
	DLY_ms(200);
	LCD_Print(0,2,"left---continue",0);
	LCD_Print(0,3,"right--new game",0);

	for(;;)
	{
		if(!(LEFT))
		{
			unsigned char n=0;
			for(i=0;i<4;i++)
	        {
	            for(j=0;j<4;j++)
	            {
					x[i][j] = FRAM_ReadMemory(0,n)<<8; //high 8 bits
					x[i][j] += FRAM_ReadMemory(1,n);
					n++;
				}
			}
			score = FRAM_ReadMemory(0,20)<<8;
			score += FRAM_ReadMemory(0,21);
			break;
		}
		else if(!(RIGHT))
		{
			start();start();
			break;
		}
	}

	LCD_clear();
	LCD_Print(10,0,"score:0",0);

	for(i=0;i<4;i++)
	{
		LCD_Set_Pos(3+23*i,7);LCD_data_tx(0x3F);
		LCD_Set_Pos(3+23*i,6);LCD_data_tx(0xF9);
		LCD_Set_Pos(3+23*i,5);LCD_data_tx(0xFF);
		LCD_Set_Pos(3+23*i,4);LCD_data_tx(0xCF);
		LCD_Set_Pos(3+23*i,3);LCD_data_tx(0xFE);
		LCD_Set_Pos(3+23*i,2);LCD_data_tx(0x7F);
		LCD_Set_Pos(3+23*i,1);LCD_data_tx(0xF0);
	}
	for(i=0;i<4;i++)
	{
		LCD_Set_Pos(24+23*i,7);LCD_data_tx(0x3F);
		LCD_Set_Pos(24+23*i,6);LCD_data_tx(0xF9);
		LCD_Set_Pos(24+23*i,5);LCD_data_tx(0xFF);
		LCD_Set_Pos(24+23*i,4);LCD_data_tx(0xCF);
		LCD_Set_Pos(24+23*i,3);LCD_data_tx(0xFE);
		LCD_Set_Pos(24+23*i,2);LCD_data_tx(0x7F);
		LCD_Set_Pos(24+23*i,1);LCD_data_tx(0xF0);
	}
	for(i=0;i<4;i++)
	{
		for(j=4;j<24;j++)
		{
			LCD_Set_Pos(j+23*i,7);LCD_data_tx(0x20);
			LCD_Set_Pos(j+23*i,6);LCD_data_tx(0x09);
			LCD_Set_Pos(j+23*i,4);LCD_data_tx(0x48);
			LCD_Set_Pos(j+23*i,3);LCD_data_tx(0x02);
			LCD_Set_Pos(j+23*i,2);LCD_data_tx(0x40);
			LCD_Set_Pos(j+23*i,1);LCD_data_tx(0x10);
		}
	}

	GICR=0x80; //enable INT1
}

void LCD_Game_Print(unsigned char x,unsigned char y,unsigned int data)
{
	///*
	unsigned char data_str[4]="";
	unsigned char i=0,j=0,k=0,l=0,m=0;

	if(data) itostr(data,data_str);
	
	if(x==0)
	{
		LCD_Set_Pos(5+23*y,1);
		for(i=0;i<19;i++) LCD_data_tx(0x00|0x10); //clear
		LCD_Set_Pos(5+23*y,2);
		for(i=0;i<19;i++) LCD_data_tx(0x00|0x40); //clear
		
		if(data<16)
		{
			LCD_Set_Pos(11+23*y,1);
			if(data)
			{
				for(i=0;i<6;i++)     
				{
					LCD_data_tx((F6x8[data_str[0]-46][i]<<6)|0x10);
				}
			}
			LCD_Set_Pos(11+23*y,2);
			if(data)
			{
				for(i=0;i<6;i++)     
				{
					LCD_data_tx((F6x8[data_str[0]-46][i]>>2)|0x40);
				}
			}
		}
		else if(data<128)
		{
			LCD_Set_Pos(8+23*y,1);
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][j]<<6)|0x10);
			}
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][j]<<6)|0x10);
			}
			LCD_Set_Pos(8+23*y,2);
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][j]>>2)|0x40);
			}
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][j]>>2)|0x40);
			}
		}
		else if(data<1024)
		{
			LCD_Set_Pos(5+23*y,1);
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][k]<<6)|0x10);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][k]<<6)|0x10);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[2]-46][k]<<6)|0x10);
			}
			LCD_Set_Pos(5+23*y,2);
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][k]>>2)|0x40);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][k]>>2)|0x40);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[2]-46][k]>>2)|0x40);
			}
		}
		else if(data==1024)
		{
			LCD_Set_Pos(5+23*y,1);
			for(l=0;l<19;l++)     
			{
				LCD_data_tx((NUM_1024[l]<<6)|0x10);
			}
			LCD_Set_Pos(5+23*y,2);
			for(l=0;l<19;l++)     
			{
				LCD_data_tx((NUM_1024[l]>>2)|0x40);
			}
		}
		else
		{
			LCD_Set_Pos(11+23*y,1);
			for(m=0;m<9;m++)     
			{
				LCD_data_tx((NUM_2048[m]<<6)|0x10);
			}
			LCD_Set_Pos(11+23*y,2);
			for(m=0;m<9;m++)     
			{
				LCD_data_tx((NUM_2048[m]>>2)|0x40);
			}
		}
	}
	else if(x==1)
	{
		LCD_Set_Pos(5+23*y,3);
		for(i=0;i<19;i++) LCD_data_tx(0x00|0x02); //clear
		LCD_Set_Pos(5+23*y,4);
		for(i=0;i<19;i++) LCD_data_tx(0x00|0x48); //clear
		if(data<16)
		{
			LCD_Set_Pos(11+23*y,3);
			if(data)
			{
				for(i=0;i<6;i++)     
				{
					LCD_data_tx((F6x8[data_str[0]-46][i]<<3)|0x02);
				}
			}
			LCD_Set_Pos(11+23*y,4);
			if(data)
			{
				for(i=0;i<6;i++)     
				{
					LCD_data_tx((F6x8[data_str[0]-46][i]>>5)|0x48);
				}
			}
		}
		else if(data<128)
		{
			LCD_Set_Pos(8+23*y,3);
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][j]<<3)|0x02);
			}
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][j]<<3)|0x02);
			}
			LCD_Set_Pos(8+23*y,4);
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][j]>>5)|0x48);
			}
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][j]>>5)|0x48);
			}
		}
		else if(data<1024)
		{
			LCD_Set_Pos(5+23*y,3);
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][k]<<3)|0x02);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][k]<<3)|0x02);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[2]-46][k]<<3)|0x02);
			}
			LCD_Set_Pos(5+23*y,4);
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][k]>>5)|0x48);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][k]>>5)|0x48);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[2]-46][k]>>5)|0x48);
			}
		}
		else if(data==1024)
		{
			LCD_Set_Pos(5+23*y,3);
			for(l=0;l<19;l++)     
			{
				LCD_data_tx((NUM_1024[l]<<3)|0x02);
			}
			LCD_Set_Pos(5+23*y,4);
			for(l=0;l<19;l++)     
			{
				LCD_data_tx((NUM_1024[l]>>5)|0x48);
			}
		}
		else
		{
			LCD_Set_Pos(11+23*y,3);
			for(m=0;m<9;m++)     
			{
				LCD_data_tx((NUM_2048[m]<<3)|0x02);
			}
			LCD_Set_Pos(11+23*y,4);
			for(m=0;m<9;m++)     
			{
				LCD_data_tx((NUM_2048[m]>>5)|0x48);
			}
		}
	}
	else if(x==2)
	{
		LCD_Set_Pos(5+23*y,5);
		for(i=0;i<19;i++) LCD_data_tx(0x00); //clear
		if(data<16)
		{
			LCD_Set_Pos(11+23*y,5);
			if(data)
			{
				for(i=0;i<6;i++)     
				{
					LCD_data_tx(F6x8[data_str[0]-46][i]);
				}
			}
		}
		else if(data<128)
		{
			LCD_Set_Pos(8+23*y,5);
			for(j=0;j<6;j++)     
			{
				LCD_data_tx(F6x8[data_str[0]-46][j]);
			}
			for(j=0;j<6;j++)     
			{
				LCD_data_tx(F6x8[data_str[1]-46][j]);
			}
		}
		else if(data<1024)
		{
			LCD_Set_Pos(5+23*y,5);
			for(k=0;k<6;k++)     
			{
				LCD_data_tx(F6x8[data_str[0]-46][k]);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx(F6x8[data_str[1]-46][k]);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx(F6x8[data_str[2]-46][k]);
			}
		}
		else if(data==1024)
		{
			LCD_Set_Pos(5+23*y,5);
			for(l=0;l<19;l++)     
			{
				LCD_data_tx(NUM_1024[l]);
			}
		}
		else
		{
			LCD_Set_Pos(11+23*y,5);
			for(m=0;m<9;m++)     
			{
				LCD_data_tx(NUM_2048[m]);
			}
		}
	}
	else if(x==3)
	{
		LCD_Set_Pos(5+23*y,6);
		for(i=0;i<19;i++) LCD_data_tx(0x00|0x09); //clear
		LCD_Set_Pos(5+23*y,7);
		for(i=0;i<19;i++) LCD_data_tx(0x00|0x20); //clear
		if(data<16)
		{
			LCD_Set_Pos(11+23*y,6);
			if(data)
			{
				for(i=0;i<6;i++)     
				{
					LCD_data_tx((F6x8[data_str[0]-46][i]<<5)|0x09);
				}
			}
			LCD_Set_Pos(11+23*y,7);
			if(data)
			{
				for(i=0;i<6;i++)     
				{
					LCD_data_tx((F6x8[data_str[0]-46][i]>>3)|0x20);
				}
			}
		}
		else if(data<128)
		{
			LCD_Set_Pos(8+23*y,6);
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][j]<<5)|0x09);
			}
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][j]<<5)|0x09);
			}
			LCD_Set_Pos(8+23*y,7);
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][j]>>3)|0x20);
			}
			for(j=0;j<6;j++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][j]>>3)|0x20);
			}
		}
		else if(data<1024)
		{
			LCD_Set_Pos(5+23*y,6);
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][k]<<5)|0x09);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][k]<<5)|0x09);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[2]-46][k]<<5)|0x09);
			}
			LCD_Set_Pos(5+23*y,7);
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[0]-46][k]>>3)|0x20);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[1]-46][k]>>3)|0x20);
			}
			for(k=0;k<6;k++)     
			{
				LCD_data_tx((F6x8[data_str[2]-46][k]>>3)|0x20);
			}
		}
		else if(data==1024)
		{
			LCD_Set_Pos(5+23*y,6);
			for(l=0;l<19;l++)     
			{
				LCD_data_tx((NUM_1024[l]<<5)|0x09);
			}
			LCD_Set_Pos(5+23*y,7);
			for(l=0;l<19;l++)     
			{
				LCD_data_tx((NUM_1024[l]>>3)|0x20);
			}
		}
		else
		{
			LCD_Set_Pos(11+23*y,6);
			for(m=0;m<9;m++)     
			{
				LCD_data_tx((NUM_2048[m]<<5)|0x09);
			}
			LCD_Set_Pos(11+23*y,7);
			for(m=0;m<9;m++)     
			{
				LCD_data_tx((NUM_2048[m]>>3)|0x20);
			}
		}
	}
	//*/
}

void LCD_OUT(void)
{
	unsigned char SCORE[5]="";
	unsigned char i=0,j=0;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
			LCD_Game_Print(i,j,x[i][j]);
	}
	itostr(score,SCORE);
	LCD_Print(46,0,SCORE,0);
}

void LCD_GameOver(void)
{
	LCD_Print(20,4,"game over!",0);
}

void LCD_GameWin(void)
{
	LCD_Print(20,4,"you win!",0);
}
