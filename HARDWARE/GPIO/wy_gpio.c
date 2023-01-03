#include "wy_headfile.h"

/*****************************************************
*@Func:	初始化GPIO口
******
*@para:
******
*@desc: 这里PA4口用于测试程序运行的时间，搭配TIMEPORT使用
eg:
		TIMEPORT = 1;
		delay_us(10); // 这里放你要测试的程序，运行时间精确到1us(一般打印的时间要比此段程序运行的真实时间增加2us)
		TIMEPORT = 0;
		tempI = GetCaptureTime();
		LCD_ShowInt(156,110,200,16,16,tempI);//显示ADC的值
******************************************************/	
void Gpio_Init(void)
{
	GPIO_InitTypeDef PA4_InitStructure;
	
	// 对所使用到的外设时钟进行使能（一定要先使能时钟再对端口进行初始化）
	// IO口使用的是APB2分频器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 使能APB2总线上GPIOA的时钟。
	PA4_InitStructure.GPIO_Pin = GPIO_Pin_4;				// 测试程序运行时间-->PA4 端口配置
	PA4_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 设置IO口速度为50MHz
	PA4_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// 设置为推挽模式
	GPIO_Init(GPIOA, &PA4_InitStructure);					// 对PA4口进行初始化
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);						// 对PA4输出低
}
