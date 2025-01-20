#ifndef __Serial_h
#define __Serial_h

#include "stm32f10x.h"

#define Voice_TXD           GPIO_Pin_9
#define Voice_RXD           GPIO_Pin_10
#define BlueTooth_TXD       GPIO_Pin_10
#define BlueTooth_RXD       GPIO_Pin_11

typedef enum 
{
    State_Wait_Head,
    State_Receive_Data,
    State_Waite_End
}Serial_State;//串口接收状态

typedef enum
{
    NotFinish,
    Finish
}Receive_Flag;//串口接收完成标志位

typedef enum
{
    WakeUp,
    Forward,
    BackWard,
    TurnLeft,
    TurnRight,
    Swing,
    LieDown,
    SitDown,
    TailWag,
    Sleep,
    SwingFast,
    Woof,
    StandUp,
    JumpForward,
    JumpBackward,
    SleepDown
}Inst;//动作指令

typedef enum
{
    OverTime,
    NotOverTime
}Timeout;//串口接收是否超时

typedef enum
{
    isError,
    notError
}ErrorType;//串口是否接收到错误数据

extern Receive_Flag Serial_RxFlag ;//串口模块是否接收完成
 
extern uint8_t RxDataPack[];//串口接收到的数据包

extern const uint8_t Inst_SleepDown[];//主动进入睡眠模式

extern ErrorType ErrorFlag;//串口接收出错标志位
extern Timeout TimeOut_Flag;//超时标志位


void Voice_Init(void);
void BlueTooth_Init(void);
Inst Def_ActMode(void);
void Voice_SendDataPack(const uint8_t *DataPack);

#endif
