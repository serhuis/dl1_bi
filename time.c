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
	//
	//SoundStop();							// Disable interrupts of timer
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
// Function		: void VLO_TimerCalibr(void)
// Parameters	: None
// Return		: None
// Description	: Calculation calibration value of VLO timer
//--------------------------------------------------------------------------------
u16 VLO_GetPeriod(void) {
	
	// Start timer 8MHz
	TACTL = TASSEL_2 + MC_1;           	 	// SMCLK, up mode
	//			
	CCR0 =0xFFFF;                    		// Period 2.5mS
	CCTL1 = 0; 			                    // CCR1 reset/set
	TACCTL0 = 0;							// Разрешаем прерывание таймера по достижению значения TACCCR0.
	//
	while (fTimer50msOn == 0) {}
	fTimer50msOn = 0;
	//while (fTimer50msOn == 0) {}
	
	return TAR;
	
}


//--------------------------------------------------------------------------------
// Function		: void Timer_A0_Init(void)
// Parameters	: period in us (1..65535) - for 1 MHz DCO
// Return		: None
// Description	: Function initiates delay
//--------------------------------------------------------------------------------
void Timer_A0_Init(void) {
	_BIC_SR(GIE);    					// Запрещаем прерывания
	//

//	fLPM3 = 0;							// Выключаем режим энергосбережения	
	//
	TA0R = 0;
	TA0CTL 	 = TASSEL_2 + MC_1 + ID_0;  // SMCLK, up mode, div = 1
	TA0CCR0  = SYS_TICK_TIME - 1;   	// Period T(us) * F(MHz)
	TA0CCTL0 = CCIE;					// Разрешаем прерывание таймера по достижению значения TACCCR0.
	//
	_BIS_SR(GIE);    					// Разрешаем прерывания

}

//--------------------------------------------------------------------------------
// Function		: void Timer_A_SetDelay(u16 period)
// Parameters	: period in us (1..65535) - for 8 MHz DCO
// Return		: None
// Description	: Function initiates delay
//--------------------------------------------------------------------------------
void Timer_A_SetDelay(u16 period) {
	
	if (period == 0) return;
	
	_BIC_SR(GIE);    					// Запрещаем прерывания
	
	fTimerA_Enable = 1;
	fTimerA_Repeat = 0;
	//
	fLPM3 		= 0;					// Выключаем режим энергосбережения	
	//
	TA0R = 0;
	TACTL 	 = TASSEL_2 + MC_1 + ID_3;  // SMCLK, up mode, div = 8
	CCR0 	 = period - 1;      		// Period T(us) * F(MHz)
	TACCTL0 = CCIE;						// Разрешаем прерывание таймера по достижению значения TACCCR0.
	//
	_BIS_SR(GIE);    					// Разрешаем прерывания
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


//--------------------------------------------------------------------------------
// Function		: void TimerA1_DelayUs(u16 time)
// Parameters	: period in us (1..32768) - for 1 MHz DCO
// Return		: None
// Description	: Function initiates 
//--------------------------------------------------------------------------------
#pragma optimize=none
void TimerA0_DelayUs(u16 time) {
	_BIC_SR(GIE);    					// Запрещаем прерывания
	
	//
	TA0R = 0;
	TA0CTL 	 = TASSEL_2 + MC_1;     	// SMCLK, up mode, div = 8
	TA0CCR0 	 = time - 1 - 35;      	   	// Period T(us) * F(MHz)
	//
	_BIS_SR(GIE);    						// Разрешаем прерывания
	
	while ((TA0CCTL0 & CCIFG) == 0);
	
	TA0CTL 	 = 0;
	TA0CCTL0 = 0;

}


//--------------------------------------------------------------------------------
// Function		: void SoundStart(u8 snd_ind)
// Parameters	: period in us (1..32768) - for 16 MHz DCO
// Return		: None
// Description	: Function initiates 
//--------------------------------------------------------------------------------
void Timer_A_Off(void) {
	_BIC_SR(GIE);    			// Запрещаем прерывания
	//
	TACTL = 0;  
	TACCTL0 = 0;				// Запрещаем прерывание таймера по достижению значения TACCCR0.
	//
//	fLPM3 = 1;					// Включаем режим энергосбережения		
	//
	_BIS_SR(GIE);    			// Разрешаем прерывания

}