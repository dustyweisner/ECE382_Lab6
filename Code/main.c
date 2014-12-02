/*
*Creator: Dustin Weisner
*1 Dec 2014 
*USAFA DFEC
*Documentation: I recieved help with setting and clearing motor pins from C1C Cassie McPeek.
*	I used much of the code from previous labs for this lab. 
*	I also used Dr. Coulston's code for initialization of pins and Timer controls.
*/

#include <msp430.h>
#include "main.h"

// intialize variables
int8	newIrPacket = FALSE;
int16	packetData[48];
int8	packetIndex = 0;
int32	irPacket = 0x00000000;
int8 packetCount =0;
int8 sum=0;
unsigned char flagged=0;
unsigned char movement=0;

void main(void) {
	// === Initialize system ================================================
	IFG1=0; /* clear interrupt flag1 */
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer
    initMSP430();

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
	P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

	P2DIR |= BIT4;							// P2.2 is associated with TA1CCR2
	P2SEL |= BIT4;							// P2.2 is associated with TA1CCTL2



	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 100;							// set signal period

    TA1CCR1 = 20;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 80;
    TA1CCTL2 = OUTMOD_3;

    // P2.0 - Timer1_A, capture: CCI0A input, compare: Out0 output
    // P2.1 - Timer1_A, capture: CCI1A input, compare: Out1 output
    // P2.3 - Timer1_A, capture: CCI0B input, compare: Out0 output
    // P2.5 - Timer1_A, capture: CCI2B input, compare: Out2 output

    while(1)  {

    		if(flagged==1) {
    			_disable_interrupt();
    			packetCount = 0;
    			while(packetData[packetCount]==2) {
    				packetCount++;
    			}
    			while(packetCount<33) {

    				irPacket+=packetData[packetCount];
    				irPacket<<=1;
    				packetCount++;
    			}
    			irPacket+=packetData[packetCount];




    			if(irPacket == SEL) {
					stopMovingForward();
					stopMovingBackward();
					movement=0;
				}

    			if (movement == 0) {

					if(irPacket == CH_UP) {
						moveForward();
						movement = 1;
					}
					if(irPacket == CH_DW) {
						moveBackward();
						movement = 1;
					}
					if(irPacket == CH_L) {
						moveLeft();
					}
					if(irPacket == CH_R) {
						moveRight();
					}
    			}

    			unsigned int i;
    			for(i=0; i<0xFFFF;i++);
    			for(i=0; i<0xFFFF;i++);
    			for(i=0; i<0xFFFF;i++);
    			packetIndex=0;
    			irPacket=0x00000000;
    			flagged =0;
    			_enable_interrupt();
    		} else {
    			irPacket = 0x00000000;
    		}




    	} // end if new IR packet arrived
} //end forever loop

// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	P1OUT &= ~(BIT0 | BIT6);			// An turn the LED off

	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}

// -----------------------------------------------------------------------
// Since the IR decoder is connected to P2.6, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global irPacket variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on P2.6
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we going into the logic 1 half of the bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {					// read the current pin level
		case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;

			// if the pulse duration is within range of a possible 0 or 1 data value,
			// the pulse duration is changed to a 0 or a 1. If the pulse duration is within the
			// range of a possible start bit, the pulse duration is changed to a 2 so that
			// in the main loop, the irPacket doesn't include start bits.
			if (pulseDuration >= minLogic1Pulse && pulseDuration <= maxLogic1Pulse) {
				pulseDuration = 1;
			} else if (pulseDuration >= minLogic0Pulse && pulseDuration <= maxLogic0Pulse ) {
				pulseDuration = 0;
			}else {
				pulseDuration = 2;
			}

			// Set all of the new pulse durations to the array of packetData
			packetData[packetIndex++] = pulseDuration;
			LOW_2_HIGH; // Setup pin interrupr on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;						// time measurements are based at time 0
			HIGH_2_LOW; 						// Setup pin interrupr on falling edge
			break;
	} // end switch

	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry

	// set an interrupt flag for if the program has experienced an interrupt (a remote button press)
	if (packetIndex > 33) {
		flagged=1;
	}
} // end pinChange ISR


// Timer interrupt below was not used:

// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------
//#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
//__interrupt void timerOverflow (void) {
//	TACTL &= ~TAIFG;
//}


