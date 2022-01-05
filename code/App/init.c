#include "init.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多

uint32 HMI_flash_ex = 0;

void System_Init()
{
  char uart_rx_buf = 0;
  uint8 HMI_rx = 0;
  uint8 NRF_switch = 0;
  
  
  /************************  初始化开始  ************************************/
  led_init(LED_MAX);
  led(LED_MAX,LED_ON);
  
  /************************ 电机初始化 ************************************/
  ftm_pwm_init(MOTOR_FTM, MOTOR1_CH, MOTOR_HZ, 0);//左轮前
  ftm_pwm_init(MOTOR_FTM, MOTOR2_CH, MOTOR_HZ, 0);//左轮后  
  ftm_pwm_init(MOTOR_FTM, MOTOR3_CH, MOTOR_HZ, 0);//右轮前
  ftm_pwm_init(MOTOR_FTM, MOTOR4_CH, MOTOR_HZ, 0);//右轮后
  
  /************************ 配置 K60 的优先级  *****************************/
  NVIC_SetPriorityGrouping(4);      //参数范围（0~4）4表示4bit全为抢占优先级，没有压优先级
  NVIC_SetPriority(PORTA_IRQn,0);         // 参数越小优先级越高，摄像头
  NVIC_SetPriority(PORTE_IRQn,1);         // NRF
  NVIC_SetPriority(DMA0_IRQn,2);          // 摄像头
  NVIC_SetPriority(PIT0_IRQn,3);          // 定时器
  NVIC_SetPriority(PIT1_IRQn,4);          // 定时器
  NVIC_SetPriority(UART4_RX_TX_IRQn,5);   // 串口
  
  /************************ 摄像头 初始化 **********************************/
  camera_init(imgbuff);                                   // 摄像头初始化，把图像采集到 ImageBuff 地址
  set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);	// 设置 PORTA 的中断服务函数为 PORTA_IRQHandler
  set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      // 设置 DMA0 的中断服务函数为 DMA0_IRQHandler
  
  /************************ 解码器初始化 **********************************/ 
  ftm_quad_init(FTM1);                                    //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
  ftm_quad_init(FTM2);                                    //FTM2 正交解码初始化（所用的管脚可查 port_cfg.h ）
                                                               //使用定时器0触发，10ms执行一次
  
  /************************** UART串口 初始化  ***********************/
  uart_init(UART4, 9600); //用于串口屏
  
  /************************ 无线模块 NRF 初始化  ***********************/
  #if 1
    uint32 i = 20;
    while(!nrf_init());
    //配置中断服务函数
    set_vector_handler(PORTE_VECTORn, PORTE_IRQHandler);
    enable_irq(PORTE_IRQn);
    nrf_msg_init();
    
    while(i--)
    {
      nrf_msg_tx(COM_RETRAN, nrf_tx_buff);// COM_RETRAN复位传输，丢弃之前接收到的数据
    }
  #endif
  
  /************************ FLASH和串口屏初始化 *****************************/
  HMI_flash_ex = flash_read(SECTOR_NUM, 0, uint32);
  //显示当前保存的阈值
  printf("t0.txt=\"%d\"",(uint8)(HMI_flash_ex>>24));
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);  
  //显示当前保存的转圈速度
  printf("t1.txt=\"%d\"",(uint8)(HMI_flash_ex>>16));
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);  
  //显示当前保存的追光速度
  printf("t2.txt=\"%d\"",(uint8)(HMI_flash_ex>>8));
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);  
  //显示当前保存的NRF开关状态
  printf("b6.txt=\"%d\"",(uint8)(HMI_flash_ex>>0) & 0x01);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);  
  
  while(HMI_rx != 0x08)
  {
    if(uart_querychar(UART4,&uart_rx_buf)==1)     //查询接收1个字符，保存到 ch里
    {
      HMI_rx = uart_rx_buf;
      //调节阈值
      if(HMI_rx == 0x01)
      {
        HMI_flash_ex += (1<<24);
      }
      if(HMI_rx == 0x02)
      {
        HMI_flash_ex -= (1<<24);
      }
      //调节转圈速度
      if(HMI_rx == 0x03)
      {
        HMI_flash_ex += (1<<16);
      }
      if(HMI_rx == 0x04)
      {
        HMI_flash_ex -= (1<<16);
      }
      //调节追光速度
      if(HMI_rx == 0x05)
      {
       HMI_flash_ex += (1<<8);
      }
      if(HMI_rx == 0x06)
      {
        HMI_flash_ex -= (1<<8);
      }
      //开关NRF
      if(HMI_rx == 0x07)
      {
        NRF_switch = ~NRF_switch;
        if(NRF_switch)
          HMI_flash_ex |= (1<<0);
        else      
          HMI_flash_ex &= ~(1<<0);
      }
      //显示当前阈值
      printf("t0.txt=\"%d\"",(uint8)(HMI_flash_ex>>24));
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      //显示当前转圈速度
      printf("t1.txt=\"%d\"",(uint8)(HMI_flash_ex>>16));
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      //显示当前追光速度
      printf("t2.txt=\"%d\"",(uint8)(HMI_flash_ex>>8));
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);    
      //显示NRF开关状态
      printf("b6.txt=\"%d\"",(uint8)(HMI_flash_ex>>0) & 0x01);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);   
    }
  }
  while(!flash_erase_sector(SECTOR_NUM));                     //擦除扇区
  while(!flash_write(SECTOR_NUM, 0, HMI_flash_ex));  //写入数据到扇区，偏移地址为0，必须一次写入4字节 
  
  /************************ 摄像头阈值初始化 *********************************/
  SCCB_WriteByte(OV7725_CNST, (uint8)(HMI_flash_ex>>24));	//改变图像阈值   
  
  /************************ 图像采集 ***************************************/
  camera_get_img();	// 先采集一次图像
  
  /************************ 定时器 初始化  *********************************/ 
  pit_init_ms(PIT0, 10);                              	// 初始化PIT0，定时时间为： 10ms
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);	// 设置PIT0的中断服务函数为 PIT0_IRQHandler
  enable_irq (PIT0_IRQn); 				// 使能中断
  
  //尽量使PIT1和PIT2的定时时间大，才能使定时更加准确
  pit_init_ms(PIT1, 10);                           	// 初始化PIT1，定时时间为： 20ms
  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler); 	// 设置PIT1的中断服务函数为 PIT1_IRQHandler
  enable_irq (PIT1_IRQn); 				// 使能中断
  
  /************************  初始化结束  ************************************/
  led(LED_MAX,LED_OFF);// 打开LED指示灯		全部初始化成功
}