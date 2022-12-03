# Digital Electronics 2 - Project

## Description of the project

Application using an analog joy-stick (2 ADC channels, 1 push button), rotary encoder (+ push button), and Digilent PmodCLP LCD module.
We chose to implement three different features for this project : A timer, a clock and a stopwatch. The joystick will allow the user to choose between the 3 modes, and a click on the joystick push button will allow to setup the mode selected. Then, the joystick is used to select hours/minutes/seconds, and the rotary encoder is used to choose the value. A click on the rotary push button allows to reset all the values. Then, another click on the joystick push button exits the setting mode and goes back to selection. When selected, the timer and stopwatch are launched with a click on the rotary encoder push button.

## Team members

* Thibault Sourdin
* Nico (I don't know your surname sorry ^^)

## Hardware description

The screen Digilent PmodCLP uses a ... controller. Therefore, it could be connected as we did during the labs :
PIN description

The joystick has 2 variable tension outputs : one for the vertical direction and one for the horizontal direction. They were connected to ADC Channels 0 and 1 on the micro controller (which corresponds to PC0 and PC1) The push button is used to trigger and external interrupt and is therefore connected to external interrupt INT1 (on pin PD3).

The rotary encoder has 2 digital outputs : CLK and DT. CLK triggers the external interrupt INT0 (on pin PD2). DT is connected to a simple GPIO (PD1). The push button is connected to PD0 and triggers the interrupt PCINT2 on pin change.

TODO : add diagram.

## Software description

Put flowchats of your algorithm(s). Write descriptive text of your libraries and source files. Put direct links to these files in `src` or `lib` folders.

## Video

Insert a link to a short video with your practical implementation example (1-3 minutes, e.g. on YouTube).

## References

1. Write your text here.
2. ...