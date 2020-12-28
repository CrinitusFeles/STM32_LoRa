#include "main.h"
#include "sdio.h"
#include "fat32.h"  

void LED_BLINK(){
//	battary_voltage = ADC1->JDR1 * 2 * 2.93 / 4095;
//	ADC1->CR |= ADC_CR_JADSTART;
	uint16_t adc_data = adc_single_conversion(ADC1);

#ifdef SENSORS_PLUGGED

	uint16_t s1_raw = 0;
	uint16_t s2_raw = 0;
	uint16_t s3_raw = 0;
	uint16_t s4_raw = 0;
	uint16_t s5_raw = 0;

	SYSTEM_I2C_error_counter += TMP1075_read_raw_temperature(I2C3, 0x4F, &s1_raw);
	SYSTEM_I2C_error_counter += TMP1075_read_raw_temperature(I2C3, 0x4B, &s2_raw);
	SYSTEM_I2C_error_counter += TMP1075_read_raw_temperature(I2C3, 0x4D, &s3_raw);
	SYSTEM_I2C_error_counter += TMP1075_read_raw_temperature(I2C3, 0x48, &s4_raw);
	SYSTEM_I2C_error_counter += TMP1075_read_raw_temperature(I2C3, 0x4E, &s5_raw);
	if(SYSTEM_I2C_error_counter != 0){
		I2C_Clear_Error_Flags(I2C3);
		SYSTEM_I2C_error_counter = 0;
		SYSTEM_I2C_error_flag = 1;
	}

	current_station_stats.sensors_data[0] = (s1_raw);
	current_station_stats.sensors_data[1] = (s2_raw);
	current_station_stats.sensors_data[2] = (s3_raw);
	current_station_stats.sensors_data[3] = (s4_raw);
	current_station_stats.sensors_data[4] = (s5_raw);

	tmp1075_temp1 = TMP1075_raw_to_float(s1_raw);
	tmp1075_temp2 = TMP1075_raw_to_float(s2_raw);
	tmp1075_temp3 = TMP1075_raw_to_float(s3_raw);
	tmp1075_temp4 = TMP1075_raw_to_float(s4_raw);
	tmp1075_temp5 = TMP1075_raw_to_float(s5_raw);

#endif
}

void UART_SEND(){
//	UART_tx(USART1, 0x48);
//	UART_tx(USART1, 0x0D);
	if(FIFO_COUNT(fifo) > 0){
		UART_tx(USART1, FIFO_FRONT(fifo));
		FIFO_POP(fifo);
	}
}
void SEND_SENSORS_DATA(){
	static uint8_t first_enter = 1;
	if(FIFO_COUNT(sensors_data_rx_fifo) > 0){

		if(first_enter){
			UART_tx_array(USART1, "Sensors data: ");
			first_enter = 0;
		}
		UART_tx(USART1, FIFO_FRONT(sensors_data_rx_fifo));
		FIFO_POP(sensors_data_rx_fifo);
		uint8_t count = FIFO_COUNT(sensors_data_rx_fifo);
		if(count == 0){
			UART_tx(USART1, '\n');
			first_enter = 1;
		}
	}
}
void LCD_PINBALL(){
	if(x_v == 159) x_dir *= -1;
	else if(x_v == 30) x_dir *= -1;
	if(y_h == 112) y_dir *= -1;
	else if(y_h == 15) y_dir *= -1;
	//NVIC_DisableIRQ(USART1_IRQn);
	Draw_V_Line(SPI1, x_v, y_v_s, y_v_e, backgroundcolor);
	Draw_H_Line(SPI1, x_h_s, x_h_e, y_h, backgroundcolor);
	//�������
	Draw_V_Line(SPI1, x_v+3, y_v_s, y_v_e, backgroundcolor);
	Draw_H_Line(SPI1, x_h_s, x_h_e, y_h+2, backgroundcolor);
	//
	x_v += x_dir;
	y_h += y_dir;
	FillArea(SPI1, x_v, x_v+2, y_h, y_h+2, RED);
	//NVIC_EnableIRQ(USART1_IRQn);
//	DrawPixel(x_v, y_h, RED);

	//Draw_V_DottedLine(x_v, y_v_s, y_v_e, 5, 5, YELLOW, BLUE);
	//Draw_H_DottedLine(x_h_s, x_h_e, y_h, 5, 5, YELLOW, BLUE);
}


