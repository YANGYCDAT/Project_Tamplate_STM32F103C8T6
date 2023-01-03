#include "wy_headfile.h"


//uchar TrigFlag = 1;						  	// ������������������־λ
u32 UlTansferTime = 0;                   		// �ɼ����ĸߵ�ƽʱ��
u32 UlCurDistance = 0;                   		// ��ǰ����



//-------------------------------------------------------------------------------------------------------------------
//  @brief    	���������ų�ʼ��
//  @param
//  @param
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void UlInit(void)
{
//	GPIO_InitTypeDef PA8_InitStructure;// ����PA8��GPIO�����ýṹ��
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// ʹ��APB2������GPIOA��ʱ�ӡ�
//	PA8_InitStructure.GPIO_Pin = GPIO_Pin_8;				// ���Գ�������ʱ��-->PA8 �˿�����
//	PA8_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����IO���ٶ�Ϊ50MHz
//	PA8_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// ����Ϊ����ģʽ
//	GPIO_Init(GPIOA, &PA8_InitStructure);					// ��PA8�ڽ��г�ʼ��
//	GPIO_ResetBits(GPIOA,GPIO_Pin_8);						// ��PA8�����
	
	GPIO_InitTypeDef PA0_InitStructure;
	
	// ����ʹ�õ�������ʱ�ӽ���ʹ�ܣ�һ��Ҫ��ʹ��ʱ���ٶԶ˿ڽ��г�ʼ����
	// IO��ʹ�õ���APB2��Ƶ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// ʹ��APB2������GPIOA��ʱ�ӡ�
	PA0_InitStructure.GPIO_Pin = GPIO_Pin_0;				// ������Trig
	PA0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����IO���ٶ�Ϊ50MHz
	PA0_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// ����Ϊ����ģʽ
	GPIO_Init(GPIOA, &PA0_InitStructure);					// ��PA0�ڽ��г�ʼ��
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						// ��PA0�����
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief    	�������������ͺ���
//  @param
//  @param
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void TrigUl(void)
{
    TRIG = 0;
    TRIG = 1;							// ��ഥ���źŵķ���
    delay_us(15);
    TRIG = 0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief    	��������ȡ���
//  @param
//  @param
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void UlGetDistance(void)
{
    TrigUl();										// ����������������
	UlTansferTime = GetCaptureTime();				// ͳ�Ƹߵ�ƽʱ��		
    UlCurDistance = UlTansferTime * 1.7 / 100;  	// ת��Ϊ����
	delay_ms(60);
}
