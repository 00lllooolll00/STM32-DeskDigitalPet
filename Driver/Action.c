#include "Action.h"

DelayTask delay[MaxDelayTasks];//延时任务数组
__IO uint64_t System_Tick = 0;//当前系统时间
uint16_t MoveSpeed = 200;//运动延时
HelloState HelloFlag = NotInHello;//Hello标志位
JumpLeg JumpForward_F = Leg_State_1;//向前跳是否需要交换腿
JumpLeg JumpBackward_F = Leg_State_1;//向后跳是否需要交换腿
ProChange Change_Flag = nProactive_Change;//主动进入模式变换标志位

/**
 * @brief 创建一个延时任务
 * 
 * @param TaskIndex 任务索引 取值范围0——13
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
 * @param Index 延时任务索引 0——13
 * @return delayStatus 执行状态
 */
delayStatus Is_DelayDone(uint8_t Index)
{
    if(delay[Index].ActiveFlag == isActive)
    {
        return delayNotFinish;
    }
    else
    {
        return delayFinish;
    } 
}

/**
 * @brief 判断当前延时任务是否结束
 * 
 * @param Index 延时任务索引 0——13
 */
void Def_TaskState(uint8_t Index)
{
    if(delay[Index].ActiveFlag == isActive && (System_Tick - delay[Index].StartTime) >= delay[Index].Duration)
    {
        delay[Index].ActiveFlag = notActive;
    }
}

/**
 * @brief 将所有延时任务清除
 * 
 */
void Clear_AllTask(void)
{
    for(uint8_t i = 0;i < MaxDelayTasks;i ++)
    {
        delay[i].StartTime = System_Tick;
        delay[i].Duration = 0xFFFF;
        delay[i].ActiveFlag = notActive;
    }
}

/**
 * @brief 动作初始化
 * 
 */
void Action_Init(void)
{
    Servo_Init();
    JumpForward_F = Leg_State_1;
    JumpBackward_F = Leg_State_1;
    WagFlag = Wag_Off;
}

/**
 * @brief 立正
 * 
 */
void Action_StandUp(void)
{
    HelloFlag = NotInHello;
    Set_FLLeg(90);
    Set_FRLeg(90);
    CreateDelayTask(0,150);
    while(Is_DelayDone(0) != delayFinish);

    Set_HLLeg(90);
    Set_HRLeg(90);

    if(WagFlag == Wag_On)
    {
        Action_TailWag();
    }
}

/**
 * @brief 坐下
 * 
 */
void Action_SitDown(void)
{
    HelloFlag = NotInHello;
    Set_FLLeg(90);
    Set_FRLeg(90);
    CreateDelayTask(1,150);
    while(Is_DelayDone(1) != delayFinish);

    Set_HLLeg(20);
    Set_HRLeg(20);

    if(WagFlag == Wag_On)
    {
        Action_TailWag();
    }

}

/**
 * @brief 趴下
 * 
 */
void Action_LieDown(void)
{
    HelloFlag = NotInHello;
    Set_FLLeg(20);
    Set_FRLeg(20);
    CreateDelayTask(2,150);
    while(Is_DelayDone(2) != delayFinish);

    Set_HLLeg(20);
    Set_HRLeg(20);

    if(WagFlag == Wag_On)
    {
        Action_TailWag();
    }
}

/**
 * @brief 前进
 * 
 */
void Action_Forward(void)
{
    HelloFlag = NotInHello;
    Set_FRLeg(45);
    Set_HLLeg(45);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FLLeg(135);
    Set_HRLeg(135);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FRLeg(90);
    Set_HLLeg(90);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FLLeg(90);
    Set_HRLeg(90);

    Set_FLLeg(45);
    Set_HRLeg(45);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FRLeg(135);
    Set_HLLeg(135);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FLLeg(90);
    Set_HRLeg(90);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FRLeg(90);
    Set_HLLeg(90);
}

/**
 * @brief 后退
 * 
 */
void Action_Backward(void)
{
    HelloFlag = NotInHello;
    Set_FRLeg(135);
    Set_HLLeg(135);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FLLeg(45);
    Set_HRLeg(45);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FRLeg(90);
    Set_HLLeg(90);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FLLeg(90);
    Set_HRLeg(90);

    Set_FLLeg(135);
    Set_HRLeg(135);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FRLeg(45);
    Set_HLLeg(45);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FLLeg(90);
    Set_HRLeg(90);
    CreateDelayTask(3,MoveSpeed);
    while(Is_DelayDone(3) != delayFinish);
    Set_FRLeg(90);
    Set_HLLeg(90);
}

/**
 * @brief 向左转
 * 
 */
void Action_TurnLeft(void)
{
    HelloFlag = NotInHello;
    Set_FRLeg(45);
    Set_HLLeg(135);
    CreateDelayTask(4,MoveSpeed);
    while(Is_DelayDone(4) != delayFinish);
    Set_FLLeg(45);
    Set_HRLeg(135);
    CreateDelayTask(4,MoveSpeed);
    while(Is_DelayDone(4) != delayFinish);
    Set_FRLeg(90);
    Set_HLLeg(90);
    CreateDelayTask(4,MoveSpeed);
    while(Is_DelayDone(4) != delayFinish);
    Set_FLLeg(90);
    Set_HRLeg(90);
}

/**
 * @brief 向右转
 * 
 */
