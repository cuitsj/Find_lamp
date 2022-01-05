#include "common.h"
#include "include.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_rx_buff) + COM_LEN);     //图像地址

//函数声明
void PIT0_IRQHandler();
void PORTE_IRQHandler();

uint8  var1 = 0, var2;
uint16 var3, var4;
uint32 var5=0, var6=0;
uint8  mid_point;

Site_t site     = {120, 90};                           //显示图像左上角位置
Size_t size     = {80, 60};                      //显示区域图像大小
static FATFS fs;    //文件系统
static FIL   src;   //文件
uint8 zoomflag = 0;

void  main(void)
{
  
  pit_init_ms(PIT1, 100);                           	// 初始化PIT1，定时时间为： 20ms
  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler); 	// 设置PIT1的中断服务函数为 PIT1_IRQHandler
  enable_irq (PIT1_IRQn); 				// 使能中断
   
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //图像大小

    com_e     com;
    nrf_result_e nrf_result;

    /************************ 配置 K60 的优先级  ***********************/
    NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
    NVIC_SetPriority(PORTE_IRQn,0);         //配置优先级
    NVIC_SetPriority(PIT0_IRQn,1);          //配置优先级

    /************************ LCD 液晶屏 初始化  ***********************/
    LCD_init();

    /************************ 无线模块 NRF 初始化  ***********************/
    while(!nrf_init());
    //配置中断服务函数
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);//设置 PORTE 的中断服务函数为 PORTE_VECTORn
    enable_irq(PORTE_IRQn);
    nrf_msg_init();                                     //无线模块消息初始化

    /*********************** 按键消息 初始化  ***********************/
//    key_event_init(KEY_MAX);                 //按键消息初始化
//    pit_init_ms(PIT0,10);               //pit 定时中断(用于按键定时扫描)
//    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);    //设置 PIT0 的中断服务函数为 PIT0_IRQHandler
//    enable_irq(PIT0_IRQn);
    
    site.x = 116;
    site.y = 86;
    size.W = 88;
    size.H = 68;
    LCD_rectangle(site, size, WHITE);
    site.x = 120;
    site.y = 90;
    size.W = 80;
    size.H = 60;

    while(1)
    {
        /************************ 无线发送和接收数据  ***********************/
        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
            if(nrf_result == NRF_RESULT_RX_VALID)
            {
                switch(com)
                {
                    case COM_IMG:
                      LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
                      break;
                    case COM_VAR:  
                      site.x = 120;
                      site.y = 90+var2;
                      size.W = 80;
                      size.H = 1;
                      LCD_rectangle(site, size, GREEN);//显示行
                      site.x = 120+var1;
                      site.y = 90;
                      size.W = 1;
                      size.H = 60;
                      LCD_rectangle(site, size, GREEN);//显示列
//                      site.x = 120+var3;
//                      site.y = 90;
//                      size.W = 1;
//                      size.H = 60;
//                      LCD_rectangle(site, size, GREEN);//显示列
//                      site.x = 120+var4;
//                      site.y = 90;
//                      size.W = 1;
//                      size.H = 60;
//                      LCD_rectangle(site, size, GREEN);//显示列
//                      site.x = 120;
//                      site.y = 90+var5;
//                      size.W = 80;
//                      size.H = 1;
//                      LCD_rectangle(site, size, GREEN);//显示行
                      site.x = 120;
                      site.y = 90;
                      size.W = 80;
                      size.H = 60;
                      break;
                    default:
                      break;
                }
            }
        }while(nrf_result != NRF_RESULT_RX_NO);  
        /*********************** 按键消息 处理  ***********************/
        //deal_key_event();
    }
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
 *  @brief      PIT0中断服务函数
 *  @since      v5.0
 */
