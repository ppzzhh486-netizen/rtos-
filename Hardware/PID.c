#include "stm32f10x.h"                  // Device header
#include "PID.h"

// 初始化PID参数
void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float minOut, float maxOut) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->target = 0;
    pid->actual = 0;
    pid->error = 0;
    pid->lastError = 0;
    pid->integral = 0;
    pid->derivative = 0;
    pid->output = 0;
    pid->maxOutput = maxOut;
    pid->minOutput = minOut;
}

// PID计算（返回输出值）
float PID_Calc(PID_TypeDef* pid, float target, float actual) {
    // 更新目标值和实际值
    pid->target = target;
    pid->actual = actual;
    
    // 计算当前误差
    pid->error = target - actual;
    
    // 计算积分项（带限幅，防止积分饱和）
    pid->integral += pid->error;
    if (pid->integral > pid->maxOutput / pid->Ki) {  // 积分限幅：避免积分超过最大输出
        pid->integral = pid->maxOutput / pid->Ki;
    } else if (pid->integral < pid->minOutput / pid->Ki) {
        pid->integral = pid->minOutput / pid->Ki;
    }
    
    // 计算微分项（当前误差 - 上一次误差）
    pid->derivative = pid->error - pid->lastError;
    pid->lastError = pid->error;  // 保存当前误差供下次使用
    
    // 计算PID输出
    pid->output = pid->Kp * pid->error + pid->Ki * pid->integral + pid->Kd * pid->derivative;
    
    // 输出限幅
    if (pid->output > pid->maxOutput) {
        pid->output = pid->maxOutput;
    } else if (pid->output < pid->minOutput) {
        pid->output = pid->minOutput;
    }
    
    return pid->output;
}
