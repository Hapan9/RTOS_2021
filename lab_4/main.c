#include "main.h"

int main(void){
	
	initGPIO();
	osTimerId_t id0, id1, id2;																								//Timer ID identifies the timer.
	
	// creates a periodic timer:
	id0 = osTimerNew(callback, osTimerPeriodic, (void *)0, NULL); 						// (void*)0 is passed as an argument
	id1 = osTimerNew(callback, osTimerPeriodic, (void *)1, NULL); 						// (void*)1 is passed as an argument
	id2 = osTimerNew(callback, osTimerPeriodic, (void *)2, NULL); 						// (void*)2 is passed as an argument
	
 	osKernelInitialize();

	osTimerStart(id0, 500U);                                                 // to the callback function
	osTimerStart(id1, 10U);                                                   // to the callback function
	osTimerStart(id2, 10U);                                                   // to the callback function

	osThreadNew(thread1, NULL, NULL);
	
	osKernelStart();
	
	while(1){

	}
}
	
void callback (void *argument) {
  int32_t arg = (int32_t)argument; 						// cast back argument '0', '1' or '2'
	
	// do something, i.e. set thread/event flags

	switch(arg)
	{
		case 0:
			PWM1();
			break;
		case 1:
			PWM2();
			break;
		case 2:
			PWM3();
			break;
	}
}

void initGPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);						//Enable GPIOC clock
	
	GPIO_InitTypeDef	GPIO_InitStructureA3;													//Init structure for A3
	GPIO_InitTypeDef	GPIO_InitStructureB11;												//Init structure for B11
	GPIO_InitTypeDef	GPIO_InitStructureC13;												//Init structure for C13

	GPIO_InitStructureA3.GPIO_Pin = GPIO_Pin_3;											//GPIOA pin3
	GPIO_InitStructureA3.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureA3.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode
	
	GPIO_InitStructureB11.GPIO_Pin = GPIO_Pin_11;										//GPIOB pin11
	GPIO_InitStructureB11.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureB11.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode
	
	GPIO_InitStructureC13.GPIO_Pin = GPIO_Pin_13;										//GPIOC pin13
	GPIO_InitStructureC13.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureC13.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode

	GPIO_Init(GPIOA, &GPIO_InitStructureA3);												//Init GPIOA Pin3
	GPIO_Init(GPIOB, &GPIO_InitStructureB11);												//Init GPIOB Pin11	
	GPIO_Init(GPIOC, &GPIO_InitStructureC13);												//Init GPIOC Pin13

	GPIOC->ODR ^= GPIO_ODR_ODR13;																		//togl C13
}

void thread1(void* argum)
{
	while(1){

	}
}

void PWM1(void)
{
	GPIOC->ODR ^= GPIO_ODR_ODR13;																//togl C13
	osDelay(500U);
	GPIOC->ODR ^= GPIO_ODR_ODR13;																//togl C13
}

void PWM2(void)
{
	GPIOA->ODR ^= GPIO_ODR_ODR3;																//togl A3
	osDelay(10U);
	GPIOA->ODR ^= GPIO_ODR_ODR3;																//togl A3
}

void PWM3(void)
{
	GPIOB->ODR ^= GPIO_ODR_ODR11;																//togl B11
	osDelay(5U);
	GPIOB->ODR ^= GPIO_ODR_ODR11;																//togl B11
}