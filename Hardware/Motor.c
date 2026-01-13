#include "stm32f10x.h"             
#include "PWM.h"
 
//左右电机初始化
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//电机控制模拟输入端
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//控制电机->PA4 PA5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// 新增：左电机方向引脚（PA2、PA3）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	PWM_Init();
}
 
//电机向前
void Right_moto_go(void)
{
	//正转
	GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	GPIO_SetBits(GPIOC, GPIO_Pin_3);
}
 
//电机向后
void Right_moto_back(void)
{
	//反转
	GPIO_SetBits(GPIOC, GPIO_Pin_2);
	GPIO_ResetBits(GPIOC, GPIO_Pin_3);
}
 
//电机停止
void Right_moto_Stop(void)
{
	//停车
	GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	GPIO_ResetBits(GPIOC, GPIO_Pin_3);
}
// 新增：左电机控制
void Left_moto_go(void) 
{ 
	GPIO_ResetBits(GPIOC, GPIO_Pin_0); 
	GPIO_SetBits(GPIOC, GPIO_Pin_1); 
}

void Left_moto_back(void) 
{
	GPIO_SetBits(GPIOC, GPIO_Pin_0); 
	GPIO_ResetBits(GPIOC, GPIO_Pin_1); 
}
void Left_moto_Stop(void) 
{ 
	GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);
}
