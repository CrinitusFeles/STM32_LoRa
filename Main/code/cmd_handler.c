/*
 * cmd_handler.c
 *
 *  Created on: 18 окт. 2020 г.
 *      Author: Gandalf
 */


#include "main.h"

void CMD_Handler(uint8_t *cmd_string){
	volatile uint8_t cmd = cmd_string[3];
	volatile uint8_t offset = cmd_string[1];
	switch (offset) {
		case 0:
			LED1_REGISTER_Handler(cmd);
			break;
		case 1:
			LED2_REGISTER_Handler(cmd);
			break;
		case 2:
			LED3_REGISTER_Handler(cmd);
			break;
		case 3:
			BUZZ_REGISTER1_Handler(cmd); //frequency
			break;
		case 4:
			BUZZ_REGISTER2_Handler(cmd); //duration
			break;
		case 5:
			BUZZ_REGISTER3_Handler(cmd); //volume
			break;
		case 6:
			lora_struct.ADDH = cmd;
			break;
		case 7:
			lora_struct.ADDL = cmd;
			break;
		case 8:
			*(uint8_t*)(&lora_struct.SPED) = cmd;
			break;
		case 9:
			lora_struct.CHAN = cmd;
			break;
		case 10:
			lora_struct.MODE = cmd;
			break;
		case 11:
			*(uint8_t*)(&lora_struct.OPTION) = cmd;
			break;
		case 12:
			LORA_REGISTER1_Handler(cmd);  //(in sleep mode) send instruction
			break;
		case 13:
			LORA_REGISTER2_Handler(cmd);  //send data
			break;
		case 14:
			TMP1075_CONFIG_REGISTER_Handler(cmd);
			break;
		case 15:
			RETRANSLATE_CMD_PACKET(cmd);
			break;
		case 16:
			REQUEST_DATA(cmd); //tmp1075
			break;
		case 17:
			PC_CONNECT(cmd);
			break;
		case 18:
			FIFO_PUSH(long_cmd_fifo, cmd); //forming long cmd packet
			break;
		case 19:
			SEND_UNIC_CMD(cmd);
			break;
		case 20:
			LoRa_self_address = cmd;
			break;
		case 21:
			NVIC_SystemReset();
		case 22:
			TIME_TO_RESET = cmd;
		case 23:
			ADC_REQUEST();
//		case 15:
//			TMP1075_GET_DATA_Handler(cmd);
//			break;
//		case 16:
//			TMP1075_GET_CONFIG_Handler(cmd);
//			break;
//		case 17:
//			SD_REGISTER(cmd);
//			break;


		default:
			UART_tx_array(USART1, "incorrect cmd");
			UART_tx(USART1, 0x0D);
			break;
	}
}

void SEND_UNIC_CMD(uint8_t cmd){
	uint16_t crc16 = Crc16((long_cmd_fifo.buf + (long_cmd_fifo.tail & (FIFO_SIZE(long_cmd_fifo)-1))), FIFO_COUNT(long_cmd_fifo));
	FIFO_PUSH(long_cmd_fifo, crc16 >> 8);
	FIFO_PUSH(long_cmd_fifo, crc16 & 0xFF);
	if(lora_struct.MODE != LORA_NORMAL_MODE){
		set_lora_normal_mode();
		DWT_Delay_ms(100);
	}
	for(uint8_t i = 0; FIFO_COUNT(long_cmd_fifo) != 0; i++){
		UART_tx(USART3, FIFO_FRONT(long_cmd_fifo));
		FIFO_POP(long_cmd_fifo);
	}
	if(PC_CONNECT_START == 1){
		TIME_TO_RESET_COUNTER = 0;
		UART_tx_array(USART1, "OK\n");

	}

}
void set_lora_normal_mode(){
	gpio_state(LoRa_M1, LOW);
	gpio_state(LoRa_M0, LOW);
	lora_struct.MODE = LORA_NORMAL_MODE;
}
void get_lora_parameters(){
	UART_tx(USART3, 0xC1);
	UART_tx(USART3, 0xC1);
	UART_tx(USART3, 0xC1);
}
uint8_t num_arr[10];
void PC_CONNECT(uint8_t cmd){
	PC_CONNECT_START = 1;
//	UART_tx_array(USART1, "Device ID: ");
//	UART_tx(USART1, LoRa_self_address + 48);
	UART_tx(USART1, LoRa_self_address);
	for(uint8_t i = 0; i < 7; i++)
		UART_tx(USART1, *((uint8_t*)(&lora_struct) + i));
	UART_tx(USART1, '\n');
}

