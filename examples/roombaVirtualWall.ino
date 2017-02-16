#include "IRsend.h"

/*
 * LED pulses through PWM (infrared)
 * 
 * @author Cody King
 * @date 1/3/2017
 */

//#define irPin 3  //IR carrier output

IRsend irsend;  

void setup() 
{
  // Arduino pin 3 is hardcoded in library
  // pin 5 (OC2B, timer 2) on ATmega328
  irsend.enableIROut(38);  
}

void loop() 
{ 
  irsend.mark(1000);  //pwm enabled for 1 millisecond (1000 micro) (arg is in microseconds)
  irsend.space(1000);  //pwn disabled for 1 millisecond (arg is in microseconds)
}
