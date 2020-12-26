#include "stm32l431xx.h"
#include "pwm.h"
#include "gpio.h"
#include "System.h"


#define TIMER_DEVIDER 2 //TIMx->PSC

int global_freq; //���������� ���������� ��� �������� �������, ������������ � ������� �������������. ������ �������� ���� ���-�� �������� �������
				 // ChangeFilling(TIM_TypeDef *TIMx , int channel, int fill) �.�. � ��� ���� ���������� ������� ���������� ����.

void PWM_init(TIM_TypeDef *TIMx, int channel, int freq, int fill){
	/*
	 * ������� ������������� ������ �������.
	 * TIM_TypeDef *TIMx - ������� �������, ��������������� ��� �����, �������� TIM1, TIM3 � �.�.
	 * int channel - ����� ������ �������. ��������� �� ���� ����� 1-4 ��� �������� �� ������������� ������ PWM_CH1-PWM_CH4.
	 * int freq - ������� ����. ���������� ��������� TIMER_DEVIDER. ��� ������ �������� TIMER_DEVIDER, ��� �� ������� ������� ����� �������� ���.
	 * 			  ��� TIMER_DEVIDER=1 � ������� ������������ ������� = 72��� ����������� ������� ��� = 550��.
	 * 			  ����� ������� ��� ���������� ������� ��� PWM_freq = timer_clk / (TIMx->ARR * (TIMx->PSC + 1))
	 * int fill - ������� ���������� ����. ����� ��������� �������� 0-100.
	 */

	global_freq = freq;

	if(TIMx == TIM1) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
//		if(channel == 1) 	  gpio_init(PA8, Alternative_PP, Max_50_MHz);
//		else if(channel == 2) gpio_init(PA9, Alternative_PP, Max_50_MHz);
//		else if(channel == 3) gpio_init(PA10, Alternative_PP, Max_50_MHz);
//		else if(channel == 4) gpio_init(PA11, Alternative_PP, Max_50_MHz);
	}

//		if(channel == PWM_CH1) 	  gpio_init(PA6, Alternative_PP, Max_50_MHz);
//		else if(channel == PWM_CH2) gpio_init(PA7, Alternative_PP, Max_50_MHz);
//		else if(channel == PWM_CH3) gpio_init(PB0, Alternative_PP, Max_50_MHz);
//		else if(channel == PWM_CH4) gpio_init(PB1, Alternative_PP, Max_50_MHz);

	else if(TIMx == TIM15){
		RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
	}
	else if(TIMx == TIM2){
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	}
	TIMx->PSC = TIMER_DEVIDER; // !�������! (��� ���������� �������� �������� ������ ������ ��������)

	int pwm_freq = F_CPU / ((TIMER_DEVIDER + 1) * freq); //��� TIMx->PSC = 1 ����������� ������� 550�� (�.�. TIMx->ARR 16 ���)
	TIMx->ARR = pwm_freq; //��� TIMx->PSC = 1 ����������� ������� 550�� (�.�. TIMx->ARR 16 ���)

	switch (channel) {  //�������� ��������� �����
		case PWM_CH1:
			TIMx->CCR1 = pwm_freq * fill / 100; 	//������� ����������
			TIMx->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;		//PWM mode 1, ��������� ��� 1 �����
			break;
		case PWM_CH2:
			TIMx->CCR2 = pwm_freq * fill / 100;	//������� ����������
			TIMx->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;		//PWM mode 1, ��������� ��� 2 �����
			break;
		case PWM_CH3:
			TIMx->CCR3 = pwm_freq * fill / 100;	//������� ����������
			TIMx->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0;	 	//PWM mode 1, ��������� ��� 3 �����
			break;
		case PWM_CH4:
			TIMx->CCR4 = pwm_freq * fill / 100;	//������� ����������
			TIMx->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0;		//PWM mode 1, ��������� ��� 4 �����
			break;
		default:
			break;
	}
	TIMx->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P | TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC1E | TIM_CCER_CC1P;
	TIMx->BDTR |= TIM_BDTR_MOE; 	//���������� ���� ��� ������ ��� ����������� �������� TIMx_CCER. �������... �������� � ��� �����...
	TIMx->CR1 &= ~TIM_CR1_DIR; 	//������� �����
	TIMx->CR1 &= ~TIM_CR1_CMS; 	//������������ �� ������, Fast PWM
	TIMx->CR1 |= TIM_CR1_CEN;		//��������� ������
}

