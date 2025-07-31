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
  uart_init(2000000);
  TIM1_Config_Init(); // 专门用于PWM输出
  TIM2_Config_Init(); // 专门用于数据采样+计算+控制
  TIM3_Config_Init(); // 专门用于显示
    
  pid_init(&pid1, kp1, ki1, kd1);
  pid_init(&pid2, kp2, ki2, kd2);
  pid_init(&pid3, kp3, ki3, kd3);
  pll_init(&pll_v, Ts, 100, 100);


  OLED_Clear();
}

int main(void) {
  bsp_Init();
  while (1) {

      
  }
}
