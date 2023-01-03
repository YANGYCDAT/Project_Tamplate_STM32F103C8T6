#include "wy_headfile.h"

DataHostUnion DataToHost;			// ���͵���λ�������ݹ�ͬ��
DataHostUnion DataFromHost;			// ���������λ�����ݵĹ�ͬ��
DataCVUnion DataFromCV;				// �������OPENCV���ݵĹ�ͬ��
DataStruct3 DataBlueRec;			// ���������������ȡ���ַ���������

char USART_SEND_BUF[USART_SEND_LEN]; // ����������Ŵ����͵�����
float Variable[NUM_MAX];			 // ���Ҫ���͵ı���


char UsartRecData[USART_REC_LEN]; 	 // ���յ������ݣ���Ž��յ�����������
char Var_Str_Rec[VAR_NUM][Var_STR_LEN]; // ��ŷ�������������ַ���(������������)

char Usart2RecData[USART_REC_LEN]; 	 // ���յ������ݣ���Ž��յ�����������
char USART2_RX_BUF[USART_REC_LEN];   // ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
u16 USART2_RX_STA;         			 // ����״̬���

//char Usart3RecData[USART_REC_LEN]; 	// ���յ������ݣ���Ž��յ�����������
char USART3_RX_BUF[USART_REC_LEN];  	// ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
u16 USART3_RX_STA;         			 		// ����״̬���

/*****************************************************
*@Func:	����Э�鷢������
******
*@para:
******
*@desc: Ĭ�ϵ�Э��Ϊ��@ + ���� + �س���
******************************************************/	
void USART_SendStr_AsDeal(USART_TypeDef* USARTx, char* Uart_Str)
{
    char* p = Uart_Str;
    USART_SendData(USARTx, DATAHEAD1);               //�����׼����
	USART_SendData(USARTx, DATAHEAD2);               //�����׼����
    while(*p != '\0')
    {
        USART_SendData(USARTx, *p++); // ������һλ����
    }
    USART_SendData(USARTx, DATATAIL1);               //����β�����1
    USART_SendData(USARTx, DATATAIL2);               //����β�����2
}

void USART_SendStr(USART_TypeDef* USARTx, char* Uart_Str)
{
    char* p = Uart_Str;
    while(*p != '\0')
    {
        USART_SendData(USARTx, *p++); // ������һλ����
    }
}

void USART_SendInt(USART_TypeDef* USARTx, int Uart_Int)
{
    char Uart_str[10];
    sprintf((char*)Uart_str, "%5d", Uart_Int); // ��u16) ��ӡ��(char)��
    USART_SendStr(USARTx, Uart_str);
}


void USART_SendFloat(USART_TypeDef* USARTx, float Uart_Float)
{
    char Uart_str[10];
    sprintf((char*)Uart_str, "%7.2f", Uart_Float); // ��u16) ��ӡ��(char)��
    USART_SendStr(USARTx, Uart_str);
}

/*****************************************************
*@Func:	�������ڳ�ʼ��
******
*@para:
******
*@desc: ֻ�򿪽����жϣ����򿪷����ж�
******************************************************/	
void Blue_Init(u8 ocp_nvc, u8 fow_nvc, u32 bound) 
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc ; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1
}



/*****************************************************
*@Func:	�����������ͱ��������ſƴ���������
******
*@para:
******
*@desc: 
******************************************************/		
void USART_SendToKeYou(USART_TypeDef* USARTx)
{
    u8 i;
    float temp = 0.0;
    u8 ch;
	
	/*���������Ҫ���͵ı�����ע������ĸ������ܳ����趨������͸���NUM_MAX*/
	Variable[0] =  adc1;
//	Variable[0] =  GapTime;
//	Variable[2] =  5;
//	Variable[3] =  MotorASpeed;
//	Variable[4] =  MotorBSpeed;
//	Variable[5] =  GapTime;
//	Variable[6] =  5;
//	Variable[7] =  gyrox;
//	Variable[8] =  gyroy;
//	Variable[9] =  gyroz;
//	Variable[10] =  aacx;
//	Variable[11] =  aacy;
//	Variable[12] =  aacz;
//	Variable[13] =  tempI;	
//	Variable[14] = angle_cur;
//	Variable[15] = Balance_pid.errIntg;


	/*���������ſƴ���������֮���ͨ��Э��*/
	USART_SendData(USARTx, 0x55);
	USART_SendData(USARTx, 0x11);
	USART_SendData(USARTx, 0xaa);
	USART_SendData(USARTx, 0x55);
	USART_SendData(USARTx, 0xaa);
	USART_SendData(USARTx, 0xff);
	USART_SendData(USARTx, 0x01);
	USART_SendData(USARTx, NUM_MAX);

    for(i = 0; i < NUM_MAX; i++)// ��λ����(stm32���õ��Ǵ�˴洢���ݵķ�ʽ�������ֽڴ���ڸߵ�ַ��)
    {
        temp = Variable[i];

		ch = BYTE0(temp);
		USART_SendData(USARTx, ch);
		
        ch = BYTE1(temp);
        USART_SendData(USARTx, ch);

        ch = BYTE2(temp);
        USART_SendData(USARTx, ch);

        ch = BYTE3(temp);
        USART_SendData(USARTx, ch);
    }
	
    USART_SendData(USARTx, 0x01);// βУ���
}



/*****************************************************
*@Func:	���ڷ�����������
******
*@para:	
******
*@desc: ע������������ַ��������������ַ�������Ч�����м�û�п��ֽڣ����ַ������м������
����������ַ����ķ�ʽ��������������Ļ������ʹ���ݷ��Ͳ���������Ϊ���������ַ��ͻ�ֹͣ���͡�
���������Ҫ��������Union�д���ֽ������ַ�������ʵ�ַ��͸������͵����ݡ�
******************************************************/	
void USART_SendData_AsDeal(USART_TypeDef* USARTx, char* DataArray, int DataSize)
{
	char* p = DataArray;
    USART_SendData(USARTx, DATAHEAD1);               //�����׼����
	USART_SendData(USARTx, DATAHEAD2);               //�����׼����
    while(DataSize--)
    {
        USART_SendData(USARTx, *p++); // ������һλ����
    }
    USART_SendData(USARTx, DATATAIL1);               //����β�����1
    USART_SendData(USARTx, DATATAIL2);               //����β�����2
}
