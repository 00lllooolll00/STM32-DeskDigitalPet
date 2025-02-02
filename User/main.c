#include "stm32f10x.h"                  // Device header
#include "Action.h"
#include "Emoji.h"
#include "Serial.h"
#include "TIM.h"
#include "Ultrasonic.h"
#include "Led.h"
#include "Delay.h"

uint32_t inSleep_Counter = 0;//进入睡眠计数
uint16_t Error_Counter = 0;//串口超时计数

/**
 * @brief 动作配置函数
 * 
 */
void ActionConfig(void)
{
	static Inst NowState = Sleep;
	static Inst LastState;
	if(Serial_RxFlag == Finish || Change_Flag == Proactive_Change)
	{
		if(NowState != SpeedDown && NowState != SpeedUp && NowState != TailWag)
		{
			LastState = NowState;
		}
		NowState = Def_ActMode();
		Change_Flag = nProactive_Change;
		Serial_RxFlag = NotFinish;
	}
	switch(NowState)
		{
			case TailWag:
				(WagFlag == Wag_On)?(WagFlag = Wag_Off):(WagFlag = Wag_On);
				if(WagFlag == Wag_On)
				{
					Led_On();
				}
				else
				{
					Led_Off();
				}
				Action_TailWag();
				NowState = LastState;
			break;

			case Forward:
				// if(Get_Distance() <= 3)//检测前方3cm是否有障碍物，有则后退
				// {
				// 	Emoji_Turn(Emoji_Scared);
				// 	Action_Backward();
				// }
				// else
				// {
				// 	Emoji_Turn(Emoji_Normal);
				// 	Action_Forward();			
				// }
								
				Emoji_Turn(Emoji_Normal);
				Action_Forward();			
			break;

			case BackWard:
				Emoji_Turn(Emoji_Normal);
				Action_Backward();
			break;

			case TurnLeft:
				Emoji_Turn(Emoji_Cute);				
				Action_TurnLeft();
			break;
				
			case TurnRight:
				Emoji_Turn(Emoji_Cute);				
				Action_TurnRight();
			break;

			case Swing:
				Emoji_Turn(Emoji_Laugh);
				Action_Swing();
			break;

			case LieDown:
				Emoji_Turn(Emoji_Happy);
				Action_LieDown();
			break;

			case SitDown:
				Emoji_Turn(Emoji_Happy);
				Action_SitDown();
			break;

			case Sleep:
				Emoji_Turn(Emoji_Sleep);
				Action_LieDown();
			break;

			case Woof:
			
				Emoji_Turn(Emoji_Angry);
			break;

			case StandUp:
				Emoji_Turn(Emoji_Normal);
				Action_StandUp();
			break;

			case JumpForward:
				Emoji_Turn(Emoji_Grinning);				
				Action_JumpForward();
			break;

			case JumpBackward:
				Emoji_Turn(Emoji_Grinning);				
				Action_JumpBackward();
			break;

			case SpeedUp:
				if(MoveSpeed >= 140)
				{
					MoveSpeed -= 20;
				}
				else
				{
					Voice_SendDataPack(Inst_SpeedMax);
				}
				NowState = LastState;
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
				NowState = LastState;
			break;

			case SayHello:
				Emoji_Turn(Emoji_Cute);				
				Action_SayHello();
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
	Led_Init();
	TIM4_Init();
	Ultrasonic_Init();
	Action_Init();

	while(1)
	{
		ActionConfig();
		if(inSleep_Counter >= 60000 * 3)//3分钟没有收到消息则自动进入睡眠模式
		{
			inSleep_Counter = 0;//计数器清空
			Emoji_Turn(Emoji_Sleep);//切换表情
			Action_LieDown();
			Voice_SendDataPack(Inst_SleepDown);//向语音模块发送数据包告诉进入睡眠模式
			Delay_ms(2000);
			OLED_Clear();
			OLED_Update();

			TIM_Cmd(TIM4,DISABLE);//关闭TIM4避免中断打断睡眠
			TIM_Cmd(TIM3,DISABLE);
			TIM_Cmd(TIM2,DISABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4,DISABLE);//关闭所有时钟
			__WFI();//进入睡眠模式
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4,ENABLE);//打开所有时钟
			TIM_Cmd(TIM4,ENABLE);//退出睡眠模式先打开TIM4
			TIM_Cmd(TIM3,ENABLE);
			TIM_Cmd(TIM2,ENABLE);

		}
	}
}
/******************************************主函数*/

/*TIM4 1ms中断************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		/*睡眠模式进入*/
		if(Serial_RxFlag == NotFinish)//如果没接收到串口指令则开始计时
		{
			if( ++inSleep_Counter == 60000 * 4)
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
