/*
 * cmd_handler.h
 *
 *  Created on: 20 сент. 2020 г.
 *      Author: Gandalf
 */

#ifndef INC_CMD_HANDLER_H_
#define INC_CMD_HANDLER_H_
#include "cmd_handler.h"
#include "stm32l4xx.h"
void CMD_Handler(uint8_t *cmd_string);
void TMP1075_CONFIG_REGISTER_Handler(uint8_t cmd);
void BUZZ_stop();
void BUZZ_REGISTER1_Handler(uint8_t cmd);
void BUZZ_REGISTER2_Handler(uint8_t cmd);
void BUZZ_REGISTER3_Handler(uint8_t cmd);
void LORA_REGISTER1_Handler(uint8_t cmd);
void LORA_REGISTER2_Handler(uint8_t cmd);
void REQUEST_DATA(uint8_t cmd);
void RETRANSLATE_CMD_PACKET(uint8_t cmd);
void PC_CONNECT(uint8_t cmd);
void SEND_UNIC_CMD(uint8_t cmd);
void set_lora_normal_mode();
/*
 * LEDn_REGISTER
 * 0:3 - 000 - N/U
 * 		 001 - N/U
 * 		 010 - LEDn_R init bin mode
 * 		 011 - LEDn_R init PWM mode
 * 		 100 - LEDn_G init bin mode
 * 		 101 - LEDn_G init PWM mode
 * 		 110 - LEDn_B init bin mode
 * 		 111 - LEDn_B init PWM mode
 * 3:8 - PWM duty cycle
 *
 */


/*
 * BUZZ_REGISTER1
 * 0:8 - Frequency
 */

/*
 * BUZZ_REGISTER2
 * 0:8 - Duration
 */

/*
 * BUZZ_REGISTER3
 * 0:2 - Volume (0%, 33%, 66%, 100%)
 * 2 - "1" - Search noise Enable  | "0" - Search noise Disable
 */

/*
 * LORA_REGISTER0
 * 0:8 - HEAD
 */

/*
 * LORA_REGISTER1
 * 0:8 - Address HIGH
 */

/*
 * LORA_REGISTER2
 * 0:8 - Address LOW
 */

/*
 * LORA_REGISTER3 (SPED)
 * 7,6 - Parity
 * 5,4,3 - baud rate
 * 2,1,0 - air data rate
 */

/*
 * LORA_REGISTER4
 * 7,6,5: N/A
 * 4-0:Communication channel(410M + CHAN*1M), default 17H(433MHz)
 */

/*
 * LORA_REGISTER5 (OPTION)
 * 7 : 	"0" - Transparent transmission mode
 * 		"1" - Fixed transmission mode
 * 6 : 	"1" - TXD and AUX push-pull outputs, RXD pull-up inputs
 *		"0" - TXD, AUX open-collector outputs, RXD open-collector inputs
 * 5,4,3 - wireless wake-up time
 * 2 - 	0-Turn off FEC
 *		1-Turn on FEC(Default)
 * 1,0 - transmission power
 */

/*
 * LORA_REGISTER6
 * 0:4 -000 - N/U
 * 		001 - C0 + working parameters
 * 		010 - C1+C1+C1
 * 		011 - C2 + working parameters
 * 		100 - C3+C3+C3
 * 		101 - C4+C4+C4
 * 		110 - Sleep mode (M0=1; M1=1)
 * 		111 - Normal mode (M0=0; M1=0)
 * 4:8- N/U
 */

/*
 * LORA_REGISTER7
 * 0:8 Send data
 */

#endif /* INC_CMD_HANDLER_H_ */
