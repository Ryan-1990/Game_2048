#include "FM25L16.h"
#include "game_console.h"


void FRAM_init(void)
{
	DDRB |= (1<<2)|(1<<5)|(1<<7); //HOLD & MOSI & SCK (output)
	PORTB |= (1<<2); //set HOLD high
	DDRB &= ~(1<<6); //MISO (input)
	//PORTB |= (1<<6); //pull-up
	
	DDRD |= (1<<2)|(1<<4); //WP & CS (output)
	PORTD|=(1<<2);
	EROM_CS_1;
}

void write_byte(unsigned char abyte)
{
	///*
	unsigned char i;   
	       
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
	//*/
}

unsigned char read_byte(void)
{
	///*
	unsigned char i,spi_dat=0;  
	SCLK_0;
	for(i=0;i<8;i++)  
	{                                  
		SCLK_0; 
		spi_dat=(spi_dat<<1);  
		if(FRAM_SO) spi_dat|=0x01;	
		else spi_dat&=~0x01; 
		SCLK_1;	 	 	
	}  

	return (spi_dat); 
	//*/ 
}

//Memory Writes:
//Format: WREN op-code, WRITE op-code, MSB Address, LSB Address, Data-in, (Data-in, Data-in, ...)
void FRAM_WriteMemory(unsigned char Addrh,unsigned char Addrl,unsigned char Data)
{
	///*
	CS_1;
	SCLK_1;
	EROM_CS_0;
	write_byte(SPI_WREN);
	EROM_CS_1;
	EROM_CS_0;
	write_byte(SPI_WRITE);
	write_byte(Addrh);
	write_byte(Addrl);//Addr
	write_byte(Data);//Data
	EROM_CS_1;
	//*/
}

//Memory Reads
//Format: READ op-code, MSB Address, LSB Address, Data-out, (Data-out, Data-out, ...)
unsigned char FRAM_ReadMemory(unsigned char Addrh,unsigned char Addrl)
{
	///*
	unsigned char i;

	CS_1;
	SCLK_1;
	EROM_CS_0;
	write_byte(SPI_READ);
	write_byte(Addrh);
	write_byte(Addrl);//Addr
	i=read_byte();
	EROM_CS_1;
	return (i);
	//*/
}
