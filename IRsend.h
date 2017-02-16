//******************************************************************************
// IrLib by Cody King
// Jan 2017
// Adapted from IRremote (details below)
// 
// -------------------------------------------------------------------------------
//
// IRremote
// Version 2.0.1 June, 2015
// Copyright 2009 Ken Shirriff
// For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
//
// Interrupt code based on NECIRrcv by Joe Knapp
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
// Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
//
//******************************************************************************

#ifndef IRsend_h
#define IRsend_h

//------------------------------------------------------------------------------
// The ISR header contains several useful macros the user may wish to use
//
#include "IRremoteInt.h"

//------------------------------------------------------------------------------
// Set DEBUG to 1 for lots of lovely debug output
//
#define DEBUG  0

//------------------------------------------------------------------------------
// Debug directives
//
#if DEBUG
#	define DBG_PRINT(...)    Serial.print(__VA_ARGS__)
#	define DBG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#else
#	define DBG_PRINT(...)
#	define DBG_PRINTLN(...)
#endif

//------------------------------------------------------------------------------
// Main class for sending IR
//
class IRsend
{
	public:
		IRsend () { }

		void custom_delay_usec  (unsigned long uSecs);
		void enableIROut 	    (int khz);
		void mark        		(unsigned int usec);
		void space       		(unsigned int usec);
		void sendRaw     		(const unsigned int buf[],  unsigned int len,  unsigned int hz);		
};

#endif