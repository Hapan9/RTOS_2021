  
#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "cmsis_os2.h"

void TIM4_Init(void);
void initGPIO(void);
void delay(uint32_t milliseconds);
void PWM1(void* argum);
void PWM2(void* argum);

#endif /*_MAIN_H*/