void REQUEST_DATA(uint8_t cmd){
	uint8_t tx_packet[30] = {0};
	volatile uint8_t address_counter = 0;
	uint8_t CRC_SIZE = 2;
	tx_packet[0] = 0xFF;
	tx_packet[2] = 0x01;
	if(LoRa_last_rx_packet[2] == 2){ //SHORT_CMD_PACKET
		tx_packet[3] = LoRa_last_rx_packet[3] + sensors_data_size + 2; // n байт данных, байт счетчика, байт адреса в цепочке
		tx_packet[1] = 0x00;
		tx_packet[4] = 0x00;
		tx_packet[5] = 0x00;
		for(uint8_t i = 0; i < sensors_data_size; i++){
			tx_packet[6 + i] = (*((uint8_t*)(current_station_stats.sensors_data) + i));
		}
	}
	else if(LoRa_last_rx_packet[2] == 0){ //LONG_CMD_PACKET
		tx_packet[3] = LoRa_last_rx_packet[3] + sensors_data_size; // добавляется только n байт данных
		tx_packet[4] = 0x00;
		for(uint8_t i = 5, j = LoRa_last_rx_packet[3] - CRC_SIZE - 2 - 2; i < LoRa_last_rx_packet[3] - CRC_SIZE - 2-1; i++, j--){ // разворачиваем адреса
			tx_packet[i] = LoRa_last_rx_packet[j];
			address_counter++;
		}
		tx_packet[4 + address_counter + 1] = 0x00; // последний адрес в цепи - адрес базы
		tx_packet[1] = tx_packet[5];

		switch (cmd) {
			case 0x00: // запрос данных только с датчиков
				for(uint8_t i = 0; i < sensors_data_size; i++){
					tx_packet[5 + address_counter + 1 + i] = (*((uint8_t*)(current_station_stats.sensors_data) + i));
				}
				break;
			default:
				break;
		}
	}
	tx_packet[tx_packet[3]-1 - CRC_SIZE - 1] = cmd; //data type
	// data_len = packet_len - CRC_SIZE - data_len_byte - data_type_byte - header - address_chain
	tx_packet[tx_packet[3]-1 - CRC_SIZE] = tx_packet[3] - CRC_SIZE - 1 - 1 - 5 - (address_counter + 1);
	uint16_t crc16 = Crc16(tx_packet, tx_packet[3]-2);
	tx_packet[tx_packet[3]-2] = crc16 >> 8;
	tx_packet[tx_packet[3]-1] = crc16 & 0xFF;

	for(uint8_t i = 0; i < tx_packet[3]; i++){
		UART_tx(USART3, tx_packet[i]);
	}


}
void RETRANSLATE_CMD_PACKET(uint8_t cmd){
#define LONG_PACKET
#ifndef LONG_PACKET
	uint8_t packet_len = 7;
					   //(start, addr, type, length,    cmd_r, cmd_b, crc)
	uint8_t tx_packet[] = {0xFF, 0x01, 0x02, packet_len, 0x00, cmd, 0x00};
#else
	uint8_t packet_len = 12;
					   //(start, addr, type, length,  counter, addr1, addr2, addr3, cmd_r, cmd_b, crc1, crc2)
	uint8_t tx_packet[] = {0xFF, 0x01, 0x00, packet_len, 0x00, 0x01, 0x02, 0x03, 0x10, cmd, 0x00, 0x00};
#endif
	uint16_t packet_crc = Crc16(tx_packet, packet_len-2);
	tx_packet[packet_len-2] = packet_crc >> 8;
	tx_packet[packet_len-1] = packet_crc & 0xFF;

	for(uint8_t i = 0; i < packet_len; i++){
		UART_tx(USART3, tx_packet[i]);
	}
}
void ADC_REQUEST(){
	UART_tx(USART3, (uint16_t)battary_voltage);
	UART_tx(USART3, (uint16_t)((uint32_t)(battary_voltage*100) % 100));
}
void TMP1075_CONFIG_REGISTER_Handler(uint8_t cmd){
	tmp1075_sensor1_config = cmd;
	tmp1075_sensor2_config = cmd;
	tmp1075_sensor3_config = cmd;
	tmp1075_sensor4_config = cmd;
	tmp1075_sensor5_config = cmd;
	TMP1075_write_config(I2C3, 0x4F, cmd);
	TMP1075_write_config(I2C3, 0x4B, cmd);
	TMP1075_write_config(I2C3, 0x4D, cmd);
	TMP1075_write_config(I2C3, 0x48, cmd);
	TMP1075_write_config(I2C3, 0x4E, cmd);
}
void BUZZ_stop(){
	PWM_stop_single(TIM15, PWM_CH2);
}
void BUZZ_REGISTER1_Handler(uint8_t cmd){
	buzz_frequency = 440 + cmd * 2;
}

