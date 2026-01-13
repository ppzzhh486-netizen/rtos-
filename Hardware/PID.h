#ifndef __PID_H
#define __PID_H

typedef struct {
    float Kp;           // 比例系数
    float Ki;           // 积分系数
    float Kd;           // 微分系数
    float target;       // 目标值
    float actual;       // 实际值
    float error;        // 当前误差
    float lastError;    // 上一次误差
    float integral;     // 积分项
    float derivative;   // 微分项
    float output;       // 输出值
    float maxOutput;    // 输出最大值（限幅）
    float minOutput;    // 输出最小值（限幅）
} PID_TypeDef;

void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float minOut, float maxOut);
float PID_Calc(PID_TypeDef* pid, float target, float actual);

#endif
