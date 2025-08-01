#include "stm32f4xx.h"
#include "global.h"

//标志位变量
uint16_t pll_is_locked=0;
char test_flag=0;
char zero_flag=0;
char on_off=0;
char reset_flag=0;
char mode=2;    //变流器1（逆变）模式；变流器2（整流）模式
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
float  sample2real_k[8] = {-0.1062f, -0.1062f, 1.9569f, 1.9295f, 1.0f, 1.0f, 1.0f, 1.0f};
float  sample2real_b[8]={0.000f,0.000f,0.000f,-0.17956f,0.13811f,0.0f,0.0f,0.0f};
float  Compensation[8]={0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};

//外部库
prtype pr1,pr2,pr3,pr4;
pidtype pid1,pid2,pid3,pid4,pid5,pid6;

//锁相变量
plltype  pll_v,pll_v3;

//输入信号
//电压采样
float ac_voltage_ab=0.0f;
float ac_voltage_bc=0.0f;
float ac_voltage_ca=0.0f;

float ac_voltage_a=0.0f;
float ac_voltage_b=0.0f;
float ac_voltage_c=0.0f;

float ac_voltage_alpha = 0;
float ac_voltage_beta = 0;
float ac_voltage_d = 0;
float ac_voltage_q = 0;

float ac_voltage_peak = 0.0f;
float ac_vrms=0.0f;

//电流采样
float ac_current_a = 0.0f;
float ac_current_b = 0.0f;
float ac_current_c = 0.0f;

float ac_current_alpha = 0;
float ac_current_beta = 0;
float ac_current_d = 0;
float ac_current_q = 0;

float ac_current_peak = 0.0f;
float ac_crms = 0.0f;


//期望输出
float target_frequent=50.0f;

float target_ac_vrms = 32.0f;
float target_ac_current=2.0f;

//调制波形输出
float outputwave_ac_current_d = 0.0f;
float outputwave_ac_current_q = 0.0f;

//电压内角（离网模式）
float internal_theta=0.0f;
float theta=0;
float omega=h_pi;

//显示变量
unsigned char menu_index=0;
unsigned char input_buffer[10]={0};
unsigned char input_index = 0;
unsigned disp_page=0;