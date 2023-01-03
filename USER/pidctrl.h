/*********************************************************************************************************************
 * @writer        ����
 * @date       		2021-1-14
 * @brief					λ��ʽPID�����㷨
 ********************************************************************************************************************/

#ifndef __PIDCTRL_H_
#define __PIDCTRL_H_

struct pidCtrl_t
{
  float kp, ki, kd;
  float errCurr, errIntg, errDiff, errPrev;
};
struct pidCtrl_t_1
{
  float kp, ki, kd;
  float errCurr, errPrev, errPrev2, pidPrev;	
};

//λ��ʽpid
/**
 * @brief ����PID�ṹ�����
 *
 * @param _pid  ��Ҫ�����õ�PIDָ��
 * @param _kp  ����ϵ��
 * @param _ki  ����ϵ��
 * @param _kd  ΢��ϵ��
 */
void PIDCTRL_Setup(struct pidCtrl_t *_pid, float _kp, float _ki, float _kd);

/**
 * @brief ����PIDƫ��
 *
 * @param _pid  PID�����ṹָ��
 * @param _err  ��ǰƫ��
 */
void PIDCTRL_ErrUpdate(struct pidCtrl_t *_pid, float _err);

/**
 * @brief ����PID�����
 *
 * @param _pid  PID�����ṹָ��
 * @return float PID������
 */
float PIDCTRL_CalcPIDGain(struct pidCtrl_t *_pid);
//����ʽpid
/**
 * @brief ����PIDƫ��
 *
 * @param _pid  PID�����ṹָ��
 * @param _err  ��ǰƫ��
 */
void PIDCTRL_ErrUpdate_1(struct pidCtrl_t_1 *_pid, float _err);

/**
 * @brief ����PID�����
 *
 * @param _pid  PID�����ṹָ��
 * @return float PID������
 */
float PIDCTRL_CalcPIDGain_1(struct pidCtrl_t_1 *_pid);
#endif
