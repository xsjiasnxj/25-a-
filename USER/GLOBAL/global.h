#ifndef	__GLOBAL_H__
#define	__GLOBAL_H__

#include "pid.h"
#include "pll.h"
#include "sogi.h"
#include "Vofa.h"
#include "key.h"
#include "pr.h"
#include "IIR2.h"
#include "svpwm.h"

#define h_pi 					314.15926535f
#define M_PI 					3.1415926535f
#define sqrt2 					1.41421356f

#define Ts 			            0.0001f         //采样时间(2kHZ)
#define disp_time   			0.01f

#define MI_MAX                  0.98f           //调制比最大值
//#define pid_time   			    0.001f


//频率设置
#define CKTIM	        ((u32)168000000uL)  //主频
#define TIM_CKTIM	    ((u32) 84000000uL)  //定时器频率
#define PWM_PRSC        ((u8)0)            //TIM3分频系数
#define PWM_FREQ        ((u16) 10000)      //PWM频率(Hz)
#define PWM_PERIOD      ((u16) (TIM_CKTIM / (u32)( 2*PWM_FREQ *(PWM_PRSC+1))))  //因为是中心对齐计数模式，所以要乘2
#define pwm_full_duty    PWM_PERIOD
//采样+控制
#define TIM2_PRSC		((u8)4)
#define TIM2_PERIOD     ((u16) (TIM_CKTIM / (u32)(1.0f/Ts*(TIM2_PRSC+1))))
//显示
#define TIM3_PRSC		((u8)400)
#define TIM3_PERIOD     ((u16) (TIM_CKTIM / (u32)(1.0f/disp_time*(TIM3_PRSC+1))))


////控制
//#define TIM4_PRSC		((u8)4)
//#define TIM4_PERIOD  ((u16) (CKTIM / (u32)( 1.0f/pid_time*(TIM4_PRSC+1))))

//pid参数


#define kp1 0.0005f
#define ki1 0.001f
#define kd1 0.0000f


#define kp2 2.0f
#define ki2 10.0f
#define kd2 0.0f

#define kp3 2.0f
#define ki3 10.0f
#define kd3 0.0f



//标志位变量
extern uint16_t pll_is_locked;
extern char test_flag;
extern char on_off;
extern char mode;
extern char zero_flag;
extern char reset_flag;
extern char protected_flag;
extern char precharge;
//vofa
extern Vofa_HandleTypedef vofa1;
extern float vofa_databuffer[8];
//按键
extern Key_HandleTypeDef key1;
//采样变量
extern uint16_t adc_buffer[8];
extern float adc_real[8];

extern uint16_t test_num;
extern uint16_t filter_cnt;
extern uint16_t timer_cnt;


//采样还原参数(k,b，补偿)
extern float  sample2real_k[8];
extern float  sample2real_b[8];
extern float  Compensation[8];

//外部库
extern prtype pr1,pr2,pr3,pr4;
extern pidtype pid1,pid2,pid3,pid4,pid5,pid6;

//锁相变量
extern plltype  pll_v,pll_v3;
// 输入信号
//电压采样
extern float ac_voltage_ab;
extern float ac_voltage_bc;
extern float ac_voltage_ca;

extern float ac_voltage_a;
extern float ac_voltage_b;
extern float ac_voltage_c;

extern float ac_voltage_alpha;
extern float ac_voltage_beta;
extern float ac_voltage_d;
extern float ac_voltage_q;

extern float ac_voltage_peak;
extern float ac_vrms;

//电流采样
extern float ac_current_a;
extern float ac_current_b;
extern float ac_current_c;

extern float ac_current_alpha;
extern float ac_current_beta;
extern float ac_current_d;
extern float ac_current_q;

extern float ac_current_peak;
extern float ac_crms;

//期望输出
extern float target_frequent;
extern float target_ac_vrms;
extern float target_ac_current;

// 调制波形输出
extern float outputwave_ac_current_d;
extern float outputwave_ac_current_q;

//电压内角（离/并网模式）
extern float internal_theta;
extern float theta;
extern float omega;

//显示变量
extern unsigned char menu_index;
extern unsigned char input_buffer[10];
extern unsigned char input_index;
extern unsigned disp_page;

#endif