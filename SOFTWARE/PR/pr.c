#include "pr.h"
#include "global.h"
#include "math.h"

void pr_init(prtype *pr, float kp, float kr, float w0, float wc, float ts) {
  // 初始化设定值和实际值
  pr->set_value = 0.0;
  pr->actual_value = 0.0;

  // 设置准PR控制器参数
  pr->kp = kp; // 比例增益
  pr->kr = kr; // 谐振增益
  pr->w0 = w0; // 谐振频率 (通常为基波频率，如50Hz对应314.16 rad/s)
  pr->wc = wc; // 截止频率
  pr->ts = ts; // 采样时间

  // 计算中间变量
  float a = 2.0f / ts;
  float a_sq = a * a;
  float w0_sq = w0 * w0;
  float b0 = 2.0f * kr * wc * a;

  // 计算分母系数
  float a0 = a_sq + 2.0f * wc * a + w0_sq;
  float a1 = -2.0f * a_sq + 2.0f * w0_sq;
  float a2 = a_sq - 2.0f * wc * a + w0_sq;

  // 归一化系数
  pr->b0_prime = b0 / a0;
  pr->b2_prime = -b0 / a0; // b2 = -b0
  pr->a1_prime = a1 / a0;
  pr->a2_prime = a2 / a0;

  // 初始化状态变量
  pr->error_prev1 = 0.0f;
  pr->error_prev2 = 0.0f;
  pr->yr_prev1 = 0.0f;
  pr->yr_prev2 = 0.0f;
}

float pr_realize(prtype *pr, float set, float actual)
{
  // 更新设定值和实际值
  pr->set_value = set;
  pr->actual_value = actual;
  
  // 计算当前误差
  float error = pr->set_value - pr->actual_value;

  // 计算谐振部分输出
  float yr = pr->b0_prime * error
           + pr->b2_prime * pr->error_prev2
           - pr->a1_prime * pr->yr_prev1
           - pr->a2_prime * pr->yr_prev2;

  // 比例部分 + 谐振部分
  float output = pr->kp * error + yr;

  // 更新状态变量
  pr->error_prev2 = pr->error_prev1;
  pr->error_prev1 = error;
  pr->yr_prev2 = pr->yr_prev1;
  pr->yr_prev1 = yr;

  return output;
}

float pr_limited(prtype *pr, float set, float actual, float min,
         float max) {
  // 更新设定值和实际值
  pr->set_value = set;
  pr->actual_value = actual;
  
  // 计算当前误差
  float error = pr->set_value - pr->actual_value;

  // 计算谐振部分输出
  float yr = pr->b0_prime * error
           + pr->b2_prime * pr->error_prev2
           - pr->a1_prime * pr->yr_prev1
           - pr->a2_prime * pr->yr_prev2;

  // 比例部分 + 谐振部分
  float output = pr->kp * error + yr;

  // 限幅处理
  if (output > max) {
    output = max;
  }
  else if (output < min) {
    output = min;
  }

  // 更新状态变量
  pr->error_prev2 = pr->error_prev1;
  pr->error_prev1 = error;
  pr->yr_prev2 = pr->yr_prev1;
  pr->yr_prev1 = yr;

  return output;
}