void BUZZ_REGISTER2_Handler(uint8_t cmd){
	buzz_duration = 10 + cmd * 4;
}
void BUZZ_REGISTER3_Handler(uint8_t cmd){
	handler = BUZZ_stop;
	gpio_init(BUZZ, PB15_TIM15_CH2, Push_pull, no_pull, High_speed);
	PWM_init(TIM15, PWM_CH2, buzz_frequency, 10 + (cmd & 0x03) * 30);
	PWM_start_single(TIM15, PWM_CH2);
	TIM_init(TIM7, buzz_duration, 0);
}
void LORA_REGISTER1_Handler(uint8_t cmd){
	uint8_t instruction1[] = {0xC0, lora_struct.ADDH, lora_struct.ADDL, *(uint8_t*)(&lora_struct.SPED), lora_struct.CHAN, *(uint8_t*)(&lora_struct.OPTION), '\0'};
	uint8_t instruction2[] = {0xC1, 0xC1, 0xC1, '\0'};
	uint8_t instruction3[] = {0xC2, lora_struct.ADDH, lora_struct.ADDL, *(uint8_t*)(&lora_struct.SPED), lora_struct.CHAN, *(uint8_t*)(&lora_struct.OPTION), '\0'};
	uint8_t instruction4[] = {0xC3, 0xC3, 0xC3, '\0'};
	uint8_t instruction5[] = {0xC4, 0xC4, 0xC4, '\0'};
	switch (cmd) {
		case 1:
			UART_tx_array(USART3, (char*)instruction1);
			break;
		case 2:
			UART_tx_array(USART3, (char*)instruction2);
			break;
		case 3:
			UART_tx_array(USART3, (char*)instruction3);
			break;
		case 4:
			UART_tx_array(USART3, (char*)instruction4);
			break;
		case 5:
			UART_tx_array(USART3, (char*)instruction5);
			break;
		case 6:
			gpio_state(LoRa_M0, HIGH);
			gpio_state(LoRa_M1, HIGH);
			lora_struct.MODE = LORA_SLEEP_MODE;
			UART_tx_array(USART1, "Transmit mode disabled\r\n");
			break;
		case 7:
			gpio_state(LoRa_M0, LOW);
			gpio_state(LoRa_M1, LOW);
			lora_struct.MODE = LORA_NORMAL_MODE;
			UART_tx_array(USART1, "Transmit mode enabled\r\n");
			break;
		case 8:
			if(lora_struct.MODE == LORA_SLEEP_MODE){
				UART_tx(USART3, 0xC2);
				UART_tx(USART3, lora_struct.ADDH);
				UART_tx(USART3, lora_struct.ADDL);
//				UART_tx(USART3, *(uint8_t*)&lora_struct.SPED.SPED_REG_TRUE);
				UART_tx(USART3, (lora_struct.SPED.SPED_REG_REVERSE.parity << 6) | (lora_struct.SPED.SPED_REG_REVERSE.uart_bps << 3) |
						lora_struct.SPED.SPED_REG_REVERSE.air_bps);
				UART_tx(USART3, lora_struct.CHAN);
//				UART_tx(USART3, *(uint8_t*)&lora_struct.OPTION.OPTION_REG_TRUE);
				UART_tx(USART3, (lora_struct.OPTION.OPTION_REG_REVERSE.fixed_transmission << 7) |
						(lora_struct.OPTION.OPTION_REG_REVERSE.io_drive_mode << 6) |
						(lora_struct.OPTION.OPTION_REG_REVERSE.wake_up_time << 3) |
						(lora_struct.OPTION.OPTION_REG_REVERSE.FEC_switch << 2) |
						(lora_struct.OPTION.OPTION_REG_REVERSE.tx_power));
			}
			break;
		case 9:
			if(lora_struct.MODE == LORA_SLEEP_MODE){ // to eeprom
				UART_tx(USART3, 0xC0);
				UART_tx(USART3, lora_struct.ADDH);
				UART_tx(USART3, lora_struct.ADDL);
				UART_tx(USART3, *(uint8_t*)&lora_struct.SPED.SPED_REG_REVERSE);
				UART_tx(USART3, lora_struct.CHAN);
				UART_tx(USART3, *(uint8_t*)&lora_struct.OPTION.OPTION_REG_REVERSE);
			}
			break;
		default:
			break;
	}
}

void LORA_REGISTER2_Handler(uint8_t cmd){

	UART_tx(USART1, LoRa_self_address);
	UART_tx(USART1, lora_struct.HEAD);
	UART_tx(USART1, lora_struct.ADDH);
	UART_tx(USART1, lora_struct.ADDL);
	UART_tx(USART1, *(uint8_t*)&lora_struct.SPED.SPED_REG_REVERSE);
	UART_tx(USART1, lora_struct.CHAN);
	UART_tx(USART1, *(uint8_t*)&lora_struct.OPTION.OPTION_REG_REVERSE);
	UART_tx(USART1, lora_struct.MODE);
	UART_tx(USART1, '\n');


}
