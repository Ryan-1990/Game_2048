
void DLY_ms(unsigned int ms);
void LCD_command_tx(unsigned char a);
void LCD_data_tx(unsigned char a);
void LCD_init(void);
void LCD_CLS(void);
void LCD_clear(void);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch,unsigned char flag);
void LCD_Print(unsigned char x,unsigned char y,unsigned char * ch,unsigned char flag);
void LCD_Draw_Battery(unsigned char percent);
void LCD_game_init(void);
void LCD_OUT(void);
void LCD_GameOver(void);
void LCD_GameWin(void);
