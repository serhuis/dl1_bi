/*********************************************************************
 *
 *      Definitions of MAIN routine
 *
 *********************************************************************
 * FileName:        main.h
 * Version:			1.00
 *
 * Processor:       MSP430G2xxx
 * Complier:        IAR Workbench for MSP430 v5.50 or higher
 *                  
 * Company:         ARTON
 *
 * Author               Date      	Version	  		Comment
 *--------------------------------------------------------------------
 * Oleg Semeniuk	 16.01.2014    	1.00		Release for v1.00

 ********************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__

/*********************************************************************************/
/*                                 INCLUDES                                      */
/*********************************************************************************/
#include "type.h"
#include  "hardware.h"
#include  "flash.h"
#include  "type.h"
#include  "delay.h"
//#include  "soft_uart.h"
#include "time.h"


/*********************************************************************************/
/*                               DEFINITIONS                                     */
/*********************************************************************************/
	 
// Options for preprocessor
#define SYS_FAULT_ENABLE		1	/* 1 - detect System Fault, 0 - no detect System Fault */
#define CRC_ENABLE				1	/* 1 - control memory enable, 0 - control memory disable */

#define	SYNC_PERIOD			100 		/* in ticks */
#define IR_TIMEOUT			1			/**/
#define	IR_PULSES				8				/*number of IR signal pulses*/
#define IR_DUTY					48			/*pulse width of IRED pulse*/
#define	IR_PAUSE				148			/*pause of IRED pulses*/

#define	TIME_1S				20 		/* in 50ms */
#define	TIME_2S				40 		/* in 50ms */
#define	TIME_4S				80 		/* in 50ms */
#define	TIME_5S				100		/* in 50ms */
	
#define SYS_TICK_TIME			10000	/* in us */
#define DELAY_1MS					1000	/* in us */
	 
// Modes of device
#define	MODE_NORM			0x04
#define	MODE_FAULT		0x07
#define	MODE_TEST			0x08


// Other constants
#define T50MS_DIV			152		/* DIV from T=1/3200Hz  to T=50ms */

typedef struct  {
		u16	 bTimer50msOn 	: 1;				/* Закончился 50мс интервал от WDT	*/
		u16	 bTimerA_On			: 1;        		/* End of Timer A period	*/
		u16	 bTimerA_Repeat	: 1;        		/* Flag: Multipulse of timer	*/
		u16	 bButtonDownOn	: 1;        		/* Была нажата кнопка	*/
		u16	 bButtonUpOn		: 1;        		/* Была отжата кнопка	*/
		u16	 bIRStartPulse	: 1;        		/* Flag: IR Start pulse is received */
		u16	 bLineDownOn		: 1;				/* For Soft UART */
		u16	 bLPM3					: 1;        		/* Флаг режима низкого потребления	*/
		u16	 bInterconnect	: 1;        		/* Flag link of Interconnect	*/
		u16	 bStartPulse		: 1;        		/* Flag: Start pulse is received */
		u16	 bIrYimerOn			: 1;        						
		u16	 : 1;				
		u16	 : 1;				
		u16	 : 1;			
		u16	 : 1;       
		u16	 : 1;       

		
}tFlags;
/*
		fIrTimer
		IR_PULSES
		IR_DUTY
		IR_PAUSE
		*/

// Низкоуровневые события & flags
#define	fTimer50msOn	f.bTimer50msOn			/* Закончился 50мс интервал от WDT	*/
#define	fTimerA_On	f.bTimerA_On         	/* End of Timer A period	*/
#define	fTimerA_Repeat	f.bTimerA_Repeat       	/* Flag: Multi pulse of timer	*/
#define	fButtonDownOn	f.bButtonDownOn         /* Была нажата кнопка	*/
#define	fButtonUpOn	f.bButtonUpOn           /* Была отжата кнопка	*/
#define	fEndOfSamples	f.bEndOfSamples         /* Flag: End of samples of ADC */
#define	fIRStartPulse	f.bIRStartPulse         /* Flag: IR Start pulse is received */
#define	fRxLineDownOn	f.bLineDownOn			/* For Soft UART */
#define	fLPM3		f.bLPM3                 /* Флаг режима низкого потребления	*/

#define	fInterconnect	f.bInterconnect         /* Flag link of Interconnect	*/
#define	fIrTimerOn		f.bIrYimerOn           		/* Flag: HUSH mode	*/
#define	fWaitStartPulse	f.bWaitStartPulse  		/* */
#define	fTimerA_Enable	f.bTimerA_Enable        /* Flag: Timer A Enable	*/

#define	fRFButtonDownOn	f.bRFButtonDownOn       /* Была нажата кнопка RF	*/
#define	fSendingGraph	f.bSendingGraph         /* 	*/

#define	fStartPulse	f.bStartPulse           /* Flag: Start pulse is received */


typedef union {
	 u8	 byte;
	 struct  {
		u8	: 1;
		u8	: 1;
		u8	: 1;
		u8	: 1;
		u8	: 1;
		u8	fFaultSWReset	: 1;
		u8	fFaultCRC	: 1;
	 };
}tFault;




#define SIGNAL_ARRAY_LEN	10


/*********************************************************************************/
/*                        EXTERNAL VARIABLES                                     */
/*********************************************************************************/
//extern tFlags 	f;					//system flags
extern tFault	DeviceFault;
extern u16 		dark;
extern u16 		delta;
extern u8 		Timer50msCounter;

//extern s16		signal_array[SIGNAL_ARRAY_LEN];




/*********************************************************************************/
/*                                FUNCTIONS                                      */
/*********************************************************************************/
// Function prototipes
void DeviceStart(void);
u16  DefineCurLevel(void);
void SetLimitCompens(void);
void SetLevels(u16 norm);
void Led_Flash(u16 duration);
void ADC_Measure(u16 ch, u16 refout, u8 count);
u16  GetVCC(u8 boost_stop);
void DeviceDiagnostics(void);

#endif  /* __MAIN_H__ */




