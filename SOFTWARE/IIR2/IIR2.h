#ifndef __IIR2_H__
#define __IIR2_H__

// 陷波滤波器状态结构体
typedef struct
{
    float x1, x2;     // 输入延迟 x[n-1], x[n-2]
    float y1, y2;     // 输出延迟 y[n-1], y[n-2]
    float b0, b1, b2; // 分子系数
    float a1, a2;     // 分母系数 (a0=1已归一化)
    int initialized;  // 初始化标志
} NotchFilter;

int notch_filter_init(NotchFilter *filter, float wn, float df, float Ts);
float notch_filter_process(NotchFilter *filter, float input);
int notch_filter_process_block(NotchFilter *filter, const float *input,
                               float *output, int length);
void notch_filter_reset(NotchFilter *filter);

#endif