int main(){

	init_global_variables();
    System_Init();
	// printf("start\n");

#ifdef SCREEN_PLUGGED
	cs_low();
	FullFill(SPI1, backgroundcolor);
//	PlotAxes(SPI1, Y_axis_begin, Y_axis_end, Y_axis_coordinate, X_axis_begin, X_axis_end, X_axis_coordinate, dashSize, color, backgroundcolor);
	DrawString5x7(SPI1, 10, 10, (uint8_t*)"UART receive:", YELLOW, BLACK);
	DrawString5x7(SPI1, 10, 20, (uint8_t*)"LoRa receive:", YELLOW, BLACK);
	DrawString5x7(SPI1, 10, 30, (uint8_t*)"counter:", YELLOW, BLACK);
	DrawString5x7(SPI1, 70, 30, NumbersArray(button_pressed_counter, number_array), YELLOW, BLACK);
	DrawString5x7(SPI1, 10, 40, (uint8_t*)"mVoltage:", YELLOW, BLACK);
	DrawString5x7(SPI1, 80, 40, NumbersArray((uint16_t)(battary_voltage*1000), vbat_arr), YELLOW, BLACK);
#ifdef SENSORS_PLUGGED
	DrawString5x7(SPI1, 10, 50, (uint8_t*)"Sensor 1:", YELLOW, BLACK);
	DrawString5x7(SPI1, 10, 60, (uint8_t*)"Sensor 2:", YELLOW, BLACK);
	DrawString5x7(SPI1, 10, 70, (uint8_t*)"Sensor 3:", YELLOW, BLACK);
	DrawString5x7(SPI1, 10, 80, (uint8_t*)"Sensor 4:", YELLOW, BLACK);
	DrawString5x7(SPI1, 10, 90, (uint8_t*)"Sensor 5:", YELLOW, BLACK);
#endif
#endif

//	uint32_t LED_delay = DWT_GetMicro();


	SDMMC_INIT();
//	sd_last_result = SD_Init();
//	FAT32_init();

    while(1){
	delay_action(1000, 0, LED_BLINK);

   	// delay_action(15, 1, LCD_PINBALL);
	delay_action(100, 2, UART_SEND);
	delay_action(100, 3, SEND_SENSORS_DATA);
#ifdef SCREEN_PLUGGED
    	if((DWT_GetMicro() - LED_delay) > 50000){
    		LED_delay = DWT_GetMicro();
    		DrawString5x7(SPI1, 70, 30, NumbersArray(button_pressed_counter, number_array), YELLOW, BLACK);
    		DrawString5x7(SPI1, 85, 30, (uint8_t*)"     ", YELLOW, BLACK);
    		DrawString5x7(SPI1, 80, 40, NumbersArray((uint16_t)(battary_voltage*1000), vbat_arr), YELLOW, BLACK);
#ifdef SENSORS_PLUGGED
    		DrawString5x7(SPI1, 100, 50, NumbersArray((uint16_t)(tmp1075_temp1), vbat_arr), YELLOW, BLACK);
    		DrawString5x7(SPI1, 100, 60, NumbersArray((uint16_t)(tmp1075_temp2), vbat_arr), YELLOW, BLACK);
    		DrawString5x7(SPI1, 100, 70, NumbersArray((uint16_t)(tmp1075_temp3), vbat_arr), YELLOW, BLACK);
    		DrawString5x7(SPI1, 100, 80, NumbersArray((uint16_t)(tmp1075_temp4), vbat_arr), YELLOW, BLACK);
    		DrawString5x7(SPI1, 100, 90, NumbersArray((uint16_t)(tmp1075_temp5), vbat_arr), YELLOW, BLACK);

    		DrawString5x7(SPI1, 115, 50, (uint8_t*)string_form(arr, frac_to_int(tmp1075_temp1)), YELLOW, BLACK);
    		DrawString5x7(SPI1, 115, 60, (uint8_t*)string_form(arr, frac_to_int(tmp1075_temp2)), YELLOW, BLACK);
    		DrawString5x7(SPI1, 115, 70, (uint8_t*)string_form(arr, frac_to_int(tmp1075_temp3)), YELLOW, BLACK);
    		DrawString5x7(SPI1, 115, 80, (uint8_t*)string_form(arr, frac_to_int(tmp1075_temp4)), YELLOW, BLACK);
    		DrawString5x7(SPI1, 115, 90, (uint8_t*)string_form(arr, frac_to_int(tmp1075_temp5)), YELLOW, BLACK);
#endif
    	}
#endif
    }
}


