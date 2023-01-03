#ifndef __WY_KEY_H
#define __WY_KEY_H
#include "wy_headfile.h"

#define KEY0 PEin(4)	// 读取按键0
#define KEY1 PEin(3)	// 读取按键1
#define KEYUP PAin(0)	// 读取按键WK_UP

enum KEY   //按键操作的类型
{
	up = 1,  //4.0
	down,    //4.1
	left,    //4.2
	right,   //4.3
	ok,      //同时按下4.2和4.3
	undo,    //不按
};
typedef enum KEY key; 


void Key_Init(void);				// 按键初始化
key KEY_Scan(u8 mode);	// 扫描所有按键


#endif
