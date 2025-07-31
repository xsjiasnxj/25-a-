#include "main.h"
#include "AD7606.h"
#include "OLED.h"
#include "Vofa.h"
#include "arm_math.h"
#include "buzzer.h"
#include "delay.h"
#include "exti.h"
#include "global.h"
#include "gpio.h"
#include "key.h"
#include "math.h"
#include "pll.h"
#include "sogi.h"
#include "spi.h"
#include "stm32f4xx.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"

/***********************************************************************
 * @ 函数名  ： bsp_Init
 * @ 功能说明： 所有外设初始化函数，方便管理
 * @ 参数    ：
 * @ 返回值  ： 无
 *********************************************************************/
void bsp_Init() {

  //para1:svpwmtype, para2:sqrt3*u_alpha_peak, para3:2*max_duty

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // 设置系统中断优先级分组 4
  delay_init(168);
  AD7606_Init();
  SPI2_Init();
  GPIO_Config_Init();
  EXTIX_Init();
  OLED_Init();
  sogi_init(&sogi1,Ts,omega);
  uart_init(2000000);
  notch_filter_init(&nf1,200*M_PI,0.707,Ts);
  TIM1_Config_Init(); // 专门用于PWM输出
  TIM2_Config_Init(); // 专门用于数据采样+计算+控制
  TIM3_Config_Init(); // 专门用于显示
    
  pid_init(&pid1, kp1, ki1, kd1);
  pr_init(&pr1,kp_pr1,kr_pr1,100*M_PI,2*M_PI,Ts);
  pr_init(&pr2 ,kp_pr2,kr_pr2,100*M_PI,2*M_PI,Ts);   
  pll_init(&pll_v, Ts, 100, 80);
  svpwm_init(&svpwm1,30, Ts);

  OLED_Clear();
}

int main(void) {
  bsp_Init();
  while (1) {

      
  }
}
