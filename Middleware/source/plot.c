/*
 * plot.c
 *
 *  Created on: 6 мар. 2019 г.
 *      Author: crinitusfeles
 */
#include "stm32l4xx.h"
#include "plot.h"
#include "SPI.h"
#include "ST7735.h"

uint8_t y = 119;

uint8_t arr2[3];
uint8_t space[2]= " \0";
uint8_t dashArrayCoordinate[5];
uint16_t y_axis_array_numbers[93];
uint8_t counter = 0;
void PlotAxes(SPI_TypeDef *SPIx, uint8_t Y_axis_begin, uint8_t Y_axis_end, uint8_t Y_axis_coordinate,
			uint8_t X_axis_begin, uint8_t X_axis_end, uint8_t X_axis_coordinate,
			uint8_t dashSize, uint16_t color, uint16_t backgroundcolor){
	uint16_t lowLimit = 400;

	for(uint8_t j = 0; j < 93; j++, lowLimit += 50){
		y_axis_array_numbers[j] = lowLimit;
	}
	//оси
	Draw_V_Line(SPIx, Y_axis_coordinate, Y_axis_begin, Y_axis_end, color);
	Draw_H_Line(SPIx, X_axis_begin, X_axis_end, X_axis_coordinate, color);

	//вертикальные черточки
	for(uint8_t x_coordinate = X_axis_begin + 12; x_coordinate <= X_axis_end; x_coordinate += 12){
		Draw_V_Line(SPIx, x_coordinate, Y_axis_end - dashSize, Y_axis_end, color);
	}
	//горизонтальные черточки
	for(uint8_t y_coordinate = Y_axis_end; y_coordinate - Y_axis_begin > 16; y_coordinate -= 20){
		Draw_H_Line(SPIx, X_axis_begin, X_axis_begin + dashSize, y_coordinate, RED);
		dashArrayCoordinate[counter++] = y_coordinate;
	}
	//числа вертикальной оси
	DynamicAxisScale(SPIx, 400, color, backgroundcolor);

	//нижний ряд чисел 1 - 10
	//for(uint8_t x_coordinate = X_axis_begin + 12; x_coordinate <= X_axis_end; x_coordinate += 12){
		//DrawChar5x7(35, y, '1', color, backgroundcolor);
	//}
	NumbersArray(10 , arr2);
	DrawString5x7(SPIx, 33, y, arr2, color, backgroundcolor);
	DrawChar5x7(SPIx, 50, y, '9', color, backgroundcolor);
	DrawChar5x7(SPIx, 62, y, '8', color, backgroundcolor);
	DrawChar5x7(SPIx, 74, y, '7', color, backgroundcolor);
	DrawChar5x7(SPIx, 86, y, '6', color, backgroundcolor);
	DrawChar5x7(SPIx, 98, y, '5', color, backgroundcolor);
	DrawChar5x7(SPIx, 110, y, '4', color, backgroundcolor);
	DrawChar5x7(SPIx, 122, y, '3', color, backgroundcolor);
	DrawChar5x7(SPIx, 134, y, '2', color, backgroundcolor);
	DrawChar5x7(SPIx, 146, y, '1', color, backgroundcolor);

}

uint8_t Scale(uint16_t data, uint16_t y_min, uint16_t y_max, uint8_t y_axis_end, uint8_t pixels){ //returns data in interval (y_min - y_max)
	return y_axis_end - (data - y_min) * pixels / (y_max - y_min);
}

uint8_t CO2number[5];
uint8_t co[] = {'C', 'O', '\0'};
void PlotCO2(SPI_TypeDef *SPIx, uint16_t CO2, uint16_t color, uint16_t backgroundcolor){
	DrawString7x11(SPIx, 98, 3, co, color, backgroundcolor);
	DrawChar5x7(SPIx, 115, 8, '2', color, backgroundcolor);
	DrawChar7x11(SPIx, 120, 3, ':', color, backgroundcolor);
	NumbersArray(CO2, CO2number);
	if(CO2 < 1000){
		DrawString7x11(SPIx, 126, 3, CO2number, color, backgroundcolor);
		DrawString7x11(SPIx, 150, 3, space, color, backgroundcolor);
	}
	else DrawString7x11(SPIx, 126, 3, CO2number, color, backgroundcolor);
}
uint8_t TempNumber[3];
void PlotTemp(SPI_TypeDef *SPIx, uint8_t temperature, uint16_t color, uint16_t backgroundcolor){
	uint8_t temp[] = {'T', '=', '\0'};
	NumbersArray(temperature, TempNumber);
	DrawString7x11(SPIx, 38, 3, temp, color, backgroundcolor);
	DrawString7x11(SPIx, 52, 3, TempNumber, color, backgroundcolor);
}


