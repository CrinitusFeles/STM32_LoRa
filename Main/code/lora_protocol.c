/*
 * lora_protocol.c
 *
 *  Created on: 18 окт. 2020 г.
 *      Author: Gandalf
 */

#include "lora_protocol.h"
#include "tim.h"


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
	LONG_CMD_PACKET, // пакет длинного типа, который будет передаваться от станции к станции, пока не достигнет адресата
	DATA_PACKET, // в ответ на команду запроса данных формируется данный тип пакета, содержащий актуальные данные с датчиков
	SHORT_CMD_PACKET,
	ACK_PACKET, // при успешном принятии пакета, нужно маякнуть отправителю, что мы его успешно приняли.
				//Иначе, он попытается отправить его еще раз, после чего отправит FUCKUP пакет на базу
	FUCKUP_PACKET // Когда станция совсем не отвечает, нужно известить об этом базу, для чего и формиуется данный пакет
} cmd_types;

uint8_t retranslate_cmd[4] = {0};

int8_t LoRa_protocol_handler(uint8_t rx){

	static uint8_t hlp_m_counter = 0;

	hlp_rx_buffer[hlp_m_counter] = rx;

	switch (hlp_m_counter) {
		case 0: // проверяем первый принятый байт
			if(START_BYTE != 0xFF){ // игнорируем все, что не является стартовым байтом
				hlp_rx_buffer[0] = 0;
				hlp_m_counter = 0;
				return START_BYTE_ERROR;
			}
			else{
//				TIM_init(TIM7, 200);
			}
			break;
		case 1: // если предыдущий байт был стартовым, то проверяем второй принятый байт
			if(ADDRESS_BYTE != (LoRa_self_address & 0x7F)){ // обрываем прием если данный пакет предназначен не для нас ( не совпадает адрес получателя )
				hlp_rx_buffer[0] = 0;
				hlp_rx_buffer[1] = 0;
				hlp_m_counter = 0;
				return ADDRESS_BYTE_ERROR;
			}
			break;
		case 2: // если пакет предназначен для нас, то смотрим на тип пакета
			if(CMD_TYPE_BYTE > 3){ // в случае неизвестного типа пакета обрываем прием данных
				hlp_rx_buffer[0] = 0;
				hlp_rx_buffer[1] = 0;
				hlp_rx_buffer[2] = 0;
				hlp_m_counter = 0;
				return CMD_TYPE_ERROR;
			}
			break;
		case 3: // смотрим на длину принимаего пакета
			if(CMD_TYPE_BYTE == SHORT_CMD_PACKET && PACKET_LENGTH_BYTE != (6 + CRC_SIZE)){ // если мы ожидаем короткий пакет, а отправляется подозрительно большое количество данных, то прерываем прием
				hlp_rx_buffer[0] = 0;
				hlp_rx_buffer[1] = 0;
				hlp_rx_buffer[2] = 0;
				hlp_rx_buffer[3] = 0;
				hlp_m_counter = 0;
				return PACKET_LENGTH_ERROR;
			}
		// в зависимости от типа пакета будем ожидать разное количество байт
		default:
			break;
	}
	if(TIME_TO_RESET != 0){ // если мы что-то получаем по радиоканалу, то сбрасываем таймер перезагрузки
		TIME_TO_RESET_COUNTER = 0;
	}
	if(CMD_TYPE_BYTE == ACK_PACKET){
		uint16_t real_crc = Crc16(hlp_rx_buffer, 3);
		if(real_crc != (hlp_rx_buffer[5] | (hlp_rx_buffer[4] << 8))){
			hlp_m_counter = 0;
			return CRC_ERROR;
		}
	}
	if(hlp_m_counter == PACKET_LAST_INDEX && CMD_TYPE_BYTE == SHORT_CMD_PACKET){ // если закончили принимать короткий пакет, то начинаем проверку CRC
		uint16_t real_crc = Crc16(hlp_rx_buffer, PACKET_LENGTH_BYTE-2); //передаем принятый пакет за исключением байта CRC
		if(real_crc != PACKET_CRC16_BYTE){
			hlp_m_counter = 0;
			return CRC_ERROR;
		}
		else{
			for(uint8_t i = 0; i < PACKET_LENGTH_BYTE; i++){ //запомним данные последнего пакета
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
	if(hlp_m_counter == (PACKET_LENGTH_BYTE - 1)){ // если закончили принимать длинный пакет,

//		uint8_t real_crc = Crc8(hlp_rx_buffer, PACKET_LENGTH_BYTE-1); //
		volatile uint16_t real_crc = Crc16(hlp_rx_buffer, PACKET_LENGTH_BYTE-2);
		volatile uint16_t rx_rcr = PACKET_CRC16_BYTE;
		if(real_crc != rx_rcr){ 						// то проверяем CRC,
			hlp_m_counter = 0;
			return CRC_ERROR;
		}
		else{
			for(uint8_t i = 0; i < PACKET_LENGTH_BYTE; i++){ //запомним данные последнего пакета
				LoRa_last_rx_packet[i] = hlp_rx_buffer[i];
			}
		}
		//мы приняли корректный пакет. Дальше решаем, что с ним делать
//		hlp_rx_buffer[5 + hlp_rx_buffer[4]] |= 0x80;  //выставляем бит ACK напротив своего адреса
		if(CMD_TYPE_BYTE == LONG_CMD_PACKET){ // если пришел пакет с командой, то
			// пакет ретрансляции предназначался для нас если
			if(((hlp_rx_buffer[PACKET_LAST_INDEX - (CRC_SIZE + 2)]) == ADDRESS_BYTE) && //последний адрес в списке равен нашему
			   (hlp_rx_buffer[5 + hlp_rx_buffer[4]] == hlp_rx_buffer[PACKET_LAST_INDEX - (CRC_SIZE+2)])){ // и счетчик адресов досчитал до последнего
				retranslate_cmd[0] = 'r';
				retranslate_cmd[1] = PACKET_LAST_LAST_DATA;
				retranslate_cmd[2] = 'b';
				retranslate_cmd[3] = PACKET_LAST_DATA;
				CMD_Handler(retranslate_cmd);
				*(uint32_t*)retranslate_cmd = (uint32_t)0;
			}
			else{ // если пакет предназначался не для нас, то
				hlp_rx_buffer[4] += 1;						 //инкрементируем счетчик адресатов
				hlp_rx_buffer[1] = hlp_rx_buffer[5 + hlp_rx_buffer[4]]; //будем отправлять сообщение следующему по цепочке

				uint16_t crc16 = Crc16(hlp_rx_buffer, PACKET_LENGTH_BYTE-2);
				hlp_rx_buffer[PACKET_LENGTH_BYTE-2] = crc16 >> 8;
				hlp_rx_buffer[PACKET_LENGTH_BYTE-1] = crc16 & 0xFF; //пересчитываем CRC нового пакета
				//TODO: UART SEND ACK ANSWER TO PREVIOUS SENDER
				for(uint8_t i = 0; i < PACKET_LENGTH_BYTE; i++){
					UART_tx(USART3, hlp_rx_buffer[i]);// ретранслируем модифицированный пакет
				}
			}
		}
		else if(CMD_TYPE_BYTE == DATA_PACKET){ // если пакет с данными
			if(((hlp_rx_buffer[PACKET_LAST_INDEX - CRC_SIZE - 2 - hlp_rx_buffer[PACKET_LAST_INDEX - CRC_SIZE]]) == ADDRESS_BYTE) && //последний адрес в списке равен нашему
			   (hlp_rx_buffer[5 + hlp_rx_buffer[4]] == hlp_rx_buffer[PACKET_LAST_INDEX - CRC_SIZE - 2 - hlp_rx_buffer[PACKET_LAST_INDEX - CRC_SIZE]])){ // и счетчик адресов досчитал до последнего
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
			else{ // если пакет предназначался не для нас, то
				Send_ACK_packet(hlp_rx_buffer[4 + hlp_rx_buffer[4] - 1]);
				hlp_rx_buffer[4] += 1;						 //инкрементируем счетчик адресатов
				hlp_rx_buffer[1] = hlp_rx_buffer[5 + hlp_rx_buffer[4]]; //будем отправлять сообщение следующему по цепочке

				uint16_t crc16 = Crc16(hlp_rx_buffer, PACKET_LENGTH_BYTE-2);
				hlp_rx_buffer[PACKET_LENGTH_BYTE-2] = crc16 >> 8; //пересчитываем CRC нового пакета
				hlp_rx_buffer[PACKET_LENGTH_BYTE-1] = crc16 & 0xFF;
				//TODO: UART SEND ACK ANSWER TO PREVIOUS SENDER
				for(uint8_t i = 0; i < PACKET_LENGTH_BYTE; i++){
					UART_tx(USART3, hlp_rx_buffer[i]);// ретранслируем модифицированный пакет
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

void Send_ACK_packet(uint8_t addr){
	uint8_t ack_packet[] = {0xFF, addr, ACK_PACKET, 0, 0};
	uint16_t crc16 = Crc16(ack_packet, 3);
	ack_packet[4] = crc16 >> 8; //пересчитываем CRC нового пакета
	ack_packet[5] = crc16 & 0xFF;
	for(uint8_t i = 0; i < 5; i++){
		UART_tx(USART3, ack_packet[i]);// ретранслируем модифицированный пакет
	}
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
  MaxLen: 15 байт(127 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
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
  MaxLen: 4095 байт (32767 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
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
