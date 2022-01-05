#include "motor.h"

int16 MOTOR1_speed = 0;//编码器测试左轮速度
int16 MOTOR2_speed = 0;//编码器测试右轮速度

int Left_motor_speed = 0, Right_motor_speed = 0; //设定的电机速度

int MOTOR1_DUTY = 0;
int MOTOR2_DUTY = 0;
int MOTOR3_DUTY = 0;
int MOTOR4_DUTY = 0;

int16 number1,number2;

void MOTOR_measure()
{
  int16 Pulses1, Pulses2;

  Pulses1 = ftm_quad_get(FTM1);
  ftm_quad_clean(FTM1);
  Pulses2 = ftm_quad_get(FTM2);
  ftm_quad_clean(FTM2);

  MOTOR2_speed = -Pulses1; 
  MOTOR1_speed = Pulses2;
}

//直线速度
void run_speed(int16 Run_speed)
{
  if (light_ln < 20 && light_ln != 0)
  {
      if (road_mid > 50 || road_mid < 30)
      {
        //左右轮差速算法
        if(road_mid < 40)
        {
          Left_motor_speed = (int)(Run_speed - ((float)(40 - road_mid) * 1));
          Right_motor_speed = Run_speed;
        }
        if(road_mid > 40)
        {
          Left_motor_speed = Run_speed;
          Right_motor_speed = (int)(Run_speed - ((float)(road_mid - 40) * 1));
        }
        if(road_mid == 40)
        {
          Left_motor_speed = Run_speed;
          Right_motor_speed = Run_speed;
        }
      }
      else
      {
        //左右轮差速算法
        if(road_mid < 40)
        {
          Left_motor_speed = (int)(Run_speed - ((float)(40 - road_mid) * 0.5));
          Right_motor_speed = Run_speed;
        }
        if(road_mid > 40)
        {
          Left_motor_speed = Run_speed;
          Right_motor_speed = (int)(Run_speed - ((float)(road_mid - 40) * 0.5));
        }
        if(road_mid == 40)
        {
          Left_motor_speed = Run_speed;
          Right_motor_speed = Run_speed;
        }
      }
  }
  else if (light_ln >= 20 || light_ln == 0)
  {
      if (road_mid > 50 || road_mid < 30)
      {
        //左右轮差速算法
        if(road_mid < 40)
        {
          Left_motor_speed = (int)(Run_speed - ((float)(40 - road_mid) * 2));
          Right_motor_speed = Run_speed;
        }
        if(road_mid > 40)
        {
          Left_motor_speed = Run_speed;
          Right_motor_speed = (int)(Run_speed - ((float)(road_mid - 40) * 2));
        }
        if(road_mid == 40)
        {
          Left_motor_speed = Run_speed;
          Right_motor_speed = Run_speed;
        }
      }
      else
      {
        //左右轮差速算法
        if(road_mid < 40)
        {
          Left_motor_speed = (int)(Run_speed - ((float)(40 - road_mid) * 1));
          Right_motor_speed = Run_speed;
        }
        if(road_mid > 40)
        {
          Left_motor_speed = Run_speed;
          Right_motor_speed = (int)(Run_speed - ((float)(road_mid - 40) * 1));
        }
        if(road_mid == 40)
        {
          Left_motor_speed = Run_speed;
          Right_motor_speed = Run_speed;
        }
      }
  }
}

//转圈差速
void orbit_speed(int16 Div_speed)
{
  if (Div_speed >= 0)
  {
    Left_motor_speed = -Div_speed;
    Right_motor_speed = Div_speed;
  }
  else if (Div_speed < 0)
  {
    Left_motor_speed = -Div_speed;
    Right_motor_speed = Div_speed;
  }
}
//左轮PID调节
void Left_motor_pid(int16 SetSpeed)
{
  int NowSpeed = MOTOR1_speed;
  int left_duty = 0;
  int iError = 0;
  static float sum_error = 0;
  static int LastError = 0;

//1当前差、差积分、限幅
  iError = (SetSpeed - NowSpeed);
  sum_error = sum_error + iError;
  if (sum_error > 1000) 
  {
    sum_error = 1000;
  }
  if (sum_error < -1000) 
  {
    sum_error = -1000;
  }
  
//2速度差大于-80进行正常PID，否则直接满转减速，（减速更迅猛）
  if(iError > -50)
  {
    left_duty = iError * 12 + (int32)(sum_error * 1.9) + (iError - LastError) * 5;
  }
  else
  {
    left_duty = -900;
  }
  
//3限幅
  if (left_duty > 900)
  {
    left_duty = 900;
  }
  if (left_duty < -900) 
  {
    left_duty = -900; 
  }

//4记录上一次差
  LastError = iError;

//5输出
  if(left_duty >= 0)
  {
    MOTOR1_DUTY = left_duty;
    MOTOR2_DUTY = 0;
  }
  else
  {
    MOTOR1_DUTY = 0;
    MOTOR2_DUTY = -left_duty;
  }
}



//右轮PID调节
void Right_motor_pid(int16 SetSpeed)
{
  int NowSpeed = MOTOR2_speed;
  int right_duty = 0;
  int iError = 0;
  static float sum_error = 0;
  static int LastError = 0;

//1当前差、差积分、限幅
  iError = (SetSpeed - NowSpeed);
  sum_error = sum_error + iError;
  if (sum_error > 1000) 
  {
    sum_error = 1000;
  }
  if (sum_error < -1000) 
  {
    sum_error = -1000;
  }
  
//2速度差大于-80进行正常PID，否则直接满转减速，（减速更迅猛）
  if(iError > -50)
  {
    right_duty = iError * 12 + (int32)(sum_error * 1.9) + (iError - LastError) * 5;
  }
  else
  {
    right_duty = -900;
  }
  
//3限幅
  if (right_duty > 900)
  {
    right_duty = 900;
  }
  if (right_duty < -900) 
  {
    right_duty = -900; 
  }

//4记录上一次差
  LastError = iError;

//5输出
  if(right_duty >= 0)
  {
    MOTOR3_DUTY = right_duty;
    MOTOR4_DUTY = 0;
  }
  else
  {
    MOTOR3_DUTY = 0;
    MOTOR4_DUTY = -right_duty;
  }
}


