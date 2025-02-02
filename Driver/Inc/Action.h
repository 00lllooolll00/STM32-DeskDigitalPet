#ifndef __Action_h
#define __Action_h

#include "stm32f10x.h"
#include "Servo.h"
#include "Serial.h"

#define MaxDelayTasks   20

typedef enum
{
    nProactive_Change,
    Proactive_Change
}ProChange;//主动进入模式更改

typedef enum
{
    notActive,
    isActive
    
}ActiveState;//延时任务激活状态

typedef enum
{
    delayNotFinish,
    delayFinish
}delayStatus;//延时任务完成与否

typedef enum
{
    Leg_State_1,
    Leg_State_2
}JumpLeg;//跳跃时腿部交换

typedef enum
{
    InHello,
    NotInHello
}HelloState;//是否在hello中

typedef struct
{
    uint64_t StartTime;//延时任务开始时间
    uint16_t Duration;//延时时间
    ActiveState ActiveFlag;//激活标志位
}DelayTask;//延时任务结构体


extern __IO uint64_t System_Tick;//当前系统时间
extern DelayTask delay[];//延时任务数组
extern uint16_t MoveSpeed;//运动延时
extern ProChange Change_Flag;//主动进入模式变换标志位

void CreateDelayTask(uint8_t TaskIndex,uint16_t xms);
void Def_TaskState(uint8_t Index);
void Action_Init(void);
void Action_StandUp(void);
void Action_SitDown(void);
void Action_LieDown(void);
void Action_Forward(void);
void Action_Backward(void);
void Action_TurnLeft(void);
void Action_TurnRight(void);
void Action_Swing(void);
void Action_TailWag(void);
void Action_JumpForward(void);
void Action_JumpBackward(void);
void Action_SayHello(void);

#endif
