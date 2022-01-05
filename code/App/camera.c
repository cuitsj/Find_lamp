#include "camera.h"

uint8 colour[2] = {1, 0}; //0表示黑、1表示红，此处可以改变表示黑白的数字

uint8 find_col()//0表示黑1表示红
{
  
  /********************* 图像扫描区域示意图 **********************
   ___________________________________________________________
  |     scan_upleft->|____scan_downbound____|<-scan_upright    |
  |  ________________|___scan_dividbound__|________________  |
  |  |                                                    |  |
  |  |                                                    |  |
  |  |                                                    |  |
  |  |<-scan_downleft                     scan_downright->|  |
  |  |                                                    |  |
  |  |                                                    |  |
  |  |                                                    |  |
  |  |____________________scan_upbound__________________|  |
  |__________________________________________________________|

   ************************* ending ****************************/
  
  uint8 scan_point, scan_upbound = (uint8)(HMI_flash_ex>>16), scan_downbound = 3, scan_dividbound = 15; //扫描上下界
  uint8 scan_upleft = 20, scan_upright = 60, scan_downleft = 10, scan_downright = 70;
  uint8 get_right_flag[61], get_left_flag[61], get_mid_flag[61]; //丢线标志
  int8 i, j;
  int8 left_bound[61], right_bound[61], mid_point[61];
  uint16 midpoint_sum = 0;
  uint8 midpoint_num = 0;
  uint8 road_midpoint;
  
  for(i=scan_downbound; i<=scan_upbound; i++)//从9到scan_upbound进行扫描
  {
    get_right_flag[i] = 0;
    get_left_flag[i] = 0;
    get_mid_flag[i] = 0; 
    
   if (i < scan_dividbound)
   {
     /********************************** 开始找灯的左边界 **************************************/
    for(j=scan_upleft,scan_point=scan_upleft; j<=scan_upright; j++)//开始找左边界
    {
      if (img[i*80 + j] == 0 && img[i*80 + j + 1] == 1)//找到了边界
      {
        left_bound[i] = j - 1;//找到了左边界
        get_left_flag[i] = 1;          
        break;
      }
      else if (img[i*80 + scan_point] == 0 && img[i*80 + scan_point + 1] == 0)//全黑
      {
        if (scan_point == scan_upright)  //该行没有左边界
        {
          get_left_flag[i] = 0;
          left_bound[i] = 0;    //全黑丢线，默认0
          break;
        }
        scan_point++;
      }
      else if(img[i*80 + j] == 1)//红点在起始列上
      {
          get_left_flag[i] = 1;
          left_bound[i] = scan_upleft;  
          break;
      }
    }
/********************************** 找灯左边界结束 **************************************/
/********************************** 开始找灯的右边界 **************************************/
    for(j=scan_upright,scan_point=scan_upright; j>=scan_upleft; j--)//开始找右边界
    {
      if (img[i*80 + j] == 0 && img[i*80 + j - 1] == 1)
      {
        get_right_flag[i] = 1;
        right_bound[i] = j + 1;
        break; 
      }
      else if (img[i*80 + scan_point] == 0 && img[i*80 + scan_point - 1] == 0)//全黑
      {
        if (scan_point == scan_upleft) // 该行无右边界
        {
          right_bound[i] = 0;//全黑丢线，默认0
          get_right_flag[i] = 0;
          break;
        }
        scan_point--;
      }
      else if (img[i*80 + j] == 1)//右边界在起始线上
      {
        get_right_flag[i] = 1;
        right_bound[i] = scan_upright;
        break; 
      }
    }            
/********************************** 找灯的右边界结束 **************************************/          
   }
   else if (i >= scan_dividbound)
   {
      /********************************** 开始找灯的左边界 **************************************/
          for(j=scan_downleft,scan_point=scan_downleft; j<=scan_downright; j++)//开始找左边界
          {
            if (img[i*80 + j] == 0 && img[i*80 + j + 1] == 1)//找到了边界
            {
              left_bound[i] = j - 1;//找到了左边界
              get_left_flag[i] = 1;          
              break;
            }
            else if (img[i*80 + scan_point] == 0 && img[i*80 + scan_point + 1] == 0)//全黑
            {
              if (scan_point == scan_downright)  //该行没有左边界
              {
                get_left_flag[i] = 0;
                left_bound[i] = 0;    //全黑丢线，默认0
                break;
              }
              scan_point++;
            }
            else if(img[i*80 + j] == 1)//红点在起始列上
            {
                get_left_flag[i] = 1;
                left_bound[i] = scan_downleft;  
                break;
            }
          }
      /********************************** 找灯左边界结束 **************************************/
      /********************************** 开始找灯的右边界 **************************************/
          for(j=scan_downright,scan_point=scan_downright; j>=scan_downleft; j--)//开始找右边界
          {
            if (img[i*80 + j] == 0 && img[i*80 + j - 1] == 1)
            {
              get_right_flag[i] = 1;
              right_bound[i] = j + 1;
              break; 
            }
            else if (img[i*80 + scan_point] == 0 && img[i*80 + scan_point - 1] == 0)//全黑
            {
              if (scan_point == scan_downleft) // 该行无右边界
              {
                right_bound[i] = 0;//全黑丢线，默认0
                get_right_flag[i] = 0;
                break;
              }
              scan_point--;
            }
            else if (img[i*80 + j] == 1)//右边界在起始线上
            {
              get_right_flag[i] = 1;
              right_bound[i] = scan_downright;
              break; 
            }
          }            
      /********************************** 找灯的右边界结束 **************************************/              
   }
   /********************************** 处理该行找线结果 **************************************/
          if (get_left_flag[i] == 1 && get_right_flag[i] == 1)              //得到左右边界中点
          {
            mid_point[i] = (left_bound[i] + right_bound[i])/2;
            get_mid_flag[i] = 1;
          }
      /********************************** 该行找线结果处理完毕 **************************************/
        }//进入下一行 
 for (i=scan_downbound; i<=scan_upbound; i++)
 {
     if (get_mid_flag[i] == 1)
      {
        midpoint_sum += mid_point[i];
        midpoint_num++;
      }
 }
 
 if (midpoint_num == 0)
 {
   road_midpoint = 0;
 }
 else if (midpoint_num > 0)
 {
   road_midpoint = midpoint_sum/midpoint_num;
 }
 return road_midpoint;
}

