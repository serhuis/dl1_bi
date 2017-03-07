/**********************************************************************************
 *
 *      Hardware routines
 *
 **********************************************************************************
 * FileName:        hardware.c
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
/*********************************************************************************/
/*                                 INCLUDES                                      */
/*********************************************************************************/
#include  "hardware.h"
#include  "main.h"

/*********************************************************************************/
/*                                 VARIABLES                                     */
/*********************************************************************************/
	 
extern __regvar __no_init tFlags 	f 	 @ __R4; 



/*********************************************************************************/
/*                                FUNCTIONS                                      */
/*********************************************************************************/



//--------------------------------------------------------------------------------
// Function		: __interrupt void PORT1_ISR(void)
// Parameters	: None
// Return		: None
// Description	: GPIO Interrupt routine for Soft UART & Button Press 
//--------------------------------------------------------------------------------
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void) {   
	/*
	if (BUT_IFG & BUT_BIT) {
		BUT_IFG &= ~BUT_BIT;    		// �������� ���� ����������
        //
		if (BUT_IES & BUT_BIT) {		// ��������� �����
			BUT_IES &= ~BUT_BIT;
			fButtonDownOn = 1;
		}else{
			BUT_IES |= BUT_BIT;
			fButtonUpOn = 1;
		}
		//
		__bic_SR_register_on_exit(LPM3_bits);               // Clear LPM3 bits from 0(SR) �����������
	}
	*/
}



//--------------------------------------------------------------------------------
// Function		: __interrupt void PORT2_ISR(void)
// Parameters	: None
// Return		: None
// Description	: GPIO Interrupt routine for RF Button Press 
//--------------------------------------------------------------------------------
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void) {   
	
	//
	if ((INTER_IE & INTER_BIT) && (INTER_IFG & INTER_BIT)) {
		INTER_IFG &= ~INTER_BIT;    			// �������� ���� ����������
        //
		BCSCTL1 = CALBC1_8MHZ; 					// ���������� ������� 8 M��
		DCOCTL =  CALDCO_8MHZ;
		//
		fRxLineDownOn = 1;
		//
		//__bic_SR_register_on_exit(LPM3_bits);   // Clear LPM3 bits from 0(SR) �����������
		
	}
}

//---------------------------------------------------------------------------------
// Function		: void INTER_INT_Init(void)
// Parameters	: None
// Return		: None
// Description	: Initialization interrupts from Intercom (Soft UART)
//---------------------------------------------------------------------------------
void INTER_INT_Init(void) {
	//
	INTER_REN |= INTER_BIT; 		// PULL enable
	INTER_OUT |= INTER_BIT;			// PULLUP Resistor 
	
	INTER_DIR &= ~INTER_BIT;
	INTER_IES |= INTER_BIT;  		// ���������� �� �������� �� 1 � 0, 
                					// ��������������� ��������������� ����� IES.x = 1.
	INTER_IFG &= ~INTER_BIT; 		// ��� �������������� ������������ ������������ ����������,
                					// �������� ��� ���� ��� P1.3 �� ���������� ����������
	INTER_IE  |= INTER_BIT;   		// ��������� ���������� ��� P1.3
}


//---------------------------------------------------------------------------------
// Function		: void GPIO_Init(void)
// Parameters	: None
// Return		: None
// Description	: Initialization GPIO 
//---------------------------------------------------------------------------------
void GPIO_Init(void) {

	P1OUT = 0;	
	P2OUT = 0;
	
	RED_DIR 		|= RED_BIT;
	YEL_DIR 		|= YEL_BIT;
	
	

	P1SEL = 0;
	P2SEL = 0;
	
	INTER_INT_Init();				// Init interrupts from Intercom
}

// End hardware.c
