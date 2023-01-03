#include "wy_headfile.h"


/*****************************************************
*@Func:	定时器6中断服务程序
******
*@para:
******
*@desc:
******************************************************/
void TIM6_IRQHandler(void)   //TIM6中断
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM1更新(溢出）中断发生与否
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  	//清除TIMx更新中断标志
		
		
//		TIMEPORT = 1;
		
		
		/*这里放采样程序*/
		Get_MotorA_Speed();
		Get_MotorB_Speed();
		mpu_dmp_get_data(&pitch,&roll,&yaw);   		// 获取DMP解算后的欧拉角
		/*******4ms********/
		
		
		/*这里放控制程序*/
		delay_ms(5);
		/***************/

		
		
		
//		TIMEPORT = 0;
//		delay_us(10);
//		GapTime = GetCaptureTime();
    }
}


/*****************************************************
*@Func:	定时器7中断服务程序
******
*@para:
******
*@desc:	交互中断程序，放蓝牙和OLED，OLED只有在调参的时候才使用，使用的时候关闭控制中断
******************************************************/
void TIM7_IRQHandler(void)   //TIM6中断
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM1更新(溢出）中断发生与否
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  	//清除TIMx更新中断标志
		
		
//		TIMEPORT = 1;
		
		
		/*这里放蓝牙传上位机程序*/
		USART_SendToKeYou(USART3);
		/**********6.5ms************/
		
		
//		TIMEPORT = 0;
//		delay_us(10);
//		GapTime = GetCaptureTime();
    }
}



/*****************************************************
*@Func:	定时中断初始化
******
*@para:
timer	时钟号的枚举变量
ocp_nvc	抢占优先级
fow_nvc 次优先级
mode 定时区间选择，0为s级（1s-30s），1为ms级（1ms-1000ms), 2为us级（1us-1000us)
n 定时时间（不同模式下选择范围不同,s级1-30，ms级1-1000，us级1-1000）
******
*@desc:
通用定时器为T2/T3/T4/T5
定时器溢出时间计算公式：Tout= ((rstnum+1)*(psc+1))/Tclk
系统频率1分频时（TIM_CKD_DIV1），Tclk = 72M
定时时间最高可以达到60s
eg: 定时0.5s : rstnum = 4999, psc = 7199 (一般在此函数内部选择TIM_CKD_DIV1即可)
TIM_CKD_DIV1是最高频率了，即延时最精确在us级，此频率下延时最高可以调节到60s,但如果需要更高就需要切换了
******************************************************/
void TIM_Init(TIMER timer, u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n)
{
    u16 rstnum = 65535;  	// 溢出点
    u16 psc = 65535;		// 时钟的预分频数

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 定时区间选择
    if(mode == 0)
    {
        rstnum = 2000 * n - 1;
        psc = 35999;
        // 1s-30s是对于用户而言，实际调节区间1ms-30s
    }
    else if(mode == 1)
    {
        rstnum = 10 * n - 1;
        psc = 7199;
        // 实际调节区间0.2ms-6s
    }
    else if(mode == 2)
    {
        rstnum = n - 1;
        psc = 71;
        // 实际调节区间1us-6ms
    }

    //定时时间设定
    TIM_TimeBaseStructure.TIM_Period = rstnum;					//	设置溢出点
    TIM_TimeBaseStructure.TIM_Prescaler = psc;					//	设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//	选择时钟来源，设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //	TIM向上计数模式

    //选择定时器和设置优先级
    if(timer == TIMER1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 		//	时钟使能
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 			//	**高级定时器1需要**重复计数器计数器清零
        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 			//	根据指定的参数初始化TIMx的时间基数单位

		 
		TIM_ClearFlag(TIM1, TIM_FLAG_Update);						//  清中断，以免一启用中断后立即产生中断
        TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE ); 				//	使能指定的TIM2中断,允许更新中断

        //中断优先级NVIC设置
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  			//	TIM1更新中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	//	先占优先级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			//	从优先级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//	IRQ通道被使能
        NVIC_Init(&NVIC_InitStructure);  									//	初始化NVIC寄存器

        TIM_Cmd(TIM1, ENABLE);  //使能TIMx
    }
    else if(timer == TIMER2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 		//	时钟使能

        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			//	根据指定的参数初始化TIMx的时间基数单位

        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE ); 				//	使能指定的TIM2中断,允许更新中断

        //中断优先级NVIC设置
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  					//	TIM2中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	//	先占优先级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			//	从优先级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//	IRQ通道被使能
        NVIC_Init(&NVIC_InitStructure);  									//	初始化NVIC寄存器

        TIM_Cmd(TIM2, ENABLE);  //使能TIMx
    }
    else if(timer == TIMER3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 		//	时钟使能

        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 			//	根据指定的参数初始化TIMx的时间基数单位

        TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); 					//	使能指定的TIM3中断,允许更新中断

        //中断优先级NVIC设置
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  					//	TIM3中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	//	先占优先级0级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			//	从优先级3级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//	IRQ通道被使能
        NVIC_Init(&NVIC_InitStructure);  									//	初始化NVIC寄存器

        TIM_Cmd(TIM3, ENABLE);  //使能TIMx
    }
}


















