#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common.h"
#include "include.h"

extern uint8 road_mid;     //图像处理最终得到的赛道中点
extern uint8 img[]; 
extern uint8 light_ln;


uint8 find_col();                                //找到灯的列数
uint8 find_ln(uint8 col);                                //找到灯的行数
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);        //解压

#endif