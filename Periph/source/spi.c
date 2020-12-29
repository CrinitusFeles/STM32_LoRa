#include "stm32l4xx.h"

void spi_init(SPI_TypeDef *SPIx, int slave_pin, int speed_div_, int mode_, int data_length_, int first_bit_){
	/*
	 * -----------------------------------------------
	 *  SPI1 - APB2;
	 *
	 * 	NSS -- 	PA4 AFIO PP / Input pull-up
     *  SCK -- 	PA5 AFIO PP
	 *  MISO -- PA6 Input pull-up
	 *  MOSI -- PA7 AFIO PP
	 *  ----------------------------------------------
	 *  SPI2 - APB1;
	 *
	 *  NSS -- 	PB12 AFIO PP / Input pull-up
     *  SCK -- 	PB13 AFIO PP
	 *  MISO -- PB14 Input pull-up
	 *  MOSI -- PB15 AFIO PP
	 *  ----------------------------------------------
	 *  Mode 0 - CPOL=0; CPHA=0
	 *  Mode 1 - CPOL=0; CPHA=1
	 *  Mode 2 - CPOL=1; CPHA=0
	 *  Mode 3 - CPOL=1; CPHA=1
	 */
	if(SPIx == SPI1){
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	}
	else if(SPIx == SPI2){
		RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
	}

	SPIx->CR1 |= speed_div_ << 3;
	SPIx->CR1 |= mode_ << 0;
	SPIx->CR1 |= first_bit_ << 7;
	SPIx->CR1 |= SPI_CR1_MSTR;  //Mode Master
	SPIx->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; //Software slave management & Internal slave select


//	SPIx->CR1 |= 0x01 << 14;
	SPIx->CR1 |= 0x00 << 11;
//	SPIx->CR2 = 0x300;
//	SPIx->CR2 &= 0x04 << 8;
//	SPIx->CR2 |= 0x01 << 12;

	SPIx->CR1 |= SPI_CR1_SPE; //Enable SPI1
//	SPIx->CR2 |= 0x04 << 8;
}

void spi_send8(SPI_TypeDef *SPIx, uint8_t data){
	while(!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = (uint8_t)data;
}
void spi_send_array(SPI_TypeDef *SPIx, uint8_t *data){
	for(int i = 0; i <= sizeof(data)/sizeof(data[0]); i++){
		spi_send8(SPIx, (uint8_t)data[i]);
	}
}
void spi_send16(SPI_TypeDef *SPIx, uint16_t data){
	uint8_t littleByte = data & 0xFF;
	uint8_t bigByte = (data>>8) & 0xFF;

	while(!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = bigByte;
	while(!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = littleByte;

}
uint8_t spi_waiting_read8(SPI_TypeDef *SPIx){
	SPIx->DR = 0;
	while(!(SPIx->SR & SPI_SR_RXNE));
	return SPIx->DR;
}
uint8_t spi_request_read8(SPI_TypeDef *SPIx){
	SPIx->DR = 0;
	while(!(SPIx->SR & SPI_SR_RXNE));
	return SPIx->DR;
}
