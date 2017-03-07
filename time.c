//time.c

#include "main.h"
#include "time.h"


//--------------------------------------------------------------------------------
// Function		: __interrupt void CCR0_ISR(void)
// Parameters	: None
// Return		: None
// Description	: TIMER0 Interrupt routine
//--------------------------------------------------------------------------------
#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void) {
  fTimerA_On = 1;
	/*
	if (fRedLedFlash) {
		fRedLedFlash = 0;
		RED_CLR();
		//
		fTimerA_Enable = 0;
		TACTL = 0;  
		TACCTL0 = 0;				// Запрещаем прерывание таймера по достижению значения TACCCR0.
		//
		return;
	}
	*/
} // CCR0_ISR