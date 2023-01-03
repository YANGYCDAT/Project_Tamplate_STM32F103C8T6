#ifndef __USE_H
#define __USE_H
#include "sys.h"
#include "wy_uart.h"


void UART_GetEveryVar(void);
int substr(char dst[],char src[],int start,int len);
void Var_Update(void);

#endif
