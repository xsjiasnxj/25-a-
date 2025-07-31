#ifndef __PR_H__
#define __PR_H__



typedef struct
{
  float set_value;    // 设定值
  float actual_value; // 实际值

  // 准PR控制器参数
  float kp; // 比例增益
  float kr; // 谐振增益
  float wc; // 截止频率 (rad/s)
  float w0; // 谐振频率 (rad/s)
  float ts; // 采样时间

  // 离散化系数
  float b0_prime;
  float b2_prime;
  float a1_prime;
  float a2_prime;

  // 状态变量
  float error_prev1;  // e[k-1]
  float error_prev2;  // e[k-2]
  float yr_prev1; // yr[k-1]
  float yr_prev2; // yr[k-2]

} prtype;

void pr_init(prtype *pr, float kp, float kr, float w0, float wc, float ts);
float pr_realize(prtype *pr, float set, float actual);
float pr_limited(prtype *pr, float set, float actual, float min,
                 float max);

#endif