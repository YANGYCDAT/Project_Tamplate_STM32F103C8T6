#include "wy_headfile.h"


/*****************************************************
*@Func:	初始化LED
******
*@para: LED0:PA11  LED1:PA12
******
*@desc: 
******************************************************/	
void LED_Init(void)
{
	GPIO_InitTypeDef LED0_InitStructure;// 定义PA11的GPIO口设置结构体
	GPIO_InitTypeDef LED1_InitStructure;// 定义PA12的GPIO口设置结构体
	
	// 对所使用到的外设时钟进行使能（一定要先使能时钟再对端口进行初始化）
	// IO口使用的是APB2分频器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);// 使能APB2总线上的GPIOA的时钟。
	LED0_InitStructure.GPIO_Pin = GPIO_Pin_11;				// LED0-->PA11 端口配置
	LED0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 设置IO口速度为50MHz
	LED0_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// 设置为推挽模式
	GPIO_Init(GPIOA, &LED0_InitStructure);					// 对PA11口进行初始化
	GPIO_SetBits(GPIOA,GPIO_Pin_11);						// PA11输出高
	
	LED1_InitStructure.GPIO_Pin = GPIO_Pin_12;				// LED1-->PA12 端口配置
	LED1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 设置IO口速度为50MHz
	LED1_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// 设置为推挽模式
	GPIO_Init(GPIOA, &LED1_InitStructure);					// 对PA12口进行初始化
	GPIO_SetBits(GPIOA,GPIO_Pin_12); 						// PA12 输出高 

}

