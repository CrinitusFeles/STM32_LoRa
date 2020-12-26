/*
 * lora_protocol.c
 *
 *  Created on: 18 ���. 2020 �.
 *      Author: Gandalf
 */

#include "lora_protocol.h"
// #include "tim.h"


uint8_t hlp_rx_buffer[30] = {0};

#define CRC_SIZE				2

#define START_BYTE 				hlp_rx_buffer[0]
#define ADDRESS_BYTE 			hlp_rx_buffer[1]
#define CMD_TYPE_BYTE 			hlp_rx_buffer[2]
#define PACKET_LENGTH_BYTE 		hlp_rx_buffer[3]
#define PACKET_CRC8_BYTE 		hlp_rx_buffer[hlp_rx_buffer[3]-1]
#define PACKET_CRC16_BYTE 		(hlp_rx_buffer[hlp_rx_buffer[3]-1] | (hlp_rx_buffer[hlp_rx_buffer[3]-2] << 8))
#define PACKET_LAST_DATA		hlp_rx_buffer[hlp_rx_buffer[3]-1 - CRC_SIZE]
#define PACKET_LAST_INDEX		(hlp_rx_buffer[3]-1)
#define PACKET_LAST_LAST_DATA	hlp_rx_buffer[hlp_rx_buffer[3]-1 - (CRC_SIZE + 1)]

#define PACKET_DATA_LENGTH		hlp_rx_buffer[hlp_rx_buffer[3] - (CRC_SIZE + 1)]
#define PACKET_DATA_TYPE		hlp_rx_buffer[hlp_rx_buffer[3] - (CRC_SIZE + 2)]

//packet types
enum {
	LONG_CMD_PACKET, // ����� �������� ����, ������� ����� ������������ �� ������� � �������, ���� �� ��������� ��������
	DATA_PACKET, // � ����� �� ������� ������� ������ ����������� ������ ��� ������, ���������� ���������� ������ � ��������
	SHORT_CMD_PACKET,
	ACK_PACKET, // ��� �������� �������� ������, ����� �������� �����������, ��� �� ��� ������� �������.
				//�����, �� ���������� ��������� ��� ��� ���, ����� ���� �������� FUCKUP ����� �� ����
	FUCKUP_PACKET // ����� ������� ������ �� ��������, ����� ��������� �� ���� ����, ��� ���� � ���������� ������ �����
} cmd_types;

uint8_t retranslate_cmd[4] = {0};

