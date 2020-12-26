
#include <font7x11.h>
#include "System.h"
#include "SPI.h"
#include "delay.h"
#include "ST7735.h"
#include "gpio.h"
#include "font5x7.h"
#include <string.h>
#include "stm32l4xx.h"

void Send_CMD(SPI_TypeDef *SPIx, uint8_t cmd){
	while(SPIx->SR & SPI_SR_BSY);
	LCD_A0_Command;
	spi_send8(SPIx, cmd);
//	DMA_SPI_Send8(cmd);
	while(SPIx->SR & SPI_SR_BSY);
}
void Send_DAT8(SPI_TypeDef *SPIx, uint8_t data){

	LCD_A0_Data;
	spi_send8(SPIx, data);
//	DMA_SPI_Send8(data);

}
void Send_DAT16(SPI_TypeDef *SPIx, uint16_t data){

	LCD_A0_Data;
	spi_send16(SPIx, data);
//	DMA_SPI_Send16(data);
}
void SetArea(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y_begin, uint8_t Y_end){
	Send_CMD(SPIx, CASET);
	Send_DAT8(SPIx, 0);
	Send_DAT8(SPIx, X_begin);
	Send_DAT8(SPIx, 0);
	Send_DAT8(SPIx, X_end);
	Send_CMD(SPIx, RASET);
	Send_DAT8(SPIx, 0);
	Send_DAT8(SPIx, Y_begin);
	Send_DAT8(SPIx, 0);
	Send_DAT8(SPIx, Y_end);
	Send_CMD(SPIx, RAMWR);
}
void DrawEmptyRectangle(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y_begin, uint8_t Y_end, uint16_t color){
	SetArea(SPIx, X_begin, X_end, Y_begin, Y_begin);
	int x, y;
	for(x = X_begin; x <= X_end; x++){
		Send_DAT16(SPIx, color);
	}
	SetArea(SPIx, X_begin, X_begin, Y_begin, Y_end);
	for(y = Y_begin; y <= Y_end; y++){
		Send_DAT16(SPIx, color);
	}
	SetArea(SPIx, X_begin, X_end, Y_end, Y_end);
	for(x = X_begin; x <= X_end; x++){
		Send_DAT16(SPIx, color);
	}
		SetArea(SPIx, X_end, X_end, Y_begin, Y_end);
	for(y = Y_begin; y <= Y_end; y++){
		Send_DAT16(SPIx, color);
	}
}
void Draw_V_Line(SPI_TypeDef *SPIx, uint8_t X,  uint8_t Y_begin, uint8_t Y_end, uint16_t color){
	SetArea(SPIx, X, X, Y_begin, Y_end);
	volatile uint8_t y;
	for(y = Y_begin; y <= Y_end; y++){
		Send_DAT16(SPIx, color);
	}
}
void Draw_H_Line(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y, uint16_t color){
	SetArea(SPIx, X_begin, X_end, Y, Y);
	for(int x = X_begin; x <= X_end; x++){
		Send_DAT16(SPIx, color);
	}
}
void Draw_V_DottedLine(SPI_TypeDef *SPIx, uint8_t X,  uint8_t Y_begin, uint8_t Y_end, uint8_t freq, uint8_t dur, uint16_t color, uint16_t background_color){
	SetArea(SPIx, X, X, Y_begin, Y_end);
		volatile uint8_t y;
		for(y = Y_begin; y <= Y_end; y++){
			if(y % freq == 0){
				for(int i = 0; i < dur; i++){
					Send_DAT16(SPIx, background_color);
				}
			}
			Send_DAT16(SPIx, color);
		}
}
void Draw_H_DottedLine(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y, uint8_t freq, uint8_t dur, uint16_t color, uint16_t background_color){
	SetArea(SPIx, X_begin, X_end, Y, Y);
	for(int x = X_begin; x <= X_end; x++){
		if(x % freq == 0){
			for(int i = 0; i < dur; i++){
				Send_DAT16(SPIx, background_color);
			}
		}
		Send_DAT16(SPIx, color);
	}
}
void DrawPixel(SPI_TypeDef *SPIx, uint8_t X, uint8_t Y, uint16_t color){
	SetArea(SPIx, X, X+1, Y, Y+1);
	Send_DAT16(SPIx, color);
}
void FullFill(SPI_TypeDef *SPIx, uint16_t color){
	SetArea(SPIx, 0, 160, 0, 128);
	for(int i = 0; i < 20480; i++){
		Send_DAT16(SPIx, color);
	}
}
void FillArea(SPI_TypeDef *SPIx, uint8_t X_begin, uint8_t X_end, uint8_t Y_begin, uint8_t Y_end, uint16_t color){
	SetArea(SPIx, X_begin, X_end, Y_begin, Y_end);
		for(int i = 0; i <= (X_end - X_begin + 1)*(Y_end - Y_begin+1); i++){
		Send_DAT16(SPIx, color);
	}
}




