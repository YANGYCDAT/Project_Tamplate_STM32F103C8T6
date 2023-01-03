#include "wy_headfile.h"

u16 DecodeGapTime = 10;	// ����ʱ����
u16 PulseNum = 13; 	// ���תһȦ�������ٴ�����
u16 ReRate = 30;	// ���ٱ�
int EncoderA;		// ������A���յ���������
int EncoderB;		// ������B���յ���������
float MotorASpeed;	// ���A��ת��
float MotorBSpeed;	// ���B��ת��

float EncoderA_Arr[ENCODERKEEPTIMES] = {0};
float EncoderB_Arr[ENCODERKEEPTIMES] = {0};



/*****************************************************
*@Func:	�������漰��Դ�ĳ�ʼ��
******
*@para:	
******
*@desc:	ѡ��TIM3��Ϊ�˱������Ľ���ʱ��
******	������װֵ
******  ע�⣺ʵ��ʹ��ʱ��ת�������ƣ����ת��̫����ܷ�����ʱ��������������Ҫ����ʹ���ж�
******  Ŀǰ������һ�ζ�ʱ���ܼ�32768�Σ���������Ч�����Էֱ�����ת��
******	��ʱ���ڱ�����ģʽ�µ�������ת����������㿪ʼ�����������ת�������������ֵ��ʼ����
******************************************************/ 
void EncoderA_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 			//	���ų�ʼ���Ľṹ��  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//	��ʱ����ʼ���Ľṹ��
	TIM_ICInitTypeDef TIM_ICInitStructure; 			//	��ʱ��������ģʽ��ʼ���Ľṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	//	ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//	ʹ��GPIOAʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//	PA0��PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//	��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//	����GPIO_InitStructure�Ĳ�����ʼ��GPIOA

	TIM_TimeBaseStructure.TIM_Period = 0xffff; 				//	�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 						//	Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//	ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//	TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 				//	����TIM_TimeBaseInitStruct�Ĳ�����ʼ����ʱ��TIM3
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3��CH1��CH2ͬʱ�������ķ�Ƶ
	
	//��ʼ��TIM3���벶�����
	TIM_ICStructInit(&TIM_ICInitStructure); 							//	��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
	TIM_ICInitStructure.TIM_ICFilter = 0x03;  							// 	IC1F=0011 ���������˲��������������˴Σ�
	TIM_ICInit(TIM2, &TIM_ICInitStructure); 							//	��TIM_ICInitStructure������ʼ����ʱ��TIM2������ģʽ

	TIM_Cmd(TIM2, ENABLE); 												// ʹ�ܶ�ʱ��2
}



/*****************************************************
*@Func:	�������漰��Դ�ĳ�ʼ��
******
*@para:	
******
*@desc:	ѡ��TIM4��Ϊ�˱������Ľ���ʱ��
******	������װֵ
******  ע�⣺ʵ��ʹ��ʱ��ת�������ƣ����ת��̫����ܷ�����ʱ��������������Ҫ����ʹ���ж�
******  Ŀǰ������һ�ζ�ʱ���ܼ�32768�Σ���������Ч�����Էֱ�����ת��
******	��ʱ���ڱ�����ģʽ�µ�������ת����������㿪ʼ�����������ת�������������ֵ��ʼ����
******************************************************/ 
void EncoderB_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 			//	���ų�ʼ���Ľṹ��  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//	��ʱ����ʼ���Ľṹ��
	TIM_ICInitTypeDef TIM_ICInitStructure; 			//	��ʱ��������ģʽ��ʼ���Ľṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	//	ʹ��TIM4ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	//	ʹ��GPIOBʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//	PB6��PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//	��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//	����GPIO_InitStructure�Ĳ�����ʼ��GPIOB0

	TIM_TimeBaseStructure.TIM_Period = 0xffff; 				//	�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 						//	Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//	ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//	TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				//	����TIM_TimeBaseInitStruct�Ĳ�����ʼ����ʱ��TIM4
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3��CH1��CH2ͬʱ�������ķ�Ƶ
	
	//��ʼ��TIM4���벶�����
	TIM_ICStructInit(&TIM_ICInitStructure); 							//	��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
	TIM_ICInitStructure.TIM_ICFilter = 0x03;  							// 	IC1F=0011 ���������˲��������������˴Σ�
	TIM_ICInit(TIM4, &TIM_ICInitStructure); 							//	��TIM_ICInitStructure������ʼ����ʱ��TIM4������ģʽ

	TIM_Cmd(TIM4, ENABLE); 												// ʹ�ܶ�ʱ��4
}




/*****************************************************
*@Func:	��ȡ��ʱ������ֵ
******
*@para:	
******
*@desc:	
******************************************************/ 
void Read_EncoderA(void)
{
	if(TIM2->CNT > 0x7fff)
	{
		EncoderA = TIM2->CNT - 0xffff;	// ��ת
	}
	else
	{
		EncoderA = TIM2->CNT;			// ��ת
	}
	TIM2->CNT = 0xffff;
}



/*****************************************************
*@Func:	��ȡ��ʱ������ֵ
******
*@para:	
******
*@desc:	
******************************************************/ 
void Read_EncoderB(void)
{
	if(TIM4->CNT > 0x7fff)
	{
		EncoderB = TIM4->CNT - 0xffff;	// ��ת
	}
	else
	{
		EncoderB = TIM4->CNT;			// ��ת
	}
	TIM4->CNT = 0xffff;
}



/*****************************************************
*@Func:	��ȡ���A���ٶ�
******
*@para:	
******
*@desc:	���ص��ǵ����ת�� nȦ/s
******	ת��n = ��������/�ı�Ƶ/��Ȧ������/���ٱ�/ʱ��		
******************************************************/ 
void Get_MotorA_Speed(void)
{
	Read_EncoderA();	// ��ȡ������������������
	
	MotorASpeed = (float)EncoderA / 4 / PulseNum / ReRate / DecodeGapTime * 1000;
}



/*****************************************************
*@Func:	��ȡ���A���ٶ�
******
*@para:	
******
*@desc:	���ص��ǵ����ת�� nȦ/s
******	ת��n = ��������/�ı�Ƶ/��Ȧ������/���ٱ�/ʱ��		
******************************************************/ 
void Get_MotorB_Speed(void)
{
	Read_EncoderB();	// ��ȡ������������������
	MotorBSpeed = (float)EncoderB / 4 / PulseNum / ReRate / DecodeGapTime * 1000;
}
 


///*****************************************************
//*@Func:	������A�����˲�
//******
//*@para:	
//******
//*@desc:	ƽ���˲�		
//******************************************************/ 
//void EncodeA_Filter(void)
//{
//	
//}



///*****************************************************
//*@Func:	������B�����˲�
//******
//*@para:	
//******
//*@desc:	ƽ���˲�	
//******************************************************/ 
//void EncodeB_Filter(void)
//{

//}


