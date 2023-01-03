/*********************************************************************************************************************
 * @writer        杨浩淼
 * @date       		2021-1-14
 * @brief					位置式PID控制算法
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

//位置式pid
/**
 * @brief 设置PID结构体参数
 *
 * @param _pid  需要被设置的PID指针
 * @param _kp  比例系数
 * @param _ki  积分系数
 * @param _kd  微分系数
 */
void PIDCTRL_Setup(struct pidCtrl_t *_pid, float _kp, float _ki, float _kd);

/**
 * @brief 更新PID偏差
 *
 * @param _pid  PID参数结构指针
 * @param _err  当前偏差
 */
void PIDCTRL_ErrUpdate(struct pidCtrl_t *_pid, float _err);

/**
 * @brief 计算PID总输出
 *
 * @param _pid  PID参数结构指针
 * @return float PID结果输出
 */
float PIDCTRL_CalcPIDGain(struct pidCtrl_t *_pid);
//增量式pid
/**
 * @brief 更新PID偏差
 *
 * @param _pid  PID参数结构指针
 * @param _err  当前偏差
 */
void PIDCTRL_ErrUpdate_1(struct pidCtrl_t_1 *_pid, float _err);

/**
 * @brief 计算PID总输出
 *
 * @param _pid  PID参数结构指针
 * @return float PID结果输出
 */
float PIDCTRL_CalcPIDGain_1(struct pidCtrl_t_1 *_pid);
#endif