void PIT0_IRQHandler()
{
    key_IRQHandler();

    PIT_Flag_Clear(PIT0);
}
void deal_key_event()
{
    KEY_MSG_t keymsg;

    while(get_key_msg(& keymsg))     //获得按键就进行处理
    {
        if(keymsg.status == KEY_DOWN)
        {
            switch(keymsg.key)
            {
            case KEY_U:
              
                break;

            case KEY_D:
              
                break;

            case KEY_L:
                save_var(CAR_CTRL,CAR_STOP);
                if (var_syn(CAR_CTRL) == 1)
                {
                    site.x = 80;
                    site.y = 60;
                    LCD_str(site, "STOP!", FCOLOUR, BCOLOUR);
                    site.x = 0;
                    site.y = 0;
                }       
                break;

            case KEY_R:
               save_var(CAR_CTRL,CAR_START);
                if (var_syn(CAR_CTRL) == 1)
                {
                    site.x = 80;
                    site.y = 60;
                    LCD_str(site, "START!", FCOLOUR, BCOLOUR);
                    site.x = 0;
                    site.y = 0;
                }
                break;

            case KEY_A:
                break;

            case KEY_START:
                
                break;
            case KEY_STOP:
                      
                break;

            default:
                break;
            }
        }
        else if(keymsg.status == KEY_HOLD)
        {
            switch(keymsg.key)
            {
            case KEY_U:
                
                break;

            case KEY_D:
               
                break;

            case KEY_L:
                save_var(CAR_CTRL,CAR_STOP);
                while(var_syn(CAR_CTRL)==0);
                site.x = 80;
                site.y = 60;
                LCD_str(site, "STOP!", FCOLOUR, BCOLOUR);
                site.x = 0;
                site.y = 0;
                break;

            case KEY_R:
               save_var(CAR_CTRL,CAR_START);
                while(var_syn(CAR_CTRL)==0);   
                site.x = 80;
                site.y = 60;
                LCD_str(site, "START!", FCOLOUR, BCOLOUR);
                site.x = 0;
                site.y = 0;
                break;

            case KEY_A:   

                break;

            case KEY_START:
                
                break;

            case KEY_STOP:
                
                break;

            default:            //不需要处理 cancel
                break;
            }
        }
    }
}

void img_sd_init(void)
{
    int     res;
    char    myfilename[20];
    uint16  imgsize[] = {
                            CAMERA_H,
                            CAMERA_W,
                            //CAMERA_COLOR,
                         };      //高、宽、摄像头颜色类型
    uint32  mybw;
    uint32  Imag_num = 0;

    
    f_mount(0, &fs);

    do
    {
        Imag_num ++;
        sprintf(myfilename, "0:/FIRE%d.sd", Imag_num);
        res = f_open( &src , myfilename, FA_CREATE_NEW | FA_WRITE);
        if( res == FR_DISK_ERR)
        {
          site.x = 0;
          site.y = 60;
          LCD_str(site, "no sdcard!", FCOLOUR, BCOLOUR);
          site.x = 0;
          site.y = 0;
        }
        if(fs.fs_type == 0)
        {
            src.fs = 0;
            return;
        }

    }while(res == FR_EXIST);        //如果文件存在，则命名继续加1

    if ( res == FR_OK )
    {
      site.x = 0;
      site.y = 60;
      LCD_str(site, "open success!", FCOLOUR, BCOLOUR);
      site.x = 0;
      site.y = 0;
      res = f_write(&src, imgsize, sizeof(imgsize),&mybw);  //先写入高和宽，方便上位机处理
    }
    else
    {
        f_close(&src);
        src.fs = 0;
    }
}




void img_sd_save(uint8 * imgaddr,uint32 size)
{
#define F_SYNC_TIME   30      //保存 n 次后 才 同步

    int   res;
    uint32 mybw;
    static uint8 time = 0;
    //uint32 size = CAMERA_SIZE;

    if(src.fs != 0)
    {
        time ++;

        res = f_write(&src, imgaddr, size ,&mybw);

        if(res != FR_OK)
        {
          site.x = 0;
          site.y = 100;
          LCD_str(site, "write failed!", FCOLOUR, BCOLOUR);
          site.x = 0;
          site.y = 0;
          f_close(&src);
          src.fs = 0;
        }

        if(time > F_SYNC_TIME)
        {
            time = 0 ;
            f_sync(&src);
        }
    }
}

void img_sd_exit(void)
{
    f_close(&src);
    src.fs = 0;
}
