#include "wy_headfile.h"

float tempF;				// 临时浮点型变量
u32 tempI;					// 临时整型变量
u32 GapTime;				// 存放捕获的时间

u8 WhileFlag = 0;			

int main(void)
{	
	/*延时函数初始化*/
    delay_init();			// 初始化延时函数
	/*****************/
	
	
	/*优先级初始化*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		// 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	/*****************/
	
	
	__disable_irq();					// 关闭总中断
	
	
	/*PWM通道*/
	TIM4_PWM_Init(0, 10);			// PWM输出	10hz

	
	/*ADC通道*/
	Adc_Init();						// ADC通道初始化，精度为16位（PA1）
	
	
	/*输入捕获*/
	General_Cap_Init(0,0,1,1000);	// 微秒级捕获，1000us进入一次中断，优先级0-0，使用定时器3通道4(PB1)

	
	/*串口通信初始化*/
	uart_init(2,2,1152000);			// 串口1,优先级2-2,（PA9/PA10)
//	OpenMV_Init(0,1,115200);		// OPENMV串口初始化，串口2，串口波特率为115200，中断优先级2-2, PA2/PA3
	/*****************/
	
	
	/*定时中断初始化*/
	TIM_Init(TIMER1, 1, 0, 1, 20);	// 定时器6,控制程序，采集数据以及控制算法,中断优先级为1-0,采集时间是4ms,（防止被打断以出现异常采样）
	/*****************/	
	
	
	/*外设初始化*/
	Gpio_Init();			// 初始化通用输出口，PA4时间测试、PA5 OLED开关
//	Key_Init();				// 按键初始化
	LED_Init();				// LED初始化(PA11/PA12)
//	UlInit();				// 超声波初始化
//	EncoderA_Init();		// 编码器A初始化
//	EncoderB_Init();		// 编码器B初始化
//	MPU_Init(); 			// MPU6050初始化 PB8/PB9/PA15  带DMP初始化时间4s,不带DMP初始化时间0.1ms	
//	OLED_Init();			// 初始化OLED 	
//	/*****************/


//	/*从FLASH读取参数*/
//	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)&CAR[0].intData, PARA_MAX*sizeof(cardata)); // 从Flash中读取参数
//	ParaInit();                           // 按模式初始化各参数
//	/***************/


//	/*根据参数创建OLED菜单*/
//	CreatMenu();                          // 创建菜单并完成参量菜单项的赋值
//    MenuInit();                           // 给状态菜单项赋值
//	/********************/
			
	
	TIM4_PWM_SetDuty(0.5,0.5);
		
	LED0 = 1;
	LED1 = 1;
	delay_ms(500);
	LED0 = 0;
	LED1 = 0;

	/*开启总中断*/
	__enable_irq();				// 开启总中断
	/***********/
	
	while(1)
    {	
		/*输入捕获测试程序运行时间*/
		TIMEPORT = 1;
		
		/*在这里放待测程序*/
		USART_SendToKeYou(USART1);						// 发送变量到上位机（波特率1152000下发送1个变量用时130us，16个变量用时730us)

		adc1 = Get_Adc(1);								// ADC采样一次(用时25us)
		
		
		
//		delay_ms(10);									// 用于测试输入捕获
		/****************/
		GapTime = GetCaptureTime();
		TIMEPORT = 0;
		delay_us(2);									// 用于保障输入捕获
		/**********************/
	}	
	
}
	



//		/*输入捕获测试程序运行时间*/
//		TIMEPORT = 1;
//		
//			/*在这里放待测程序*/
//				delay_ms(10);
//			/****************/
//	
//		TIMEPORT = 0;
//		delay_ms(1);
//		GapTime = GetCaptureTime();
//		/**********************/



//		/*这里放OLED交互程序*/
//		if(!WhileFlag && OLEDFLAG)
//		{
//			__disable_irq();					// 关闭总中断
//			WhileFlag = 1;
//			PrintMenu();						  // 打印菜单
//			LED1 = 0;
//			delay_ms(100);
//			LED1 = 1;
//		}
//		else if(WhileFlag && OLEDFLAG)
//		{
//			KeyOperation(KEY_Scan(0));      	// 按键操作
//		}
//		else if(WhileFlag && !OLEDFLAG)
//		{
//			__enable_irq();						// 开启总中断
//			WhileFlag = 0;
//			LED0 = 0;
//			delay_ms(100);
//			LED0 = 1;
//		}
		/***********************************/
//		tempI = OLEDFLAG;
//		USART_SendInt(USART3, tempI);
//		USART_SendInt(USART3, WhileFlag);
