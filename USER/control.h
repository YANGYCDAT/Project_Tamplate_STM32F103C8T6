#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "wy_headfile.h"



extern cardata CAR[PARA_MAX];            //���PARA_MAX���������ݣ��ӵ�Ƭ���ڴ��ж�ȡ
extern u16 flag;                      //���Ʋ˵����ң�0ʱ�˵�������1ʱ�ܳ�
extern int mode;                         //С����ģʽ
extern float angle_accl;                 //��������ٶȼ��㵥�����ҷ���ĽǶ�
extern float angle_gyro;                 //��������ٶȼ��㵥�����ҷ���ĽǶ�
extern float angle_cur;                  //���ֽǶ��˲��õ���ǰ�Ƕ�
extern float angle_set;                  //ƽ�����
extern float angle_prop;                 //���ٶȼƺͼ��ٶȼƵ�ռ��
extern float speed_cur;                  //��ǰ�ٶ�
extern float speed_set;                  //�趨�ٶ�
extern float speed_add;                  //���ٷ�������δʹ�ã�
extern float speed_motor;                //���ݳ�����������ٶȣ���δʹ�ã�
extern float kp_mid_l, kp_mid_r;         //ʹ����ƫ�Χ����Ƕ�ƫ�Χ��ϵ�����Եõ����յ�bias
extern float dir_max_l,dir_max_r;        //��б��������
extern struct pidCtrl_t Balance_pid;     //ֱ������ת��pid����
extern struct pidCtrl_t_1 Speed_pid;
extern float Balance_pidoutput;          //ֱ����pid�����
extern float Speed_pidoutput;            //�ٶȻ�pid�����
extern float Dir_bias;                   //ת��ƫ��
extern float Angle_bias;                 //�Ƕ�ƫ��
extern float bias;                       //��������ƫ��Ƕ�ƫ���ȥת��ƫ���ת��ƫ��ȥ�޸�ƽ�����angle_set��
extern u16 steer_mid ;                //�����ֵ
extern int steer;                      //���ռ�ձ�
extern u16 menu_flag;                 //���Ʋ˵����ң�0ʱ�˵�������1ʱ�ܳ�
extern u16 sample_flag;                //Ѱ�Ҹ�������ֵ��������һ������
extern u16 control_times;             //��������жϵĴ��������ڻ���Ѱ�Ҹ�·�����ֵ��ʱ��


void ParaInit(void);                             //��ԭʼС������д�뵥Ƭ���Ĵ������ú���ֻ����һ��

#endif
