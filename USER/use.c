#include "wy_headfile.h"



/*****************************************************
*@Func:	���������յ����ַ��������ÿ������
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
		/*���������ַ���ʽȡ�����н��ձ���*/
		for(i= 0; i< VAR_NUM; i++)
		{
			substr(Var_Str_Rec[i], UsartRecData, i*(Var_STR_LEN+1), Var_STR_LEN);
		}
		
		substr(Var_Str_Rec[1], UsartRecData, 7, 6);
		substr(Var_Str_Rec[2], UsartRecData, 14, 6);
		Var_Update();	// ����Ŀ�ı����ĸ���
		
		LED0 = !LED0;
		delay_ms(10);
		LED0 = !LED0;
		memset(UsartRecData, 0, sizeof(UsartRecData)); // �����յ��������
	}
}



/*****************************************************
*@Func:	��ȡ�ַ���
******
*@para: �ӵ�startλ��ʼ��ȡsrc��len���ַ���dst����
******
*@desc: 
******************************************************/	
int substr(char dst[],char src[],int start,int len)
{
	char* p = src + start;   //����ָ�����ָ����Ҫ��ȡ���ַ��ĵ�ַ
	int n = strlen(p);       //���ַ�������
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
	}                        //�����ַ�����dst��
	dst[i] = '\0';
	return 0;
}




/*****************************************************
*@Func:	����Ŀ�ı����ĸ���
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
