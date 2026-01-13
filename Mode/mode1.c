#include "stm32f10x.h"            
#include "Key.h"
#include "OLED.h"
#include "Timer.h"
#include "Motor.h"
#include "MotorRun.h"
#include "Encoder.h"
#include "PID.h"
#include "Delay.h"
#include "GreySensor.h"

PID_TypeDef speedPID;          // PID结构体
//int16_t targetSpeed = 0;       // 目标速度（单位：计数/10ms）
int16_t actualSpeed = 0;       // 实际速度（单位：计数/10ms）
int16_t pidOutput = 0;         // PID输出（PWM值）
uint8_t KeyNum = 0;
//uint8_t motorState = 0;        // 电机状态：0-停止，1-前进，2-后退
// 新增左电机变量
PID_TypeDef leftSpeedPID;        // 左电机PID结构体
//int16_t targetLeftSpeed = 0;     // 左电机目标速度（与右电机一致，保证同步）
int16_t actualLeftSpeed = 0;     // 左电机实际速度
int16_t leftPidOutput = 0;       // 左电机PID输出（PWM值）

void mode_1_style(void)
{
//	Key_Init();
//	OLED_Init();
//	Timer_Init();
//	Motor_Init();
//	Encoder_Init();
//	GreySensor_Init();
	
	// 初始化PID（参数需根据实际电机调试）
    PID_Init(&speedPID, 15.0f, 0.8f, 1.0f, -99, 99);  // 输出限幅：-900~900（对应PWM）
	// 新增：初始化左电机PID（参数与右电机一致，保证同步）
    PID_Init(&leftSpeedPID, 15.0f, 0.8f, 1.0f, -99, 99);
	
	// OLED显示初始化
     OLED_ShowString(1, 1, "RT:");  // 右电机目标速度
    OLED_ShowString(2, 1, "RA:");  // 右电机实际速度
    OLED_ShowString(3, 1, "LT:");  // 左电机目标速度
    OLED_ShowString(4, 1, "LA:");  // 左电机实际速度
    OLED_ShowString(1, 10, "Stop");
	
//	while (1)
//	{

//		
	KeyNum +=Key_GetNum();
	//电机旋转
	if(KeyNum==1)
	{
		motorState = 1;  
		GoStraight_Control ();
		OLED_ShowString(1, 10, "Go  ");
	}
	if(KeyNum == 2)
	{
		motorState = 0;  // 停止
		targetSpeed = 0;
		targetLeftSpeed = 0;
		OLED_ShowString(1, 10, "Stop");
	}
	if(KeyNum > 2)
	{
	KeyNum = 1;
	}
	
	// OLED显示双电机信息
	OLED_ShowSignedNum(1, 4, targetSpeed, 4);       // 右目标速度
	OLED_ShowSignedNum(2, 4, actualSpeed, 4);       // 右实际速度
	OLED_ShowSignedNum(3, 4, targetLeftSpeed, 4);   // 左目标速度
	OLED_ShowSignedNum(4, 4, actualLeftSpeed, 4);   // 左实际速度
		
		
		
		
		
//	}
}
 
// TIM2中断服务函数（10ms执行一次：采样速度→计算PID→控制电机）
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        // 1. 读取实际速度（编码器计数变化量/10ms）
        actualSpeed = Encoder_GetSpeed();
        actualLeftSpeed = Encoder_GetLeftSpeed();    // 左电机速度
        // 2. 根据电机状态计算PID输出
        if (motorState == 0) {  // 停止状态
            pidOutput = 0;
			leftPidOutput = 0;
        } else {
            // 右电机PID计算
            pidOutput = (int16_t)PID_Calc(&speedPID, targetSpeed, actualSpeed);
            // 左电机PID计算（目标速度与右电机一致）
            leftPidOutput = (int16_t)PID_Calc(&leftSpeedPID, targetLeftSpeed, actualLeftSpeed);
        }
        
        // 3. 根据PID输出控制电机（方向由pidOutput符号决定）
        if (pidOutput > 0) {
            Right_moto_go();  // 前进方向
            Speed_Control(pidOutput);  // 设置正PWM
        } else if (pidOutput < 0) {
            Right_moto_back();  // 后退方向
            Speed_Control(-pidOutput);  // 负PWM取绝对值
        } else {
            Right_moto_Stop();  // 停止
        }
		// 4. 控制左电机（与右电机同步动作）
        if (leftPidOutput > 0) {
            Left_moto_go();
            Left_Speed_Control(leftPidOutput);
        } else if (leftPidOutput < 0) {
            Left_moto_back();
            Left_Speed_Control(-leftPidOutput);  // 负PWM取绝对值
        } else {
            Left_moto_Stop();
        }
    }
}
