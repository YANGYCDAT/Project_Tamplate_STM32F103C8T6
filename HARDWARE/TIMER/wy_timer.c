#include "wy_headfile.h"


/*****************************************************
*@Func:	��ʱ��6�жϷ������
******
*@para:
******
*@desc:
******************************************************/
void TIM6_IRQHandler(void)   //TIM6�ж�
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM1����(������жϷ������
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  	//���TIMx�����жϱ�־
		
		
//		TIMEPORT = 1;
		
		
		/*����Ų�������*/
		Get_MotorA_Speed();
		Get_MotorB_Speed();
		mpu_dmp_get_data(&pitch,&roll,&yaw);   		// ��ȡDMP������ŷ����
		/*******4ms********/
		
		
		/*����ſ��Ƴ���*/
		delay_ms(5);
		/***************/

		
		
		
//		TIMEPORT = 0;
//		delay_us(10);
//		GapTime = GetCaptureTime();
    }
}


/*****************************************************
*@Func:	��ʱ��7�жϷ������
******
*@para:
******
*@desc:	�����жϳ��򣬷�������OLED��OLEDֻ���ڵ��ε�ʱ���ʹ�ã�ʹ�õ�ʱ��رտ����ж�
******************************************************/
void TIM7_IRQHandler(void)   //TIM6�ж�
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //���TIM1����(������жϷ������
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  	//���TIMx�����жϱ�־
		
		
//		TIMEPORT = 1;
		
		
		/*�������������λ������*/
		USART_SendToKeYou(USART3);
		/**********6.5ms************/
		
		
//		TIMEPORT = 0;
//		delay_us(10);
//		GapTime = GetCaptureTime();
    }
}



/*****************************************************
*@Func:	��ʱ�жϳ�ʼ��
******
*@para:
timer	ʱ�Ӻŵ�ö�ٱ���
ocp_nvc	��ռ���ȼ�
fow_nvc �����ȼ�
mode ��ʱ����ѡ��0Ϊs����1s-30s����1Ϊms����1ms-1000ms), 2Ϊus����1us-1000us)
n ��ʱʱ�䣨��ͬģʽ��ѡ��Χ��ͬ,s��1-30��ms��1-1000��us��1-1000��
******
*@desc:
ͨ�ö�ʱ��ΪT2/T3/T4/T5
��ʱ�����ʱ����㹫ʽ��Tout= ((rstnum+1)*(psc+1))/Tclk
ϵͳƵ��1��Ƶʱ��TIM_CKD_DIV1����Tclk = 72M
��ʱʱ����߿��Դﵽ60s
eg: ��ʱ0.5s : rstnum = 4999, psc = 7199 (һ���ڴ˺����ڲ�ѡ��TIM_CKD_DIV1����)
TIM_CKD_DIV1�����Ƶ���ˣ�����ʱ�ȷ��us������Ƶ������ʱ��߿��Ե��ڵ�60s,�������Ҫ���߾���Ҫ�л���
******************************************************/
void TIM_Init(TIMER timer, u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n)
{
    u16 rstnum = 65535;  	// �����
    u16 psc = 65535;		// ʱ�ӵ�Ԥ��Ƶ��

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ��ʱ����ѡ��
    if(mode == 0)
    {
        rstnum = 2000 * n - 1;
        psc = 35999;
        // 1s-30s�Ƕ����û����ԣ�ʵ�ʵ�������1ms-30s
    }
    else if(mode == 1)
    {
        rstnum = 10 * n - 1;
        psc = 7199;
        // ʵ�ʵ�������0.2ms-6s
    }
    else if(mode == 2)
    {
        rstnum = n - 1;
        psc = 71;
        // ʵ�ʵ�������1us-6ms
    }

    //��ʱʱ���趨
    TIM_TimeBaseStructure.TIM_Period = rstnum;					//	���������
    TIM_TimeBaseStructure.TIM_Prescaler = psc;					//	����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//	ѡ��ʱ����Դ������ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //	TIM���ϼ���ģʽ

    //ѡ��ʱ�����������ȼ�
    if(timer == TIMER1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 		//	ʱ��ʹ��
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 			//	**�߼���ʱ��1��Ҫ**�ظ�����������������
        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 			//	����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

		 
		TIM_ClearFlag(TIM1, TIM_FLAG_Update);						//  ���жϣ�����һ�����жϺ����������ж�
        TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE ); 				//	ʹ��ָ����TIM2�ж�,��������ж�

        //�ж����ȼ�NVIC����
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  			//	TIM1�����ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	//	��ռ���ȼ�
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			//	�����ȼ�
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//	IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  									//	��ʼ��NVIC�Ĵ���

        TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx
    }
    else if(timer == TIMER2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 		//	ʱ��ʹ��

        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			//	����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE ); 				//	ʹ��ָ����TIM2�ж�,��������ж�

        //�ж����ȼ�NVIC����
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  					//	TIM2�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	//	��ռ���ȼ�
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			//	�����ȼ�
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//	IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  									//	��ʼ��NVIC�Ĵ���

        TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx
    }
    else if(timer == TIMER3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 		//	ʱ��ʹ��

        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 			//	����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

        TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); 					//	ʹ��ָ����TIM3�ж�,��������ж�

        //�ж����ȼ�NVIC����
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  					//	TIM3�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	//	��ռ���ȼ�0��
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			//	�����ȼ�3��
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//	IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  									//	��ʼ��NVIC�Ĵ���

        TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
    }
}


















