#ifndef INC_RGB_LED_H_
#define INC_RGB_LED_H_

#include "stm32l4xx.h"

//extern uint8_t LED_init_flag[3];

void RGB_init(uint8_t pin, uint8_t led_num, uint8_t led_color, uint8_t mode);
void LED_Handler(uint8_t cmd, uint8_t pin, uint8_t led_num, uint8_t led_color);
void LED1_REGISTER_Handler(uint8_t cmd);
void LED2_REGISTER_Handler(uint8_t cmd);
void LED3_REGISTER_Handler(uint8_t cmd);

#endif /* INC_RGB_LED_H_ */
