#include "wy_headfile.h"


//uchar TrigFlag = 1;						  	// 超声波传感器触发标志位
u32 UlTansferTime = 0;                   		// 采集到的高电平时间
u32 UlCurDistance = 0;                   		// 当前距离



//-------------------------------------------------------------------------------------------------------------------
//  @brief    	超声波引脚初始化
//  @param
//  @param
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void UlInit(void)
{
//	GPIO_InitTypeDef PA8_InitStructure;// 定义PA8的GPIO口设置结构体
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 使能APB2总线上GPIOA的时钟。
//	PA8_InitStructure.GPIO_Pin = GPIO_Pin_8;				// 测试程序运行时间-->PA8 端口配置
//	PA8_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 设置IO口速度为50MHz
//	PA8_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// 设置为推挽模式
//	GPIO_Init(GPIOA, &PA8_InitStructure);					// 对PA8口进行初始化
//	GPIO_ResetBits(GPIOA,GPIO_Pin_8);						// 对PA8输出低
	
	GPIO_InitTypeDef PA0_InitStructure;
	
	// 对所使用到的外设时钟进行使能（一定要先使能时钟再对端口进行初始化）
	// IO口使用的是APB2分频器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 使能APB2总线上GPIOA的时钟。
	PA0_InitStructure.GPIO_Pin = GPIO_Pin_0;				// 超声波Trig
	PA0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 设置IO口速度为50MHz
	PA0_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// 设置为推挽模式
	GPIO_Init(GPIOA, &PA0_InitStructure);					// 对PA0口进行初始化
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						// 对PA0输出低
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief    	超声波触发发送函数
//  @param
//  @param
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void TrigUl(void)
{
    TRIG = 0;
    TRIG = 1;							// 测距触发信号的发送
    delay_us(15);
    TRIG = 0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief    	超声波获取物距
//  @param
//  @param
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void UlGetDistance(void)
{
    TrigUl();										// 触发超声波传感器
	UlTansferTime = GetCaptureTime();				// 统计高电平时间		
    UlCurDistance = UlTansferTime * 1.7 / 100;  	// 转换为厘米
	delay_ms(60);
}
