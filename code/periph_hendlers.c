/*
 * periph_hendlers.c
 *
 *  Created on: 18 окт. 2020 г.
 *      Author: Gandalf
 */
#include "periph_handlers.h"
#include "cmd_handler.h"
#include "tim.h"
#include "lora_protocol.h"
#include "gpio.h"


void USART1_IRQHandler(void) {
	if (USART1->ISR & USART_ISR_RXNE) {
		while(!(USART1->ISR & USART_ISR_TC));
		uart_receive_data = USART1->RDR;
		receive_buf[receive_data_counter] = uart_receive_data;
		receive_data_counter++;
		if(receive_data_counter == 1 && receive_buf[0] != 'r'){
			receive_buf[0] = 0;
			receive_data_counter = 0;
		}
		if(receive_data_counter == 3 && receive_buf[2] != 'b'){
			receive_buf[0] = 0;
			receive_buf[1] = 0;
			receive_buf[2] = 0;
			receive_data_counter = 0;
		}
		if(receive_data_counter > 3){

#ifdef SCREEN_PLUGGED
			DrawChar5x7(SPI1, 100, 10, receive_buf[0], YELLOW, BLACK);
			DrawString5x7(SPI1, 108, 10, NumbersArray(receive_buf[1], number_array), YELLOW, BLACK);
			DrawChar5x7(SPI1, 123, 10, receive_buf[2], YELLOW, BLACK);
			DrawString5x7(SPI1, 130, 10, NumbersArray(receive_buf[3], number_array), YELLOW, BLACK);
#endif
			receive_data_counter = 0;
			if(receive_buf[0] != (uint8_t)'r' || receive_buf[2] != (uint8_t)'b' || receive_buf[1] > 23){
				command_error_flag = 1;
			}
			else{
				CMD_Handler(receive_buf);

//				UART_tx_array(USART1, "Receive:");
//				UART_tx_array(USART1, (char*)receive_buf);
//				UART_tx(USART1, 0x0D);
				*(uint32_t*)receive_buf = (uint32_t)0;
			}
		}

	}
	if(USART1->ISR & USART_ISR_ORE){
 		uart_receive_data = USART1->RDR;
		USART1->ICR |= USART_ICR_ORECF;
		UART_tx_array(USART1, "USART1 OVERRUN ERROR!");
		UART_tx(USART1, 0x0D);
		if(USART1->ISR & USART_ISR_IDLE){
			USART1->ICR |= USART_ICR_IDLECF;
		}
		if(USART1->ISR & USART_ISR_NE){
			USART1->ICR |= USART_ICR_NCF;
		}
		else{
			USART1->ICR = 0xFF;
		}
	}

}
volatile uint8_t array[6] = {0};
void USART3_IRQHandler(void) {
	if (USART3->ISR & USART_ISR_RXNE) {
//		while(!(USART3->ISR & USART_ISR_TC));
		lora_receive_data = USART3->RDR;
		static uint8_t lora_init_byte_counter = 0;

		if(LoRa_init_flag == 1){
			if(LoRa_the_first_message == 0)
				LoRa_the_first_message = lora_receive_data;
			*((uint8_t*)(&lora_struct) + lora_init_byte_counter) = lora_receive_data;
			array[lora_init_byte_counter] = lora_receive_data;
			lora_init_byte_counter++;
			if(lora_init_byte_counter == 6){
				lora_init_byte_counter = 0;
				LoRa_init_flag = 0;
				gpio_state(LoRa_M0, LOW);
				gpio_state(LoRa_M1, LOW);
				lora_struct.MODE = 0;
			}
		}
		else{
			if(lora_struct.MODE == 0){
				int8_t packet_status = LoRa_protocol_handler(lora_receive_data);
				switch (packet_status) {
					case START_BYTE_ERROR:
	//					UART_tx_array(USART1, "START_BYTE_ERROR");
						break;
					case ADDRESS_BYTE_ERROR:
	//					UART_tx_array(USART1, "ADDRESS_BYTE_ERROR");
						break;
					case CMD_TYPE_ERROR:
	//					UART_tx_array(USART1, "CMD_TYPE_ERROR");
						break;
					case PACKET_LENGTH_ERROR:
	//					UART_tx_array(USART1, "PACKET_LENGTH_ERROR");
						break;
					case CRC_ERROR:
	//					UART_tx_array(USART1, "CRC_ERROR");
						break;
					case SUCCESS_RX:
		//				UART_tx_array(USART1, "SUCCESS_RX");
						break;
					default:
						break;
				}
	//			FIFO_PUSH(fifo, lora_receive_data);
			}
			else if(lora_struct.MODE == 3){
				FIFO_PUSH(fifo, lora_receive_data);
				FIFO_PUSH(fifo, '\n');
			}
		}
	}
	else if(USART3->ISR & USART_ISR_ORE){
		USART3->ICR |= USART_ICR_ORECF;
		LED_Handler(2, LED1_R, 0, 0);
		LED_Handler(2, LED2_R, 0, 0);
		LED_Handler(2, LED3_R, 0, 0);
		UART_tx_array(USART1, "USART3 OVERRUN ERROR!\r\n");
	}
}

void EXTI4_IRQHandler() {
	handler = BUZZ_stop;
//	gpio_init(BUZZ, PB15_TIM15_CH2, Push_pull, no_pull, High_speed);
//	PWM_init(TIM15, PWM_CH2, 800, 20); //PB0 100KHz 50%
//	PWM_start_single(TIM15, PWM_CH2);
	TIM_init(TIM7, 50, 0);

	button_pressed_counter++;
	EXTI->PR1 |= EXTI_PR1_PIF4;
}
void TIM7_IRQHandler(void){
#define TIM_LOOP_MODE
    TIM7_finished = 1;
    TIM7->SR &= ~TIM_SR_UIF;
#ifdef TIM_LOOP_MODE
    TIM7->DIER &= ~TIM_DIER_UIE;
#endif
	UART_tx_array(USART1, "TIM7_handler\r\n");
	handler();
}
