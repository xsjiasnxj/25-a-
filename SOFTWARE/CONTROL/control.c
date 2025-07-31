#include "stdio.h"
#include "stdlib.h"
#include "global.h"
#include "control.h"
#include "transform.h"
#include "stm32f4xx.h"
#include "main.h"
#include "math.h"
#include "arm_math.h"

float module_rate = 0.93f;
unsigned int full_duty =  pwm_full_duty;
unsigned int set_duty_a = pwm_full_duty / 2;
unsigned int set_duty_b = pwm_full_duty / 2;
unsigned int set_duty_c = pwm_full_duty / 2;

float set_ac_a =0.0f;
float set_ac_b =0.0f;
float set_ac_c=0.0f;

float max_duty = pwm_full_duty * 0.98f;
float min_duty = pwm_full_duty * 0.02f;
float v1_gain=0.0f;
float i_gain=40.0f;
void control()
{
//   	set_ac_a = pr_realize(&pr1,target_ac1_voltage_a,ac1_voltage_a)*2100;
//    set_ac_c = pr_realize(&pr2,target_ac1_voltage_c,ac1_voltage_c)*2100;
//    set_ac_b = (-set_duty_a-set_duty_c)*2100;
	if(mode==1)//逆变模式
	{
		v1_gain = v1_gain - pid_limited(&pid1, target_ac_vrms, ac_vrms, v1_gain, -10000.0f, 10000.0f);
		theta = internal_theta;
		set_ac_a = v1_gain * arm_sin_f32(theta) * 18;
		set_ac_b = v1_gain * arm_sin_f32(theta - M_PI * 2 / 3) * 18;
		set_ac_c = v1_gain * arm_sin_f32(theta + M_PI * 2 / 3) * 18;
	}
	else if(mode==2)//整流模式
	{
		abc_to_dq(ac_current_a, ac_current_b, ac_current_c, theta, &ac_current_d, &ac_current_q);

		outputwave_ac_current_d = outputwave_ac_current_d - pid_limited(&pid2, target_ac_current * sqrt2, ac_current_d, outputwave_ac_current_d, -1000.0f, 1000.0f);
		outputwave_ac_current_q = outputwave_ac_current_q - pid_limited(&pid3, 0, ac_current_q, outputwave_ac_current_q, -1000.0f, 1000.0f);
		dq_to_abc(outputwave_ac_current_d, outputwave_ac_current_q, theta, &set_ac_a, &set_ac_b, &set_ac_c);
		set_ac_a*= i_gain;
		set_ac_b*= i_gain;
		set_ac_c*= i_gain;
	}

}

void start_svpwm()
{
	//pwm3 --> tim1_channel 1
	//pwm2 --> tim1_channel 2
	//pwm3 pwm2互补输出。

    set_duty_a =  (unsigned int)(set_ac_a+pwm_full_duty/2);
    set_duty_b =  (unsigned int)(set_ac_b+pwm_full_duty/2);
	set_duty_c =  (unsigned int)(set_ac_c+pwm_full_duty/2);
    if(set_duty_a > max_duty)
	{
		TIM1->CCR1 = max_duty;
		//TIM1->CCR2 = max_duty;
	}
	else if(set_duty_a < min_duty)
	{
		TIM1->CCR1 = min_duty;
		//TIM1->CCR2 = min_duty;
	}
	else if(set_duty_b > max_duty)
	{
		TIM1->CCR2 = max_duty;
		//TIM1->CCR2 = min_duty;
	}
	else if(set_duty_b < min_duty)
	{
		TIM1->CCR2 = min_duty;
		//TIM1->CCR2 = min_duty;
	}
	else if(set_duty_c > max_duty)
	{
		TIM1->CCR3 = max_duty;
		//TIM1->CCR2 = min_duty;
	}
	else if(set_duty_c < min_duty)
	{
		TIM1->CCR3 = min_duty;
		//TIM1->CCR2 = min_duty;
	}
	else
	{
		TIM1->CCR1 = set_duty_a;
		TIM1->CCR2 = set_duty_b;
        TIM1->CCR3 = set_duty_c;
	}
//    vofa_databuffer[0] = set_duty_a;
//    vofa_databuffer[1] = ac1_vrms;
//    Vofa_JustFloat(&vofa1, vofa_databuffer, 2);
}

void stop_svpwm()
{
    GPIO_SetBits(GPIO_EN0_GPIO_Port, GPIO_EN0_Pin);
    GPIO_SetBits(GPIO_EN1_GPIO_Port, GPIO_EN1_Pin);
    GPIO_SetBits(GPIO_EN2_GPIO_Port, GPIO_EN2_Pin);
    TIM1->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC1NE); // 通道1
    TIM1->CCER &= ~(TIM_CCER_CC2E | TIM_CCER_CC2NE); // 通道2
    TIM1->CCER &= ~(TIM_CCER_CC3E | TIM_CCER_CC3NE); // 通道3
}

void reset_svpwm()
{
	GPIO_ResetBits(GPIO_EN0_GPIO_Port, GPIO_EN0_Pin);
	GPIO_ResetBits(GPIO_EN1_GPIO_Port, GPIO_EN1_Pin);
	GPIO_ResetBits(GPIO_EN2_GPIO_Port, GPIO_EN2_Pin);
	TIM1->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC1NE); // 通道1
	TIM1->CCER |= (TIM_CCER_CC2E | TIM_CCER_CC2NE); // 通道2
	TIM1->CCER |= (TIM_CCER_CC3E | TIM_CCER_CC3NE); // 通道3
}

void protect_svpwm()
{
//	if(rms_current > 3.0f)
//	{
//		stop_svpwm();
//		on_off = 0;
//        
//        pid_init(&pid1, kp1, ki1, kd1);
//        pr_init(&pr1,kp_pr1,kr_pr1,100*M_PI,0.5*M_PI,Ts);
//        
//	}

}
