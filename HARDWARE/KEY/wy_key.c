#include "wy_headfile.h"

/*****************************************************
@ Func:	按键初始化
******
@ para: 
******
@ desc:	KEY0: PE3   KEY1:PE4   KEY_UP:PA0
******************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef Key0_InitStructure;// 按键0的IO口初始化结构体
    GPIO_InitTypeDef Key1_InitStructure;// 按键1的IO口初始化结构体
    GPIO_InitTypeDef KeyUp_InitStructure;// 按键up的IO口初始化结构体
    // Key的硬件端口时钟初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOA, ENABLE);
    // Key的IO口初始化
    Key0_InitStructure.GPIO_Pin = GPIO_Pin_4;
    Key0_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    Key0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    Key1_InitStructure.GPIO_Pin = GPIO_Pin_3;
    Key1_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    Key1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    KeyUp_InitStructure.GPIO_Pin = GPIO_Pin_0;
    KeyUp_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    KeyUp_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &Key0_InitStructure);
    GPIO_Init(GPIOE, &Key1_InitStructure);
    GPIO_Init(GPIOA, &KeyUp_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}



key KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEYUP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY_R==0)return right;
		else if(KEY_D==0)return down;
		else if(KEY_U==1)return up;
	}else if(KEY0==1&&KEY1==1&&KEYUP==0)key_up=1; 	    
 	return undo;// 无按键按下
}




