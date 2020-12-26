
#ifndef ST7735
#define ST7735

/*
		 (SPI1)
	 TFT	|	STM32
	-----------------
	|LED 	-	3.3V|
	|SCK 	- 	PA5	|
	|SDA 	- 	PA7	|
	|A0		- 	PA2	|
	|RESET  - 	PA3	|
	|CS		- 	PA4	|
	|GND 	- 	GND	|
	|VCC 	- 	3.3V|
	-----------------
*/

/*
	 RED5		GREEN6	 BLUE5
	00000 | 000000 | 00000
*/

#define 	 LCD_Reset_HIGH				GPIOB->BSRR = GPIO_BSRR_BS13;
#define 	 LCD_Reset_LOW				GPIOB->BSRR = GPIO_BSRR_BR13;
#define 	 LCD_A0_Data				GPIOB->BSRR = GPIO_BSRR_BS12; //LCD
#define 	 LCD_A0_Command				GPIOB->BSRR = GPIO_BSRR_BR12;


#define NOP     0x00
#define SWRESET 0x01
#define RDDID   0x04
#define RDDST   0x09

#define SLPIN   0x10
#define SLPOUT  0x11
#define PTLON   0x12
#define NORON   0x13

#define INVOFF  0x20
#define INVON   0x21
#define DISPOFF 0x28
#define DISPON  0x29
#define CASET   0x2A
#define RASET   0x2B
#define RAMWR   0x2C
#define RAMRD   0x2E

#define PTLAR   0x30
#define COLMOD  0x3A
#define MADCTL  0x36

#define FRMCTR1 0xB1
#define FRMCTR2 0xB2
#define FRMCTR3 0xB3
#define INVCTR  0xB4
#define DISSET5 0xB6

#define PWCTR1  0xC0
#define PWCTR2  0xC1
#define PWCTR3  0xC2
#define PWCTR4  0xC3
#define PWCTR5  0xC4
#define VMCTR1  0xC5

#define RDID1   0xDA
#define RDID2   0xDB
#define RDID3   0xDC
#define RDID4   0xDD

#define PWCTR6  0xFC

#define GMCTRP1 0xE0
#define GMCTRN1 0xE1

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF




//#define color				BLUE
//#define backgroundcolor		BLACK
//#define Y_axis_begin		16
//#define Y_axis_end			115
//#define Y_axis_coordinate	28
//#define X_axis_begin		28
//#define X_axis_end			152
//#define X_axis_coordinate	115
//#define dashSize			3

void cs_low();
void cs_high();
void Send_CMD(SPI_TypeDef *SPIx, uint8_t cmd);
void Send_DAT8(SPI_TypeDef *SPIx, uint8_t data);
void Send_DAT16(SPI_TypeDef *SPIx, uint16_t data);
void SetArea(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y_begin, uint8_t Y_end);
void DrawEmptyRectangle(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y_begin, uint8_t Y_end, uint16_t color);
void Draw_V_Line(SPI_TypeDef *SPIx, uint8_t X,  uint8_t Y_begin, uint8_t Y_end, uint16_t color);
void Draw_H_Line(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y, uint16_t color);
void Draw_H_DottedLine(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y, uint8_t freq, uint8_t dur, uint16_t color, uint16_t background_color);
void Draw_V_DottedLine(SPI_TypeDef *SPIx, uint8_t X,  uint8_t Y_begin, uint8_t Y_end, uint8_t freq, uint8_t dur, uint16_t color, uint16_t background_color);
void DrawPixel(SPI_TypeDef *SPIx, uint8_t X, uint8_t Y, uint16_t color);
void DrawString7x11(SPI_TypeDef *SPIx, uint8_t x, uint8_t y, uint8_t* chr, uint16_t text_color, uint16_t background_color);
void DrawString5x7(SPI_TypeDef *SPIx, uint8_t x, uint8_t y, uint8_t* chr, uint16_t text_color, uint16_t background_color);
void DrawChar7x11(SPI_TypeDef *SPIx, uint8_t x, uint8_t y, uint8_t chr, uint16_t text_color, uint16_t background_color);
void DrawChar5x7(SPI_TypeDef *SPIx, uint8_t x, uint8_t y, uint8_t chr, uint16_t text_color, uint16_t background_color);
uint8_t *NumbersArray(uint32_t number, uint8_t *destination);
uint16_t Decoder(uint8_t bigByte, uint8_t littleByte);
void FullFill(SPI_TypeDef *SPIx, uint16_t color);
void FillArea(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y_begin, uint8_t Y_end, uint16_t color);
void Init_ST7735(SPI_TypeDef *SPIx, uint8_t A0, uint8_t Reset);

uint8_t frac_to_int(float frac);
char* string_form(char *arr, uint16_t val);
#endif //ST7735