void DrawBMP(SPI_TypeDef *SPIx, const uint8_t *array, uint32_t length){
	uint16_t i;
	SetArea(SPIx, 0, 34, 0, 34);
	for(i=0; i < length; i++){
		Send_DAT8(SPIx, array[i]);
	}
}

void DrawChar5x7(SPI_TypeDef *SPIx, uint8_t x, uint8_t y, uint8_t chr, uint16_t text_color, uint16_t background_color){
	uint8_t i,j;
	uint8_t buffer[5];
	
	memmove(buffer, &Font5x7[(chr-32)*5], 5);
	
	SetArea(SPIx, x, x+5, y, y+7);
	for(i = 0; i < 7; i++){
		for(j = 0; j < 5; j++){
			if((buffer[j] & (1<<i)) == 0){
				Send_DAT16(SPIx, background_color);
			}
			else{
				Send_DAT16(SPIx, text_color);
			}
		}
		Send_DAT16(SPIx, background_color);
	}
}


void DrawChar7x11(SPI_TypeDef *SPIx, uint8_t x, uint8_t y, uint8_t chr, uint16_t text_color, uint16_t background_color){
	uint8_t i,j;
	uint8_t buffer[11];
		
	memmove(buffer, &Font7x11[(chr - 32) * 11], 11);
	
	SetArea(SPIx, x, x + 7, y, y + 11);
	for(j = 0; j < 11; j++){
		for(i = 0; i < 7; i++){
			if((buffer[j] & (1<<i))==0){
				Send_DAT16(SPIx, background_color);
			}
			else{
				Send_DAT16(SPIx, text_color);
			}
		}
		Send_DAT16(SPIx, background_color);
	}

}
uint16_t Decoder(uint8_t bigByte, uint8_t littleByte){
    uint8_t lB = bigByte & 0x0F; // 0x9C ->  0x0C
    uint8_t bB = bigByte>>4; 	 // 0x9C ->  0x09

    uint8_t lL = littleByte & 0x0F;
    uint8_t bL = littleByte>>4;

    return lL + (bL<<4) + (lB<<8) + (bB<<12);
}
uint8_t *NumbersArray(uint32_t number, uint8_t *destination){
  uint8_t count = 0;
  do {
    uint8_t digit = number % 10;
    destination[count++] = (digit > 9) ? digit - 10 +'A' : digit + '0';
  } while ((number /= 10) != 0);
  destination[count] = '\0';
  uint8_t i;
  for (i = 0; i < count / 2; ++i) {
    uint8_t symbol = destination[i];
    destination[i] = destination[count - i - 1];
    destination[count - i - 1] = symbol;
  }
  return destination;
}
void DrawString7x11(SPI_TypeDef *SPIx, uint8_t x, uint8_t y, uint8_t* chr, uint16_t text_color, uint16_t background_color){
	uint8_t i=0;
	while(chr[i] != '\0'){
		DrawChar7x11(SPIx, x+(8*i), y, chr[i], text_color, background_color);
		i++;
	}
}
void DrawString5x7(SPI_TypeDef *SPIx, uint8_t x, uint8_t y, uint8_t* chr, uint16_t text_color, uint16_t background_color){
	uint8_t i=0;
	while(chr[i] != '\0'){
		DrawChar5x7(SPIx, x+(7*i), y, chr[i], text_color, background_color);
		i++;
	}
}
void cs_low(){
	gpio_state(PA4, LOW);
}
void cs_high(){
	gpio_state(PA4, HIGH);
}
void Init_ST7735(SPI_TypeDef *SPIx, uint8_t A0, uint8_t Reset){
//	spi_init(SPI1, PA4, div_2, Mode_0, data_8_bit, MSB);

	gpio_state(Reset, LOW);
	gpio_state(Reset, HIGH);
	
	cs_low();
//	Delay(50);
	Send_CMD(SPIx, SWRESET);
	Delay(12);

	Send_CMD(SPIx, SLPOUT);
	Delay(12);
	
	Send_CMD(SPIx, COLMOD);
	Send_DAT8(SPIx, 0x05);
	
	Send_CMD(SPIx, MADCTL);
	Send_DAT8(SPIx, 0x60);
	
	Send_CMD(SPIx, FRMCTR1);
	Send_DAT16(SPIx, 0x000F);
	Send_DAT16(SPIx, 0x000F);
	Send_DAT16(SPIx, 0x000F);
//	Delay(50);
	FullFill(SPIx, BLUE);
//	Delay(50);
	Send_CMD(SPIx, DISPON);
	Draw_H_Line(SPIx, 10, 70, 20, RED);
	cs_high();
}

uint8_t frac_to_int(float frac){
	return (frac - (uint16_t)frac) * 100;
}
char* string_form(char *arr, uint16_t val){
	arr[0] = ',';
	arr[1] = val % 10 + 48;
	arr[2] = val / 10 + 48;
	arr[3] = '\0';
	return arr;
}
