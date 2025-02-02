#ifndef __Led_H
#define __Led_H

#include "stm32f10x.h"

#define LED_PC13    GPIO_Pin_13

void Led_Init(void);
void Led_On(void);
void Led_Off(void);

#endif
