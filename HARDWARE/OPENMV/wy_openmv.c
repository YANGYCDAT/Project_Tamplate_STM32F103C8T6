# include "wy_headfile.h"

void OpenMV_Init(u8 ocp_nvc, u8 fow_nvc, u32 bound) 
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2ʱ��

    //USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA2

    //USART2_RX	  GPIOA.3��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA2

    //Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
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

    USART_Init(USART2, &USART_InitStructure); //��ʼ������2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1

}

void USART2_IRQHandler(void)                	//����2�жϷ������
{
    u8 Res;
	static u8 Receive_Suspicous = 0;
    static u8 Receive_Flag = 0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
    {
        Res = USART_ReceiveData(USART2);	//��ȡ���յ�������
        if(Res == DATAHEAD1 && !Receive_Flag) // �ж���У���1
        {
			Receive_Suspicous = 1;
        }
		else if(Res == DATAHEAD2 && Receive_Suspicous && !Receive_Flag)// �ж���У���2
		{
			USART2_RX_STA = 0;
            Receive_Flag = 1;
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ���������ݼĴ������
		}
        else if(Receive_Flag)// �ѽ��յ�������������У���
        {
            if((USART2_RX_STA & 0x8000) == 0) //����δ���
            {
                if(USART2_RX_STA & 0x4000) //���յ���0x0d
                {
                    if(Res != DATATAIL2) //���յ���0x0d����һλ���յ��Ĳ���0x0a
                    {
                        USART2_RX_STA = 0; //���մ������½���
                        Receive_Flag = 0;
                    }
                    else
                    {
                        USART2_RX_STA |= 0x8000;	//���������
//                        memcpy(Usart2RecData, USART2_RX_BUF, USART2_RX_STA & 0x3FFF); //�ѽ��ջ������е�ֵȡ����
						memcpy(DataFromCV.CharData, USART2_RX_BUF, USART2_RX_STA & 0x3FFF); //�ѽ��ջ������е�ֵȡ����
                        Receive_Flag = 0;
                    }
                }
                else //��û�յ�0X0D
                {
                    if(Res == DATATAIL1) // ������յ�0x0d��USART_RX_STA��15λ��1
                        USART2_RX_STA |= 0x4000;
                    else
                    {
                        USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res ;
                        USART2_RX_STA++;
                        if(USART2_RX_STA > (USART_REC_LEN - 1)) // ���������޶û�յ�βУ���
                        {
                            USART2_RX_STA = 0; //�������ݴ���,���¿�ʼ����
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

