#include "gpio.h"


/*
 *
 * 	 _______________________________________________________
 * 	| MODE | OTYPER | OSPEED | PUPD  |	I/O configuration	|
 * 	|______|________|________|_______|______________________|
 * 	|	   |	0	|		 | 0 | 0 | GP output | PP		|
 * 	|	   |	0	|		 | 0 | 1 | GP output | PP + PU	|
 * 	|	   |	0	|	00	 | 1 | 0 | GP output | PP + PD	|
 * 	|	   |	0	|	01	 | 1 | 1 | Reserved				|
 * 	|  01  |	1	|	10	 | 0 | 0 | GP output | OD		|
 * 	|	   |	1	|	11	 | 0 | 1 | GP output | OD + PU	|
 * 	|	   |	1	|		 | 1 | 0 | GP output | OD + PD	|
 * 	|______|____1___|________|_1___1_|_Reserved_____________|
 * 	|	   |	0	|		 | 0 | 0 | AF 		 | PP		|
 * 	|	   |	0	|		 | 0 | 1 | AF 		 | PP + PU	|
 * 	|	   |	0	|	00	 | 1 | 0 | AF 		 | PP + PD	|
 * 	|	   |	0	|	01	 | 1 | 1 | Reserved				|
 * 	|  10  |	1	|	10	 | 0 | 0 | AF 		 | OD		|
 * 	|	   |	1	|	11	 | 0 | 1 | AF 		 | OD + PU	|
 * 	|	   |	1	|		 | 1 | 0 | AF 		 | OD + PD	|
 * 	|______|____1___|________|_1___1_|_Reserved_____________|
 * 	|	   |	x	| x  | x | 0 | 0 | Input	 | Floating |
 * 	|	   |	x	| x  | x | 0 | 1 | Input	 | PU		|
 * 	|  00  |	x	| x  | x | 1 | 0 |_Input_____|_PD_______|
 * 	|	   |	x	| x  | x | 1 | 1 |       Reserved		|
 * 	|______|________|____|___|___|___|______________________|
 * 	|	   |	x	| x  | x | 0 | 0 |_In/Out____|_Analog___|
 * 	|	   |	x	| x  | x | 0 | 0 | 						|
 * 	|  11  |	x	| x  | x | 0 | 0 | 		Reserved		|
 * 	|	   |	x	| x  | x | 0 | 0 | 						|
 * 	|______|________|____|___|___|___|______________________|
 *
 */
#define NUMBER_OF_PINS_ON_EACH_PORT 	16

enum port {
	Port_A, Port_B, Port_C, Port_D, Port_E, Port_F, Port_G
};

void gpio_init(int gpio, int mode, int config, int pull_up_down, int speed){
	int pin = gpio % NUMBER_OF_PINS_ON_EACH_PORT;
	GPIO_TypeDef *gpio_port;
	switch(gpio / NUMBER_OF_PINS_ON_EACH_PORT){ //Вычисляет порт
		case Port_A:
			gpio_port = GPIOA;
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
			break;
		case Port_B:
			gpio_port = GPIOB;
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
			break;
		case Port_C:
			gpio_port = GPIOC;
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
			break;
		case Port_D:
			gpio_port = GPIOD;
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
			break;
		case Port_E:
			gpio_port = GPIOE;
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
			break;
		default:
			break;
	}

	gpio_port->PUPDR &= ~(3 << (pin * 2));
	gpio_port->PUPDR |= pull_up_down << (pin * 2);

	gpio_port->OTYPER &= ~(1 << pin);
	gpio_port->OTYPER |= (config << pin);

	gpio_port->OSPEEDR &= ~(3 << (pin * 2));
	if(speed != Input) gpio_port->OSPEEDR |= speed << (pin * 2);

	if(mode > 3){
		gpio_port->MODER &= ~(3 << (pin * 2));
		gpio_port->MODER |= 2 << (pin * 2);

		gpio_port->AFR[pin/8] &= ~(0x0F << (pin * 4));
		pin/8 == 0 ? (gpio_port->AFR[pin/8] |= ((mode-4) << (pin * 4))) : (gpio_port->AFR[pin/8] |= ((mode-4) << ((pin-8) * 4)));
	}
	else{
		gpio_port->MODER &= ~(3 << (pin * 2));
		gpio_port->MODER |= mode << (pin * 2);
	}

}
void gpio_state(int gpio, int state){
	/*
	 * Устанавливает выбранный пин в состояние state (LOW or HIGH)
	 */
	int port = gpio / NUMBER_OF_PINS_ON_EACH_PORT;
	int pin = gpio % NUMBER_OF_PINS_ON_EACH_PORT;
	switch(port){
		case Port_A:
			GPIOA->BSRR = 0x01 << (pin + (!state) * 16); //(!state_)
		break;
		case Port_B:
			GPIOB->BSRR = 0x01 << (pin + (!state) * 16);
		break;
		case Port_C:
			GPIOC->BSRR = 0x01 << (pin + (!state) * 16);
		break;
		case Port_D:
			GPIOD->BSRR = 0x01 << (pin + (!state) * 16);
		break;
		case Port_E:
			GPIOE->BSRR = 0x01 << (pin + (!state) * 16);
		break;
		default:
			break;
	}
}
void gpio_toggle(int gpio){
	/*
	 * Инвертирует состояние пина
	 */
	int port = gpio / NUMBER_OF_PINS_ON_EACH_PORT;
	int pin = gpio % NUMBER_OF_PINS_ON_EACH_PORT;
	switch(port){
		case Port_A:
			GPIOA->ODR ^= 0x01 << pin;
		break;
		case Port_B:
			GPIOB->ODR ^= 0x01 << pin;
		break;
		case Port_C:
			GPIOC->ODR ^= 0x01 << pin;
		break;
		case Port_D:
			GPIOD->ODR ^= 0x01 << pin;
		break;
		case Port_E:
			GPIOE->ODR ^= 0x01 << pin;
		break;
		default:
			break;
	}
}
int gpio_read(int gpio){
	/*
	 * Возвращает логический уровень указанного пина. ВАЖНО! только пина, а не всего порта
	 */
	int port = gpio / NUMBER_OF_PINS_ON_EACH_PORT;
	int pin = gpio % NUMBER_OF_PINS_ON_EACH_PORT;
	switch(port){
		case Port_A:
			return (GPIOA->IDR & (0x01 << pin)) >> pin;
		break;
		case Port_B:
			return (GPIOB->IDR & (0x01 << pin)) >> pin;
		break;
		case Port_C:
			return (GPIOC->IDR & (0x01 << pin)) >> pin;
		break;
		case Port_D:
			return (GPIOD->IDR & (0x01 << pin)) >> pin;
		break;
		case Port_E:
			return (GPIOE->IDR & (0x01 << pin)) >> pin;
		break;

		default:
			return PORT_ERROR;
		break;
	}
}
