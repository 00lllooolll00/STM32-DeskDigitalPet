#ifndef __Servo_h
#define __Servo_h

#include "PWM.h"

void Servo_Init(void);
void Set_FLLeg(float Angle);
void Set_FRLeg(float Angle);
void Set_HLLeg(float Angle);
void Set_HRLeg(float Angle);
void Set_Tail(float Angle);


#endif
