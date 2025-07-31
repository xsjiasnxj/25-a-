#include "slopeout.h"

SlopeOutProcessor sp1;
/**
 * @brief 初始化状态处理器
 * @param proc 处理器指针
 * @param initial_state 初始状态值
 * @param target 目标值
 * @param step 调整步长
 * @param hysteresis 滞后带宽
 */
void SlopeOut_Init(SlopeOutProcessor *proc, float initial_state,
                   float target, float step, float hysteresis)
{
    proc->current_state = initial_state;
    proc->target = target;
    proc->step = step;
    proc->hysteresis = hysteresis;
}

/**
 * @brief 执行带延迟反馈的步进调整
 * @param proc 处理器指针
 * @param shuru 当前输入信号
 * @return 新的输出值
 *
 * 说明：输出值会自动存储为下次调用的状态值
 */
float SlopeOut_Step(SlopeOutProcessor *proc, float shuru)
{
    float combined = shuru + proc->current_state;
    float output;

    if (combined < (proc->target - proc->hysteresis))
    {
        output = combined + proc->step;
    }
    else if (combined > (proc->target + proc->hysteresis))
    {
        output = combined - proc->step;
    }
    else
    {
        output = proc->target;
    }

    // 更新状态：将输出存储为下一次的输入状态 (1/z延迟)
    proc->current_state = output;

    return output;
}

/**
 * @brief 重置处理器状态（不改变配置参数）
 * @param proc 处理器指针
 * @param new_state 新的初始状态
 */
void SlopeOut_Reset(SlopeOutProcessor *proc, float new_state)
{
    proc->current_state = new_state;
}
