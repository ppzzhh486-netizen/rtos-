#include "stm32f10x.h"                  
#include "Motor.h"
#include "PWM.h"

// 调速函数（直接接收PID输出的PWM值，无需转换）
void Speed_Control(uint16_t pwm) 
	{
    if (pwm > 900) pwm = 900;  // 限幅，避免超过最大PWM（对应TIM3的ARR=20000）
    PWM_SetCompare1(pwm);
}
// 新增：左电机调速
void Left_Speed_Control(uint16_t pwm) {
    if (pwm > 99) pwm = 99;  // 左电机PWM限幅
    PWM_SetCompare2(pwm);
}

// 以下函数保留，但实际由PID控制替代
void run(uint16_t Compare) {
    // 开环控制备用（实际用PID时不直接调用）
    Speed_Control(Compare);
    Right_moto_go();
}

void backrun(uint16_t Compare) {
    // 开环控制备用
    Speed_Control(Compare);
    Right_moto_back();
}

void stop(void) {
    Right_moto_Stop();
}
