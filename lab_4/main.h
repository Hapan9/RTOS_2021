#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "cmsis_os2.h"

void initGPIO(void);
void callback (void *argument);
void thread1(void* argum);
void PWM1(void);
void PWM2(void);
void PWM3(void);

#endif /*_MAIN_H*/