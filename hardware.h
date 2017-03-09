/**********************************************************************************
 *
 *      Hardware definitions
 *
 **********************************************************************************
 * FileName:        hardware.h
 * Version:			1.00
 *
 * Processor:       MSP430G2xxx
 * Complier:        IAR Workbench for MSP430 v4.50 or higher
 *                  
 * Company:         ARTON
 *
 *
 * Author               Date      	Version	  		Comment
 *-------------------------------------------------------------------------------
 * Oleg Semeniuk	 16.11.2013    	1.00		Release for v1.00
 *

 *********************************************************************************/

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

/*********************************************************************************/
/*                                 INCLUDES                                      */
/*********************************************************************************/
#include  "msp430g2332.h"
#include  "type.h"


/*********************************************************************************/
/*                               DEFINITIONS                                     */
/*********************************************************************************/
// For Red LED
#define RED_DIR   		P1DIR
#define RED_OUT   		P1OUT
#define RED_BIT  			BIT0
#define	RED_SET()			RED_OUT |= RED_BIT
#define	RED_CLR()			RED_OUT &= ~RED_BIT
#define	RED_TGL()			RED_OUT ^= RED_BIT

// For Yellow LED
#define YEL_DIR   		P1DIR
#define YEL_OUT   		P1OUT
#define YEL_BIT  			BIT1
#define	YEL_SET()			YEL_OUT |= YEL_BIT
#define	YEL_CLR()			YEL_OUT &= ~YEL_BIT

// For Line Syncronization
#define LN_SYNC_DIR   		P1DIR
#define LN_SYNC_OUT   		P1OUT
#define LN_SYNC_BIT  			BIT4
#define	LN_SYNC_SET()			LN_SYNC_OUT |= LN_SYNC_BIT
#define	LN_SYNC_CLR()			LN_SYNC_OUT &= ~LN_SYNC_BIT

// For Red IRED Led
#define IRED_DIR   		P2DIR
#define IRED_OUT   		P2OUT
#define IRED_BIT  		BIT6
#define	IRED_SET()		IRED_OUT |= IRED_BIT
#define	IRED_CLR()		IRED_OUT &= ~IRED_BIT

// For Red IRED Sync
#define IR_SYNC_DIR   	P1DIR
#define IR_SYNC_OUT   	P1OUT
#define IR_SYNC_BIT  		BIT5
#define	IR_SYNC_SET()		IR_SYNC_OUT |= IR_SYNC_BIT
#define	IR_SYNC_CLR()		IR_SYNC_OUT &= ~IR_SYNC_BIT




// For Intercom
#define INTER_DIR			P2DIR
#define INTER_IN			P2IN
#define INTER_OUT			P2OUT
#define INTER_IES			P2IES
#define INTER_IFG			P2IFG
#define INTER_IE			P2IE
#define INTER_REN			P2REN
#define INTER_BIT			BIT7
#define	INTER_SET()		INTER_OUT |= INTER_BIT
#define	INTER_CLR()		INTER_OUT &= ~INTER_BIT


/*********************************************************************************/
/*                                FUNCTIONS                                      */
/*********************************************************************************/
// Function prototipes
void GPIO_Init(void);

#endif  /* __HARDWARE_H__ */




