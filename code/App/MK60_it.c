/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,山外科技
*     All rights reserved.
*     技术讨论：山外论坛 http://www.vcan123.com
*
*     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
*     修改内容时必须保留山外科技的版权声明。
*
* @file       MK60_it.c
* @brief      山外K60 平台中断服务函数
* @author     山外科技
* @version    v5.0
* @date       2013-06-26
*/
#include    "MK60_it.h"

/*********************************中断服务函数********************************/

/*!
*  @brief      PORTA中断服务函数
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
  uint8  n;    //引脚号
  uint32 flag;
  
  flag = PORTA_ISFR;
  PORTA_ISFR  = ~0;                                   //清中断标志位
  
  n = 29;                                             //场中断
  if(flag & (1 << n))                                 //PTA29触发中断
  {
    camera_vsync();
  }
#if ( CAMERA_USE_HREF == 1)                            //使用行中断
  n = 28;
  if(flag & (1 << n))                                 //PTA28触发中断
  {
    camera_href();
  }
#endif
}

/*!
*  @brief      PORTE中断服务函数
*  @since      v5.0
*/
void PORTE_IRQHandler()
{
  uint8  n;    //引脚号
  uint32 flag;
  
  flag = PORTE_ISFR;
  PORTE_ISFR  = ~0;                                   //清中断标志位
  
  n = 27;
  if(flag & (1 << n))                                 //PTE27触发中断
  {
    nrf_handler();
  }
}

/*!
*  @brief      DMA0中断服务函数
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
  camera_dma();
}

/*!
*  @brief      PIT0中断服务函数
*  @since      v5.0
*/
void PIT0_IRQHandler()	// 10ms进一次中断
{
  MOTOR_measure();
  Left_motor_pid(Left_motor_speed);
  Right_motor_pid(Right_motor_speed);

  ftm_pwm_duty(MOTOR_FTM, MOTOR1_CH, MOTOR1_DUTY);//左轮前
  ftm_pwm_duty(MOTOR_FTM, MOTOR2_CH, MOTOR2_DUTY);//左轮后  
  ftm_pwm_duty(MOTOR_FTM, MOTOR3_CH, MOTOR3_DUTY);//右轮前
  ftm_pwm_duty(MOTOR_FTM, MOTOR4_CH, MOTOR4_DUTY);//右轮后 
  
  PIT_Flag_Clear(PIT0);//清除定时器中断标志位
}


/*!
*  @brief      PIT1中断服务函数
*  @since      v5.0
*/
void PIT1_IRQHandler()
{
  
  PIT_Flag_Clear(PIT1);//清除定时器中断标志位
}