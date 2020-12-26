/*
 * gpio.h
 *
 *  Created on: 15 ���. 2020 �.
 *      Author: BreakingBad
 */


#ifndef INCLUDE_GPIO_H_
#define INCLUDE_GPIO_H_

//#define L496
#define L431
//#define F405

#ifdef F405
	#include "stm32f4xx.h"
	#include "f405_gpio_alt_func.h"
#endif
#ifdef L496
	#include "stm32l4xx.h"
	#include "l496_gpio_alt_func.h"
#endif
#ifdef L431
	#include "stm32l4xx.h"
	#include "l431_gpio_alt_func.h"
#endif

enum gpio_errors {
	PORT_ERROR=-3, MODE_ERROR, SPEED_ERROR
};
enum gpio {
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
	PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
	PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
	PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15
};
enum state {
	LOW, HIGH
};
enum mode {
	Input_mode, General_output, FORBIDDEN_VALUE, Analog_mode, AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15
};
enum io_configuration{
	Push_pull, Open_drain
};
enum pull_up_down{
	no_pull, pull_up, pull_down
};
enum speed {
	Low_speed, Medium_speed, High_speed, Very_high_speed, Input
};
void gpio_init(int gpio, int mode, int config, int pull_up_down, int speed);
void gpio_state(int gpio, int state);
void gpio_toggle(int gpio);
int gpio_read(int gpio);
//void GPIO_init_PB0_PB1();
//void CAN1_gpio_init();
//void CAN2_gpio_init();
#endif /* CODE_HEADER_GPIO_H_ */
/*
 * gpio.h
 *
 *  Created on: 20 ���. 2020 �.
 *      Author: ftor
 */


