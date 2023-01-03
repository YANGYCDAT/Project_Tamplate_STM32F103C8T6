#include "wy_headfile.h"


u32 TIM3_CAPTURE_STA = 0;	// ���벶��״̬����32λ��Ϊ���ܹ��������ʱ�䳤���ź�
u16	TIM3_CAPTURE_VAL;		// ���벶��ֵ


/*****************************************************
*@Func:	���벶���ʼ�������ڳ��沶��
******
*@para:
ocp_nvc	��ռ���ȼ�
fow_nvc �����ȼ�
mode ���񾫶�����ѡ��0Ϊms����1ms-1000ms), 1Ϊus����1us-1000us) ��mode=0����������Ϊ0.1ms,mode=1,��������Ϊ1us
n ���񾫶ȣ�ms��1-1000��us��1-1000��
eg�� mode = 1,n= 1000,��1us����һ��,1000us����һ���жϣ�mode = 0,n=10,��0.1ms����һ�Σ�10ms����һ���ж�
******
*@desc:		**ͨ�ò���Ҫ�͵��ݴ������벶��һ��ʹ�ã�**
ͨ�ö�ʱ��ΪT2/T3/T4/T5
��ʱ�����ʱ����㹫ʽ��Tout= ((rstnum+1)*(psc+1))/Tclk
ϵͳƵ��1��Ƶʱ��TIM_CKD_DIV1����Tclk = 72M
��ʱʱ����߿��Դﵽ60s
eg: ��ʱ0.5s : rstnum = 4999, psc = 7199 (һ���ڴ˺����ڲ�ѡ��TIM_CKD_DIV1����)
TIM_CKD_DIV1�����Ƶ���ˣ�����ʱ�ȷ��us������Ƶ������ʱ��߿��Ե��ڵ�60s,�������Ҫ���߾���Ҫ�л���

**us��ģʽ��ms��ģʽ�Ĳ���ʱ�䷶Χ���ǳ��󣬲���100s����û������ģ���֤����us������10s��������ʾ10000013us
						(us��1000us���һ�Σ����һ�����������,���ɼ�2^28�η���)

******************************************************/
void General_Cap_Init(u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n)
{
    u16 rstnum = 65535;  	// �����
    u16 psc = 65535;		// ʱ�ӵ�Ԥ��Ƶ��

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM3_ICInitStructure;				// ��ʱ��3ͨ��4���벶������

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	// ʹ��TIM3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  	// ʹ��GPIOBʱ��

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  			// PB1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			// PB1 ����
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);						// ����PB1 ����


    if(mode == 0)
    {
        rstnum = 10 * n - 1;
        psc = 7199;			// 0.1ms ����һ��,����Ϊ0.1ms
        // ʵ�ʵ�������0.2ms-6s
    }
    else if(mode == 1)
    {
        rstnum = n - 1;
        psc = 71; 			// 1us ����һ��,����Ϊ1us
        // ʵ�ʵ�������1us-6ms
    }

    //��ʼ����ʱ��3 TIM3
    TIM_TimeBaseStructure.TIM_Period = rstnum; 				// �趨�����
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 				// Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		// TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 					// ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM3���벶�����
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; 					// ͨ��4
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// �����ز���
    TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	// ֱ��ӳ��
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			// ���������Ƶ,����Ƶ
    TIM3_ICInitStructure.TIM_ICFilter = 0x03;							// IC1F=0011 ���������˲��������������˴Σ�
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);

    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  					// TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	// ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			// �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					// IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  									// ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC4, ENABLE);				// ��������ж� ,����3��ͨ�������ж�

    TIM_Cmd(TIM3, ENABLE); 											// ʹ�ܶ�ʱ��3

}



/*****************************************************
*@Func:	�õ�����ʱ��
******
*@para:
******
*@desc: *ע��һ��Ҫ�ȶ����ڲ���Ķ˿ڽ��г�ʼ��������Gpio_Init()��ʵ�֣�ʹ��PA4���Գ������е�ʱ��
******************************************************/
u32 GetCaptureTime(void)
{
    static u32 temp = 0;
    if(TIM3_CAPTURE_STA & 0X80000000)			// �ɹ�������һ��������
    {
        temp = TIM3_CAPTURE_STA & 0X3FFFFFFF;	// ��ȡ�����ڼ䶨ʱ������Ĵ���
		tempI = temp;
        temp *= 1000;							// ��ʱ�����ʱ���ܺͣ�1000us���һ�Σ�
        temp += TIM3_CAPTURE_VAL;				// ���ϼĴ����еļ���ֵ���õ��ܵĸߵ�ƽʱ��
        TIM3_CAPTURE_STA = 0;					// ������һ�β���
    }
    return temp;
}




//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{
    // �Զ�ʱ�����´������м���
    if((TIM3_CAPTURE_STA & 0X80000000) == 0)						// ��δ�������
    {
        if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
        {
            if(TIM3_CAPTURE_STA & 0X40000000)					// �Ѿ����񵽸ߵ�ƽ��
            {
                if((TIM3_CAPTURE_STA & 0X3FFFFFFF) == 0X3FFFFFFF)	// �ߵ�ƽ̫���ˣ����²���
                {
                    TIM3_CAPTURE_STA |= 0X80000000;				// ��ǳɹ�������һ��
                    TIM3_CAPTURE_VAL = 0XFFFF;
                }
                else
                {
                    TIM3_CAPTURE_STA++;
                }
            }
        }
		
        // ���ǶԲ���������м�⣨һ���������������Σ������ؽ���һ�Σ��½��ؽ�����һ�Σ�
        if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)				// ����3���������¼�
        {
            if(TIM3_CAPTURE_STA & 0X40000000)						// ��֮ǰ�Ѿ�������������
            {
                TIM3_CAPTURE_STA |= 0X80000000;						// �����γɹ����񵽵��½���
                TIM3_CAPTURE_VAL = TIM_GetCapture4(TIM3);  			// ���Ĵ����д�ŵĶ�ʱ����������
                TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising); // ͨ��4 ����Ϊ�����ز���
            } else  												// ��δ��ʼ,��һ�β���������
            {
                TIM3_CAPTURE_STA = 0;								// ���
                TIM3_CAPTURE_VAL = 0;
                TIM_SetCounter(TIM3, 0);
                TIM3_CAPTURE_STA |= 0X40000000;						// ��ǲ�����������
                TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);// ͨ��4����Ϊ�½��ز���
            }
        }
    }

    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4 | TIM_IT_Update); 		// ���ͨ��3�жϱ�־λ
}

