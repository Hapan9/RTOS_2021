#include "main.h"


int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Initialize LED which connected to PC13 */
	// Enable PORTC Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_BSRR_BS12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	
	while (1) {
			work(15, 10, 20);
		}
}

void delay(uint32_t ms)
{
	for(uint32_t i = 0; i < ms; i++)
		for(uint32_t j = 0; j < 10000; j++) {}
}

void work (uint32_t periodA, uint32_t periodB, uint32_t period){
	//struct
	GPIOA->BSRR |= GPIO_BSRR_BS4;											
	GPIOB->BSRR |= GPIO_BSRR_BS12;											
	
	delay(periodB);
	GPIOB->BRR |= GPIO_BRR_BR12;										
	
	delay(periodA - periodB);
	GPIOA->BRR |= GPIO_BRR_BR4;											
	
	delay(period - periodA);
	
	//driver
	GPIO_SetBits(GPIOC, GPIO_BSRR_BS4);
	GPIO_SetBits(GPIOC, GPIO_BSRR_BS12);
	
	delay(periodB);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	
	delay(periodA - periodB);
	GPIO_ResetBits(GPIOC, GPIO_BSRR_BS12);
	
	delay(period - periodA);
}