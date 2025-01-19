#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"

#define LFLeg       GPIO_Pin_0//左前腿
#define RFLeg       GPIO_Pin_1//右前腿
#define LHLeg       GPIO_Pin_2//左后腿
#define RHLeg       GPIO_Pin_3//右后腿
#define Tail        GPIO_Pin_6//尾巴
#define AllBody     GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_6//所有肢体

void PWM_Init(void);
void Set_TIM2CCR1(uint16_t Compare);
void Set_TIM2CCR2(uint16_t Compare);
void Set_TIM2CCR3(uint16_t Compare);
void Set_TIM2CCR4(uint16_t Compare);
void Set_TIM3CCR(uint16_t Compare);


#endif
