#include "wy_headfile.h"

cardata CAR[PARA_MAX];   


int mode = 0;
float angle_accl = 0.0f;
float angle_gyro = 0.0f;
float angle_cur = 0.0f;
float angle_set = 0.0f;
float angle_prop = 1.2f;
float speed_cur = 0.0f;
float speed_set = 170.0f;
float speed_add = 120.0f;
float speed_motor = 0.0f;
float kp_mid_l = 10.0f, kp_mid_r = 10.0f;
struct pidCtrl_t Balance_pid = {4.4f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f};
struct pidCtrl_t_1 Speed_pid = {9.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f};
float Balance_pidoutput = 0.0f;
float Speed_pidoutput = 0.0f;
float Dir_bias = 0;
float Angle_bias = 0.0f;
float dir_max_l = 10, dir_max_r = 10;
float bias = 0;
int steer = 0;
u16 menu_flag = 0;
u16 steer_mid = 2220;
u16 sample_flag = 1;   
u16 control_times = 0;



//-------------------------------------------------------------------------------------------------------------------
//  @brief         ԭʼС���������ݴ��뵥Ƭ���Ĵ���
//  @param
//  @attention     �ַ�������û�д��С���㣬���Ǹ����Ͳ���������Ҫ�ȳ���100��ת��
//-------------------------------------------------------------------------------------------------------------------
void ParaInit(void)
{
    mode = CAR[1].intData;
    switch(mode)
    {
    case 0:
        break;
    case 1:                           //����
        angle_set = 3.5;              //�趨�Ƕ�
        Balance_pid.kp = 105.0;
        Balance_pid.ki = 2.5;
        Balance_pid.kd = 30.0;
        speed_set = 190;              //�趨�ٶ�
        Speed_pid.kp = 13;
        Speed_pid.ki = 5;
        Speed_pid.kd = 1;
        kp_mid_l = 57.4;
        dir_max_l = 4.5;
        kp_mid_r = 59.4;
        dir_max_r = 5.7;
        CAR[2].floatData = angle_set;//ע��˳���ɲ����Ͳ˵������˵�˳�����
        CAR[3].intData = steer_mid;
        CAR[4].floatData = Balance_pid.kp;
        CAR[5].floatData = Balance_pid.ki;
        CAR[6].floatData = Balance_pid.kd;
        CAR[7].floatData = angle_prop;
        CAR[8].floatData = speed_set;
        CAR[9].floatData = Speed_pid.kp;
        CAR[10].floatData = Speed_pid.ki;
        CAR[11].floatData = Speed_pid.kd;
        CAR[12].floatData = kp_mid_l;
        CAR[13].floatData = dir_max_l;
        CAR[14].floatData = kp_mid_r;
        CAR[15].floatData = dir_max_r;
        STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&CAR[0].intData, PARA_MAX * sizeof(cardata));// �Ѳ������ݴ��뵥Ƭ���Ĵ���
        break;
    case 2:                           //����ģʽ
        angle_set = 3.5;              //�趨�Ƕ�
        Balance_pid.kp = 85.0;
        Balance_pid.ki = 0.8;
        Balance_pid.kd = 30.0;
        speed_set = 220;              //�趨�ٶ�
        Speed_pid.kp = 13;
        Speed_pid.ki = 5;
        Speed_pid.kd = 1;
        kp_mid_l = 60;
        kp_mid_r = 60;
        dir_max_l = 4.5;
        dir_max_r = 6.1;//5.1
        CAR[2].floatData = angle_set;//ע��˳���ɲ����Ͳ˵������˵�˳�����
        CAR[3].intData = steer_mid;
        CAR[4].floatData = Balance_pid.kp;
        CAR[5].floatData = Balance_pid.ki;
        CAR[6].floatData = Balance_pid.kd;
        CAR[7].floatData = angle_prop;
        CAR[8].floatData = speed_set;
        CAR[9].floatData = Speed_pid.kp;
        CAR[10].floatData = Speed_pid.ki;
        CAR[11].floatData = Speed_pid.kd;
        CAR[12].floatData = kp_mid_l;
        CAR[13].floatData = dir_max_l;
        CAR[14].floatData = kp_mid_r;
        CAR[15].floatData = dir_max_r;
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&CAR[0].intData, PARA_MAX * sizeof(cardata));// �Ѳ������ݴ��뵥Ƭ���Ĵ���
        break;
    case 3:                           //����
        angle_set = 3.5;              //�趨�Ƕ�
        Balance_pid.kp = 75.0;
        Balance_pid.ki = 0.4;
        Balance_pid.kd = 30.0;
        speed_set = 250;              //�趨�ٶ�
        Speed_pid.kp = 13;
        Speed_pid.ki = 5;
        Speed_pid.kd = 1;
        kp_mid_l = 60;
        kp_mid_r = 60;
        dir_max_l = 5.7;
        dir_max_r = 7.0;
        CAR[2].floatData = angle_set;//ע��˳���ɲ����Ͳ˵������˵�˳�����
        CAR[3].intData = steer_mid;
        CAR[4].floatData = Balance_pid.kp;
        CAR[5].floatData = Balance_pid.ki;
        CAR[6].floatData = Balance_pid.kd;
        CAR[7].floatData = angle_prop;
        CAR[8].floatData = speed_set;
        CAR[9].floatData = Speed_pid.kp;
        CAR[10].floatData = Speed_pid.ki;
        CAR[11].floatData = Speed_pid.kd;
        CAR[12].floatData = kp_mid_l;
        CAR[13].floatData = dir_max_l;
        CAR[14].floatData = kp_mid_r;
        CAR[15].floatData = dir_max_r;
        STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&CAR[0].intData, PARA_MAX * sizeof(cardata));// �Ѳ������ݴ��뵥Ƭ���Ĵ���
        break;
    default:	// Ĭ����ģʽ0���г�ʼ������һ����¼�����Դ�Ŀ¼����
		angle_set = 3.5;              //�趨�Ƕ�
        Balance_pid.kp = 75.0;
        Balance_pid.ki = 0.4;
        Balance_pid.kd = 30.0;
        speed_set = 250;              //�趨�ٶ�
        Speed_pid.kp = 13;
        Speed_pid.ki = 5;
        Speed_pid.kd = 1;
        kp_mid_l = 60;
        kp_mid_r = 60;
        dir_max_l = 5.7;
        dir_max_r = 7.0;
		CAR[1].intData = 1;					//ע��˳���ɲ����Ͳ˵������˵�˳�����
        CAR[2].floatData = angle_set;		
        CAR[3].intData = steer_mid;
        CAR[4].floatData = Balance_pid.kp;
        CAR[5].floatData = Balance_pid.ki;
        CAR[6].floatData = Balance_pid.kd;
        CAR[7].floatData = angle_prop;
        CAR[8].floatData = speed_set;
        CAR[9].floatData = Speed_pid.kp;
        CAR[10].floatData = Speed_pid.ki;
        CAR[11].floatData = Speed_pid.kd;
        CAR[12].floatData = kp_mid_l;
        CAR[13].floatData = dir_max_l;
        CAR[14].floatData = kp_mid_r;
        CAR[15].floatData = dir_max_r;
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&CAR[0].intData, sizeof(cardata));// �Ѳ������ݴ��뵥Ƭ���Ĵ���
        break;
    }
}