int8_t LoRa_protocol_handler(uint8_t rx){

	static uint8_t hlp_m_counter = 0;

	hlp_rx_buffer[hlp_m_counter] = rx;

	switch (hlp_m_counter) {
		case 0: // ��������� ������ �������� ����
			if(START_BYTE != 0xFF){ // ���������� ���, ��� �� �������� ��������� ������
				hlp_rx_buffer[0] = 0;
				hlp_m_counter = 0;
				return START_BYTE_ERROR;
			}
			else{
//				TIM_init(TIM7, 200);
			}
			break;
		case 1: // ���� ���������� ���� ��� ���������, �� ��������� ������ �������� ����
			if(ADDRESS_BYTE != (LoRa_self_address & 0x7F)){ // �������� ����� ���� ������ ����� ������������ �� ��� ��� ( �� ��������� ����� ���������� )
				hlp_rx_buffer[0] = 0;
				hlp_rx_buffer[1] = 0;
				hlp_m_counter = 0;
				return ADDRESS_BYTE_ERROR;
			}
			break;
		case 2: // ���� ����� ������������ ��� ���, �� ������� �� ��� ������
			if(CMD_TYPE_BYTE > 3){ // � ������ ������������ ���� ������ �������� ����� ������
				hlp_rx_buffer[0] = 0;
				hlp_rx_buffer[1] = 0;
				hlp_rx_buffer[2] = 0;
				hlp_m_counter = 0;
				return CMD_TYPE_ERROR;
			}
			break;
		case 3: // ������� �� ����� ���������� ������
			if(CMD_TYPE_BYTE == SHORT_CMD_PACKET && PACKET_LENGTH_BYTE != (6 + CRC_SIZE)){ // ���� �� ������� �������� �����, � ������������ ������������� ������� ���������� ������, �� ��������� �����
				hlp_rx_buffer[0] = 0;
				hlp_rx_buffer[1] = 0;
				hlp_rx_buffer[2] = 0;
				hlp_rx_buffer[3] = 0;
				hlp_m_counter = 0;
				return PACKET_LENGTH_ERROR;
			}
		// � ����������� �� ���� ������ ����� ������� ������ ���������� ����
		default:
			break;
	}
	if(TIME_TO_RESET != 0){ // ���� �� ���-�� �������� �� �����������, �� ���������� ������ ������������
		TIME_TO_RESET_COUNTER = 0;
	}
	if(hlp_m_counter == PACKET_LAST_INDEX && CMD_TYPE_BYTE == SHORT_CMD_PACKET){ // ���� ��������� ��������� �������� �����, �� �������� �������� CRC
		uint16_t real_crc = Crc16(hlp_rx_buffer, PACKET_LENGTH_BYTE-2); //�������� �������� ����� �� ����������� ����� CRC
		if(real_crc != PACKET_CRC16_BYTE){
			hlp_m_counter = 0;
			return CRC_ERROR;
		}
		else{
			for(uint8_t i = 0; i < PACKET_LENGTH_BYTE; i++){ //�������� ������ ���������� ������
				LoRa_last_rx_packet[i] = hlp_rx_buffer[i];
			}
		}
		retranslate_cmd[0] = 'r';
		retranslate_cmd[1] = hlp_rx_buffer[4];
		retranslate_cmd[2] = 'b';
		retranslate_cmd[3] = hlp_rx_buffer[5];
		CMD_Handler(retranslate_cmd);
		*(uint32_t*)retranslate_cmd = (uint32_t)0;
		hlp_m_counter = 0;
		packet_counter++;
		return SUCCESS_RX;
	}
	if(hlp_m_counter == (PACKET_LENGTH_BYTE - 1)){ // ���� ��������� ��������� ������� �����,

//		uint8_t real_crc = Crc8(hlp_rx_buffer, PACKET_LENGTH_BYTE-1); //
		volatile uint16_t real_crc = Crc16(hlp_rx_buffer, PACKET_LENGTH_BYTE-2);
		volatile uint16_t rx_rcr = PACKET_CRC16_BYTE;
		if(real_crc != rx_rcr){ 						// �� ��������� CRC,
			hlp_m_counter = 0;
			return CRC_ERROR;
		}
		else{
			for(uint8_t i = 0; i < PACKET_LENGTH_BYTE; i++){ //�������� ������ ���������� ������
				LoRa_last_rx_packet[i] = hlp_rx_buffer[i];
			}
		}
		//�� ������� ���������� �����. ������ ������, ��� � ��� ������
//		hlp_rx_buffer[5 + hlp_rx_buffer[4]] |= 0x80;  //���������� ��� ACK �������� ������ ������
		if(CMD_TYPE_BYTE == LONG_CMD_PACKET){ // ���� ������ ����� � ��������, ��
			// ����� ������������ �������������� ��� ��� ����
			if(((hlp_rx_buffer[PACKET_LAST_INDEX - (CRC_SIZE + 2)]) == ADDRESS_BYTE) && //��������� ����� � ������ ����� ������
			   (hlp_rx_buffer[5 + hlp_rx_buffer[4]] == hlp_rx_buffer[PACKET_LAST_INDEX - (CRC_SIZE+2)])){ // � ������� ������� �������� �� ����������
				retranslate_cmd[0] = 'r';
				retranslate_cmd[1] = PACKET_LAST_LAST_DATA;
				retranslate_cmd[2] = 'b';
				retranslate_cmd[3] = PACKET_LAST_DATA;
				CMD_Handler(retranslate_cmd);
				*(uint32_t*)retranslate_cmd = (uint32_t)0;
			}
			else{ // ���� ����� �������������� �� ��� ���, ��
				hlp_rx_buffer[4] += 1;						 //�������������� ������� ���������
				hlp_rx_buffer[1] = hlp_rx_buffer[5 + hlp_rx_buffer[4]]; //����� ���������� ��������� ���������� �� �������

				uint16_t crc16 = Crc16(hlp_rx_buffer, PACKET_LENGTH_BYTE-2);
				hlp_rx_buffer[PACKET_LENGTH_BYTE-2] = crc16 >> 8;
				hlp_rx_buffer[PACKET_LENGTH_BYTE-1] = crc16 & 0xFF; //������������� CRC ������ ������
				//TODO: UART SEND ACK ANSWER TO PREVIOUS SENDER
				for(uint8_t i = 0; i < PACKET_LENGTH_BYTE; i++){
					UART_tx(USART3, hlp_rx_buffer[i]);// ������������� ���������������� �����
				}
			}
		}
		else if(CMD_TYPE_BYTE == DATA_PACKET){ // ���� ����� � �������
			if(((hlp_rx_buffer[PACKET_LAST_INDEX - CRC_SIZE - 2 - hlp_rx_buffer[PACKET_LAST_INDEX - CRC_SIZE]]) == ADDRESS_BYTE) && //��������� ����� � ������ ����� ������
			   (hlp_rx_buffer[5 + hlp_rx_buffer[4]] == hlp_rx_buffer[PACKET_LAST_INDEX - CRC_SIZE - 2 - hlp_rx_buffer[PACKET_LAST_INDEX - CRC_SIZE]])){ // � ������� ������� �������� �� ����������
				switch (PACKET_DATA_TYPE) {
					case 0x00:
						for(uint8_t i = 0; i < PACKET_DATA_LENGTH; i++){
							LoRa_last_rx_data[i] = hlp_rx_buffer[5 + hlp_rx_buffer[4] + 1 + i];
							FIFO_PUSH(sensors_data_rx_fifo, hlp_rx_buffer[5 + hlp_rx_buffer[4] + 1 + i]);
						}
						break;
					default:
						break;
				}

			}
			else{ // ���� ����� �������������� �� ��� ���, ��
				hlp_rx_buffer[4] += 1;						 //�������������� ������� ���������
				hlp_rx_buffer[1] = hlp_rx_buffer[5 + hlp_rx_buffer[4]]; //����� ���������� ��������� ���������� �� �������

				uint16_t crc16 = Crc16(hlp_rx_buffer, PACKET_LENGTH_BYTE-2);
				hlp_rx_buffer[PACKET_LENGTH_BYTE-2] = crc16 >> 8; //������������� CRC ������ ������
				hlp_rx_buffer[PACKET_LENGTH_BYTE-1] = crc16 & 0xFF;
				//TODO: UART SEND ACK ANSWER TO PREVIOUS SENDER
				for(uint8_t i = 0; i < PACKET_LENGTH_BYTE; i++){
					UART_tx(USART3, hlp_rx_buffer[i]);// ������������� ���������������� �����
				}
			}
		}
		hlp_m_counter = 0;
		packet_counter++;
		return SUCCESS_RX;
	}
	hlp_m_counter++;
//	UART_tx(USART1, hlp_m_counter);
	return IN_PROCESS;
}

