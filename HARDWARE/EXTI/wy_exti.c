#include "wy_headfile.h"

 
//--------------------------------------------------------------------------------------
//  @brief       外部中断初始化
//  @param     
//  @param     
//  @attention   PA6下降沿触发中断，优先级为0-0
//  Sample usage:           
//--------------------------------------------------------------------------------------
void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				// 定义IO口初始化结构体
   	EXTI_InitTypeDef EXTI_InitStructure;				// 外部中断初始化结构体
 	NVIC_InitTypeDef NVIC_InitStructure;				// 中断优先级初始化结构体

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	// 使能复用功能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);// IO口时钟使能

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// 初始化Beep的PB8口
	GPIO_SetBits(GPIOA, GPIO_Pin_6);					// 初始化为高电平

   //GPIOA6	中断线以及中断初始化配置 下降沿触发 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);// 把6号线中断给GPIOE
  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;// 中断线为6号线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;// 中断模式选择，可选触发中断和事件中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;// 上/下沿双触发
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			// 中断线5-9共用一个中断函数
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级0
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}


//--------------------------------------------------------------------------------------
//  @brief       外部中断5-9服务程序
//  @param     
//  @param     
//  @attention   PA6下降沿触发中断，优先级为0-0，   中断线5-9共用一个中断函数
//  Sample usage:           
//--------------------------------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line6); 		// 清除LINE6上的中断标志位  
}
 
