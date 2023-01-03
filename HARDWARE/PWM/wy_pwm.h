#ifndef __WY_PWM_H
#define __WY_PWM_H
#include "sys.h" 

extern u16 T1_rstnum;
extern u16 T4_rstnum;

void TIM1_PWM_Init(u8 mode, u16 f);
void TIM4_PWM_Init(u8 mode, u16 f);
void TIM1_PWM_SetDuty(float C1Duty,	float C2Duty, float C3Duty, float C4Duty);
void TIM4_PWM_SetDuty(float C1Duty,	float C2Duty);

#endif
