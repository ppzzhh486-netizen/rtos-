#ifndef __GREYSENSOR_H
#define __GREYSENSOR_H

extern uint8_t motorState;
extern int16_t targetSpeed ;    
extern int16_t targetLeftSpeed ;     

extern u8 Load_flag;

void GreySensor_Init(void);
void Get_Light_TTL(void);
void GoStraight_Control (void);
void motor(uint8_t a,uint8_t b);
void GoOrNot(void);
#endif
