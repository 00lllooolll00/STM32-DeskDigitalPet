#include "stm32f10x.h"                  // Device header
#include "Action.h"
#include "Emoji.h"
#include "Serial.h"
#include "TIM.h"

uint8_t ActC_Slow;//动作配置函数减速

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

	}
}
/******************************************主函数*/

/*TIM4 1ms中断************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		if( ++ActC_Slow == 10)ActC_Slow = 0;

		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
/************************************TIM4 1ms中断*/
