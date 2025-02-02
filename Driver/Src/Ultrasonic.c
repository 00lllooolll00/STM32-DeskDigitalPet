#include "Ultrasonic.h"

uint32_t TIM_Counter_Us;//计时器 us
uint8_t Ultrasonic_Flag = 0;//完成标志位

/**
 * @brief 初始化超声波模块
 * 
 */
void Ultrasonic_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_InitTypeDef GPIO_Ultras_Init_Struct;
    GPIO_Ultras_Init_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Ultras_Init_Struct.GPIO_Pin = Trigger;
    GPIO_Ultras_Init_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_Ultras_Init_Struct);
    GPIO_Ultras_Init_Struct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Ultras_Init_Struct.GPIO_Pin = GetDistance;
    GPIO_Ultras_Init_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_Ultras_Init_Struct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);

    EXTI_InitTypeDef GPIOB_EXTI_Init_Struct;
    GPIOB_EXTI_Init_Struct.EXTI_Line = EXTI_Line8;
    GPIOB_EXTI_Init_Struct.EXTI_LineCmd = ENABLE;
    GPIOB_EXTI_Init_Struct.EXTI_Mode = EXTI_Mode_Interrupt;
    GPIOB_EXTI_Init_Struct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&GPIOB_EXTI_Init_Struct);

    NVIC_InitTypeDef NVIC_EXTI_Init_Struct;
    NVIC_EXTI_Init_Struct.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_EXTI_Init_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_EXTI_Init_Struct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_EXTI_Init_Struct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_EXTI_Init_Struct);

    TIM_InternalClockConfig(TIM6);

    TIM_TimeBaseInitTypeDef TIM6_TB_Struct;
    TIM_TimeBaseStructInit(&TIM6_TB_Struct);
    TIM6_TB_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM6_TB_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM6_TB_Struct.TIM_Period = 1000 - 1;
    TIM6_TB_Struct.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInit(TIM6,&TIM6_TB_Struct);
    
    TIM_Cmd(TIM6,DISABLE);
}

/**
 * @brief 获取距离
 * 
 * @return uint16_t 返回距离值 cm
 */
uint16_t Get_Distance(void)
{
    GPIO_SetBits(GPIOB,Trigger);
    TIM_Cmd(TIM6,ENABLE);
    while(TIM_GetCounter(TIM6) < 15);
    TIM_SetCounter(TIM6,0);
    TIM_Cmd(TIM6,ENABLE);
    GPIO_ResetBits(GPIOB,Trigger);

    while(Ultrasonic_Flag != 1);
    Ultrasonic_Flag = 0;
    return (TIM_Counter_Us * 340) / 2 * 0.0000001 *100;
}

void EXTI9_5_IRQHandler(void)
{
    if(Ultrasonic_Flag == 1)return;

    if(EXTI_GetITStatus(EXTI_Line8) == SET)
    {   
        if(GPIO_ReadOutputDataBit(GPIOB,GetDistance) == SET)
        {
            TIM_Counter_Us = TIM_GetCounter(TIM6);
            TIM_Cmd(TIM6,ENABLE);
        }
        else
        {
            TIM_Cmd(TIM6,DISABLE);
            TIM_Counter_Us = TIM_GetCounter(TIM6);
            TIM_SetCounter(TIM6,0);
            Ultrasonic_Flag = 1;
        }
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}
