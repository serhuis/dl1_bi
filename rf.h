/**************************************************************************//**
    @file       rf.h

    @brief      Radio Frequensy routines header file.
******************************************************************************/
#ifndef __RF_H
#define __RF_H


/******************************************************************************
 * INCLUDES
 */

#include "type.h"

/*******************************************************************************
* DEFINES
*/

#define RF_CHANNEL_MAX			10		/* Number of RF channels */

#define ISR_ACTION_REQUIRED     1
#define ISR_IDLE                0
#define RX_FIFO_ERROR       	0x11

#define PKTLEN                  6  	/* 1 < PKTLEN < 126 */


#define TX_PHASE_0_TIME			25  /* in sec */
#define TX_PHASE_1_PACKET_COUNT	3  	/* Count packet in series in tx phase 1 (after TX_PHASE_0_TIME */

#define RF_MODE_0  				0	/* Main RF mode (preamble = 1024 bytes) */
#define RF_MODE_1  				1 	/* Add RF mode (preamble = 12 bytes) */


#define NO_NEED_INIT			0
#define NEED_INIT				1

// Wireless command 
#define RF_COMM_ALARM_ON		1
#define RF_COMM_ALARM_OFF		2
#define RF_COMM_BAT_FAULT_ON	3


/*******************************************************************************
* LOCAL VARIABLES
*/

extern u16 packetCounter;
extern u8  packetSemaphore;
extern u8  packetSemaphore_Sniff;
extern u8  packetSemaphore_RX_overflow;


	 
/******************************************************************************
 * GLOBAL FUNCTIONS
 */

void RF_Init(void);
void RF_ReInit(u8 mode);
void RF_RunTX(void);

void RF_TXMode(u8 mode);
void RF_SendPacket(u8 data);

void RF_RunRX(void);
void RF_RunRX_Sniff(void);

void RF_SniffMode(u8 need_init, u8 mode);
void RF_IdleMode(void);
void RF_PowerDownMode(void);

u8   RF_RX_Handler(void);

void CC_IRQ_Handler(void);
void SNIFF_IRQ_Handler(void);
void RX_overflow_IRQ_Handler(void);


void RF_RegisterConfig(u8 mode);
void RF_SetPreamble(u8 mode);




#endif		//__RF_H