/********* REQUIRED FUNCTIONALITY *************
    while (1) {

    	moveForward();
		_delay_cycles(1000000);
		stopMovingForward();
		_delay_cycles(1000000*2);
		moveBackward();
		_delay_cycles(1000000);
		stopMovingBackward();
		_delay_cycles(1000000*2);
		moveSmallLeft();
		_delay_cycles(1000000*2);
		moveSmallRight();
		_delay_cycles(1000000*2);
		moveLeft();
		_delay_cycles(1000000*2);
		moveRight();
		_delay_cycles(1000000*2);

    } // end forever loop
    ********************************/
 // end main


void stopMovingBackward(void) {
		P2DIR |= BIT1;
		P2OUT |= BIT1;
		P2DIR |= BIT5;
		P2OUT |= BIT5;
		TA1CCR1 = 0;
		TA1CCR2 = 0;
		TA1CCTL1 = OUTMOD_5;
		TA1CCTL2 = OUTMOD_5;
}

void stopMovingForward(void) {
		P2DIR |= BIT0;
		P2OUT |= BIT0;
		P2DIR |= BIT3;
		P2OUT |= BIT3;
		TA1CCR1 = 0;
		TA1CCR2 = 0;
		TA1CCTL1 = OUTMOD_5;
		TA1CCTL2 = OUTMOD_5;
}

void moveForward(void) {
    	P2DIR |= BIT0;
		P2OUT &= ~BIT0;
		P2DIR |= BIT3;
		P2OUT &= ~BIT3;
		TA1CCR1 = 20;
		TA1CCR2 = 80;
		TA1CCTL1 = OUTMOD_5;
		TA1CCTL2 = OUTMOD_7;

}

void moveBackward() {
	P2DIR |= BIT1;
	P2OUT &= ~BIT1;
	P2DIR |= BIT5;
	P2OUT &= ~BIT5;
	TA1CCR1 = 50;
	TA1CCR2 = 50;
	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_5;
}

void moveSmallRight() {
	P2DIR |= BIT0;
	P2OUT &= ~BIT0;
	P2DIR |= BIT3;
	P2OUT |= BIT3;
	TA1CCR1 = 50;
	TA1CCR2 = 50;
	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_5;
	_delay_cycles(250000);
	P2DIR |= BIT0;
	P2OUT |= BIT0;
	P2DIR |= BIT3;
	P2OUT |= BIT3;
	TA1CCR1 = 0;
	TA1CCR2 = 0;
	TA1CCTL1 = OUTMOD_5;
	TA1CCTL2 = OUTMOD_5;

}

void moveSmallLeft() {
	P2DIR |= BIT0;
	P2OUT |= BIT0;
	P2DIR |= BIT3;
	P2OUT &= ~BIT3;
	TA1CCR1 = 50;
	TA1CCR2 = 50;
	TA1CCTL1 = OUTMOD_5;
	TA1CCTL2 = OUTMOD_7;
	_delay_cycles(250000);
	P2DIR |= BIT0;
	P2OUT |= BIT0;
	P2DIR |= BIT3;
	P2OUT |= BIT3;
	TA1CCR1 = 0;
	TA1CCR2 = 0;
	TA1CCTL1 = OUTMOD_5;
	TA1CCTL2 = OUTMOD_5;

}

void moveRight() {
	P2DIR |= BIT0;
	P2OUT &= ~BIT0;
	P2DIR |= BIT3;
	P2OUT |= BIT3;
	TA1CCR1 = 50;
	TA1CCR2 = 50;
	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_5;
	_delay_cycles(900000);
	P2DIR |= BIT0;
	P2OUT |= BIT0;
	P2DIR |= BIT3;
	P2OUT |= BIT3;
	TA1CCR1 = 0;
	TA1CCR2 = 0;
	TA1CCTL1 = OUTMOD_5;
	TA1CCTL2 = OUTMOD_5;

}

void moveLeft() {
	P2DIR |= BIT0;
	P2OUT |= BIT0;
	P2DIR |= BIT3;
	P2OUT &= ~BIT3;
	TA1CCR1 = 50;
	TA1CCR2 = 50;
	TA1CCTL1 = OUTMOD_5;
	TA1CCTL2 = OUTMOD_7;
	_delay_cycles(900000);
	P2DIR |= BIT0;
	P2OUT |= BIT0;
	P2DIR |= BIT3;
	P2OUT |= BIT3;
	TA1CCR1 = 0;
	TA1CCR2 = 0;
	TA1CCTL1 = OUTMOD_5;
	TA1CCTL2 = OUTMOD_5;
}

