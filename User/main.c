#include "stm32f10x.h"                  // Device header
#include "Action.h"
#include "Emoji.h"
#include "Serial.h"
#include "TIM.h"

uint8_t ActC_Slow;//动作配置函数减速
uint16_t inSleep_Counter = 0;//进入睡眠计数
uint16_t Error_Counter = 0;//串口超时计数


/**
 * @brief 动作配置函数
 * 
 */
void ActionConfig(void)
{
	if(ActC_Slow)return;
	ActC_Slow = 1;

	if(Serial_RxFlag == Finish)
	{
		Inst NowState = Def_ActMode();
		switch(NowState)
		{
			case WakeUp:

				Emoji_Turn(Emoji_Normal);
			break;

			case Forward:

				Emoji_Turn(Emoji_Normal);				
			break;

			case BackWard:

				Emoji_Turn(Emoji_Normal);				
			break;

			case TurnLeft:
				Emoji_Turn(Emoji_Cute);				
			break;
				
			case TurnRight:

				Emoji_Turn(Emoji_Cute);				
			break;

			case Swing:

				Emoji_Turn(Emoji_Laugh);
			break;

			case LieDown:

				Emoji_Turn(Emoji_Happy);
			break;

			case SitDown:

				Emoji_Turn(Emoji_Happy);
			break;

			case TailWag:

				Emoji_Turn(Emoji_Kiss);
			break;

			case Sleep:

				Emoji_Turn(Emoji_Sleep);
			break;

			case SwingFast:
				Emoji_Turn(Emoji_Happy);
			break;

			case Woof:
			
				Emoji_Turn(Emoji_Angry);
			break;

			case StandUp:

				Emoji_Turn(Emoji_Normal);
			break;

			case JumpForward:

				Emoji_Turn(Emoji_Grinning);				
			break;

			case JumpBackward:

				Emoji_Turn(Emoji_Grinning);				
			break;

			default:
			break;
		}
	}
	Serial_RxFlag = NotFinish;
}

/*主函数******************************************/
int main(void)
{
	Emoji_Init();
	Servo_Init();
	Voice_Init();
	BlueTooth_Init();
	TIM4_Init();
	
	while(1)
	{
		ActionConfig();

		if(inSleep_Counter >= 60000)//1分钟没有收到消息则自动进入睡眠模式
		{
			inSleep_Counter = 0;//计数器清空
			Emoji_Turn(Emoji_Sleep);//切换表情
			Voice_SendDataPack(Inst_SleepDown);//向语音模块发送数据包告诉进入睡眠模式
			__WFI();
		}
	}
}
/******************************************主函数*/

/*TIM4 1ms中断************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		/*程序减速*/
		if( ++ActC_Slow == 10)ActC_Slow = 0;

		/*睡眠模式进入*/
		if(Serial_RxFlag == NotFinish)//如果没接收到串口指令则开始计时
		{
			if( ++inSleep_Counter == 65534)
			{
				inSleep_Counter = 0;
			}
		}
		else
		{
			inSleep_Counter = 0;
		}

		/*串口接收超时*/
		if(ErrorFlag == isError)
		{
			if( ++Error_Counter == 500)
			{
				TimeOut_Flag = OverTime;
			}
		}
		else
		{
			Error_Counter = 0;
		}

		/*舵机*/
		System_Tick ++;//系统时间++
		for(uint8_t i = 0;i < MaxDelayTasks;i ++)//判断当前延时任务是否结束
		{
			Def_TaskState(&delay[i]);
		}
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
/************************************TIM4 1ms中断*/