void Forming_data_packet(uint8_t *rx_arr){
	uint8_t last_addr = rx_arr[3] - CRC_SIZE - 3;
	uint8_t new_size = rx_arr[3] + 6;
	uint8_t j = rx_arr[3] - (CRC_SIZE + 3) - 5;
	uint8_t buff_arr[30];
	for(uint8_t i = 0; i < rx_arr[3] - (CRC_SIZE + 3) - 5; i++, j--){
		buff_arr[i] = rx_arr[j];
	}
	for(uint8_t i = rx_arr[5]; i < rx_arr[3] - (CRC_SIZE + 3); i++){
		rx_arr[i] = buff_arr[i];
	}
	rx_arr[4] = 0; //counter = 0
	rx_arr[2] = DATA_PACKET;
	rx_arr[last_addr+1] = 0x00; //main base address
	rx_arr[last_addr+2] = 0x00; //data 1
	rx_arr[last_addr+3] = 0x00; //data 2
	rx_arr[last_addr+4] = 0x00; //data 3
	rx_arr[last_addr+5] = 0x00; //data 4
	rx_arr[last_addr+6] = 0x00; //data 5
	rx_arr[last_addr+7] = 0x00; //crc
	uint8_t new_crc = Crc8(rx_arr, new_size-1);
	rx_arr[last_addr+7] = new_crc; //crc
}

/*
  Name  : CRC-8
  Poly  : 0x31    x^8 + x^5 + x^4 + 1
  Init  : 0xFF
  Revert: false
  XorOut: 0x00
  Check : 0xF7 ("123456789")
  MaxLen: 15 ����(127 ���) - �����������
    ���������, �������, ������� � ���� �������� ������
*/
uint8_t Crc8(uint8_t *pcBlock, uint8_t len){
	uint8_t crc8 = 0xFF;
    while (len--){
        crc8 ^= *pcBlock++;
        for (uint8_t i = 0; i < 8; i++)
            crc8 = crc8 & 0x80 ? (crc8 << 1) ^ 0x31 : crc8 << 1;
    }
    return crc8;
}

/*
  Name  : CRC-16 CCITT
  Poly  : 0x1021    x^16 + x^12 + x^5 + 1
  Init  : 0xFFFF
  Revert: false
  XorOut: 0x0000
  Check : 0x29B1 ("123456789")
  MaxLen: 4095 ���� (32767 ���) - �����������
    ���������, �������, ������� � ���� �������� ������
*/
uint16_t Crc16(uint8_t *pcBlock, uint16_t len){
	uint16_t crc = 0xFFFF;
    while (len--){
        crc ^= *pcBlock++ << 8;
        for (uint8_t i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}
