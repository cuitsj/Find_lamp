#ifndef __INIT_H__
#define __INIT_H__
#include "common.h"
#include "include.h"

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)



extern uint8 * imgbuff;             //用于存储NRF传输的图像
extern uint8 nrf_rx_buff[];         //预多,用于存储NRF传输的图像
extern uint8 nrf_tx_buff[];         //预多,用于存储NRF传输的图像
extern uint32 HMI_flash_ex;

void System_Init();		 //所有模块初始化

#endif