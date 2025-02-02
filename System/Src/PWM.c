/*
    TIM2 --> 四肢
    TIM3 --> 尾巴
*/
#include "PWM.h"

/**
 * @brief 定时器PWM模式初始化
 * 
 */
void PWM_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    GPIO_InitTypeDef GPIOA_PWM_Init_Struct;
    GPIOA_PWM_Init_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIOA_PWM_Init_Struct.GPIO_Pin = AllBody;
    GPIOA_PWM_Init_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIOA_PWM_Init_Struct);

    TIM_InternalClockConfig(TIM2);
    TIM_InternalClockConfig(TIM3);

    /*TIM2********************************************/
    TIM_TimeBaseInitTypeDef TIM2_PWM_Init_Struct;
    TIM_TimeBaseStructInit(&TIM2_PWM_Init_Struct);
    TIM2_PWM_Init_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM2_PWM_Init_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM2_PWM_Init_Struct.TIM_Period = 20000 - 1;
    TIM2_PWM_Init_Struct.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInit(TIM2,&TIM2_PWM_Init_Struct);

    TIM_OCInitTypeDef TIM2_OC_Init_Struct;
    TIM_OCStructInit(&TIM2_OC_Init_Struct);
    TIM2_OC_Init_Struct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM2_OC_Init_Struct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM2_OC_Init_Struct.TIM_OutputState = TIM_OutputState_Enable;
    TIM2_OC_Init_Struct.TIM_Pulse = 0;
    TIM_OC1Init(TIM2,&TIM2_OC_Init_Struct);
    TIM_OC2Init(TIM2,&TIM2_OC_Init_Struct);
    TIM_OC3Init(TIM2,&TIM2_OC_Init_Struct);
    TIM_OC4Init(TIM2,&TIM2_OC_Init_Struct);
    /********************************************TIM2*/

    /*TIM3********************************************/
    TIM_TimeBaseInitTypeDef TIM3_PWM_Init_Struct;
    TIM_TimeBaseStructInit(&TIM3_PWM_Init_Struct);
    TIM3_PWM_Init_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM3_PWM_Init_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM3_PWM_Init_Struct.TIM_Period = 20000 - 1;
    TIM3_PWM_Init_Struct.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInit(TIM3,&TIM3_PWM_Init_Struct);

    TIM_OCInitTypeDef TIM3_OC_Init_Struct;
    TIM_OCStructInit(&TIM3_OC_Init_Struct);
    TIM3_OC_Init_Struct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM3_OC_Init_Struct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM3_OC_Init_Struct.TIM_OutputState = TIM_OutputState_Enable;
    TIM3_OC_Init_Struct.TIM_Pulse = 0;
    TIM_OC1Init(TIM3,&TIM3_OC_Init_Struct);
    /********************************************TIM3*/

    TIM_Cmd(TIM2,ENABLE);
    TIM_Cmd(TIM3,ENABLE);
}

/**
 * @brief TIM2 OC1占空比
 * 
 * @param Duty 占空比
 */
void Set_TIM2CCR1(uint16_t Duty)
{
    TIM_SetCompare1(TIM2,Duty);
}

/**
 * @brief TIM2 OC2占空比
 * 
 * @param Duty 占空比
 */
void Set_TIM2CCR2(uint16_t Duty)
{
    TIM_SetCompare2(TIM2,Duty);
}

/**
 * @brief TIM2 OC3占空比
 * 
 * @param Duty 占空比
 */
void Set_TIM2CCR3(uint16_t Duty)
{
    TIM_SetCompare3(TIM2,Duty);
}

/**
 * @brief TIM2 OC4占空比
 * 
 * @param Duty 占空比
 */
void Set_TIM2CCR4(uint16_t Duty)
{
    TIM_SetCompare4(TIM2,Duty);
}

/**
 * @brief TIM1 OC1占空比
 * 
 * @param Duty 占空比
 */
void Set_TIM3CCR(uint16_t Duty)
{
    TIM_SetCompare1(TIM3,Duty);
}
