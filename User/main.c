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
	Inst NowState = Def_ActMode();

	if(NowState == TailWag)
	{
		(WagFlag == Wag_On)?(WagFlag = Wag_Off):(WagFlag = Wag_On);
	}

	switch(NowState)
		{
			case WakeUp:

				Emoji_Turn(Emoji_Normal);
			break;

			case Forward:
				Action_Forward();
				Emoji_Turn(Emoji_Normal);				
			break;

			case BackWard:
				Action_Backward();
				Emoji_Turn(Emoji_Normal);				
			break;

			case TurnLeft:
				Action_TurnLeft();
				Emoji_Turn(Emoji_Cute);				
			break;
				
			case TurnRight:
				Action_TurnRight();
				Emoji_Turn(Emoji_Cute);				
			break;

			case Swing:
				Action_Swing();
				Emoji_Turn(Emoji_Laugh);
			break;

			case LieDown:
				Action_LieDown();
				Emoji_Turn(Emoji_Happy);
			break;

			case SitDown:
				Action_SitDown();
				Emoji_Turn(Emoji_Happy);
			break;

			case Sleep:
				Action_LieDown();
				Emoji_Turn(Emoji_Sleep);
			break;

			case Woof:
			
				Emoji_Turn(Emoji_Angry);
			break;

			case StandUp:
				Action_StandUp();
				Emoji_Turn(Emoji_Normal);
			break;

			case JumpForward:
				Action_JumpForward();
				Emoji_Turn(Emoji_Grinning);				
			break;

			case JumpBackward:
				Action_JumpBackward();
				Emoji_Turn(Emoji_Grinning);				
			break;

			case SpeedUp:
				if(MoveSpeed >= 100)
				{
					MoveSpeed -= 20;
				}
				else
				{
					Voice_SendDataPack(Inst_SpeedMax);
				}
				Emoji_Turn(Emoji_Grinning);				
			break;

			case SpeedDown:
				if(MoveSpeed <= 200)
				{
					MoveSpeed += 20;
				}
				else
				{
					Voice_SendDataPack(Inst_SpeedMin);
				}
				Emoji_Turn(Emoji_Grinning);				
			break;

			case SayHello:
				Action_SayHello();
				Emoji_Turn(Emoji_Cute);				
			break;	

			default:
			break;
		}
}
/*主函数******************************************/
int main(void)
{
	Emoji_Init();
	Serial_Init();
	TIM4_Init();
	Action_Init();

	while(1)
	{
		if(Serial_RxFlag == Finish)
		{
			ActionConfig();
			Serial_RxFlag = NotFinish;
		}
		else
		{
			if(ActC_Slow)continue;
			ActC_Slow = 1;
			ActionConfig();
		}


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
			Def_TaskState(i);
		}

		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
/************************************TIM4 1ms中断*/
