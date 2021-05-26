#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "cmsis_os2.h"
#define TRUE 0x01
#define FALSE 0x00

char checkButtonClick(void);

#define signalFlag 0x01

osThreadId_t t1;
osThreadId_t t2;
osThreadId_t t3;
uint32_t flagsSet2;
uint32_t flagsSet3;
uint32_t flagsWait2;
uint32_t flagsWait3;
void thread1(void* argum);
void thread2(void* argum);
void thread3(void* argum);
void PWM2(uint32_t workTime, uint32_t period);
void PWM3(uint32_t workTime, uint32_t period);
void initGPIO(void);

#endif /*_MAIN_H*/