#include "pidctrl.h"

void PIDCTRL_Setup(struct pidCtrl_t *_pid, float _kp, float _ki, float _kd)
{
  _pid->kp = _kp;
  _pid->ki = _ki;
  _pid->kd = _kd;
  _pid->errCurr = _pid->errIntg = _pid->errIntg = _pid->errPrev = 0.0f;
}

void PIDCTRL_ErrUpdate(struct pidCtrl_t *_pid, float _err)
{
	_pid->errPrev = _pid->errCurr;
  _pid->errCurr = _err;
  _pid->errIntg += _pid->errCurr;
  _pid->errDiff = _pid->errCurr - _pid->errPrev;
}

float PIDCTRL_CalcPIDGain(struct pidCtrl_t *_pid)
{
	return _pid->errCurr * _pid->kp + _pid->errIntg * _pid->ki + _pid->errDiff * _pid->kd;
}

void PIDCTRL_ErrUpdate_1(struct pidCtrl_t_1 *_pid, float _err)
{
	_pid->errPrev2 = _pid->errPrev;
	_pid->errPrev = _pid->errCurr;
	_pid->errCurr = _err;
}

float PIDCTRL_CalcPIDGain_1(struct pidCtrl_t_1 *_pid)
{
	return _pid->pidPrev + _pid->kp * (_pid->errCurr - _pid->errPrev) + _pid->ki * _pid->errCurr + _pid->kd * (_pid->errCurr - 2*_pid->errPrev + _pid->errPrev2);
}
