#include "TIM.h"

/**
 * @brief 1ms中断函数初始化
 * 
 */
void TIM4_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

    TIM_InternalClockConfig(TIM4);

    TIM_TimeBaseInitTypeDef TIM4_Init_Struct;
    TIM_TimeBaseStructInit(&TIM4_Init_Struct);
    TIM4_Init_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM4_Init_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM4_Init_Struct.TIM_Period = 1000 - 1;
    TIM4_Init_Struct.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInit(TIM4,&TIM4_Init_Struct);

    TIM_ClearFlag(TIM4,TIM_FLAG_Update);

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

    NVIC_InitTypeDef TIM4_NVIC_Init_Struct;
    TIM4_NVIC_Init_Struct.NVIC_IRQChannel = TIM4_IRQn;
    TIM4_NVIC_Init_Struct.NVIC_IRQChannelCmd = ENABLE;
    TIM4_NVIC_Init_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    TIM4_NVIC_Init_Struct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&TIM4_NVIC_Init_Struct);

    TIM_Cmd(TIM4,ENABLE);   
}
