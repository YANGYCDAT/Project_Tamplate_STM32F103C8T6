#include "wy_headfile.h"


u16 T1_rstnum = 0;
u16 T4_rstnum = 0;

/*****************************************************
*@Func:	PWM���ֳ�ʼ�� ��ʹ�ö˿�PB5�����
******
*@para:
PWM�����ʼ��
mode Ƶ������ѡ��0ΪHz����1Hz-1000Hz����1ΪKHZ����1KHZ-1000KHZ)
f Ƶ�ʣ�Hz����KHz����Ϊ1-1000��
duty ռ�ձ�(0-100]
******
*@desc:
TIM3 PWM���ֳ�ʼ��
ͨ�ö�ʱ��ΪT2/T3/T4/T5
PWMƵ�ʼ��㹫ʽ�����ڶ�ʱ���������f = Tclk / ((rstnum+1)*(psc+1))
ϵͳƵ��1��Ƶʱ��TIM_CKD_DIV1����Tclk = 72M
ʹ�ö˿�PB5��������޸ģ�

ռ�ձ�ԭ�� �Ƚ�ֵ = ����ֵ * ռ�ձ�(duty/100) , ����ʱ������ֵС�ڱȽ�ֵʱ������͵�ƽ
******************************************************/ 
void TIM1_PWM_Init(u8 mode, u16 f)
{  
	u16 rstnum = 65535;  	// ����ֵ
    u16 psc = 65535;		// ʱ�ӵ�Ԥ��Ƶ��

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	 // Ƶ������ѡ��
    if(mode == 0)
    {
        rstnum = 10000/f -1;
        psc = 7199;
        // ʵ�ʵ�������1/6Hz - 5000Hz  
    }
    else if(mode == 1)
    {
        rstnum = 1000/f -1;
        psc = 71;
        // ʵ�ʵ�������1/6KHz - 1000KHz
    }
	T1_rstnum = rstnum;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��   
 
   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��	GPIOA8/11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11; 				// TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// ��ʼ��GPIO
 
   //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = T1_rstnum; 				// ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				// ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 				// ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM1 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				// ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	// �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// �������:TIM����Ƚϼ��Ը�

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);              // ����Tָ���Ĳ�����ʼ������TIM8 OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);              // ����Tָ���Ĳ�����ʼ������TIM8 OC2
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);              // ����Tָ���Ĳ�����ʼ������TIM8 OC3
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);              // ����Tָ���Ĳ�����ʼ������TIM8 OC4
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);          // ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);          // ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);          // ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);          // ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM1, ENABLE);  										// ʹ��TIM1
	TIM_ARRPreloadConfig(TIM1, ENABLE); 							// ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_CtrlPWMOutputs(TIM1,ENABLE);        						// MOE �����ʹ��,�߼���ʱ�����뿪�����
	
}



/*****************************************************
*@Func:	PWM���ֳ�ʼ�� ��ʹ�ö˿�PB5�����
******
*@para:
PWM�����ʼ��
mode Ƶ������ѡ��0ΪHz����1Hz-1000Hz����1ΪKHZ����1KHZ-1000KHZ)
f Ƶ�ʣ�Hz����KHz����Ϊ1-1000��
duty ռ�ձ�(0-100]
******
*@desc:
TIM3 PWM���ֳ�ʼ��
ͨ�ö�ʱ��ΪT2/T3/T4/T5
PWMƵ�ʼ��㹫ʽ�����ڶ�ʱ���������f = Tclk / ((rstnum+1)*(psc+1))
ϵͳƵ��1��Ƶʱ��TIM_CKD_DIV1����Tclk = 72M
ʹ�ö˿�PB5��������޸ģ�

ռ�ձ�ԭ�� �Ƚ�ֵ = ����ֵ * ռ�ձ�(duty/100) , ����ʱ������ֵС�ڱȽ�ֵʱ������͵�ƽ
******************************************************/ 
void TIM4_PWM_Init(u8 mode, u16 f)
{  
	u16 rstnum = 65535;  	// ����ֵ
    u16 psc = 65535;		// ʱ�ӵ�Ԥ��Ƶ��

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	 // Ƶ������ѡ��
    if(mode == 0)
    {
        rstnum = 10000/f -1;
        psc = 7199;
        // ʵ�ʵ�������1/6Hz - 5000Hz  
    }
    else if(mode == 1)
    {
        rstnum = 1000/f -1;
        psc = 71;
        // ʵ�ʵ�������1/6KHz - 1000KHz
    }
	T4_rstnum = rstnum;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��   
 
   //���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��	PB6/PB7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 				// TIM4_CH1/2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// ��ʼ��GPIO
 
   //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = T4_rstnum; 				// ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				// ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				// ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel1/2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				// ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	// �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// �������:TIM����Ƚϼ��Ը�

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);              // ����Tָ���Ĳ�����ʼ������TIM4 OC1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);              // ����Tָ���Ĳ�����ʼ������TIM4 OC2

	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);          // ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);          // ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

	
	TIM_Cmd(TIM4, ENABLE);  										// ʹ��TIM4
	TIM_ARRPreloadConfig(TIM4, ENABLE); 							// ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);        						// MOE �����ʹ��,�߼���ʱ�����뿪�����
	
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief        	��ʱ��1��PWMͨ������ռ�ձ�
//  @param     
//  @attention    	ռ�ձ�(0-1)���������Ĳ�����-1�򲻽�������
//  @return       
//  Sample usage:  	ռ�ձ�ԭ�� �Ƚ�ֵ = ����ֵ * ռ�ձ�(duty/100) , ����ʱ������ֵС�ڱȽ�ֵʱ������͵�ƽ         
//-------------------------------------------------------------------------------------------------------------------
void TIM1_PWM_SetDuty(float C1Duty,	float C2Duty, float C3Duty, float C4Duty)
{
	u16 compare_value = 0;  // ռ�ձȵıȽ�ֵ
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
//  @brief        	��ʱ��1��PWMͨ������ռ�ձ�
//  @param     
//  @attention    	ռ�ձ�(0-1)���������Ĳ�����-1�򲻽�������
//  @return       
//  Sample usage:  	ռ�ձ�ԭ�� �Ƚ�ֵ = ����ֵ * ռ�ձ�(duty/100) , ����ʱ������ֵС�ڱȽ�ֵʱ������͵�ƽ         
//-------------------------------------------------------------------------------------------------------------------
void TIM4_PWM_SetDuty(float C1Duty,	float C2Duty)
{
	u16 compare_value = 0;  // ռ�ձȵıȽ�ֵ
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
