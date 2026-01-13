#include "stm32f10x.h"                  // Device header


uint8_t L2, L1, MID, R1, R2 ;
uint8_t motorState = 0;   
uint16_t ADC_Value =5;
int16_t targetSpeed = 0;    
int16_t targetLeftSpeed = 0; 

uint8_t red;
u8 Load_flag =0; 
u8 LoadCount,NotLoadCount;

void motor(uint8_t a,uint8_t b)
{
	
	targetLeftSpeed=a;
	targetSpeed=b;

	   	}

void GreySensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    
    GPIO_InitTypeDef GPIO_Initstucture;
    GPIO_Initstucture.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_Initstucture.GPIO_Pin= GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_Initstucture);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    

    GPIO_Initstucture.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_Initstucture.GPIO_Pin= GPIO_Pin_8 |GPIO_Pin_9 ;
    GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_Initstucture);
}


void Get_Light_TTL(void)  
{	
     if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == SET){L2 = 1;}
  	 else L2 =0;
	 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == SET){L1 = 1;}
	 else L1 =0;
	 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == SET){MID = 1;}
	 else MID =0;
	 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == SET){R1 = 1;}
	 else R1 =0;
	 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == SET){R2 = 1;}
	 else R2 =0;
}


void GoStraight_Control (void)
{
	Get_Light_TTL();
	if((L2 == 1)&&(L1 == 0)&&(MID == 0)&&(R1 == 0)&&(R2 == 0))        motor(0,40);      //0 0001
	else if((L2 == 1)&&(L1 == 1)&&(MID == 0)&&(R1 == 0)&&(R2 == 0))   motor(10,40);     //0 0011
	else if((L2 == 0)&&(L1 == 1)&&(MID == 0)&&(R1 == 0)&&(R2 == 0))   motor(25,40);     //0 0010
	else if((L2 == 0)&&(L1 == 1)&&(MID == 1)&&(R1 == 0)&&(R2 == 0))   motor(35,40);     //0 0110
	else if((L2 == 0)&&(L1 == 0)&&(MID == 1)&&(R1 == 0)&&(R2 == 0))   motor(40,40);     //0 0100  MID
	else if((L2 == 0)&&(L1 == 0)&&(MID == 1)&&(R1 == 1)&&(R2 == 0))   motor(40,35);     //0 1100
	else if((L2 == 0)&&(L1 == 0)&&(MID == 0)&&(R1 == 1)&&(R2 == 0))   motor(40,25);     //0 1000
	else if((L2 == 0)&&(L1 == 0)&&(MID == 0)&&(R1 == 1)&&(R2 == 1))   motor(40,10);     //1 1000
	else if((L2 == 0)&&(L1 == 0)&&(MID == 0)&&(R1 == 0)&&(R2 == 1))   motor(40,0);     //1 0000
    else  {motorState = 0;motor(0,0);}     //1 0000
}

void GoOrNot(void)
{
//	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13) == SET){red = 1;}else red=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == SET){red = 1;GPIO_SetBits(GPIOA,GPIO_Pin_9);}
	else {red=0;GPIO_ResetBits(GPIOA,GPIO_Pin_9);}
	if(red == 0)
	{
		NotLoadCount = 0;
		LoadCount++;
		if(LoadCount > 3)       //检测三次，确保准确性
		{
			Load_flag = 1;
		}
	}
	else if(red == 1)
	{
		LoadCount = 0;
		if(Load_flag == 1)
		{
			NotLoadCount++;
			if(NotLoadCount > 3 )  //3次程序执行到这之后再次判断
			{
				Load_flag = 2;
			}
	    }
	}
}
	

