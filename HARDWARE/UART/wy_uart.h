#ifndef __WY_UART_H
#define __WY_UART_H
#include "stm32f10x.h"

//#define DATAHEAD 0x40
#define DATAHEAD1 0x40
#define DATAHEAD2 0x3A
#define DATATAIL1 0x0d
#define DATATAIL2 0x0a
#define USART_SEND_LEN 500

#define NUM_MAX  1		// ÿ�η��͸����ſƴ��������ֵ����������������Ըģ�Խ�󷢵�Խ��
#define VAR_NUM  3 		// ÿ�ν��ձ����ĸ���
#define Var_STR_LEN 6 	// �����ַ��������ĳ���(��λ����һ��������λС��)


#define BYTE0(Temp)       (*(char *)(&Temp))     
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))


typedef struct DATASTRUCT1	// ������д����ͱ����Ľṹ��
{
	float Angle_X;
	float Angle_Y;
	float Angle_Z;
	float Speed_A;
	float Speed_B;
}DataStruct1;

typedef struct DATASTRUCT2	// ������д����ͱ����Ľṹ��
{
	float test_float1;
	int test_int1;
	char test_char1;
	char test_string1[10];
	float test_float2;
	int test_int2;
	char test_char2;
	char test_string2[10];
}DataStruct2;

typedef struct DATASTRUCT3	// ������ȡ�������յı���
{
	float kp;
	float ki;
	float kd;
}DataStruct3;

typedef union DATAHOSTUNION				// ʹ��Union���ͱ���
{
	DataStruct1 StructData;
	char CharData[sizeof(DataStruct1)];
}DataHostUnion;

typedef union DATACVUNION				// ʹ��Union���ͱ���
{
	DataStruct2 StructData;
	char CharData[sizeof(DataStruct2)];
}DataCVUnion;


extern DataHostUnion DataToHost;
extern DataHostUnion DataFromHost;
extern DataCVUnion DataFromCV;

extern DataStruct3 DataBlueRec;

extern char USART_SEND_BUF[USART_SEND_LEN];
extern char UsartRecData[USART_REC_LEN];
extern char Var_Str_Rec[VAR_NUM][Var_STR_LEN]; // ��ŷ�������������ַ���

extern char USART2_RX_BUF[USART_REC_LEN]; 		// ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART2_RX_STA;         				// ����״̬���
extern char Usart2RecData[USART_REC_LEN];		// ���һ�������Ľ������ݣ�OPENMV)

extern char USART3_RX_BUF[USART_REC_LEN];  		// ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART3_RX_STA;         				// ����״̬���
//extern char Usart3RecData[USART_REC_LEN];		// ���һ�������Ľ������ݣ���λ����



void USART_SendStr_AsDeal(USART_TypeDef* USARTx, char* Uart_Str);
void USART_SendStr(USART_TypeDef* USARTx, char* Uart_Str);
void USART_SendInt(USART_TypeDef* USARTx, int Uart_Int);
void USART_SendFloat(USART_TypeDef* USARTx, float Uart_Float);
void Blue_Init(u8 ocp_nvc, u8 fow_nvc, u32 bound);
void USART_SendToKeYou(USART_TypeDef* USARTx);
void USART_SendData_AsDeal(USART_TypeDef* USARTx, char* DataArray, int DataSize);
#endif
