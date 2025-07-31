#ifndef __KALMAN_H
#define __KALMAN_H

// 定义状态处理器结构体
typedef struct
{
    float current_state; // 当前状态值（data）
    float target;        // 目标值
    float step;          // 调整步长
    float hysteresis;    // 滞后带宽
} SlopeOutProcessor;

/**
 * @brief 初始化状态处理器
 * @param proc 处理器指针
 * @param initial_state 初始状态值
 * @param target 目标值
 * @param step 调整步长
 * @param hysteresis 滞后带宽
 */
void SlopeOut_Init(SlopeOutProcessor *proc, float initial_state,
                   float target, float step, float hysteresis);

/**
 * @brief 执行带延迟反馈的步进调整
 * @param proc 处理器指针
 * @param shuru 当前输入信号
 * @return 新的输出值
 *
 * 说明：输出值会自动存储为下次调用的状态值
 */
float SlopeOut_Step(SlopeOutProcessor *proc, float shuru);

/**
 * @brief 重置处理器状态（不改变配置参数）
 * @param proc 处理器指针
 * @param new_state 新的初始状态
 */
void SlopeOut_Reset(SlopeOutProcessor *proc, float new_state);

#endif