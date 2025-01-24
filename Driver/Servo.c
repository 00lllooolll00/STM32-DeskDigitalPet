#include "Servo.h"

/**
 * @brief 初始化所有舵机
 * 
 */
void Servo_Init(void)
{
    PWM_Init();
}

/**
 * @brief 设置左前脚
 * 
 * @param Angle 舵机角度
 */
void Set_FLLeg(float Angle)
{
    Set_TIM2CCR1(Angle / 180 * 2000 + 500);
}

/**
 * @brief 设置右前脚
 * 
 * @param Angle 舵机角度
 */
void Set_FRLeg(float Angle)
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
void Set_Tail(float Angle)
{
    Set_TIM3CCR(Angle / 180 * 2000 + 500);
}
