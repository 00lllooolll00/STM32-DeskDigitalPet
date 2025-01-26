#include "Led.h"

/**
 * @brief PC13LED初始化
 * 
 */
void Led_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef LED_Init;
	LED_Init.GPIO_Mode = GPIO_Mode_Out_PP;
	LED_Init.GPIO_Pin = LED_PC13;
	LED_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&LED_Init);

	GPIO_SetBits(GPIOC,LED_PC13);
}

/**
 * @brief 打开LED
 * 
 */
void Led_On(void)
{
    GPIO_ResetBits(GPIOC,LED_PC13);
}

/**
 * @brief 关闭LED
 * 
 */
void Led_Off(void)
{
    GPIO_SetBits(GPIOC,LED_PC13);
}
