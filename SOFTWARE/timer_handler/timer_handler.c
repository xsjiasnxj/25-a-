#include "stm32f4xx.h"
#include "timer_handler.h"
#include "global.h"
#include "AD7606.h"
#include "main.h"
#include "spwm.h"
#include "OLED.h"
#include "gpio.h"
#include "sys.h"
#include "transform.h"
#include "arm_math.h"
#include "Vofa.h"
#include "MENU.h"
#include "control.h"

// 计数变量
int cnt = 0;
int cnt_s = 0;
int init_cnt = 0;

// Ensure buffer and buffer_index are properly declared and initialized
static char buffer[32] = {0}; // Buffer to store input characters
static int buffer_index = 0;  // Index for the buffer
//// 过零比较逻辑优化，使用施密特触发器
//static int zero_crossing_state = 0; // 0: 未过零, 1: 正过零, -1: 负过零
//const float threshold_high = 0.1f;  // 高阈值
//const float threshold_low = -0.1f;  // 低阈值
// 计算有效值，取200点
static float32_t voltage_samples[1000];
static uint32_t sample_index = 0;

// 采样 + 控制
void TIM2_IRQHandler(void) {
     /********************** 作调试用 ************************************/
cnt++;
    /*******************************************************************/
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) { // 溢出中断
    
    AD7606_read();
    internal_theta = internal_theta + 2*M_PI*(target_frequent) * Ts; 
    if(internal_theta>=2*M_PI)
    {
        internal_theta-=2*M_PI;
    }
    // 采样数据
    ac1_voltage_ab = adc_real[0];
    ac1_voltage_bc = adc_real[2];
    // 将采样值存入数组
    voltage_samples[sample_index++] = ac1_voltage_ab;

    // 如果采样数组已满，计算RMS值
    if (sample_index >= 1000) {
      arm_rms_f32(voltage_samples, 1000, &ac1_vrms);
      sample_index = 0; // 重置采样索引
    }
    if(on_off&&reset_flag==1)
    {
        reset_svpwm();
        reset_flag=0;

    }
    if(on_off&&reset_flag==0)
    {
      control();
      start_svpwm();
    }
    if(on_off==0)
    {
      pid_init(&pid1, kp1, ki1, kd1);
      stop_svpwm();
      reset_flag=1;
    }
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
}

// 按键检测 + 显示
void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) { // 溢出中断
      vofa_databuffer[0]=ac1_vrms;
      Vofa_JustFloat(&vofa1,vofa_databuffer,1);
     
        OLED_Clear();
        key_command_callback(&key1); //按键回调函数，返回键值
        //键入设定值
        if ((key1.key_num >= '0' && key1.key_num <= '9') || key1.key_num == '.') {
            if (buffer_index < sizeof(buffer) - 1) {
            buffer[buffer_index++] = key1.key_num;
            buffer[buffer_index] = '\0'; // Null-terminate the string
            }
        } else if (key1.key_num == 'S') {
            if (buffer_index > 0) {
            char *endptr;
            float temp_value = strtof(buffer, &endptr); // Convert string to float with validation
            if (*endptr == '\0') { // Check if the entire string was converted
                target_frequent = temp_value;
            } else {
                // Handle invalid input (e.g., show an error message or reset the buffer)
                OLED_ShowString(0, 0, "Invalid Input", OLED_8X16);
            }
            buffer_index = 0; // Reset buffer index
            memset(buffer, 0, sizeof(buffer)); // Clear the buffer
            }
        } else if (key1.key_num == 'B') {
            if (buffer_index > 0) {
            buffer[--buffer_index] = '\0'; // Remove last character
            }
        }else if(key1.key_num == 'N')
        {
            target_frequent+=1;
        }
        else if(key1.key_num == 'F')
        {
            target_frequent-=1;
        }
        
        OLED_ShowString(0,0,"F:",OLED_8X16);OLED_ShowString(108,0,"HZ",OLED_8X16);
        // Display logic
        if (buffer_index > 0) {
            OLED_ShowString(48, 0, buffer, OLED_8X16); // Show buffer
            OLED_ReverseArea(48, 0, 108, 16);
        } else {
            if (target_frequent < 100 && target_frequent > 20) {
            OLED_ShowFloatNum(48, 0, target_frequent, 2, 2, OLED_8X16); // Show target value
            } else {
            OLED_ShowString(48, 0, "ERR", OLED_8X16); // Show error if out of range
            }
        }
        
        OLED_ShowString(0,16,"ON/OFF:",OLED_8X16);
        if(on_off) OLED_ShowString(64,16,"ON",OLED_8X16);
        else OLED_ShowString(64,16,"OFF",OLED_8X16);
       
        OLED_Update();
    
  }
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除中断标志位
}

