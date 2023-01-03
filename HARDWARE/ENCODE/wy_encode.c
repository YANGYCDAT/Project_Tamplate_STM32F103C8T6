#include "wy_headfile.h"

u16 DecodeGapTime = 10;	// 测速时间间隔
u16 PulseNum = 13; 	// 电机转一圈产生多少次脉冲
u16 ReRate = 30;	// 减速比
int EncoderA;		// 编码器A接收到的脉冲数
int EncoderB;		// 编码器B接收到的脉冲数
float MotorASpeed;	// 电机A的转速
float MotorBSpeed;	// 电机B的转速

float EncoderA_Arr[ENCODERKEEPTIMES] = {0};
float EncoderB_Arr[ENCODERKEEPTIMES] = {0};



/*****************************************************
*@Func:	编码器涉及资源的初始化
******
*@para:	
******
*@desc:	选择TIM3作为此编码器的解码时钟
******	设置重装值
******  注意：实际使用时对转速有限制，如果转速太快可能发生定时器溢出的情况，就要考虑使用中断
******  目前对脉冲一次定时仅能计32768次，超过即无效，可以分别正反转。
******	定时器在编码器模式下电机如果正转计数器会从零开始正数，如果反转计数器会从重载值开始倒数
******************************************************/ 
void EncoderA_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 			//	引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//	定时器初始化的结构体
	TIM_ICInitTypeDef TIM_ICInitStructure; 			//	定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	//	使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//	使能GPIOA时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//	PA0、PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//	浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//	根据GPIO_InitStructure的参数初始化GPIOA

	TIM_TimeBaseStructure.TIM_Period = 0xffff; 				//	设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 						//	预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//	选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//	TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 				//	根据TIM_TimeBaseInitStruct的参数初始化定时器TIM3
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	
	//初始化TIM3输入捕获参数
	TIM_ICStructInit(&TIM_ICInitStructure); 							//	把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 0x03;  							// 	IC1F=0011 配置输入滤波器（连续采样八次）
	TIM_ICInit(TIM2, &TIM_ICInitStructure); 							//	根TIM_ICInitStructure参数初始化定时器TIM2编码器模式

	TIM_Cmd(TIM2, ENABLE); 												// 使能定时器2
}



/*****************************************************
*@Func:	编码器涉及资源的初始化
******
*@para:	
******
*@desc:	选择TIM4作为此编码器的解码时钟
******	设置重装值
******  注意：实际使用时对转速有限制，如果转速太快可能发生定时器溢出的情况，就要考虑使用中断
******  目前对脉冲一次定时仅能计32768次，超过即无效，可以分别正反转。
******	定时器在编码器模式下电机如果正转计数器会从零开始正数，如果反转计数器会从重载值开始倒数
******************************************************/ 
void EncoderB_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 			//	引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//	定时器初始化的结构体
	TIM_ICInitTypeDef TIM_ICInitStructure; 			//	定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	//	使能TIM4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	//	使能GPIOB时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//	PB6、PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//	浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//	根据GPIO_InitStructure的参数初始化GPIOB0

	TIM_TimeBaseStructure.TIM_Period = 0xffff; 				//	设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 						//	预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//	选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//	TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				//	根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	
	//初始化TIM4输入捕获参数
	TIM_ICStructInit(&TIM_ICInitStructure); 							//	把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 0x03;  							// 	IC1F=0011 配置输入滤波器（连续采样八次）
	TIM_ICInit(TIM4, &TIM_ICInitStructure); 							//	根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

	TIM_Cmd(TIM4, ENABLE); 												// 使能定时器4
}




/*****************************************************
*@Func:	读取定时器计数值
******
*@para:	
******
*@desc:	
******************************************************/ 
void Read_EncoderA(void)
{
	if(TIM2->CNT > 0x7fff)
	{
		EncoderA = TIM2->CNT - 0xffff;	// 反转
	}
	else
	{
		EncoderA = TIM2->CNT;			// 正转
	}
	TIM2->CNT = 0xffff;
}



/*****************************************************
*@Func:	读取定时器计数值
******
*@para:	
******
*@desc:	
******************************************************/ 
void Read_EncoderB(void)
{
	if(TIM4->CNT > 0x7fff)
	{
		EncoderB = TIM4->CNT - 0xffff;	// 反转
	}
	else
	{
		EncoderB = TIM4->CNT;			// 正转
	}
	TIM4->CNT = 0xffff;
}



/*****************************************************
*@Func:	获取电机A的速度
******
*@para:	
******
*@desc:	返回的是电机的转速 n圈/s
******	转速n = 总脉冲数/四倍频/单圈脉冲数/减速比/时间		
******************************************************/ 
void Get_MotorA_Speed(void)
{
	Read_EncoderA();	// 获取编码器产生的脉冲数
	
	MotorASpeed = (float)EncoderA / 4 / PulseNum / ReRate / DecodeGapTime * 1000;
}



/*****************************************************
*@Func:	获取电机A的速度
******
*@para:	
******
*@desc:	返回的是电机的转速 n圈/s
******	转速n = 总脉冲数/四倍频/单圈脉冲数/减速比/时间		
******************************************************/ 
void Get_MotorB_Speed(void)
{
	Read_EncoderB();	// 获取编码器产生的脉冲数
	MotorBSpeed = (float)EncoderB / 4 / PulseNum / ReRate / DecodeGapTime * 1000;
}
 


///*****************************************************
//*@Func:	编码器A测速滤波
//******
//*@para:	
//******
//*@desc:	平滑滤波		
//******************************************************/ 
//void EncodeA_Filter(void)
//{
//	
//}



///*****************************************************
//*@Func:	编码器B测速滤波
//******
//*@para:	
//******
//*@desc:	平滑滤波	
//******************************************************/ 
//void EncodeB_Filter(void)
//{

//}


