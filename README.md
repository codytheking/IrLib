# IrLib Arduino Library

This is the code used to pulse an infrared led at 38 kHz. It uses PWM on pin 3 of
the ATmega328P. The code can be adapted to be used on other Atmel microcontrollers.

This code was taken from [Arduino-IRremote](https://github.com/z3t0/Arduino-IRremote) (originally done by Ken Shirriff). All I did 
was take out the parts I didn't need for this project, whch slimmed down the code significantly.

I used an Arduino UNO to program the microcontroller in the Arduino IDE. The folder irLib can be imported as
a library in the Arduino IDE.

See below for more information on the Roomba Virtual Wall project.

## Version 1.0

## Installation

1. Download the zip of the repository.
2. Extract the zip file.
3. Move the "IrLib" folder that has been extracted to your libraries directory.
4. In the Arduino IDE go to "Sketch" > "Include Library" > "IrLib"

## Hardware Details

The Roomba 600 comes with one virtual wall that keeps the Roomba from going into other rooms, so you can keep it confined to just one room. Unfortunately, you may have a room that has more than one doorway without a door and you are left to block one doorway with objects. An additional virtual wall costs around $45, so my wife's family asked me to make one on my own. I did some research and it turned out I could make one for around $30. Most of the parts are bought in bulk, so if you have spare electrical parts laying around you could probably make it for about $15 (cost of microcontroller, solderless breadboard or perfboard, and infrared led).

#### Specifications

Microcontroler: Atmel ATmega328P

Power source: Battery enclosure for 3 AA batteries (~4.5 V)

Capacitors: 2 at 100 uF

Resistors: 10 kOhm for microcontroller, 220 Ohm for LEDs.

![alt text](https://i.imgur.com/P9Zy8Zm.jpg "Arduino 1")

![alt text](https://i.imgur.com/vmRaqAa.jpg "Arduino 2")

![alt text](https://i.imgur.com/rfO74aY.jpg "Arduino 3")
