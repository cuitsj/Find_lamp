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

/*!
*  @brief      PIT1中断服务函数
*  @since      v5.0
*/
void PIT1_IRQHandler()
{

    updata_var(VAR1);
    var_display(VAR1); 
    updata_var(VAR2);
    var_display(VAR2); 
//    updata_var(VAR3);
//    var_display(VAR3); 
//    updata_var(VAR4);
//    var_display(VAR4); 
//    updata_var(VAR5);
//    var_display(VAR5); 
  
  PIT_Flag_Clear(PIT1);//清除定时器中断标志位
}

