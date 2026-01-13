#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "mode1.h"
/////////////////
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
//模式一
TaskHandle_t myTask_1_Handler;
void myTask_1(void *arg)
{
	while (1)
	{	
		mode_1_style();
	}
}
int main(void)
 {
	Key_Init();
	OLED_Init();
	Timer_Init();
	Motor_Init();
	Encoder_Init();
	GreySensor_Init();
	xTaskCreate(myTask_1,"myTask_1",256,NULL,2,&myTask_1_Handler);
	vTaskStartScheduler();

	while (1)
	{
		
	}
}
