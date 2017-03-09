/**********************************************************************************
 *
 *      ASD-10QR MAIN routine
 * 
 **********************************************************************************
 * FileName:        main.c
 * Version:			1.00
 *
 * Processor:       MSP430G2xxx
 * Complier:        IAR Workbench for MSP430 v4.50 or higher
 *                  
 * Company:         ARTON
 *
 * Software License Agreement
 *
 * The software supplied herewith by ARTON Incorporated
 * (the "Company") for its devices is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on ARTON Inc products. The
 * software is owned by the Company and/or its Author, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * Author               Date      	Version	  		Comment
 *--------------------------------------------------------------------------------
 * Oleg Semeniuk	 16.01.2014    	1.00		Release for v1.00
 *
 *********************************************************************************/


/*********************************************************************************/
/*                                 INCLUDES                                      */
/*********************************************************************************/


#include <stdio.h>                    /* standard I/O .h-file                */
#include <string.h>                   /* string and memory functions         */


#include  "main.h"



/*********************************************************************************/
/*                                 VARIABLES                                     */
/*********************************************************************************/

__regvar __no_init tFlags 	f 	 @ __R4; 	// Set of state machine flags

//
u16		timerMain;					// Текущее значение главного таймера
u16		mainPeriodCounter;			// Counter of main time period
u8 		DeviceMode; 				// Mode of device

tFault	DeviceFault = {0};			// Current Faults flags
u16		tempC;						// Current temperature in deg C

u8 		Timer50msCounter = 0;		// Counter of 50ms ticks

volatile u32	led_r;
volatile u32	led_y;
volatile u32	led_sh = 0;

u8 		fTimerA1_On = 0;

//const u8 fault_sequence[4 * 2] = {MODE_FAULT, 10, MODE_NORM, 5, MODE_FIRE, 0, 0};	// 0 = ~~

u8	reference;
tCfgReg	cfg_reg;

u8	jp1_state = 0;	// 0 - JP1 Open, 1 - JP1 Close

u8	adc_process = 0;		// ADC low level semafore

u16	light_timer = 0;	// For led lighting

u8	light_sync = 0;		// For led lighting syncronisation


/*********************************************************************************/
/*                                FUNCTIONS                                      */
/*********************************************************************************/
// --- Declarations ---

//u16  AverageData(u16 * data_ptr, u8 len);
//u8   RX_PacketParser(void);
//void ADC_Measure(u16 ch, u16 refout, u8 count);
//void ADC_Measure_TEMP(u16 ch, u16 refout, u8 count);


void JP1_Define(void);




//--------------------------------------------------------------------------------
// Function		: void DeviceStart(void)
// Parameters	: None
// Return		: None
// Description	: Function executes initialization variable at start of device
//--------------------------------------------------------------------------------
void DeviceStart(void) {

	DeviceFault.byte = 0;		// Reset faults flags
	//		
	VLO_TimerCalibr();			// Calibration VLO Timer
	//
}


//--------------------------------------------------------------------------------
// Function		: void LedValueManager(void)
// Parameters	: None
// Return		: None
// Description	: 
//--------------------------------------------------------------------------------
void LedValueManager(void) {
  /*
	if (DeviceFault.byte) {
		DeviceMode = MODE_FAULT;
		led_r = 0;
		
		if (DeviceFault.fELStrobNone) {
			// Electrical sync is fault
			led_y = LED_PULSE_3;
		}else
		if (DeviceFault.fSignal_Hi) {
			// Level signal is big
			led_y = LED_PULSE_2;
		}else
		if (DeviceFault.fSignal_Low) {
			// Level signal is big
			led_y = LED_PULSE_1;
		}else	
		if (DeviceFault.fFaultDrift) {
			// Level signal is big
			led_y = LED_PULSE_4;
		}
	}else{
		if (DeviceMode == MODE_FAULT) {
			DeviceMode = MODE_NORM;
			led_r = 0;
			led_y = 0;
		}
	}
  */
}

