/*
 * uart.c
 *
 *  Created on: 3 ����. 2020 �.
 *      Author: BilboBaggins
 */
#include "stm32l431xx.h"
#include "uart.h"

#define APB2_CLK 80000000
#define APB1_CLK 80000000

void UART_init(USART_TypeDef *USARTx, int baudrate){
	if(USARTx == USART1){
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		NVIC_EnableIRQ(USART1_IRQn);
		USARTx->BRR = APB2_CLK / baudrate;
	}
	else if(USARTx == USART2){
		RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
		NVIC_EnableIRQ(USART2_IRQn);
		USARTx->BRR = APB1_CLK / baudrate;
	}
	else if(USARTx == USART3){
		RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;
		NVIC_EnableIRQ(USART3_IRQn);
		USARTx->BRR = APB1_CLK / baudrate;
	}
	else return;


	USARTx->CR1 &= ~(0x01 << 28); //word length 8 bit
	USARTx->CR1 &= ~(0x01 << 12); //word length 8 bit

	USARTx->CR1 &= ~(0x01 << 10); //parity disable
//	USARTx->CR1 |= parity_control_ << 10;

	USARTx->CR2 &= ~(0x03 << 12); //1 stop bit
//	USARTx->CR2 |= stop_bits_ << 12;

	USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USARTx->CR1 |= USART_CR1_UE; // enable USART
	USARTx->CR1 |= USART_CR1_RXNEIE; // RXNE Int ON


}
void UART_tx(USART_TypeDef *USARTx, uint8_t data){
	while(!(USARTx->ISR & USART_ISR_TC));
	USARTx->TDR = data;
}
void UART_tx_array(USART_TypeDef *USARTx, char *array){
	for(uint8_t i = 0; array[i] != '\0'; i++) {
		UART_tx(USARTx, array[i]);
	}
}

