/*
 * plot.h
 *
 *  Created on: 6 мар. 2019 г.
 *      Author: crinitusfeles
 */

#ifndef CODE_INCLUDE_PLOT_H_
#define CODE_INCLUDE_PLOT_H_
#include "stm32l4xx.h"

void PlotAxes(SPI_TypeDef *SPIx, uint8_t Y_axis_bigin, uint8_t Y_axis_end, uint8_t Y_axis_coordinate,
		uint8_t X_axis_bigin, uint8_t X_axis_end, uint8_t X_axis_coordinate,
		uint8_t dashSize, uint16_t colour, uint16_t backgroundColour);
uint8_t Scale(uint16_t data, uint16_t y_min, uint16_t y_max, uint8_t y_axis_end, uint8_t pixels);
uint16_t DynamicAxisScale(SPI_TypeDef *SPIx, uint16_t data, uint16_t color, uint16_t backgroundcolor);
void PlotGist(SPI_TypeDef *SPIx, uint8_t X_axis_begin, uint8_t X_axis_end, uint8_t Y_axis_begin, uint8_t Y_axis_end, uint16_t data, uint16_t color, uint16_t backgroundcolor);
void PlotCO2(SPI_TypeDef *SPIx, uint16_t CO2, uint16_t color, uint16_t backgroundcolor);
void PlotTemp(SPI_TypeDef *SPIx, uint8_t temperature, uint16_t color, uint16_t backgroundcolor);
uint16_t GetInterval(uint16_t data);
#endif /* CODE_INCLUDE_PLOT_H_ */
