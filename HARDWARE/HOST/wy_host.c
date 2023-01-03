# include "wy_headfile.h"



void Host_Init(u8 ocp_nvc, u8 fow_nvc, u32 bound) 
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟

    //USART3_TX   GPIOB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA2

    //USART3_RX	  GPIOB11初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA2

    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc ; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;		//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USART3, ENABLE);                    //使能串口3

}

void USART3_IRQHandler(void)                	//串口2中断服务程序
{
    u8 Res;
	static u8 Receive_Suspicous = 0;
    static u8 Receive_Flag = 0;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
    {
        Res = USART_ReceiveData(USART3);	//读取接收到的数据
		if(Res == DATAHEAD1 && !Receive_Flag) // 判断首校验符1
        {
			Receive_Suspicous = 1;
        }
        else if(Res == DATAHEAD2 && Receive_Suspicous && !Receive_Flag) // 判断首校验符
        {
            USART3_RX_STA = 0;
            Receive_Flag = 1;
            memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF)); // 将接收数据寄存器清空
        }
        else if(Receive_Flag)// 已接收到首校验符
        {
            if((USART3_RX_STA & 0x8000) == 0) //接收未完成
            {
                if(USART3_RX_STA & 0x4000) //接收到了0x0d
                {
                    if(Res != DATATAIL2) //接收到了0x0d但下一位接收到的不是0x0a
                    {
                        USART3_RX_STA = 0; //接收错误重新接收
                        Receive_Flag = 0;
                    }
                    else
                    {
                        USART3_RX_STA |= 0x8000;	//接收完成了
//                        memcpy(Usart3RecData, USART3_RX_BUF, sizeof(USART3_RX_BUF)); //把接收缓存器中的值取出来
						memcpy(DataFromHost.CharData, USART3_RX_BUF, USART3_RX_STA & 0x3FFF); //把接收缓存器中的值取出来
                        Receive_Flag = 0;
                    }
                }
                else //还没收到0X0D
                {
                    if(Res == DATATAIL1) // 如果接收到0x0d，USART_RX_STA第15位置1
                        USART3_RX_STA |= 0x4000;
                    else
                    {
                        USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = Res ;
                        USART3_RX_STA++;
                        if(USART3_RX_STA > (USART_REC_LEN - 1)) // 超出接收限额还没收到尾校验符
                        {
                            USART3_RX_STA = 0; //接收数据错误,重新开始接收
                            Receive_Flag = 0;
                        }
                    }
                }
            }
        }
		else
		{
			Receive_Suspicous = 0;	// 确保连续接收到两个首校验符
			Receive_Flag = 0;
		}
    }
}
