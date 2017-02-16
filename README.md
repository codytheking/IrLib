# IrLib Arduino Library

This is the code used to pulse an infrared led at 38 kHz. It uses PWM on pin 3 of
the ATmega328P. The code can be adapted to be used on other Atmel microcontrollers.

This code was taken from [Arduino-IRremote](https://github.com/z3t0/Arduino-IRremote) (originally done by Ken Shirriff). All I did 
was take out the parts I didn't need for this project, whch slimmed down the code significantly.

I used an Arduino UNO to program the microcontroller in the Arduino IDE. The folder irLib can be imported as
a library in the Arduino IDE.

For more information on the Roomba Virtual Wall project go to [my website](http://codyjking.weebly.com/electronics-projects.html).

## Version 1.0

## Installation

1. Download the zip of the repository.
2. Extract the zip file.
3. Move the "IrLib" folder that has been extracted to your libraries directory.
4. In the Arduino IDE go to "Sketch" > "Include Library" > "IrLib"
