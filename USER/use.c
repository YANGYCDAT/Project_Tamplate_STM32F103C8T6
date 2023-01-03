#include "wy_headfile.h"



/*****************************************************
*@Func:	将蓝牙接收到的字符串分配给每个变量
******
*@para: 
******
*@desc: 
******************************************************/	
void UART_GetEveryVar()
{
	int i = 0;
	if(UsartRecData[0] != '\0')
	{
		/*在这里以字符形式取出所有接收变量*/
		for(i= 0; i< VAR_NUM; i++)
		{
			substr(Var_Str_Rec[i], UsartRecData, i*(Var_STR_LEN+1), Var_STR_LEN);
		}
		
		substr(Var_Str_Rec[1], UsartRecData, 7, 6);
		substr(Var_Str_Rec[2], UsartRecData, 14, 6);
		Var_Update();	// 接收目的变量的更新
		
		LED0 = !LED0;
		delay_ms(10);
		LED0 = !LED0;
		memset(UsartRecData, 0, sizeof(UsartRecData)); // 将接收的数据清空
	}
}



/*****************************************************
*@Func:	提取字符段
******
*@para: 从第start位开始提取src里len个字符到dst里面
******
*@desc: 
******************************************************/	
int substr(char dst[],char src[],int start,int len)
{
	char* p = src + start;   //定义指针变量指向需要提取的字符的地址
	int n = strlen(p);       //求字符串长度
	int i = 0;
	if(n < len)
	{
		len = n;
	}
	while(len != 0)
	{
		dst[i] = src[i+start];
		len --;
		i++;
	}                        //复制字符串到dst中
	dst[i] = '\0';
	return 0;
}




/*****************************************************
*@Func:	接收目的变量的更新
******
*@para: 
******
*@desc: 
******************************************************/
void Var_Update(void)
{
	DataBlueRec.kp = (float)atof(Var_Str_Rec[0]);
	DataBlueRec.ki = (float)atof(Var_Str_Rec[1]);
	DataBlueRec.kd = (float)atof(Var_Str_Rec[2]);
}
