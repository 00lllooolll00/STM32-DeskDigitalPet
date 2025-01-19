#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "Servo.h"

int main(void)
{
	OLED_Init();
	Servo_Init();

	while(1)
	{
		
	}
	
}
