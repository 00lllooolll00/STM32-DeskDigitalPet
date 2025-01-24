/*
USART1 ---> VOICE (GPIOA)
USART3 ---> BlueTooth(GPIOB)
语音通讯的优先级大于蓝牙
 */
#include "Serial.h"

Receive_Flag Serial_RxFlag = NotFinish;//串口模块是否接收完成

Serial_State Receive_State = State_Wait_Head;//串口接收的状态

WagState WagFlag = Wag_Off;//摇尾巴标志位

ErrorType ErrorFlag = notError;//串口接收出错标志位
Timeout TimeOut_Flag = NotOverTime;//超时标志位

uint8_t RxDataPack[3];//串口接收到的数据包

const uint8_t Inst_WakeUp[3] = {0x00,0x00,0x00};//唤醒指令
const uint8_t Inst_Forward[3] = {0xAA,0xAB,0xAC};//前进指令
const uint8_t Inst_Backward[3] = {0xBA,0xBB,0xBC};//后退指令
const uint8_t Inst_TurnLeft[3] = {0xCA,0xCB,0xCC};//左转指令
const uint8_t Inst_TurnRight[3] = {0xDA,0xDB,0xDC};//右转指令
const uint8_t Inst_Swing[3] = {0xEA,0xEB,0xEC};//摇摆指令
const uint8_t Inst_LieDown[3] = {0xFA,0xFB,0xFC};//趴下指令
const uint8_t Inst_SitDown[3] = {0x0A,0x0B,0x0C};//蹲下指令
const uint8_t Inst_TailWag[3] = {0x1A,0x1B,0x1C};//摇尾巴指令
const uint8_t Inst_Sleep[3] = {0x2A,0x2B,0x2C};//睡眠指令
const uint8_t Inst_Woof[3] = {0x4A,0x4B,0x4C};//叫两声指令
const uint8_t Inst_StandUp[3] = {0x5A,0x5B,0x5C};//立正指令
const uint8_t Inst_JumpForward[3] = {0x6A,0x6B,0x6C};//向前跳指令
const uint8_t Inst_JumpBackward[3] = {0x7A,0x7B,0x7C};//向后跳指令
const uint8_t Inst_SpeedUp[3] = {0x8A,0x8B,0x8C};//加速
const uint8_t Inst_SpeedDown[3] = {0x9A,0x9B,0x9C};//减速
const uint8_t Inst_SayHello[3] = {0x1B,0x3B,0x5B};//打招呼

const uint8_t Inst_SleepDown[3] = {0xF1,0xF2,0xF3};//主动进入睡眠模式
const uint8_t Inst_SpeedMax[3] = {0xF4,0xF5,0xF6};//速度最大
const uint8_t Inst_SpeedMin[3] = {0xE1,0xE2,0xE3};//速度最小

const uint8_t *Inst_Lst[Instr_Num] = {
    Inst_WakeUp,Inst_Forward,Inst_Backward,
    Inst_TurnLeft,Inst_TurnRight,Inst_Swing,
    Inst_LieDown,Inst_SitDown,Inst_TailWag,
    Inst_Sleep,Inst_Woof, Inst_StandUp,
    Inst_JumpForward,Inst_JumpBackward,Inst_SpeedUp,
    Inst_SpeedDown,Inst_SayHello
};//所有指令集合

/**
 * @brief 初始化语音通讯模块
 * 
 */
void Voice_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIOA_Voice_Init_Sturct;
    GPIOA_Voice_Init_Sturct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIOA_Voice_Init_Sturct.GPIO_Pin = Voice_TXD;
    GPIOA_Voice_Init_Sturct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIOA_Voice_Init_Sturct);

    GPIOA_Voice_Init_Sturct.GPIO_Mode = GPIO_Mode_IPU;
    GPIOA_Voice_Init_Sturct.GPIO_Pin = Voice_RXD;
    GPIOA_Voice_Init_Sturct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIOA_Voice_Init_Sturct);

    USART_InitTypeDef USART1_Voice_Init_Struct;
    USART1_Voice_Init_Struct.USART_BaudRate = 9600;
    USART1_Voice_Init_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART1_Voice_Init_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART1_Voice_Init_Struct.USART_Parity = USART_Parity_No;
    USART1_Voice_Init_Struct.USART_StopBits = USART_StopBits_1;
    USART1_Voice_Init_Struct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1,&USART1_Voice_Init_Struct);

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_Voice_Init_Struct;
    NVIC_Voice_Init_Struct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_Voice_Init_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Voice_Init_Struct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_Voice_Init_Struct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_Voice_Init_Struct);

    USART_Cmd(USART1,ENABLE);
}

/**
 * @brief 初始化蓝牙通讯模块
 * 
 */
void BlueTooth_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

    GPIO_InitTypeDef GPIOB_BlueTooth_Init;
    GPIOB_BlueTooth_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIOB_BlueTooth_Init.GPIO_Pin = BlueTooth_TXD;
    GPIOB_BlueTooth_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIOB_BlueTooth_Init);

    GPIOB_BlueTooth_Init.GPIO_Mode = GPIO_Mode_IPU;
    GPIOB_BlueTooth_Init.GPIO_Pin = BlueTooth_RXD;
    GPIOB_BlueTooth_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIOB_BlueTooth_Init);

    USART_InitTypeDef USART3_BlueTooth_Init;
    USART3_BlueTooth_Init.USART_BaudRate = 9600;
    USART3_BlueTooth_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART3_BlueTooth_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART3_BlueTooth_Init.USART_Parity = USART_Parity_No;
    USART3_BlueTooth_Init.USART_StopBits = USART_StopBits_1;
    USART3_BlueTooth_Init.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART3,&USART3_BlueTooth_Init);

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);

    NVIC_InitTypeDef NVIC_BlueTooth_Init_Struct;
    NVIC_BlueTooth_Init_Struct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_BlueTooth_Init_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_BlueTooth_Init_Struct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_BlueTooth_Init_Struct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_BlueTooth_Init_Struct);

    USART_Cmd(USART3,ENABLE);
}

