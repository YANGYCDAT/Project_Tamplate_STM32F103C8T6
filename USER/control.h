#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "wy_headfile.h"



extern cardata CAR[PARA_MAX];            //存放PARA_MAX个参量数据，从单片机内存中读取
extern u16 flag;                      //控制菜单悬挂：0时菜单工作，1时跑车
extern int mode;                         //小车的模式
extern float angle_accl;                 //由三轴加速度计算单车左右方向的角度
extern float angle_gyro;                 //由三轴角速度计算单车左右方向的角度
extern float angle_cur;                  //两种角度滤波得到当前角度
extern float angle_set;                  //平衡零点
extern float angle_prop;                 //角速度计和加速度计的占比
extern float speed_cur;                  //当前速度
extern float speed_set;                  //设定速度
extern float speed_add;                  //加速防倒（暂未使用）
extern float speed_motor;                //根据车况给电机的速度（暂未使用）
extern float kp_mid_l, kp_mid_r;         //使中线偏差范围进入角度偏差范围的系数，以得到最终的bias
extern float dir_max_l,dir_max_r;        //倾斜角最大幅度
extern struct pidCtrl_t Balance_pid;     //直立环，转向环pid参量
extern struct pidCtrl_t_1 Speed_pid;
extern float Balance_pidoutput;          //直立环pid总输出
extern float Speed_pidoutput;            //速度环pid总输出
extern float Dir_bias;                   //转向偏差
extern float Angle_bias;                 //角度偏差
extern float bias;                       //输给舵机的偏差，角度偏差减去转向偏差（用转向偏差去修改平衡零点angle_set）
extern u16 steer_mid ;                //舵机中值
extern int steer;                      //舵机占空比
extern u16 menu_flag;                 //控制菜单悬挂：0时菜单工作，1时跑车
extern u16 sample_flag;                //寻找各电感最大值，用作归一化处理
extern u16 control_times;             //进入控制中断的次数，用于划分寻找各路电感最值的时间


void ParaInit(void);                             //将原始小车参量写入单片机寄存器，该函数只运行一次

#endif
