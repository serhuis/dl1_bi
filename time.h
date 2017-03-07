//time.h
#ifndef _TIME_H
#define _TIME_H

//defines


//declarations
void VLO_TimerCalibr(void);
void Timer_A0_Init(void);
//void Timer_A_SetDelay(u16 period);
void TimerA0_DelayUs(u16 time);
//void Timer_A_Off(void);




__interrupt void CCR0_ISR(void);

#endif