void Action_TurnRight(void)
{
    HelloFlag = NotInHello;
    Set_FLLeg(45);
    Set_HRLeg(135);
    CreateDelayTask(4,MoveSpeed);
    while(Is_DelayDone(4) != delayFinish);
    Set_FRLeg(45);
    Set_HLLeg(135);
    CreateDelayTask(4,MoveSpeed);
    while(Is_DelayDone(4) != delayFinish);
    Set_FLLeg(90);
    Set_HRLeg(90);
    CreateDelayTask(4,MoveSpeed);
    while(Is_DelayDone(4) != delayFinish);
    Set_FRLeg(90);
    Set_HLLeg(90);
}

/**
 * @brief 摇摆
 * 
 */
void Action_Swing(void)
{
    HelloFlag = NotInHello;
    for(uint8_t i = 30; i < 150;i ++)
    {
        Set_FRLeg(i);
        Set_FLLeg(i);
        Set_HLLeg(i);
        Set_HRLeg(i);
        CreateDelayTask(5,6);
        while(Is_DelayDone(5) != delayFinish);
    }

    for(uint8_t i = 150; i > 30 ;i --)
    {
        Set_FRLeg(i);
        Set_FLLeg(i);
        Set_HLLeg(i);
        Set_HRLeg(i);
        CreateDelayTask(5,6);
        while(Is_DelayDone(5) != delayFinish);
    }
}

/**
 * @brief 摇尾巴
 * 
 */
void Action_TailWag(void)
{
    HelloFlag = NotInHello;
    CreateDelayTask(13,150);
    while(Is_DelayDone(13) != delayFinish);
    for(uint8_t i = 30; i < 150;i ++)
    {
        if(WagFlag == Wag_Off)
        {
           break;
        }
        Set_Tail(i);
        CreateDelayTask(6,6);
        while(Is_DelayDone(6) != delayFinish);
    }

    for(uint8_t i = 150; i > 30 ;i --)
    {
        if(WagFlag == Wag_Off)
        {
            break;
        }
        Set_Tail(i);
        CreateDelayTask(6,6);
        while(Is_DelayDone(6) != delayFinish);
    }
}

/**
 * @brief 向前跳
 * 
 */
void Action_JumpForward(void)
{
    HelloFlag = NotInHello;
    static uint8_t In_Times = 1;
    if(JumpForward_F == Leg_State_1)
    {
        Set_FLLeg(140);
        Set_HRLeg(35);
        CreateDelayTask(7,MoveSpeed);
        while(Is_DelayDone(7) != delayFinish);
        Set_FRLeg(140);
        Set_HLLeg(35);
        CreateDelayTask(8,MoveSpeed + 80);
        while(Is_DelayDone(8) != delayFinish);
        JumpForward_F = Leg_State_2;
    }
    else
    {
        Set_FRLeg(140);
        Set_HLLeg(35);
        CreateDelayTask(7,MoveSpeed);
        while(Is_DelayDone(7) != delayFinish);
        Set_FLLeg(140);
        Set_HRLeg(35);
        CreateDelayTask(8,MoveSpeed + 80);
        while(Is_DelayDone(8) != delayFinish);
        JumpForward_F = Leg_State_1;
        In_Times = 2;
    }
    Set_HLLeg(90);
    Set_HRLeg(90);
    CreateDelayTask(12,150);
    while(Is_DelayDone(12) != delayFinish);
    Set_FLLeg(90);
    Set_FRLeg(90);
    if(In_Times == 2)
    {
        In_Times = 1;
        for(uint8_t i = 0;i < 3;i ++)
        {
            RxDataPack[i] = Inst_StandUp[i];
            Change_Flag = Proactive_Change;
        }
    }
}

/**
 * @brief 向后跳
 * 
 */
void Action_JumpBackward(void)
{
    HelloFlag = NotInHello;
    static uint8_t In_Times = 1;
    if(JumpBackward_F == Leg_State_1)
    {   
        Set_HRLeg(35);
        Set_FLLeg(140);
        CreateDelayTask(9,MoveSpeed);
        while(Is_DelayDone(9) != delayFinish);
        Set_HLLeg(35);
        Set_FRLeg(140);
        CreateDelayTask(9,MoveSpeed);
        while(Is_DelayDone(9) != delayFinish);
        JumpBackward_F = Leg_State_2;
    }
    else
    {
        Set_HLLeg(35);
        Set_FRLeg(140);
        CreateDelayTask(9,MoveSpeed);
        while(Is_DelayDone(9) != delayFinish);
        Set_HRLeg(35);
        Set_FLLeg(140);
        CreateDelayTask(9,MoveSpeed);
        while(Is_DelayDone(9) != delayFinish);
        JumpBackward_F = Leg_State_1;
        In_Times = 2;
    }
    Set_HLLeg(90);
    Set_HRLeg(90);
    CreateDelayTask(12,150);
    while(Is_DelayDone(12) != delayFinish);
    Set_FLLeg(90);
    Set_FRLeg(90);
    if(In_Times == 2)
    {
        In_Times = 1;
        for(uint8_t i = 0;i < 3;i ++)
        {
            RxDataPack[i] = Inst_StandUp[i];
            Change_Flag = Proactive_Change;
        }
    }
}

/**
 * @brief 打招呼
 * 
 */
void Action_SayHello(void)
{
    if(HelloFlag == NotInHello)
    {
        Set_HLLeg(30);
        Set_HRLeg(45);
        CreateDelayTask(10,150);
        while(Is_DelayDone(10) != delayFinish);
        Set_FRLeg(90);
        HelloFlag = InHello;
    }
    for(int i = 0;i < 45;i ++)
    {
        Set_FLLeg(i);
        CreateDelayTask(11,10);
        while(Is_DelayDone(11) != delayFinish);
    }
    for(int i = 45;i > 0;i --)
    {
        Set_FLLeg(i);
        CreateDelayTask(11,10);
        while(Is_DelayDone(11) != delayFinish);
    }
}
