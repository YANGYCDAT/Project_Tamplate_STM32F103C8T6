# include "wy_headfile.h"



void Host_Init(u8 ocp_nvc, u8 fow_nvc, u32 bound) 
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��

    //USART3_TX   GPIOB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA2

    //USART3_RX	  GPIOB11��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA2

    //Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc ; //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;		//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3

}

void USART3_IRQHandler(void)                	//����2�жϷ������
{
    u8 Res;
	static u8 Receive_Suspicous = 0;
    static u8 Receive_Flag = 0;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
    {
        Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
		if(Res == DATAHEAD1 && !Receive_Flag) // �ж���У���1
        {
			Receive_Suspicous = 1;
        }
        else if(Res == DATAHEAD2 && Receive_Suspicous && !Receive_Flag) // �ж���У���
        {
            USART3_RX_STA = 0;
            Receive_Flag = 1;
            memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF)); // ���������ݼĴ������
        }
        else if(Receive_Flag)// �ѽ��յ���У���
        {
            if((USART3_RX_STA & 0x8000) == 0) //����δ���
            {
                if(USART3_RX_STA & 0x4000) //���յ���0x0d
                {
                    if(Res != DATATAIL2) //���յ���0x0d����һλ���յ��Ĳ���0x0a
                    {
                        USART3_RX_STA = 0; //���մ������½���
                        Receive_Flag = 0;
                    }
                    else
                    {
                        USART3_RX_STA |= 0x8000;	//���������
//                        memcpy(Usart3RecData, USART3_RX_BUF, sizeof(USART3_RX_BUF)); //�ѽ��ջ������е�ֵȡ����
						memcpy(DataFromHost.CharData, USART3_RX_BUF, USART3_RX_STA & 0x3FFF); //�ѽ��ջ������е�ֵȡ����
                        Receive_Flag = 0;
                    }
                }
                else //��û�յ�0X0D
                {
                    if(Res == DATATAIL1) // ������յ�0x0d��USART_RX_STA��15λ��1
                        USART3_RX_STA |= 0x4000;
                    else
                    {
                        USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = Res ;
                        USART3_RX_STA++;
                        if(USART3_RX_STA > (USART_REC_LEN - 1)) // ���������޶û�յ�βУ���
                        {
                            USART3_RX_STA = 0; //�������ݴ���,���¿�ʼ����
                            Receive_Flag = 0;
                        }
                    }
                }
            }
        }
		else
		{
			Receive_Suspicous = 0;	// ȷ���������յ�������У���
			Receive_Flag = 0;
		}
    }
}
