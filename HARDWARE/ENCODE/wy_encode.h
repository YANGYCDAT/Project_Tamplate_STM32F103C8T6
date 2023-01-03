#ifndef __TIMER_H
#define __TIMER_H	
#include "sys.h"
#include "stm32f10x_tim.h"

#define ENCODERKEEPTIMES 3

extern float MotorASpeed;
extern int EncoderA;
extern float MotorBSpeed;
extern int EncoderB;
extern u16 DecodeGapTime;
	
void EncoderA_Init(void);
void Read_EncoderA(void);
void Get_MotorA_Speed(void);
void EncoderB_Init(void);
void Read_EncoderB(void);
void Get_MotorB_Speed(void);

#endif