/**
 * @brief 初始化串口
 * 
 */
void Serial_Init(void)
{
    Voice_Init();
    BlueTooth_Init();
}

/*语音通讯中断***************************************************************/
void USART1_IRQHandler(void)
{
    static uint8_t Voice_pRxData = 0;//数据接收位数
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
    {   
        uint8_t Voice_RxData = USART_ReceiveData(USART1);
        switch(Receive_State)
        {
            case State_Wait_Head:
                if( Voice_RxData == 0xFF && Serial_RxFlag == NotFinish)
                {
                    Receive_State = State_Receive_Data;
                }
            break;

            case State_Receive_Data:
                RxDataPack[Voice_pRxData] = Voice_RxData;
                if( ++Voice_pRxData == 3)
                {
                    Voice_pRxData = 0;
                    if(RxDataPack[0] == Inst_TailWag[0] && RxDataPack[1] == Inst_TailWag[1] && RxDataPack[2] == Inst_TailWag[2])
                    {
                        (WagFlag == Wag_On)?(WagFlag =Wag_Off):(WagFlag = Wag_On);
                    }
                    Receive_State = State_Waite_End;
                }
            break;

            case State_Waite_End:
                if( Voice_RxData == 0xFE)
                {
                    Receive_State = State_Wait_Head;
                    Serial_RxFlag = Finish;
                    if(ErrorFlag == isError)
                    {
                        TimeOut_Flag = NotOverTime;
                        ErrorFlag = notError;
                        Receive_State = State_Wait_Head;
                    }
                }
                else
                {
                   ErrorFlag = isError;
                   if(TimeOut_Flag == OverTime)
                   {
                        TimeOut_Flag = NotOverTime;
                        ErrorFlag = notError;
                        Receive_State = State_Wait_Head;
                        Serial_RxFlag = NotFinish;
                        return;
                   }

                }
            break;
        }
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}
/***************************************************************语音通讯中断*/

/*蓝牙通讯中断***************************************************************/
void USART3_IRQHandler(void)
{
    static uint8_t BlueTooth_pRxData = 0;//数据接收位数
    if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
    {
        uint8_t BlueTooth_RxData = USART_ReceiveData(USART3);
        switch(Receive_State)
        {
            case State_Wait_Head:
                if( BlueTooth_RxData == 0xFF && Serial_RxFlag == NotFinish)
                {
                    Receive_State = State_Receive_Data;
                }
            break;

            case State_Receive_Data:
                RxDataPack[BlueTooth_pRxData] = BlueTooth_RxData;
                if( ++BlueTooth_pRxData == 3)
                {
                    BlueTooth_pRxData = 0;
                    if(RxDataPack[0] == Inst_TailWag[0] && RxDataPack[1] == Inst_TailWag[1] && RxDataPack[2] == Inst_TailWag[2])
                    {
                        (WagFlag == Wag_On)?(WagFlag =Wag_Off):(WagFlag = Wag_On);
                    }
                    Receive_State = State_Waite_End;
                }
            break;

            case State_Waite_End:
                if( BlueTooth_RxData == 0xFE)
                {
                    Receive_State = State_Wait_Head;
                    Serial_RxFlag = Finish;
                    if(ErrorFlag == isError)
                    {
                        TimeOut_Flag = NotOverTime;
                        ErrorFlag = notError;
                        Receive_State = State_Wait_Head;
                    }
                }
                else
                {
                   ErrorFlag = isError;
                   if(TimeOut_Flag == OverTime)
                   {
                        TimeOut_Flag = NotOverTime;
                        ErrorFlag = notError;
                        Receive_State = State_Wait_Head;
                        Serial_RxFlag = NotFinish;
                        return;
                   }
                }
            break;
        }
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    }
}
/***************************************************************蓝牙通讯中断*/

/**
 * @brief 根据接收到的数据判断当前指令
 * 
 * @return Inst 返回指令
 */
Inst Def_ActMode(void)
{
    for(uint8_t j = 0;j < Instr_Num;j ++)
    {
        for(uint8_t i = 0;i < 3;i ++)
        {
            if(RxDataPack[i] != Inst_Lst[j][i])
            {
                break;
            }
            else
            {
                return (Inst)j;   
            }
        }
        
    }
    return (Inst)17;
}

/**
 * @brief 发送一个字节给语音模块
 * 
 * @param Byte 字节数据
 */
void Voice_SendByte(uint8_t Byte)
{
    USART_SendData(USART1,Byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) != SET);
}

/**
 * @brief 发送一个数据包给语音模块
 * 
 * @param DataPack 数据包数组
 */
void Voice_SendDataPack(const uint8_t *DataPack)
{
    Voice_SendByte(0xAA);
    Voice_SendByte(0x55);
    for(uint8_t i = 0;i < 3;i ++)
    {
        Voice_SendByte(DataPack[i]);
    }
    Voice_SendByte(0x55);
    Voice_SendByte(0xAA);
}