//========================================================
//                 ---  M A I N  ----
//========================================================
//--------------------------------------------------------------------------------
// Function		: void main(void)
// Parameters	: None
// Return		: None
// Description	: Main function. Contains main loop.
//--------------------------------------------------------------------------------
void main(void) {
	u8	led_clk;
	u16	led_timer = 0;
	// Initialization variables and GPIO
	
	WDTCTL = WDTPW + WDTHOLD;				// отключаем сторожевой таймер

	// GIPIO Init
	GPIO_Init();
		
	// Init internal RC osc.
	BCSCTL1 = CALBC1_1MHZ; 					// Используем частоту 1 MГц
	DCOCTL =  CALDCO_1MHZ;
	
	DelayMs(500);
	
	// Initialization code for VLO
	__set_R4_register(0);
	//
	BCSCTL3 |= LFXT1S_2;                    // Select VLO as low freq clock
	// End initialization code
	
	WDTCTL = WDT_ADLY_250;                   // Interval timer	/* for 50 ms */
	//WDTCTL = WDT_ADLY_1_9;                   // Interval timer	/* for 5.9 ms */
	IE1 |= WDTIE;                           // Enable WDT interrupt
	//
	//fLPM3 = 1;								// Enable LOW power mode
	//
	if (IFG1 & WDTIFG) {
		// Reset WDT
		#if (SYS_FAULT_ENABLE == 1)
		DeviceFault.fFaultSWReset = 1;
		#endif
	}
	IFG1 = 0;
	//	
	DeviceMode = MODE_NORM;
	
	DelayMs(1000);
	
	Led_Flash(10);
	DelayMs(300);
	Led_Flash(10);
	
	DelayMs(3000);
	
	_BIS_SR(GIE);    					// Interrupt enable
	DeviceStart();

	cfg_reg = CONFIG->config_reg;
	
	Timer_A0_Init();
	
	
// *****************************************************************
// ******************   M A I N   L O O P  *************************
// *****************************************************************
	while(1) {
//-------------------------------------------------------------------------------
		//
		// ******** Обработчики событий ********
		//
//-------------------------------------------------------------------------------
		
//-------------------------------------------------------------------------------
// TimerA0 Event		
//-------------------------------------------------------------------------------
		if (fTimerA_On) {				// Получен следующий интервал timer
			fTimerA_On = 0;
/*			
			BCSCTL1 = CALBC1_16MHZ; 					// Используем частоту 8 MГц
			DCOCTL =  CALDCO_16MHZ;
			//
			TA1CCR0  = 16 * SYS_TICK_TIME - 1;   	// Period T(us) * F(MHz)
			//
			
			_BIC_SR(GIE);    			// Запрещаем прерывания
			Timer_A_Off();
			_BIS_SR(GIE);    			// Разрешаем прерывания
			
			//TEST2_CLR();
			
			timerA1_blank = 4;
*/			
		}

//-------------------------------------------------------------------------------
// fTimer50msOn Event				
//-------------------------------------------------------------------------------
		if (fTimer50msOn) {		// Получен следующий 50мс интервал
			fTimer50msOn = 0;
			//
			Timer50msCounter = 0;
			RED_CLR();
		}
		
//-------------------------------------------------------------------------------
// TimerA1 Event (SysTick)
//-------------------------------------------------------------------------------
		if (fTimerA1_On) {				// Получен следующий интервал timer
			fTimerA1_On = 0;
			
			//TEST2_CLR();
			//TEST2_OUT ^= TEST2_BIT;
			
			//
			if (timerMain) {
				timerMain--;
				if (timerMain == 1) {
					if (DeviceMode == MODE_TEST) {
						DeviceMode = MODE_NORM;
						RED_CLR();
						YEL_CLR();
					}
				}
			}

	/*		
			if (timerA1_blank) {
				timerA1_blank--;
			}else{

				// Indication
				//
				if (light_timer) {
					if ((DeviceMode == MODE_NORM) || (DeviceMode == MODE_PREFIRE)) {
						RED_SET();
						YEL_CLR();
					}else
					//
					if (DeviceMode == MODE_CALIBR) {
						RED_SET();
						YEL_SET();
					}
					//
					light_timer--;
				}else{
					//
					if ((DeviceMode == MODE_NORM) || (DeviceMode == MODE_CALIBR) || (DeviceMode == MODE_PREFIRE)) {
						RED_CLR();
						YEL_CLR();
					}
				}

			} // End indication
			//
*/
			
		} // if (fTimer50msOn)

	} // while(1)
}


//--------------------------------------------------------------------------------
// Function		: __interrupt void watchdog_timer (void)
// Parameters	: None
// Return		: None
// Description	: WDT Interrupt routine
//--------------------------------------------------------------------------------
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void) {
	
	if (fTimer50msOn) {
		if (++Timer50msCounter == 0) {		// > ~ 12sec
			//!!!WDTCTL = WDTCTL;				// Hardware RESET
			RED_SET();
		}
	}
	fTimer50msOn = 1;

	
//	__bic_SR_register_on_exit(LPM3_bits);                   // Clear LPM3 bits from 0(SR)
}




//--------------------------------------------------------------------------------
// Function		: void Led_Flash(u16 duration)
// Parameters	: duration - duration Red LED flash in ms
// Return		: None
// Description	: Flashing red LED
//--------------------------------------------------------------------------------
void Led_Flash(u16 duration) {
	RED_SET();
	YEL_SET();
	DelayMs(duration);
	RED_CLR();
	YEL_CLR();
}

//--------------------------------------------------------------------------------
// Function		: void ADC10_ISR(void)
// Parameters	: None
// Return		: None
// Description	: ADC10 interrupt service routine
//--------------------------------------------------------------------------------
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {

	//ADC10AE0 &= ~0x0F;                      // Save only VRef Out
	adc_process = 0;
	fEndOfSamples = 1;
	
	//__bic_SR_register_on_exit(CPUOFF);      // Clear CPUOFF bit from 0(SR)
	
}

//--------------------------------------------------------------------------------
// Function		: void DeviceDiagnostics(void)
// Parameters	: None
// Return		: None
// Description	: Procedure of the diagnostics device
//--------------------------------------------------------------------------------
void DeviceDiagnostics(void) {
	
	//
#if (TEMP_DET_ENABLE == 1)
//	ADC_Measure_TEMP(ADC_CH_TEMP, REFOUT, ADC_CH_DATA_LEN);
//	tempC = AverageData(adc_data1, ADC_CH_DATA_LEN);
#endif
	
	// Diagnostic BOOST	
	//
//	DelayMs(100);
	//
	//
		
#if (CRC_ENABLE == 1)
	// Check CS of Memory
	DeviceFault.fFaultCRC = 0;
	if (GetPropertiesCS() != CONFIG->CS) {
		DeviceFault.fFaultCRC = 1;
	}
#endif
}

//--------------------------------------------------------------------------------
// Function		: void u8 JP1_Define(void)
// Parameters	: None
// Return		: 0 - JP1 Open, 1 - JP1 Close
// Description	: Definition of JP1 state
//--------------------------------------------------------------------------------
void JP1_Define(void) {
/*	u16 buf[4];
	u16 res;
	
	res = ADC_Measure_Simple(ADC_BUT, buf, 4);
	
	if (res > 600) return; 
	if (res > 100) {
		jp1_state = 0;		// JP1 is Open 
	}else{
		jp1_state = 1;		// JP1 is Close
	}	
*/
        
	return;	
}


// End of main.c
