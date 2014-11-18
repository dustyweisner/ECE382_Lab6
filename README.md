ECE382_Lab6
===========

PWM - Robot Motion


__*PRELAB*__

__Hardware Setup__  
This Lab requires me to demonstrate movement forward, backward, a small (< 45 degree) turn left and right, and a large (> 45 degree) turn left and right. The robot should perform these movements sequentially, completely disconnected from a computer (no USB cord). Before I begin to set it up, I must think of a way to connect all of the pins and hardware to achieve robot control.  First, I will show the connection of the pins as if I had the software already created and ready for upload. To upload the program onto the MSP430 chip for a "USB-less" circuit, I will first pull the 3 jumpers off of the MSP430 Launchpad's VCC, TEST, and RST. Then I will connect wires from those three to their specific places on the MSP430 chip. The GND goes to pin 20 in figure 1, the VCC goes to pin 1, and TEST to pin 17. Then I will hook the MSP430 Launchpad to the USB cable and upload the program to the MSP430 chip. I will also make sure that the MSP430 chip is the correct MSP430 chip type. Then once the program is hardcoded into the MSP430 chip, the following circuit can be hooked up:

FIGURE 1


The following is a chart of the pulse width modulations that will control the wheel:

|Movement|Left Motor +|Left Motor -|Right Motor +|Right Motor -|
|:--|:--:|:--:|:--:|:--:|
|Forward|PWM|0|PWM|0|
|Back|0|PWM|0|PWM|
|Right|PWM|0|0|PWM|
|Left|0|PWM|PWM|0|
|Right 45|PWM/2|0|0|PWM/2|
|Left 45|0|PWM/2|PWM/2|0|

__Software Setup__  
T
