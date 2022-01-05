#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "common.h"
#include "include.h"

#define MOTOR_FTM FTM3
#define MOTOR1_CH FTM_CH5 //左轮前
#define MOTOR2_CH FTM_CH4 //左轮后  
#define MOTOR3_CH FTM_CH7 //右轮前
#define MOTOR4_CH FTM_CH6 //右轮后 
#define MOTOR_HZ 20000

extern int16 MOTOR1_speed;
extern int16 MOTOR2_speed;

extern int Left_motor_speed, Right_motor_speed;

extern int MOTOR1_DUTY;
extern int MOTOR2_DUTY;
extern int MOTOR3_DUTY;
extern int MOTOR4_DUTY;

void MOTOR_measure();
void run_speed(int16 Run_speed);
void orbit_speed(int16 Div_speed);
void Left_motor_pid(int16 SetSpeed);
void Right_motor_pid(int16 SetSpeed);


#endif