/*
 * rcc.c
 *
 *  Created on: 20 ���. 2020 �.
 *      Author: ftor
 */


#include "rcc.h"
#include "System.h"
#include "stm32l431xx.h"

int RCC_init(){
	//RCC_DeInit();
	if(!(RCC->CR & RCC_CR_HSERDY)){ //check HSE if it didn't launch, then launch
		RCC->CR |= RCC_CR_HSEON;
		while(!(RCC->CR & RCC_CR_HSERDY)); //waiting while HSE launching
	}
	RCC->CR &= ~(RCC_CR_PLLON);
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
//	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM;

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	RCC->PLLCFGR |= (20 << 8); //HSE(8MHz) * VCO(20) / PLLR(2) = 80Mhz

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
//	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));

	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	/*
	 * Number of wait states according to CPU clock (HCLK) frequency
	 * 	__________________________________________
	 * 	|  LATENCY  |_____________HCLK____________|		Range 1: High-performance range.	 *
	 * 	|___________|_Vcore Range1_|_Vcore Range2_|		Range 2: Low-power range
	 * 	| 	0 WS	|	  <16	   |	  <6	  |
	 * 	|	1 WS	|	  <32	   |	  <12	  |
	 * 	|	2 WS	|	  <48	   |	  <18	  |
	 * 	|	3 WS	|	  <64	   |	  <24	  |
	 * 	|___4 WS____|_____<80______|______<30_____|
	 *
	 */
	FLASH->ACR &= 0xFFFFFFF8; //clear register
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;

	// The maximum frequency of the AHB, the APB1 and the APB2 domains is 80 MHz.
	uint32_t AHB_presc = RCC_CFGR_HPRE_0;
	uint32_t APB1_presc = RCC_CFGR_PPRE1_0;
	uint32_t APB2_presc = RCC_CFGR_PPRE2_0;
	uint8_t system_clk = F_CPU / 1000000;

	RCC->CFGR |= AHB_presc | APB1_presc | APB2_presc; //AHB not devided (72MHz) | APB1 not devided (72MHz) | APB2 not devided (72MHz)

	RCC->CFGR |= RCC_CFGR_SW_PLL;

	RCC->CR &= ~(RCC_CR_HSION);

	uint32_t timings = 0; //  unused - 00000000 | AHB - 00000000 | APB2 - 00000000 | APB1 - 00000000
	timings |= ((system_clk / (AHB_presc >> 4)) << 16) | (system_clk / (APB2_presc >> 11) << 0) | ((system_clk / (APB1_presc >> 8)) << 8);
	return timings;

}
