/*
 * lora_protocol.h
 *
 *  Created on: 20 ���. 2020 �.
 *      Author: ftor
 */

#ifndef INC_LORA_PROTOCOL_H_
#define INC_LORA_PROTOCOL_H_

#include "stm32l4xx.h"
#include "cmd_handler.h"
#include "uart.h"
#include "global_variables.h"

typedef struct LoRa_packet {
	uint8_t start_byte;
	uint8_t our_address;
	uint8_t cmd_type;
	uint8_t counter;
	uint8_t ack_0 : 1;
	uint8_t addr0 : 7;
	uint8_t ack_1 : 1;
	uint8_t addr1 : 7;
	uint8_t ack_2 : 1;
	uint8_t addr2 : 7;
	uint8_t data0;
	uint8_t data1;
	uint8_t data2;
	uint8_t data3;
	uint8_t data4;
	uint16_t crc;
}LoRa_Packet;

enum {START_BYTE_ERROR=-5, ADDRESS_BYTE_ERROR, CMD_TYPE_ERROR, PACKET_LENGTH_ERROR, CRC_ERROR, SUCCESS_RX, IN_PROCESS} packet_errors;

int8_t LoRa_protocol_handler(uint8_t rx);
void Forming_data_packet(uint8_t *rx_arr);
uint8_t Crc8(uint8_t *pcBlock, uint8_t len);
uint16_t Crc16(uint8_t *pcBlock, uint16_t len);
void Send_ACK_packet(uint8_t addr);

#endif /* INC_LORA_PROTOCOL_H_ */
