#ifndef _WY_ULTRASONIC_H_
#define _WY_ULTRASONIC_H_
#include "sys.h"

#define TRIG PAout(0)

//extern uchar TrigFlag;
extern u32 UlTansferTime;          	// 采集到的高电平时间
extern u32 UlCurDistance;          	// 当前距离

void UlInit(void);
void TrigUl(void);					// 触发超声波发送距离数据
void UlGetDistance(void);			// 获取当前距离

#endif
