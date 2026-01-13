#include "stm32f10x.h"                  // Device header
//static int16_t encoderCount = 0;  // 累计计数
static int16_t lastCount = 0;     // 上一次计数
static int16_t lastLeftCount = 0;

//测速
void Encoder_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
    //TIM1的CH1和CH2
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
 
	//在TI1和TI2上计数
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//A相
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//输入滤波器：最大滤波强度
	TIM_ICInit(TIM8, &TIM_ICInitStructure);//B相
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	//定时器编码器接口配置  //通道不反相
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM8, ENABLE);// 启动 TIM8，开始采集右电机编码器脉冲
	
	// 新增：左电机编码器（TIM5，PA0/PA1）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  // TIM5时钟使能
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  // 左编码器引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 左编码器定时器配置
    TIM_TimeBaseInitStructure.TIM_Period = 65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
    // 左编码器通道配置
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
    TIM_Cmd(TIM5, ENABLE);
}
 // 获取单位时间内的编码器计数（用于计算速度）
int16_t Encoder_GetSpeed(void) {
    int16_t currentCount = TIM_GetCounter(TIM8);
    int16_t delta = currentCount - lastCount;  // 计算计数变化量
    lastCount = currentCount;  // 保存当前计数
    
    // 处理溢出（16位计数器溢出修正）
    if (delta > 32767) {
        delta -= 65536;
    } else if (delta < -32768) {
        delta += 65536;
    }
    return delta;
}
// 新增：左电机速度获取
int16_t Encoder_GetLeftSpeed(void) {
    int16_t currentCount = TIM_GetCounter(TIM5);
    int16_t delta = currentCount - lastLeftCount;
    lastLeftCount = currentCount;
    // 溢出修正
    if (delta > 32767) delta -= 65536;
    else if (delta < -32768) delta += 65536;
    return delta;
}

// 重置编码器计数
void Encoder_Reset(void) {
    TIM_SetCounter(TIM8, 0);
    lastCount = 0;
	
	// 新增：重置左电机编码器
    TIM_SetCounter(TIM5, 0);
    lastLeftCount = 0;
    
}
