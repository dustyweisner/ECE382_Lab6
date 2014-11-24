ECE382_Lab6
===========

PWM - Robot Motion


__*PRELAB*__

__Hardware Setup__  
This Lab requires me to demonstrate movement forward, backward, a small (< 45 degree) turn left and right, and a large (> 45 degree) turn left and right. The robot should perform these movements sequentially, completely disconnected from a computer (no USB cord). Before I begin to set it up, I must think of a way to connect all of the pins and hardware to achieve robot control.  First, I will show the connection of the pins as if I had the software already created and ready for upload. To upload the program onto the MSP430 chip for a "USB-less" circuit, I will first pull the 3 jumpers off of the MSP430 Launchpad's VCC, TEST, and RST. Then I will connect wires from those three to their specific places on the MSP430 chip. The GND goes to pin 20 in figure 1, the VCC goes to pin 1, and TEST to pin 17. Then I will hook the MSP430 Launchpad to the USB cable and upload the program to the MSP430 chip. I will also make sure that the MSP430 chip is the correct MSP430 chip type. Then once the program is hardcoded into the MSP430 chip, the following circuit can be hooked up:

__FIGURE 1__

![](https://github.com/dustyweisner/ECE382_Lab6/blob/master/Images/HardwareSchematic.gif?raw=true)

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
