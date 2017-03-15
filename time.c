//time.c

#include "main.h"
#include "time.h"


//Declarations
extern __regvar __no_init tFlags 	f 	 @ __R4; 

//--------------------------------------------------------------------------------
// Function		: void VLO_TimerCalibr(void)
// Parameters	: None
// Return		: None
// Description	: Calculation calibration value of VLO timer
//--------------------------------------------------------------------------------
void VLO_TimerCalibr(void) {
	u16 clk;
	
	// Start timer 1MHz
	TACTL = TASSEL_2 + MC_1 + ID_3;     	 	// SMCLK, up mode  / 8
	//			
	CCR0 = 62500 - 1;	                    // Period 0.5sec
	CCTL1 = 0; 			                    // CCR1 reset/set
	TACCTL0 = CCIE;							// Разрешаем прерывание таймера по достижению значения TACCCR0.
	//
	clk = 0;
	while (1) {
		if (fTimer50msOn) {
			fTimer50msOn = 0;
			clk++;
		}
		if (fTimerA_On) {
			fTimerA_On = 0;
			clk++;
			break;
		}
	}
	TACTL = 0;  
	TACCTL0 = 0;				// Запрещаем прерывание таймера по достижению значения TACCCR0.
	//
	if (clk != CONFIG->timer_calibr) {
		StoragePropertyWord(eeTIMER_CALIBR_OFFSET, clk * 2);
		#if (CRC_ENABLE == 1)
		SavePropertyCS();
		#endif
	}
	//
}


//--------------------------------------------------------------------------------
// Function		: void Timer_A0_Init(void)
// Parameters	: period in us (1..65535) - for 1 MHz DCO
// Return		: None
// Description	: Function initiates delay
//--------------------------------------------------------------------------------
void SysTimerInit(void) {
	_BIC_SR(GIE);    					// Запрещаем прерывания
	//

	TA0R = 0;
	TA0CTL 	 = TASSEL_2 + MC_1 + ID_0;  // SMCLK, up mode, div = 1
	TA0CCR0  = SYS_TICK_TIME - 1;   	// Period T(us) * F(MHz)
	TA0CCR1 = DELAY_1MS;							//for 1 ms delay;
	TA0CCTL0 = CCIE;					// Разрешаем прерывание таймера по достижению значения TACCCR0.
	//
	_BIS_SR(GIE);    					// Разрешаем прерывания

}

void T0_delay(void)
{
	TA0CCTL1 = CCIE;
}

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

#pragma vector = TIMER0_A1_VECTOR
__interrupt void CCR1_ISR(void) {
  fIrTimerOn = 1;
	TACCTL1 = 0;
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
