#ifndef __MK60_IT_H__
#define __MK60_IT_H__

/*                          重新定义中断向量表
 *  先取消默认的中断向量元素宏定义        #undef  VECTOR_xxx
 *  在重新定义到自己编写的中断函数      #define VECTOR_xxx    xxx_IRQHandler
 *  例如：
 *       #undef  VECTOR_003                         先取消映射到中断向量表里的中断函数地址宏定义
 *       #define VECTOR_003    HardFault_Handler    重新定义硬件上访中断服务函数
 */

#include    "common.h"
#include    "include.h"


void PORTA_IRQHandler(); //摄像头场中断服务函数
void PORTE_IRQHandler(); //NRF中断服务函数
void DMA0_IRQHandler();  //摄像头DMA中断服务函数
void PIT0_IRQHandler();  //定时函数*****按键检测&停车检测&灯塔检测****
void PIT1_IRQHandler();  //定时函数 定时起跑

#endif
