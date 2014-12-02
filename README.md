ECE382_Lab6
===========

PWM - Robot Motion


__*PRELAB*__

__Hardware Setup__  
This Lab requires me to demonstrate movement forward, backward, a small (< 45 degree) turn left and right, and a large (> 45 degree) turn left and right. The robot should perform these movements sequentially, completely disconnected from a computer (no USB cord). Before I begin to set it up, I must think of a way to connect all of the pins and hardware to achieve robot control.  First, I will show the connection of the pins as if I had the software already created and ready for upload. To upload the program onto the MSP430 chip for a "USB-less" circuit, I will first pull the 3 jumpers off of the MSP430 Launchpad's VCC, TEST, and RST. Then I will connect wires from those three to their specific places on the MSP430 chip. The GND goes to pin 20 in figure 1, the VCC goes to pin 1, and TEST to pin 17. Then I will hook the MSP430 Launchpad to the USB cable and upload the program to the MSP430 chip. I will also make sure that the MSP430 chip is the correct MSP430 chip type. Then once the program is hardcoded into the MSP430 chip, the following circuit can be hooked up:

__FIGURE 1__

![](https://github.com/dustyweisner/ECE382_Lab6/blob/master/Images/HardwareSchematic.gif?raw=true)

CORRECTIONS TO FIGURE 1:  
-Connect the capacitor from Vdd to Ground, not across the rails.
-Changed the motor pins to P2.0, P2.1, P2.3, and P2.5

The following is a chart of the pulse width modulations that will control the wheel (This idea was inspired by C2C Clayton Jaksha, and I thought it was a vital and simple snippet of information):

|Movement|Left Motor +|Left Motor -|Right Motor +|Right Motor -|
|:--|:--:|:--:|:--:|:--:|
|Forward|PWM|0|PWM|0|
|Back|0|PWM|0|PWM|
|Right|PWM|0|0|PWM|
|Left|0|PWM|PWM|0|
|Right < 45|PWM/2|0|0|PWM/2|
|Left < 45|0|PWM/2|PWM/2|0|

__Software Setup__  
Each remote button press will be just like in my ECE382_Lab5 where there will be an interrupt that then goes through many if loops according to the remote button that was pressed. But instead of turning on LEDs or making an etch-a-sketch game, the if loops will control the motors. The MSP430 pins that I will use for the motors are P1.2, P1.3, P1.4, P1.5; which respectively correspond to the Left Motor +, Left Motor -, Right Motor +, and Right Motor -. If the remote button that I choose for a certain function is pressed, there will be a method underneath that will make the motor happen. The following are the methods that will be used: moveForward(), moveBackward(), moveRight(), moveLeft(), moveLittleRight(), and moveLittleLeft(). TA0CCRO will control the left motor, and TA1CCRO will control the right motor. 

__Lab__  
__Required Functionality__  
The required functionality required me to demonstrate movement forward, backward, a small (< 45 degree) turn left and right, and a large (> 45 degree) turn left and right. The robot should perform these movements sequentially, completely disconnected from a computer (no USB cord). To begin, I had to see what the pins were capturing:

    // P2.0 - Timer1_A, capture: CCI0A input, compare: Out0 output
    // P2.1 - Timer1_A, capture: CCI1A input, compare: Out1 output
    // P2.3 - Timer1_A, capture: CCI0B input, compare: Out0 output
    // P2.5 - Timer1_A, capture: CCI2B input, compare: Out2 output

I used pins P2.2 and P2.4 for TA1CCR1 and TA1CCR2. Then I initialized the prescalar to ID_3, TASSEL_2, and MC_1. I also set my duty cycle for TA1CCR1 to 20 with a reset/set mode and TA1CCR2 to 80 with the set/reset mode:

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

Then I created the main program containing the moves (written as methods) and delays. The methods were not created yet, but it gave me an idea of where to start:

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

First, I started with the `moveForward()` method by making the following:

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

At first, I could not get the wheels to spin at the same direction, so I just switched the motor pins. BIT0 and BIT3 cleared made the wheels spin forward. Then I did the same but for the backward method and realized that the BIT1 and BIT5 cleared made the wheels spin backwards:

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

Then I moved on to stop the wheels. All I had to do was set the motor pins. To move right and left, I would set one pin (0 - left wheel or 3 - right wheel). By clearing the left wheel, I could spin the robot right. This method is shown below:

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

Notice that I had to turn the robot for a delayed amount of time, and then I would stop both my wheels...the execution of a turn could not continue like a forward or backward movement. To make a smaller or larger turn right or left, I only had to delay for longer (bigger turn) or shorter (shorter turn).

__A Functionality__ 
The A functionality of the lab required me to move the robot using button pushes on the tv remote from lab 5. All I did was import the initializations and variables, `afunc5.h`, and interrupts. The header file contained all of the tv remote button press information from lab 5. The initializations are as follows:

    IFG1=0; /* clear interrupt flag1 */
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer
    initMSP430();

Of course, I had to include the header file and my variables that made the irPacket from lab 5 work. I did nothing more to the code, except for in the main. The interrupt `pinChange` and `initMSP430()` were also included with no changes.

However, the main took a little bit of work, but not much. From the lab 5 main, I only changed the if statements of the tv remote turning on the LED lights to using the movement methods that I create in the Lab 6 required functionality section:

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

Above was the only code that I had to mess with for the A functionality. I programmed the up button to move the robot forward and the same for left, right, and down (back) buttons. The select button was a movement stopper. For debugging purposes, I had to make the robot only susceptible to movement change only if the robot was stopped. Before, my robot was not recieving well. I also added another delay loop at the end of my disabled interrupt cycle for longer time between button presses as to not screw up the irPacket. This concludes Lab 6 A functionality.


__Documentation__
I recieved help with setting and clearing motor pins from C1C Cassie McPeek. I used much of the code from previous labs for this lab. I also used Dr. Coulston's code for initialization of pins and Timer controls.
