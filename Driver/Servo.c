#include "Servo.h"

/**
 * @brief 初始化所有舵机
 * 
 */
void Servo_Init(void)
{
    PWM_Init();
    Set_LFLeg(0);
    Set_LRLeg(0);
    Set_HLLeg(0);
    Set_HRLeg(0);
}

/**
 * @brief 设置左前脚
 * 
 * @param Angle 舵机角度
 */
void Set_LFLeg(float Angle)
{
    Set_TIM2CCR1(Angle / 180 * 2000 + 500);
}

/**
 * @brief 设置右前脚
 * 
 * @param Angle 舵机角度
 */
void Set_LRLeg(float Angle)
{
    Set_TIM2CCR2((180 - Angle) / 180 * 2000 + 500);
}

/**
 * @brief 设置左后脚
 * 
 * @param Angle 舵机角度
 */
void Set_HLLeg(float Angle)
{
    Set_TIM2CCR3(Angle / 180 * 2000 + 500);
}

/**
 * @brief 设置右后脚
 * 
 * @param Angle 舵机角度
 */
void Set_HRLeg(float Angle)
{
    Set_TIM2CCR4((180 - Angle) / 180 * 2000 + 500);
}

/**
 * @brief 设置尾巴摇动
 * 
 * @param Angle 角度
 */
void Ser_Tail(float Angle)
{
    Set_TIM3CCR(Angle / 180 * 2000 + 500);
}
