#ifndef __Action_h
#define __Action_h

#include "stm32f10x.h"
#include "Servo.h"

#define MaxDelayTasks   10

typedef enum
{
    isActive,
    notActive
}ActiveState;//延时任务激活状态

typedef enum
{
    delayNotFinish,
    delayFinish
}delayStatus;//延时任务完成与否

typedef struct
{
    uint16_t StartTime;//延时任务开始时间
    uint16_t Duration;//延时时间
    ActiveState ActiveFlag;//激活标志位
}DelayTask;//延时任务结构体

extern uint32_t System_Tick;//当前系统时间
extern DelayTask delay[];//延时任务数组

void CreateDelayTask(uint8_t TaskIndex,uint16_t xms);
void Def_TaskState(DelayTask *Task);
void Action_Init(void);


#endif
