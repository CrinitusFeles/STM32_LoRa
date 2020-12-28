#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32l4xx.h"
#include <stdio.h>
#include "formating_output.h"
#include "delay.h"
#include "gpio.h"
#include "rcc.h"
#include "pwm.h"
#include "uart.h"
#include "spi.h"
#include "font5x7.h"
#include <font7x11.h>
#include "ST7735.h"
#include "dwt.h"
#include "rgb_led.h"
#include "i2c.h"
#include "tmp1075.h"
#include "adc.h"
#include "global_variables.h"
#include "lora_protocol.h"
#include "periph_handlers.h"
#include "tim.h"
#include "exti.h"
#include "fifo.h"

//#define SCREEN_PLUGGED
#define SENSORS_PLUGGED

#define LED1_R		PA0		//TIM2_CH1
#define LED1_G		PA1		//TIM2_CH2
#define LED1_B		PA2		//TIM2_CH3

#define LED2_R		PA10	//TIM1_CH3
#define LED2_G		PA11	//TIM1_CH4
#define LED2_B		PB14	//TIM15_CH1

#define LED3_R		PA3		//TIM2_CH4
#define LED3_G		PA8		//TIM1_CH1
#define LED3_B		PA9		//TIM1_CH2

#define BUZZ		PB15	//TIM15_CH2

#define USART1_RX	PB7
#define USART1_TX	PB6

#define I2C3_SDA	PC1
#define I2C3_SCL	PC0

#define VBAT		PC3		//ADC1_IN4
#define ADC_EN		PC2

#define LoRa_AUX	PC13
#define LoRa_M0		PB0
#define LoRa_M1		PB1
#define LoRa_TX		PC4 	//USART3
#define LoRa_RX		PC5 	//USART3

#define TFT_A0		PB12
#define TFT_RES		PB13

#define SIDE_BUTTON	PB4
#define BUTTON1		PC6
#define BUTTON2		PC7
#define BUTTON3		PA12
#define BUTTON4		PB5
#define BUTTON5		PB2

#define LED1_REGISTER		0
#define LED2_REGISTER		1
#define LED3_REGISTER		2
#define BUZZ_REGISTER		3
#define RTC_REGISTER		4
#define LCD_REGISTER		5
#define LORA_REGISTER0		6
#define LORA_REGISTER1		7
#define LORA_REGISTER2		8
#define LORA_REGISTER3		9
#define LORA_REGISTER4		10
#define LORA_REGISTER5		11
#define LORA_REGISTER6		12
#define SD_REGISTER			13
#define ADC_REGISTER		14


#define SD_D0				PC9
#define SD_D1				PC8
#define SD_D2				PC10
#define SD_D3				PC11
#define SD_CK				PC12
#define SD_CMD				PD2


//------ FOR LCD ------- //

#define color				BLUE
#define backgroundcolor		BLACK
#define Y_axis_begin		16
#define Y_axis_end			115
#define Y_axis_coordinate	28
#define X_axis_begin		28
#define X_axis_end			152
#define X_axis_coordinate	115
#define dashSize			3

//-----------------------//

void (*handler)();


#endif /* INC_MAIN_H_ */
