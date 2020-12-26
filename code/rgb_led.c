
#include "rgb_led.h"
#include "main.h"
#include "gpio.h"
#include "pwm.h"

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


void RGB_init(uint8_t pin, uint8_t led_num, uint8_t led_color, uint8_t mode){
	if(mode){
		if(LED_init_flag[led_num] & (1 << led_color)){
			return;
		}
		if(led_color == 2 && led_num == 1){
			gpio_init(LED2_B, PB14_TIM15_CH1, Push_pull, no_pull, High_speed);
		}
		else{
			gpio_init(pin, AF1, Push_pull, no_pull, High_speed);
		}
		LED_init_flag[led_num] &= ~(1 << (4 + led_color));
		LED_init_flag[led_num] |= (1 << led_color);
	}
	else{
		if(LED_init_flag[led_num] & (1 << (4 + led_color))){
			return;
		}
		gpio_init(pin, General_output, Push_pull, no_pull, High_speed);
		LED_init_flag[led_num] |= (1 << (4 + led_color));
		LED_init_flag[led_num] &= ~(1 << led_color);
	}
}

void LED_Handler(uint8_t cmd, uint8_t pin, uint8_t led_num, uint8_t led_color){
	if(((cmd & 0x07) % 2) == 1){	//cmd init PWM mode
		RGB_init(pin, led_num, led_color, 1);

		if(led_num == 0){
			PWM_init(TIM2, led_color + 1, 3000, (cmd >> 3) * 3);
			PWM_start_single(TIM2, led_color + 1);
		}
		else if(led_num == 1){
			if(led_color < 2){
				PWM_init(TIM1, led_color + 3, 3000, (cmd >> 3) * 3);
				PWM_start_single(TIM1, led_color + 3);
			}
			else if(led_color == 2){
				PWM_init(TIM15, PWM_CH1, 3000, (cmd >> 3) * 3);
				PWM_start_single(TIM15, PWM_CH1);
			}
		}
		else if(led_num == 2){
			if(led_color == 0){
				PWM_init(TIM2, PWM_CH4, 3000, (cmd >> 3) * 3);
				PWM_start_single(TIM2, PWM_CH4);
			}
			else if(led_color == 1 || led_color == 2){
				PWM_init(TIM1, led_color, 3000, (cmd >> 3) * 3);
				PWM_start_single(TIM1, led_color);
			}
		}

	}
	else if(((cmd & 0x07) % 2) == 0){ //cmd init bin mode
		RGB_init(pin, led_num, led_color, 0);
		(cmd >> 3) > 0 ? gpio_state(pin, HIGH) : gpio_state(pin, LOW);
	}
}

void LED1_REGISTER_Handler(uint8_t cmd){
	if(((cmd & 0x07) == 2) || ((cmd & 0x07) == 3)){
		LED_Handler(cmd, LED1_R, 0, 0);
	}
	else if(((cmd & 0x07) == 4) || ((cmd & 0x07) == 5)){
		LED_Handler(cmd, LED1_G, 0, 1);
	}
	else if(((cmd & 0x07) == 6) || ((cmd & 0x07) == 7)){
		LED_Handler(cmd, LED1_B, 0, 2);
	}
}
void LED2_REGISTER_Handler(uint8_t cmd){
	if(((cmd & 0x07) == 2) || ((cmd & 0x07) == 3)){
		LED_Handler(cmd, LED2_R, 1, 0);
	}
	else if(((cmd & 0x07) == 4) || ((cmd & 0x07) == 5)){
		LED_Handler(cmd, LED2_G, 1, 1);
	}
	else if(((cmd & 0x07) == 6) || ((cmd & 0x07) == 7)){
		LED_Handler(cmd, LED2_B, 1, 2);
	}
}
void LED3_REGISTER_Handler(uint8_t cmd){
	if(((cmd & 0x07) == 2) || ((cmd & 0x07) == 3)){
		LED_Handler(cmd, LED3_R, 2, 0);
	}
	else if(((cmd & 0x07) == 4) || ((cmd & 0x07) == 5)){
		LED_Handler(cmd, LED3_G, 2, 1);
	}
	else if(((cmd & 0x07) == 6) || ((cmd & 0x07) == 7)){
		LED_Handler(cmd, LED3_B, 2, 2);
	}
}
