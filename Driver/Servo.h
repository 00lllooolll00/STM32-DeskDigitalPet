#ifndef __Servo_h
#define __Servo_h

#include "TIM.h"

void Servo_Init(void);
void Set_LFLeg(float Angle);
void Set_LRLeg(float Angle);
void Set_HLLeg(float Angle);
void Set_HRLeg(float Angle);
void Ser_Tail(float Angle);


#endif
