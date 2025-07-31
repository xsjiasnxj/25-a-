#include "stm32f4xx.h"
#include "global.h"
#include "key.h"
#include "IIR2.h"
//标志位变量
uint16_t pll_is_locked=0;
char test_flag=0;
char zero_flag=0;
char on_off=0;
char reset_flag=0;
char mode=0;
char protected_flag=0;
char precharge=1;
//vofa
Vofa_HandleTypedef vofa1;
float vofa_databuffer[8];
//按键
Key_HandleTypeDef key1;

//采样变量
uint16_t adc_buffer[8]={0};
float adc_real[8]={0};

uint16_t test_num;
uint16_t filter_cnt=0;
uint16_t timer_cnt=0;


//采样还原参数(k,b，补偿)
float  sample2real_k[8]={0.10737575f,1.94f,0.1062f,1.0f,1.0f,1.0f,1.0f,1.0f};
float  sample2real_b[8]={0.000f,0.005f,0.000f,0.0f,0.0f,0.0f,0.0f,0.0f};
float  Compensation[8]={0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};

//外部库
prtype pr1,pr2,pr3,pr4;
pidtype pid1,pid2,pid3,pid4,pid5,pid6;
NotchFilter nf1;
svpwmtype svpwm1,svpwm2;
sogitype sogi1;

//锁相变量
plltype  pll_v,pll_v3;

//float input_voltage_test;
//float test_voltage_target=1.854;
//float test_voltage_target_d=1;

float MI=0.5f;      //调制比

float current_rate = 0.5f;//电流比
//输入信号
float ac1_voltage_ab=0.0f;
float ac1_voltage_bc=0.0f;
float ac1_voltage_ca=0.0f;
float ac1_voltage_peak=0.0f;

float ac1_voltage_a=0.0f;
float ac1_voltage_b=0.0f;
float ac1_voltage_c=0.0f;

float ac2_voltage=0.0f;
float ac1_current=0.0f;
float ac1_vrms=0.0f;

//期望输出
float target_frequent=50.0f;

float target_ac1_voltage_a=0.0f;
float target_ac1_voltage_b=0.0f;
float target_ac1_voltage_c=0.0f;

float target_ac1_vrms = 32.0f;
float target_ac2_vrms = 0.0f;
float target_ac1_current=0.0f;


//电压内角（离网模式）
float internal_theta=0.0f;
float theta=0;
float omega=h_pi;

float ac1_voltage_alpha=0;
float ac1_voltage_beta=0;
float ac1_voltage_d=0;
float ac1_voltage_q=0;

float ac2_voltage_alpha=0;
float ac2_voltage_beta=0;
float ac2_voltage_d=0;
float ac2_voltage_q=0;


float ac1_current_alpha=0;
float ac1_current_beta=0;
float ac1_current_d=0;
float ac1_current_q=0;


float ac2_current_alpha=0;
float ac2_current_beta=0;
float ac2_current_d=0;
float ac2_current_q=0;

//调制波形
float output_wave=4100.0f;

//显示变量
unsigned char menu_index=0;
unsigned char input_buffer[10]={0};
unsigned char input_index = 0;
unsigned disp_page=0;