#include "main.h"

int main(void){
	
	initGPIO();
	
 	osKernelInitialize();
	t1 = osThreadNew(thread1, NULL, NULL);							//Thread that checks the button on B11
	t2 = osThreadNew(thread2, NULL, NULL);							//Thread that creates PWM for C13 and doubles period and worktime when button is pressed
	t3 = osThreadNew(thread3, NULL, NULL);							//Thread that creates PWM for A3 and divides period and worktime by 2 when button is pressed
	osKernelStart();
	
	while(1){

	}
}

void initGPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);						//Enable GPIOC clock
	
	GPIO_InitTypeDef	GPIO_InitStructureB11;												//Init structure for B11
	GPIO_InitTypeDef	GPIO_InitStructureA3;													//Init structure for A3
	GPIO_InitTypeDef	GPIO_InitStructureC13;												//Init structure for C13

	GPIO_InitStructureB11.GPIO_Pin = GPIO_Pin_11;										//GPIOB pin11
	GPIO_InitStructureB11.GPIO_Mode = GPIO_Mode_IPU;								//Input Pull-Up
	GPIO_InitStructureB11.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode
	
	GPIO_InitStructureA3.GPIO_Pin = GPIO_Pin_3;											//GPIOA pin3
	GPIO_InitStructureA3.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureA3.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode
	
	GPIO_InitStructureC13.GPIO_Pin = GPIO_Pin_13;										//GPIOC pin13
	GPIO_InitStructureC13.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureC13.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode

	GPIO_Init(GPIOB, &GPIO_InitStructureB11);												//Init GPIOB Pin11	
	GPIO_Init(GPIOA, &GPIO_InitStructureA3);												//Init GPIOA Pin3
	GPIO_Init(GPIOC, &GPIO_InitStructureC13);												//Init GPIOC Pin13

	GPIOC->ODR ^= GPIO_ODR_ODR13;																		//toggle C13
}

void thread1(void* argum)
{
	while(1){
		if (checkButtonClick())
		{
			flagsSet2 = osThreadFlagsSet(t2, signalFlag);									//Set signal to t2 Thread with flag 0x01
			flagsSet3 = osThreadFlagsSet(t3, signalFlag);									//Set signal to t3 Thread with flag 0x01
		}
	}
}

void thread2(void* argum)
{
	uint32_t workTime = 150U;
	uint32_t period = 2500U;
	
	while(1){
		PWM2(workTime, period);

		flagsWait2 = osThreadFlagsWait(signalFlag, osFlagsWaitAny, 0);		//If flag 0x01 is set, returns thread flags before clearing; No time-out
		if (flagsWait2 == signalFlag)																			//True if signal from t1 Thread is set
		{
			workTime *= 2;
			period *= 2;
			flagsWait2 = 0;																									//Set to 0 to clear the value
		}
	}
}

void thread3(void* argum)
{
	uint32_t workTime = 30U;
	uint32_t period = 50U;
	
	while(1){
		PWM3(workTime, period);

		flagsWait3 = osThreadFlagsWait(signalFlag, osFlagsWaitAny, 0);			//If flag 0x01 is set, returns thread flags before clearing; No time-out
		if (flagsWait3 == signalFlag)																			//True if signal from t1 Thread is set
		{
			workTime /= 2;
			period /= 2;
			flagsWait3 = 0;																								//Set to 0 to clear the value
		}
	}
}

void PWM2(uint32_t workTime, uint32_t period)
{
		GPIOC->ODR ^= GPIO_ODR_ODR13;																//toggle C13
		osDelay(workTime);
		GPIOC->ODR ^= GPIO_ODR_ODR13;																//toggle C13
		osDelay(period - workTime);
}

void PWM3(uint32_t workTime, uint32_t period)
{
		GPIOA->ODR ^= GPIO_ODR_ODR3;																//toggle A3
		osDelay(workTime);
		GPIOA->ODR ^= GPIO_ODR_ODR3;																//toggle A3
		osDelay(period - workTime);
}

char checkButtonClick(void)
{
	uint32_t pin = GPIOB->IDR & GPIO_IDR_IDR11;										//Return Input Data Register value for B11
	if (!pin){																										//Check if value is 0x00 (button is pressed)
		return TRUE;																								
	}
	
	return TRUE;
}

