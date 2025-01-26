#ifndef __Ultrasonic_h
#define __Ultrasonic_h

#include "stm32f10x.h"

#define Trigger         GPIO_Pin_7//触发超声波引脚
#define GetDistance     GPIO_Pin_8//获取超声波距离引脚

void Ultrasonic_Init(void);
uint16_t Get_Distance(void);

#endif
