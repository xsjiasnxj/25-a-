#include <math.h>
#include "IIR2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/**
 * 计算双线性变换系数
 * @param filter: 滤波器结构体指针
 * @param wn: 陷波中心角频率
 * @param df: 阻尼系数（0-1）
 * @param Ts: 采样周期 (秒)
 * @return: 0成功，-1失败
 */

int notch_filter_init(NotchFilter *filter, float wn, float df,float Ts)
{
    if (!filter || wn <= 0 || df<= 0 || df >= 1 || Ts <= 0)
    {
        return -1; // 参数错误
    }
    // 计算预校正频率
    float w_pr = (2.0f / Ts) * tanf(wn * Ts / 2.0f);

    // 使用双线性变换 (Tustin变换) s = 2/Ts * (z-1)/(z+1)
    float W2 = w_pr * w_pr;
    float T2 = Ts * Ts;
    // 归一化系数
    float a0 = T2 * W2 + 4.0 * df * Ts * w_pr + 4.0;
    // 分子系数
    filter->b2 = (T2 * W2 + 4) / a0;
    filter->b1 = (2 * T2 * W2 - 8) / a0;
    filter->b0 = (T2 * W2 + 4) / a0;

    // 分母系数
    filter->a2 = (T2 * W2 - 4 * df * Ts * w_pr + 4) / a0;
    filter->a1 = (2 * T2 * W2 - 8) / a0;

    // 初始化延迟元素
    filter->x1 = filter->x2 = 0.0;
    filter->y1 = filter->y2 = 0.0;
    filter->initialized = 1;

    return 0;
}

/**
 * 处理单个样本
 * @param filter: 滤波器结构体指针
 * @param input: 输入样本
 * @return: 滤波后的输出样本
 */
float notch_filter_process(NotchFilter *filter, float input)
{
    if (!filter || !filter->initialized)
    {
        return input;
    }

    // 差分方程: y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
    float output = filter->b0 * input +
                   filter->b1 * filter->x1 +
                   filter->b2 * filter->x2 -
                   filter->a1 * filter->y1 -
                   filter->a2 * filter->y2;

    // 更新延迟元素
    filter->x2 = filter->x1;
    filter->x1 = input;
    filter->y2 = filter->y1;
    filter->y1 = output;

    return output;
}

/**
 * 批量处理样本
 */
int notch_filter_process_block(NotchFilter *filter, const float *input,
                               float *output, int length)
{
    if (!filter || !input || !output || length <= 0)
    {
        return -1;
    }

    for (int i = 0; i < length; i++)
    {
        output[i] = notch_filter_process(filter, input[i]);
    }

    return 0;
}

/**
 * 重置滤波器状态
 */
void notch_filter_reset(NotchFilter *filter)
{
    if (filter)
    {
        filter->x1 = filter->x2 = 0.0;
        filter->y1 = filter->y2 = 0.0;
    }
}
