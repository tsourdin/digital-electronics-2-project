# Flowcharts   

Here are all the flowcharts from the functions, witha brief explanation of the functionalities of each one. Those functions are:

### ISR(TIMER1_OVF_vect)
When we are in setting mode (we want to set a value) this function is in charge either deleting the actual value of the LCD screen or printing the actual value on it.

![](https://github.com/tsourdin/digital-electronics-2-project/blob/main/platformio-project/Documentation/timer1_ovf_vect.drawio.png)

### ISR(TIMER2_OVF_vect)
It is used for setting the time values of the modes. The overflow occurs every 100ms.

![](https://github.com/tsourdin/digital-electronics-2-project/blob/main/platformio-project/Documentation/TIM2_OVF_VECT.png)
### ISR(ADC_vect)
Based on the ADC values that are read from the joystick we define the direction that is pointing the joystick so we can move arround the modes (clock, stopwatch and timer). 

![](https://github.com/tsourdin/digital-electronics-2-project/blob/main/platformio-project/Documentation/ADC_VECT.drawio.png)
### ISR(PCINT0_vect)
This function calculates the next mode based on the joystick position and the actual mode.--- NO IDEA---

![](https://github.com/tsourdin/digital-electronics-2-project/blob/main/platformio-project/Documentation/PCINT0_VECT.jpeg)
### ISR(INT0_vect)
Function triggered by the rotatory encoder, it is used to change the value of a parameter ----ADD MORE INFO--

![](https://github.com/tsourdin/digital-electronics-2-project/blob/main/platformio-project/Documentation/IN0_VECT.png)

### ISR(INT1_vect)

![](https://github.com/tsourdin/digital-electronics-2-project/blob/main/platformio-project/Documentation/INT1_VECT.drawio.png)
### ISR(PCINT2_vect)

![](https://github.com/tsourdin/digital-electronics-2-project/blob/main/platformio-project/Documentation/PCINT2_VECT.drawio.png)

