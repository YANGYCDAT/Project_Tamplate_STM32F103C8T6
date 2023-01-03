#include "wy_headfile.h"

/*****************************************************
*@Func:	��ʼ��GPIO��
******
*@para:
******
*@desc: ����PA4�����ڲ��Գ������е�ʱ�䣬����TIMEPORTʹ��
eg:
		TIMEPORT = 1;
		delay_us(10); // �������Ҫ���Եĳ�������ʱ�侫ȷ��1us(һ���ӡ��ʱ��Ҫ�ȴ˶γ������е���ʵʱ������2us)
		TIMEPORT = 0;
		tempI = GetCaptureTime();
		LCD_ShowInt(156,110,200,16,16,tempI);//��ʾADC��ֵ
******************************************************/	
void Gpio_Init(void)
{
	GPIO_InitTypeDef PA4_InitStructure;
	
	// ����ʹ�õ�������ʱ�ӽ���ʹ�ܣ�һ��Ҫ��ʹ��ʱ���ٶԶ˿ڽ��г�ʼ����
	// IO��ʹ�õ���APB2��Ƶ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// ʹ��APB2������GPIOA��ʱ�ӡ�
	PA4_InitStructure.GPIO_Pin = GPIO_Pin_4;				// ���Գ�������ʱ��-->PA4 �˿�����
	PA4_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����IO���ٶ�Ϊ50MHz
	PA4_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// ����Ϊ����ģʽ
	GPIO_Init(GPIOA, &PA4_InitStructure);					// ��PA4�ڽ��г�ʼ��
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);						// ��PA4�����
}
