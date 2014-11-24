#include <msp430.h>
//#include "main.h"

void main(void) {
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
	P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

	P2DIR |= BIT4;							// P2.2 is associated with TA1CCR2
	P2SEL |= BIT4;							// P2.2 is associated with TA1CCTL2



	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 100;							// set signal period

    TA1CCR1 = 50;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 50;
    TA1CCTL2 = OUTMOD_3;

    // P2.0 - Timer1_A, capture: CCI0A input, compare: Out0 output
    // P2.1 - Timer1_A, capture: CCI1A input, compare: Out1 output
    // P2.3 - Timer1_A, capture: CCI0B input, compare: Out0 output
    // P2.5 - Timer1_A, capture: CCI2B input, compare: Out2 output


    while (1) {

    	// Forward
    	P2DIR |= BIT0;
		P2OUT &= ~BIT0;
		P2DIR |= BIT3;
		P2OUT &= ~BIT3;
		TA1CCR1 = 50;
		TA1CCR2 = 50;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000);

		//Stop
		P2DIR |= BIT0;
		P2OUT |= BIT0;
		P2DIR |= BIT3;
		P2OUT |= BIT3;
		TA1CCR1 = 0;
		TA1CCR2 = 0;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000*2);

		// Backward
		P2DIR |= BIT1;
		P2OUT &= ~BIT1;
		P2DIR |= BIT5;
		P2OUT &= ~BIT5;
		TA1CCR1 = 50;
		TA1CCR2 = 50;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000);

		// Stop
		P2DIR |= BIT1;
		P2OUT |= BIT1;
		P2DIR |= BIT5;
		P2OUT |= BIT5;
		TA1CCR1 = 0;
		TA1CCR2 = 0;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000*2);

		// small left
		P2DIR |= BIT0;
		P2OUT |= BIT0;
		P2DIR |= BIT3;
		P2OUT &= ~BIT3;
		TA1CCR1 = 50;
		TA1CCR2 = 50;
		TA1CCTL1 = OUTMOD_3;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(100000);

		// Stop
		P2DIR |= BIT0;
		P2OUT |= BIT0;
		P2DIR |= BIT3;
		P2OUT |= BIT3;
		TA1CCR1 = 0;
		TA1CCR2 = 0;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000*2);

		// small right
		P2DIR |= BIT0;
		P2OUT &= ~BIT0;
		P2DIR |= BIT3;
		P2OUT |= BIT3;
		TA1CCR1 = 50;
		TA1CCR2 = 50;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_3;
		_delay_cycles(100000);

		// Stop
		P2DIR |= BIT0;
		P2OUT |= BIT0;
		P2DIR |= BIT3;
		P2OUT |= BIT3;
		TA1CCR1 = 0;
		TA1CCR2 = 0;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000*2);


		// big left
		P2DIR |= BIT0;
		P2OUT |= BIT0;
		P2DIR |= BIT3;
		P2OUT &= ~BIT3;
		TA1CCR1 = 50;
		TA1CCR2 = 50;
		TA1CCTL1 = OUTMOD_3;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000);

		// Stop
		P2DIR |= BIT0;
		P2OUT |= BIT0;
		P2DIR |= BIT3;
		P2OUT |= BIT3;
		TA1CCR1 = 0;
		TA1CCR2 = 0;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000*2);

		// big right
		P2DIR |= BIT0;
		P2OUT &= ~BIT0;
		P2DIR |= BIT3;
		P2OUT |= BIT3;
		TA1CCR1 = 50;
		TA1CCR2 = 50;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_3;
		_delay_cycles(1000000);

		// Stop
		P2DIR |= BIT0;
		P2OUT |= BIT0;
		P2DIR |= BIT3;
		P2OUT |= BIT3;
		TA1CCR1 = 0;
		TA1CCR2 = 0;
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		_delay_cycles(1000000*2);



    } // end loop
} // end main
//void stopMoving() {
//	P2DIR |= BIT0;
//	P2OUT |= BIT0;
//	P2DIR |= BIT1;
//	P2OUT |= BIT1;
//	P2DIR |= BIT3;
//	P2OUT |= BIT3;
//	P2DIR |= BIT5;
//	P2OUT |= BIT5;
//}
//
//void moveForward() {
//	P2DIR |= BIT1;
//	P2OUT &= ~BIT1;
//	P2DIR |= BIT5;
//	P2OUT &= ~BIT5;
//
//}
//
//void moveBackward() {
//
//}
//
//void moveSmallRight() {
//
//}
//
//void moveSmallLeft() {
//
//}
//
//void moveRight() {
//
//}
//
//void moveLeft() {
//
//}
//
