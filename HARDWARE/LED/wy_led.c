#include "wy_headfile.h"


/*****************************************************
*@Func:	��ʼ��LED
******
*@para: LED0:PA11  LED1:PA12
******
*@desc: 
******************************************************/	
void LED_Init(void)
{
	GPIO_InitTypeDef LED0_InitStructure;// ����PA11��GPIO�����ýṹ��
	GPIO_InitTypeDef LED1_InitStructure;// ����PA12��GPIO�����ýṹ��
	
	// ����ʹ�õ�������ʱ�ӽ���ʹ�ܣ�һ��Ҫ��ʹ��ʱ���ٶԶ˿ڽ��г�ʼ����
	// IO��ʹ�õ���APB2��Ƶ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);// ʹ��APB2�����ϵ�GPIOA��ʱ�ӡ�
	LED0_InitStructure.GPIO_Pin = GPIO_Pin_11;				// LED0-->PA11 �˿�����
	LED0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����IO���ٶ�Ϊ50MHz
	LED0_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// ����Ϊ����ģʽ
	GPIO_Init(GPIOA, &LED0_InitStructure);					// ��PA11�ڽ��г�ʼ��
	GPIO_SetBits(GPIOA,GPIO_Pin_11);						// PA11�����
	
	LED1_InitStructure.GPIO_Pin = GPIO_Pin_12;				// LED1-->PA12 �˿�����
	LED1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����IO���ٶ�Ϊ50MHz
	LED1_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// ����Ϊ����ģʽ
	GPIO_Init(GPIOA, &LED1_InitStructure);					// ��PA12�ڽ��г�ʼ��
	GPIO_SetBits(GPIOA,GPIO_Pin_12); 						// PA12 ����� 

}

