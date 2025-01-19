/*
USART1 ---> VOICE (GPIOA)
USART3 ---> BlueTooth(GPIOB)
语音通讯的优先级大于蓝牙
 */
#include "Serial.h"

Receive_State Voice_RxFlag = NotFinish;//语音模块是否接收完成
Receive_State BlueTooth_RxFlag = NotFinish;//蓝牙模块是否接收完成

Serial_State Voice_State = State_Wait_Head;//语音通讯的状态
Serial_State BlueTooth_State = State_Wait_Head;//蓝牙通讯的状态

uint8_t Voice_RxDataPack[3];//语音接收到的数据包
uint8_t BlueTooth_RxDataPack[3];//蓝牙接收到的数据包

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

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitTypeDef NVIC_Voice_Init_Struct;
    NVIC_Voice_Init_Struct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_Voice_Init_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Voice_Init_Struct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_Voice_Init_Struct.NVIC_IRQChannelSubPriority = 0;
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
    NVIC_BlueTooth_Init_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_BlueTooth_Init_Struct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_BlueTooth_Init_Struct);

    USART_Cmd(USART3,ENABLE);
}

/*语音通讯中断***************************************************************/
void USART1_IRQHandler(void)
{
    uint8_t Voice_RxData = 0x00;//当前接收到的数据
    static uint8_t Voice_pRxData = 0;//数据接收位数
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET && Voice_RxFlag != NotFinish)
    {   
        Voice_RxData = USART_ReceiveData(USART1);
        switch(Voice_State)
        {
            case State_Wait_Head:
                if( Voice_RxData == 0xFF)
                {
                    Voice_State = State_Receive_Data;
                }
                else
                {
                    Voice_State = State_Wait_Head;
                }
            break;

            case State_Receive_Data:
                Voice_RxDataPack[Voice_pRxData] = Voice_RxData;
                if( ++Voice_pRxData == 3)
                {
                    Voice_pRxData = 0;
                    Voice_State = State_Waite_End;
                }
            break;

            case State_Waite_End:
                if( Voice_RxData == 0xFE)
                {
                    Voice_State = State_Wait_Head;
                    Voice_RxFlag = Finish;
                }
                else
                {
                    Voice_State = State_Waite_End;
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
    uint8_t BlueTooth_RxData = 0x00;//当前接收到的数据
    static uint8_t BlueTooth_pRxData = 0;//数据接收位数
    if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET && BlueTooth_RxFlag == NotFinish)
    {
        BlueTooth_RxData = USART_ReceiveData(USART3);
        switch(BlueTooth_State)
        {
            case State_Wait_Head:
                if( BlueTooth_RxData == 0xFF)
                {
                    BlueTooth_State = State_Receive_Data;
                }
                else
                {
                    BlueTooth_State = State_Wait_Head;
                }
            break;

            case State_Receive_Data:
                BlueTooth_RxDataPack[BlueTooth_pRxData] = BlueTooth_RxData;
                if( ++BlueTooth_pRxData == 3)
                {
                    BlueTooth_pRxData = 0;
                    BlueTooth_State = State_Waite_End;
                }
            break;

            case State_Waite_End:
                if( BlueTooth_RxData == 0xFE)
                {
                    BlueTooth_State = State_Wait_Head;
                    BlueTooth_RxFlag = Finish;
                }
                else
                {
                    BlueTooth_State = State_Waite_End;
                }
            break;
        }
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    }
}
/***************************************************************蓝牙通讯中断*/
