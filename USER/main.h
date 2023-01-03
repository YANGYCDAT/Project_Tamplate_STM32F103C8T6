#ifndef __MAIN_H
#define __MAIN_H

#define  TIMEPORT PAout(4)  // 结合输入捕获测试程序运行时间的端口
#define  OLEDFLAG PAin(5)   // OLED启用标志

extern float tempF;				// 临时浮点型变量
extern u32 tempI;				// 临时整型变量
extern u32 GapTime;				// 存放捕获的时间


#endif
