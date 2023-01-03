#include "wy_headfile.h"


u16 T1_rstnum = 0;
u16 T4_rstnum = 0;

/*****************************************************
*@Func:	PWM部分初始化 （使用端口PB5输出）
******
*@para:
PWM输出初始化
mode 频率区间选择，0为Hz级（1Hz-1000Hz），1为KHZ级（1KHZ-1000KHZ)
f 频率（Hz级与KHz级均为1-1000）
duty 占空比(0-100]
******
*@desc:
TIM3 PWM部分初始化
通用定时器为T2/T3/T4/T5
PWM频率计算公式（基于定时器溢出）：f = Tclk / ((rstnum+1)*(psc+1))
系统频率1分频时（TIM_CKD_DIV1），Tclk = 72M
使用端口PB5输出（可修改）

占空比原理： 比较值 = 重载值 * 占空比(duty/100) , 当定时器计数值小于比较值时，输出低电平
******************************************************/ 
void TIM1_PWM_Init(u8 mode, u16 f)
{  
	u16 rstnum = 65535;  	// 重载值
    u16 psc = 65535;		// 时钟的预分频数

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	 // 频率区间选择
    if(mode == 0)
    {
        rstnum = 10000/f -1;
        psc = 7199;
        // 实际调节区间1/6Hz - 5000Hz  
    }
    else if(mode == 1)
    {
        rstnum = 1000/f -1;
        psc = 71;
        // 实际调节区间1/6KHz - 1000KHz
    }
	T1_rstnum = rstnum;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//使能定时器1时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟   
 
   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形	GPIOA8/11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11; 				// TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// 初始化GPIO
 
   //初始化TIM1
	TIM_TimeBaseStructure.TIM_Period = T1_rstnum; 				// 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				// 设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 				// 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM1 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				// 选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	// 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// 输出极性:TIM输出比较极性高

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);              // 根据T指定的参数初始化外设TIM8 OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);              // 根据T指定的参数初始化外设TIM8 OC2
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);              // 根据T指定的参数初始化外设TIM8 OC3
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);              // 根据T指定的参数初始化外设TIM8 OC4
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);          // 使能TIM1在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);          // 使能TIM1在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);          // 使能TIM1在CCR2上的预装载寄存器
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);          // 使能TIM1在CCR2上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  										// 使能TIM1
	TIM_ARRPreloadConfig(TIM1, ENABLE); 							// 使能TIMx在ARR上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM1,ENABLE);        						// MOE 主输出使能,高级定时器必须开启这个
	
}



/*****************************************************
*@Func:	PWM部分初始化 （使用端口PB5输出）
******
*@para:
PWM输出初始化
mode 频率区间选择，0为Hz级（1Hz-1000Hz），1为KHZ级（1KHZ-1000KHZ)
f 频率（Hz级与KHz级均为1-1000）
duty 占空比(0-100]
******
*@desc:
TIM3 PWM部分初始化
通用定时器为T2/T3/T4/T5
PWM频率计算公式（基于定时器溢出）：f = Tclk / ((rstnum+1)*(psc+1))
系统频率1分频时（TIM_CKD_DIV1），Tclk = 72M
使用端口PB5输出（可修改）

占空比原理： 比较值 = 重载值 * 占空比(duty/100) , 当定时器计数值小于比较值时，输出低电平
******************************************************/ 
void TIM4_PWM_Init(u8 mode, u16 f)
{  
	u16 rstnum = 65535;  	// 重载值
    u16 psc = 65535;		// 时钟的预分频数

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	 // 频率区间选择
    if(mode == 0)
    {
        rstnum = 10000/f -1;
        psc = 7199;
        // 实际调节区间1/6Hz - 5000Hz  
    }
    else if(mode == 1)
    {
        rstnum = 1000/f -1;
        psc = 71;
        // 实际调节区间1/6KHz - 1000KHz
    }
	T4_rstnum = rstnum;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器1时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟   
 
   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形	PB6/PB7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 				// TIM4_CH1/2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// 初始化GPIO
 
   //初始化TIM1
	TIM_TimeBaseStructure.TIM_Period = T4_rstnum; 				// 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				// 设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				// 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel1/2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				// 选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	// 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// 输出极性:TIM输出比较极性高

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);              // 根据T指定的参数初始化外设TIM4 OC1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);              // 根据T指定的参数初始化外设TIM4 OC2

	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);          // 使能TIM4在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);          // 使能TIM4在CCR2上的预装载寄存器

	
	TIM_Cmd(TIM4, ENABLE);  										// 使能TIM4
	TIM_ARRPreloadConfig(TIM4, ENABLE); 							// 使能TIMx在ARR上的预装载寄存器
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);        						// MOE 主输出使能,高级定时器必须开启这个
	
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief        	定时器1的PWM通道设置占空比
//  @param     
//  @attention    	占空比(0-1)，如果传入的参数是-1则不进行设置
//  @return       
//  Sample usage:  	占空比原理： 比较值 = 重载值 * 占空比(duty/100) , 当定时器计数值小于比较值时，输出低电平         
//-------------------------------------------------------------------------------------------------------------------
void TIM1_PWM_SetDuty(float C1Duty,	float C2Duty, float C3Duty, float C4Duty)
{
	u16 compare_value = 0;  // 占空比的比较值
	if(C1Duty != -1)
	{
		C1Duty = 1 - C1Duty;
		if(C1Duty >= 0 && C1Duty <= 1)
		{
			compare_value = (u16)(C1Duty * (T1_rstnum + 1));
			TIM_SetCompare1(TIM1, compare_value);
		}
	}
	if(C2Duty != -1)
	{
		C2Duty = 1 - C2Duty;
		if(C2Duty >= 0 && C2Duty <= 1)
		{
			compare_value = (u16)(C2Duty * (T1_rstnum + 1));
			TIM_SetCompare2(TIM1, compare_value);
		}
	}
	if(C3Duty != -1)
	{
		C3Duty = 1 - C3Duty;
		if(C3Duty >= 0 && C3Duty <= 1)
		{
			compare_value = (u16)(C3Duty * (T1_rstnum + 1));
			TIM_SetCompare3(TIM1, compare_value);
		}
	}
	if(C4Duty != -1)
	{
		C4Duty = 1 - C4Duty;
		if(C4Duty >= 0 && C4Duty <= 1)
		{
			compare_value = (u16)(C4Duty * (T1_rstnum + 1));
			TIM_SetCompare4(TIM1, compare_value);
		}
	}
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief        	定时器1的PWM通道设置占空比
//  @param     
//  @attention    	占空比(0-1)，如果传入的参数是-1则不进行设置
//  @return       
//  Sample usage:  	占空比原理： 比较值 = 重载值 * 占空比(duty/100) , 当定时器计数值小于比较值时，输出低电平         
//-------------------------------------------------------------------------------------------------------------------
void TIM4_PWM_SetDuty(float C1Duty,	float C2Duty)
{
	u16 compare_value = 0;  // 占空比的比较值
	if(C1Duty != -1)
	{
		C1Duty = 1 - C1Duty;
		if(C1Duty >= 0 && C1Duty <= 1)
		{
			compare_value = (u16)(C1Duty * (T4_rstnum + 1));
			TIM_SetCompare1(TIM4, compare_value);
		}
	}
	if(C2Duty != -1)
	{
		C2Duty = 1 - C2Duty;
		if(C2Duty >= 0 && C2Duty <= 1)
		{
			compare_value = (u16)(C2Duty * (T4_rstnum + 1));
			TIM_SetCompare2(TIM4, compare_value);
		}
	}
}
