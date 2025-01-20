#include "Action.h"

DelayTask delay[MaxDelayTasks];//延时任务数组
uint32_t System_Tick = 0;//当前系统时间

/**
 * @brief 创建一个延时任务
 * 
 * @param TaskIndex 任务索引 取值范围0——10
 * @param xms 延时时间(ms)
 */
void CreateDelayTask(uint8_t TaskIndex,uint16_t xms)
{
    delay[TaskIndex].StartTime = System_Tick;
    delay[TaskIndex].Duration = xms;
    delay[TaskIndex].ActiveFlag = isActive;
}

/**
 * @brief 返回当前任务的执行状态
 * 
 * @param Task 延时任务 delay[0——10]
 * @return delayStatus 执行状态
 */
delayStatus Is_DelayDone(DelayTask *Task)
{
    if(Task->ActiveFlag == isActive)
    {
        return delayFinish;
    }
    else
    {
        return delayNotFinish;
    } 
}

/**
 * @brief 判断当前延时任务是否结束
 * 
 * @param Task 延时任务 delay[0——10]
 */
void Def_TaskState(DelayTask *Task)
{
    if(Task->ActiveFlag == isActive && (System_Tick - Task->StartTime) >= Task->Duration)
    {
        Task->ActiveFlag = notActive;
    }
}

/**
 * @brief 动作初始化
 * 
 */
void Action_Init(void)
{
    Servo_Init();
}