uint8 find_ln(uint8 col)//0表示黑1表示红
{
  uint8 i,upbound = 0, downbound = 0;
  
  for(i=3; i<=(uint8)(HMI_flash_ex>>16); i++)//开始找左边界
  {
    if (img[i*80 + col] == 0 && img[(i+1)*80 + col] == 1)//找到了边界
    {
      upbound = i;
    }
    if (img[i*80 + col] == 1 && img[(i+1)*80 + col] == 0)//找到了边界
    {
      downbound = i;
      break;
    }
  }
  if (upbound == 0 || downbound == 0)
  {
    for(i=3; i<=(uint8)(HMI_flash_ex>>16); i++)//开始找左边界
    {
      if (img[i*80 + col - 1] == 0 && img[(i+1)*80 + col - 1] == 1)//找到了边界
      {
        upbound = i;
      }
      if (img[i*80 + col - 1] == 1 && img[(i+1)*80 + col - 1] == 0)//找到了边界
      {
        downbound = i;
        break;
      }
    }
  }
  if (upbound == 0 || downbound == 0)
  {
    for(i=3; i<=(uint8)(HMI_flash_ex>>16); i++)//开始找左边界
    {
      if (img[i*80 + col + 1] == 0 && img[(i+1)*80 + col + 1] == 1)//找到了边界
      {
        upbound = i;
      }
      if (img[i*80 + col + 1] == 1 && img[(i+1)*80 + col + 1] == 0)//找到了边界
      {
        downbound = i;
        break;
      }
    }
  }
  
    
  return (upbound + downbound)/2;
}

/*!
*  @brief      二值化图像解压（空间 换 时间 解压）
*  @param      dst             图像解压目的地址
*  @param      src             图像解压源地址
*  @param      srclen          二值化图像的占用空间大小
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  while(srclen --)
  {
    tmpsrc = *src++;
    *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}

