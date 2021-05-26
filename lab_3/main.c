#include "main.h"

int main(void){
	initGPIO();
	TIM4_Init();
	
	osKernelInitialize();
	osThreadNew(PWM1, NULL, NULL);
	osThreadNew(PWM2, NULL, NULL);
	osKernelStart();
	
	while(1){

	}
}
	

void initGPIO(void)
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
}

void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef timer;																	//Init Time Base structure
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);						//Enable TIM4 clock
	
	TIM_TimeBaseStructInit(&timer);																	//Fill structures's fields with default values
	timer.TIM_Prescaler = 63999; 																		//Specifies the prescaler value to one tick every 1 ms
	timer.TIM_Period = 1000 - 1;																		//1 s, Specifies the period value to be loaded into the active 
																																				//Auto-Reload Register and the time interruption is generated
	TIM_TimeBaseInit(TIM4, &timer);																	//Init TIM4
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);											//Configure TIM4 to generate interruption at update(perepovnennya)
	TIM_Cmd(TIM4, ENABLE);																					//Start TIM4
	
	//Enable the Interrupt
	NVIC_InitTypeDef nvic;																					//Init Nested Vector Interrupt Controller structure
	nvic.NVIC_IRQChannel = TIM4_IRQn;																//Specify the IRQ (Interrupt ReQuest) channel
	nvic.NVIC_IRQChannelPreemptionPriority = 0;											//Set Priority
	nvic.NVIC_IRQChannelSubPriority = 0;														//Set Subpriority
	nvic.NVIC_IRQChannelCmd = ENABLE;																//Enable IRQ Channel
	NVIC_Init(&nvic);																								//Init NVIC
}
 

uint32_t ticks_delay = 0;																										//variable for global time

void TIM4_IRQHandler(void) {																								
  ticks_delay++;																														//add 1 each time interrupt happens
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);																//clear interruption bit
}


void delay(uint32_t sec) {
  uint32_t start = ticks_delay;																							//capture time the function was called
  while((ticks_delay - start) < sec);															//compare time that passed with the delay
}

void PWM1(void* argum)
{
	while(1){
		GPIOA->BSRR |= GPIO_BSRR_BS4;											
		GPIOB->BSRR |= GPIO_BSRR_BS12;											
		
		delay(10);
		GPIOB->BRR |= GPIO_BRR_BR12;										
		
		delay(5);
		GPIOA->BRR |= GPIO_BRR_BR4;											
		
		delay(5);
	}
}

void PWM2(void* argum)
{
	while(1){
		GPIO_SetBits(GPIOC, GPIO_BSRR_BS4);
		GPIO_SetBits(GPIOC, GPIO_BSRR_BS12);
		
		delay(5);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		
		delay(10);
		GPIO_ResetBits(GPIOC, GPIO_BSRR_BS12);
		
		delay(10);
	}
}