void PWM_start_single(TIM_TypeDef *TIMx , int channel){
	TIMx->CCER |= (0x3 << (4 * (channel-1))); //�������� �� ����� �����.
}

void PWM_start_all(TIM_TypeDef *TIMx ){
	//�������� �� ����� �����.
	TIMx->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P | TIM_CCER_CC3E | TIM_CCER_CC3P; //| TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC1E | TIM_CCER_CC1P;
}

void PWM_stop_all(TIM_TypeDef *TIMx ){
	//������� ��� ��������� ���� ������� ������������
	TIMx->CCER &= ~(TIM_CCER_CC4E | TIM_CCER_CC4P | TIM_CCER_CC3E | TIM_CCER_CC3P); //| TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC1E | TIM_CCER_CC1P);
}

void PWM_change_fill(TIM_TypeDef *TIMx , int channel, int fill){
	//�������� ���������� ���� �� ��������� ������.
	/*
	 * TIM_TypeDef *TIMx - ������� �������, ��������������� ��� �����, �������� TIM1, TIM3 � �.�.
	 * int channel - ����� ������ �������. ��������� �� ���� ����� 1-4 ��� �������� �� ������������� ������ PWM_CH1-PWM_CH4.
	 * int fill - ������� ���������� ����. ����� ��������� �������� 0-100.
	 */

	if(channel == PWM_CH1) TIMx->CCR1 = F_CPU / ((TIMER_DEVIDER + 1) * global_freq) * fill / 100; // F_CPU / ((TIMER_DEVIDER + 1) * global_freq) - ������� ����
	else if(channel == PWM_CH2) TIMx->CCR2 = F_CPU / ((TIMER_DEVIDER + 1) * global_freq) * fill / 100; // PWM_freq * fill /100 - ������� ����������
	else if(channel == PWM_CH3) TIMx->CCR3 = F_CPU / ((TIMER_DEVIDER + 1) * global_freq) * fill / 100;
	else if(channel == PWM_CH4) TIMx->CCR4 = F_CPU / ((TIMER_DEVIDER + 1) * global_freq) * fill / 100;
}

void PWM_stop_single(TIM_TypeDef *TIMx , int channel){
	/*
	 * ������� ���������� ������ ���������� ������.
	 */
	TIMx->CCER &= ~(0x3 << (4 * (channel-1)));
//	TIMx->BDTR &= ~TIM_BDTR_MOE;
//	TIMx->CR1 &= ~TIM_CR1_CEN;

}

void PWM_deinit(TIM_TypeDef *TIMx ){
	/*
	 * ������� ��������������� ���������� �������, �.�. ��������� ���, ��������� �������� ������� � ���� �� � ��������� ���������
	 * TIM_TypeDef *TIMx - ������� �������, ��������������� ��� �����, �������� TIM1, TIM3 � �.�.
	 */
	TIMx->CCMR2 = 0;
	TIMx->CCMR1 = 0;
	TIMx->CCR1 = 0;
	TIMx->CCR2 = 0;
	TIMx->CCR3 = 0;
	TIMx->CCR4 = 0;
	TIMx->CCER = 0;
	TIMx->ARR = 0;
	if(TIMx == TIM1) {
		RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
//		gpio_init(PA8, General_PP, Max_2_MHz);
//		gpio_init(PA9, General_PP, Max_2_MHz);
//		gpio_init(PA10, General_PP, Max_2_MHz);
//		gpio_init(PA11, General_PP, Max_2_MHz);
	}

}
