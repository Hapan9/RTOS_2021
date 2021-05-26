#include "main.h"

int main(void){
	//init peripherals
	initGPIO();
	SysTick_Init();
	TIM4_PWM_Init(199);
	
	while(1){
		
		GPIOC->ODR ^= GPIO_ODR_ODR13;																//togl C13
		delay(1000);
		
		if (checkButtonClick(GPIOB->IDR & GPIO_IDR_IDR0)){					//if btn on PB0 pressed
			incrTIM4PulseWidth();																			//increment TIM4 PWM pulse width
			delay(100);
		}
		if (checkButtonClick(GPIOB->IDR & GPIO_IDR_IDR1)){					//if btn on PB1 pressed
			ChangeSysTickStatus();																		//togl SysTick
			delay(1000);
		}
	}
}
	

void initGPIO(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);						//Enable GPIOC clock
	
	GPIO_InitTypeDef  GPIO_InitStructureA1;													//Init pin initializaion structures
	GPIO_InitTypeDef  GPIO_InitStructureB0_B1;											
	GPIO_InitTypeDef 	port;
	GPIO_InitTypeDef	GPIO_InitStructureC13;
	
	GPIO_StructInit(&GPIO_InitStructureA1);													//Fill structures's fields with default values
	GPIO_StructInit(&GPIO_InitStructureB0_B1);
	GPIO_StructInit(&port);
	
	GPIO_InitStructureA1.GPIO_Pin = GPIO_Pin_1;											//GPIOA pin1
	GPIO_InitStructureA1.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureA1.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode
	
	GPIO_InitStructureB0_B1.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;			//GPIOB pin0 & pin1
	GPIO_InitStructureB0_B1.GPIO_Mode = GPIO_Mode_IPU;							//input pull-up
	GPIO_InitStructureB0_B1.GPIO_Speed = GPIO_Speed_2MHz;						//2 MHz in mode
	
	port.GPIO_Mode = GPIO_Mode_AF_PP;																//Alternate function Push&Pull
	port.GPIO_Pin = GPIO_Pin_0;																			//GPIOA pin0
	port.GPIO_Speed = GPIO_Speed_2MHz;															//2 MHz out mode
	
	GPIO_InitStructureC13.GPIO_Pin = GPIO_Pin_13;										//GPIOC pin13
	GPIO_InitStructureC13.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureC13.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructureA1);												//Init GPIOA Pin1
	GPIO_Init(GPIOB, &GPIO_InitStructureB0_B1);											//Init GPIOB Pin0 & Pin1	
	GPIO_Init(GPIOA, &port);																				//Init GPIOA Pin0
	GPIO_Init(GPIOC, &GPIO_InitStructureC13);												//Init GPIOC Pin13
}

void TIM4_PWM_Init(uint16_t period)
{
	TIM_TimeBaseInitTypeDef timer;																	//Time Base Init structure
	TIM_OCInitTypeDef timerPWM;																			//Output Compare Init structure
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);						//Enable TIM4 clock
		
	TIM_TimeBaseStructInit(&timer);																	//Fill structures's fields with default values
	timer.TIM_Prescaler = SystemCoreClock/40000U - 1;								//Specifies the prescaler value to one tick every 0.025 ms
	timer.TIM_Period = period;																			//Specifies the period value to be loaded into the active Auto-Reload Register					
	timer.TIM_ClockDivision = 1;																		//Specifies the clock division
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &timer);																	//Init TIM4
	
	TIM_OCStructInit(&timerPWM);																		//Fill structures's fields with default values
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;													//Specifies the TIM mode as PWM1
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_Pulse = 120;																				//Set PWM pulse width (relative to period)
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;									//Set PWM polarity to high (pulse is high signal)
	TIM_OC1Init(TIM4, &timerPWM);																		//init TIM4 output compare mode
	
	TIM_Cmd(TIM4, ENABLE);																					//Start TIM4
}

void incrTIM4PulseWidth(void)
{
	if (TIM4->CCR1 == ((TIM4->ARR+1)/100*60))												//if pulse width is already 60% (in TIM4->CCR1 value 120)
		TIM4->CCR1 = 1;																								//set pulse width to 0.5%
	else 
		TIM4->CCR1 += 1;																							//else increment pulse width by 0.5%
}

char checkButtonClick(uint32_t pin)
{
	if (!pin){
		delay(20);
		if (!pin)
			return TRUE;
	}
		
	return FALSE;
}

void SysTick_Init(void)
{
	SysTick->LOAD = SystemCoreClock/1000 - 1;																	//set SysTick reload period to 1 ms
	SysTick->VAL = SystemCoreClock/1000 - 1;	
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; 		//set SysTick clock source to internal and allow SysTick interrrupts
}

void ChangeSysTickStatus(void)
{
	if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == SysTick_CTRL_ENABLE_Msk) //if SysTick enabled,
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;															//disable it
	else
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;																//else enable
}


uint32_t ticks_delay = 0;																										//variable for global time

void SysTick_Handler(void) {																								
  ticks_delay++;																														//add 1 each time interrupt happens
}

void delay(uint32_t milliseconds) {
  uint32_t start = ticks_delay;																							//capture time the function was called
  while((ticks_delay - start) < milliseconds);															//compare time that passed with the delay
}