#include "stm32f10x.h"          

//定时器3->PWM调速  周期20MS  计一个数->1US
void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//电动机PWM输出 PA6
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 // 新增：左电机PWM引脚（PA7 - TIM3_CH2）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  // 左电机PWM
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//计数器 ≤ CCR 输出高，＞CCR 输出低
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//有效电平为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//允许该通道输出 PWM 信号
	TIM_OCInitStructure.TIM_Pulse = 0;		 			//CCR
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 			//PWM输出引脚
	
	// 新增：左电机PWM通道（CH2）
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	
	TIM_Cmd(TIM3, ENABLE);
}
 
//电机输出比较
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM3, Compare);//将参数 Compare 的值直接写入 TIM3 定时器的CCR1
}
// 新增：左电机PWM设置函数
void PWM_SetCompare2(uint16_t Compare)
{
    TIM_SetCompare2(TIM3, Compare);
}
