# Flowcharts   

Here are all the flowcharts from the functions, witha brief explanation of the functionalities of each one. Those functions are:

### ISR(TIMER1_OVF_vect)
When we are in setting mode (we want to set a value) this function is in charge either deleting the actual value of the LCD screen or printing the actual value on it.
![](https://github.com/tsourdin/digital-electronics-2-project/blob/main/platformio-project/Documentation/timer1_ovf_vect.drawio.png)

### ISR(TIMER2_OVF_vect)
It is used for setting the time values of the modes. The overflow occurs every 100ms.
### ISR(ADC_vect)
Based on the ADC values that are read from the joystick we define the direction that is pointing the joystick so we can move arround the modes (clock, stopwatch and timer). 
### ISR(PCINT0_vect)
This function calculates the next mode based on the joystick position and the actual mode.--- NO IDEA---
### ISR(INT0_vect)
Function triggered by the rotatory encoder, it is used to change the value of a parameter ----ADD MORE INFO--

### ISR(INT1_vect)
### ISR(PCINT2_vect)