uint8_t k;
uint16_t GetInterval(uint16_t data){
	uint16_t interval = 0;
	if(data <= 600) {
		k = 1;
		interval = 600;
	}
	else if(data <= 800) {
		k = 2;
		interval = 800;
	}
	else if(data <= 1000) {
		k = 3;
		interval = 1000;
	}
	else if(data <= 1200) {
		k = 4;
		interval = 1200;
	}
	else if(data <= 1400) {
		k = 5;
		interval = 1400;
	}
	else if(data <= 1600) {
		k = 6;
		interval = 1600;
	}
	else if(data <= 1800) {
		k = 7;
		interval = 1800;
	}
	else if(data <= 2000) {
		k = 8;
		interval = 2000;
	}
	else if(data <= 2200) {
		k = 9;
		interval = 2200;
	}
	else if(data <= 2400) {
		k = 10;
		interval = 2400;
	}
	else if(data <= 2600) {
		k = 11;
		interval = 2600;
	}
	else if(data <= 2800) {
		k = 12;
		interval = 2800;
	}
	else if(data <= 3000) {
		k = 13;
		interval = 3000;
	}
	else if(data <= 3200) {
		k = 14;
		interval = 3200;
	}
	else if(data <= 3400) {
		k = 15;
		interval = 3400;
	}
	else if(data <= 3600) {
		k = 16;
		interval = 3600;
	}
	else if(data <= 3800) {
		k = 17;
		interval = 3800;
	}
	else if(data <= 4000) {
		k = 18;
		interval = 4000;
	}
	else if(data <= 4200) {
		k = 19;
		interval = 4200;
	}
	else if(data <= 4400) {
		k = 20;
		interval = 4400;
	}
	else if(data <= 4600) {
		k = 21;
		interval = 4600;
	}
	else if(data <= 4800) {
		k = 22;
		interval = 4800;
	}
	else {
		k = 23;
		interval = 5000;
	}
	return interval;
}

uint8_t bufferNumberArray[5];
uint16_t DynamicAxisScale(SPI_TypeDef *SPIx, uint16_t data, uint16_t color, uint16_t backgroundcolor){
	uint16_t interval = GetInterval(data);
	//y_axis_array_numbers[0] = 400; //какого-то хрена при увеличении до след интервала портит первый элемент массива
	for(uint16_t i = 0, j = 0; j < 5; i += k, j++){
		NumbersArray(y_axis_array_numbers[i], bufferNumberArray);
		if(y_axis_array_numbers[i] < 1000) {
			DrawString5x7(SPIx, 1, dashArrayCoordinate[j] - 3, space, backgroundcolor, backgroundcolor);
			DrawString5x7(SPIx, 7, dashArrayCoordinate[j] - 3, bufferNumberArray, color, backgroundcolor);
		}
		else{
			DrawString5x7(SPIx, 0, dashArrayCoordinate[j] - 3, bufferNumberArray, color, backgroundcolor);
		}
	}
	return interval;
}

uint8_t DataCounter = 0;
uint16_t dataBuffer[130];

void PlotGist(SPI_TypeDef *SPIx, uint8_t X_axis_begin, uint8_t X_axis_end, uint8_t Y_axis_begin, uint8_t Y_axis_end, uint16_t data, uint16_t color, uint16_t backgroundcolor){
	uint8_t x_coordinate = X_axis_end;
	uint16_t max_interval = 600;
	//uint16_t interval = DynamicAxisScale(data, YELLOW, backgroundcolor);

	if(DataCounter == X_axis_end - (X_axis_begin)){ //если данные дошли до конца экрана
		for(uint8_t i = 1; i <= DataCounter; i++){
			dataBuffer[i] = dataBuffer[i+1];			//сдвигаем массив влево
		}
		dataBuffer[DataCounter-1] = data;
	}
	else {
		dataBuffer[DataCounter++] = data;
	}
	for(signed i = DataCounter-1; i > 0; i--){
		if(dataBuffer[i] > max_interval) max_interval = DynamicAxisScale(SPIx, dataBuffer[i], YELLOW, backgroundcolor);
	}
	for(signed i = DataCounter-1; i > 0; i--){
		Draw_V_Line(SPIx, x_coordinate, Y_axis_begin, Y_axis_end-3, backgroundcolor);
		Draw_V_Line(SPIx, x_coordinate--, Scale(dataBuffer[i], 400, max_interval, Y_axis_end-3, 80), Y_axis_end-3, color);
	}
}
