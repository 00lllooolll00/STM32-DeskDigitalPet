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
}Serial_State;

typedef enum
{
    NotFinish,
    Finish
}Receive_State;

extern Receive_State Voice_RxFlag ;//语音模块是否接收完成
extern Receive_State BlueTooth_RxFlag ;//蓝牙模块是否接收完成
 
extern uint8_t Voice_RxDataPack[];//语音接收到的数据包
extern uint8_t BlueTooth_RxDataPack[];//蓝牙接收到的数据包

void Voice_Init(void);
void BlueTooth_Init(void);

#endif
