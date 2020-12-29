/*
 * System.c
 *
 *  Created on: 18 ���. 2020 �.
 *      Author: Gandalf
 */
#include "main.h"

void System_Init(){
	SysTick_Config(millisec);
//	int tim = RCC_init();
	RCC_init();
	DWT_Init();

	gpio_init(USART1_RX, PB7_USART1_RX, Open_drain, no_pull, Input);
	gpio_init(USART1_TX, PB6_USART1_TX, Push_pull, no_pull, High_speed);

	gpio_init(LoRa_RX, PC5_USART3_RX, Push_pull, no_pull, Input);
	gpio_init(LoRa_TX, PC4_USART3_TX, Push_pull, no_pull, High_speed);
	gpio_init(LoRa_M1, General_output, Push_pull, no_pull, High_speed);
	gpio_init(LoRa_M0, General_output, Push_pull, no_pull, High_speed);
	gpio_init(LoRa_AUX, Input_mode, Open_drain, no_pull, Input);

#ifdef SCREEN_PLUGGED
	gpio_init(SPI_CS, General_output, Push_pull, no_pull, Very_high_speed);
	gpio_init(SPI_SCK, PA5_SPI1_SCK,  Push_pull, no_pull, Very_high_speed);
	gpio_init(SPI_MISO, PA6_SPI1_MISO,  Open_drain, pull_down, Input);
	gpio_init(SPI_MOSI, PA7_SPI1_MOSI,  Push_pull, no_pull, Very_high_speed);

	gpio_init(TFT_A0, General_output, Push_pull, no_pull, High_speed);
	gpio_init(TFT_RES, General_output, Push_pull, no_pull, High_speed);
#endif

	gpio_init(VBAT, Analog_mode, Open_drain, no_pull, Input);
	gpio_init(ADC_EN, General_output, Push_pull, no_pull, High_speed);

#ifdef SENSORS_PLUGGED
	gpio_init(I2C3_SCL, PC0_I2C3_SCL, Open_drain, pull_up, High_speed);
	gpio_init(I2C3_SDA, PC1_I2C3_SDA, Open_drain, pull_up, High_speed);
#endif

#ifdef SCREEN_PLUGGED
	spi_init(SPI1, SPI_CS, div_2, Mode_0, data_8_bit, MSB);
	Init_ST7735(SPI1, TFT_A0, TFT_RES);
#endif

	UART_init(USART3, 9600);
	UART_init(USART1, 9600);
//	UART_tx_array(USART1, "self address: ");
//	UART_tx(USART1, LoRa_self_address + 48);
//	UART_tx(USART1, 0x0D);
	EXTI_init(SIDE_BUTTON);

	gpio_state(ADC_EN, LOW);
//	ADC_Init();
	ADC_Init_regular();

#ifdef SENSORS_PLUGGED
	I2C_init(I2C3);

	SYSTEM_init_status += TMP1075_set_time_conversion(I2C3, 0x4F, TMP1075_CR_SLOW);
	SYSTEM_init_status += TMP1075_set_time_conversion(I2C3, 0x4B, TMP1075_CR_SLOW);
	SYSTEM_init_status += TMP1075_set_time_conversion(I2C3, 0x4D, TMP1075_CR_SLOW);
	SYSTEM_init_status += TMP1075_set_time_conversion(I2C3, 0x48, TMP1075_CR_SLOW);
	SYSTEM_init_status += TMP1075_set_time_conversion(I2C3, 0x4E, TMP1075_CR_SLOW);

	if(SYSTEM_init_status == 0){
		SYSTEM_init_status += TMP1075_one_shot_conversion_start(I2C3, 0x4F);
		SYSTEM_init_status += TMP1075_one_shot_conversion_start(I2C3, 0x4B);
		SYSTEM_init_status += TMP1075_one_shot_conversion_start(I2C3, 0x4D);
		SYSTEM_init_status += TMP1075_one_shot_conversion_start(I2C3, 0x48);
		SYSTEM_init_status += TMP1075_one_shot_conversion_start(I2C3, 0x4E);
	}
#endif

	gpio_state(LoRa_M0, HIGH); //Sleep mode
	gpio_state(LoRa_M1, HIGH);
	Delay(250);
	LoRa_init_flag = 1;
	UART_tx(USART3, 0xC1);
	UART_tx(USART3, 0xC1);
	UART_tx(USART3, 0xC1);

	gpio_init(LED1_B,  General_output, Push_pull, no_pull, High_speed);
}
