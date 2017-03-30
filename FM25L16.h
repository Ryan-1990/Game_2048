#define SPI_WREN  0x06 
#define SPI_WRDI  0x04 
#define SPI_RDSR  0x05 
#define SPI_WRSR  0x01
#define SPI_READ  0x03 
#define SPI_WRITE 0x02


void FRAM_init(void);
void FRAM_WriteMemory(unsigned char Addrh,unsigned char Addrl,unsigned char Data);
unsigned char FRAM_ReadMemory(unsigned char Addrh,unsigned char Addrl);
