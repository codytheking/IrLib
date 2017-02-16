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

#ifndef boarddefs_h
#define boarddefs_h

//------------------------------------------------------------------------------
// Defines for blinking the LED
//

#if defined(CORE_LED0_PIN)
#	define BLINKLED        CORE_LED0_PIN
#	define BLINKLED_ON()   (digitalWrite(CORE_LED0_PIN, HIGH))
#	define BLINKLED_OFF()  (digitalWrite(CORE_LED0_PIN, LOW))

#else
#	define BLINKLED        13
	#define BLINKLED_ON()  (PORTB |= B00100000)
#	define BLINKLED_OFF()  (PORTB &= B11011111)
#endif

//------------------------------------------------------------------------------
// CPU Frequency
//
#ifdef F_CPU
#	define SYSCLOCK  F_CPU     // main Arduino clock
#else
#	define SYSCLOCK  16000000  // main Arduino clock
#endif

// microseconds per clock interrupt tick
#define USECPERTICK    50

//------------------------------------------------------------------------------
// Define which timer to use
//
// Uncomment the timer you wish to use on your board.
// If you are using another library which uses timer2, you have options to
//   switch IRremote to use a different timer.
//

//#define IR_USE_TIMER1   // tx = pin 9
#define IR_USE_TIMER2     // tx = pin 3

//------------------------------------------------------------------------------
// Defines for Timer

//---------------------------------------------------------
// Timer2 (8 bits)
//

#define TIMER_RESET
#define TIMER_ENABLE_PWM    (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM   (TCCR2A &= ~(_BV(COM2B1)))
#define TIMER_ENABLE_INTR   (TIMSK2 = _BV(OCIE2A))
#define TIMER_DISABLE_INTR  (TIMSK2 = 0)
#define TIMER_INTR_NAME     TIMER2_COMPA_vect

#define TIMER_CONFIG_KHZ(val) ({ \
	const uint8_t pwmval = SYSCLOCK / 2000 / (val); \
	TCCR2A               = _BV(WGM20); \
	TCCR2B               = _BV(WGM22) | _BV(CS20); \
	OCR2A                = pwmval; \
	OCR2B                = pwmval / 3; \
})

#define TIMER_COUNT_TOP  (SYSCLOCK * USECPERTICK / 1000000)

//-----------------
#if (TIMER_COUNT_TOP < 256)
#	define TIMER_CONFIG_NORMAL() ({ \
		TCCR2A = _BV(WGM21); \
		TCCR2B = _BV(CS20); \
		OCR2A  = TIMER_COUNT_TOP; \
		TCNT2  = 0; \
	})
#else
#	define TIMER_CONFIG_NORMAL() ({ \
		TCCR2A = _BV(WGM21); \
		TCCR2B = _BV(CS21); \
		OCR2A  = TIMER_COUNT_TOP / 8; \
		TCNT2  = 0; \
	})
#endif

#define TIMER_PWM_PIN 3  // Arduino UNO (ATmega328)

#endif

/*
//---------------------------------------------------------
// Timer1 (16 bits)
//

#define TIMER_RESET
#define TIMER_ENABLE_PWM   (TCCR1A |= _BV(COM1A1))
#define TIMER_DISABLE_PWM  (TCCR1A &= ~(_BV(COM1A1)))

//-----------------

#define TIMER_ENABLE_INTR   (TIMSK1 = _BV(OCIE1A))
#define TIMER_DISABLE_INTR  (TIMSK1 = 0)


//-----------------
#define TIMER_INTR_NAME       TIMER1_COMPA_vect

#define TIMER_CONFIG_KHZ(val) ({ \
	const uint16_t pwmval = SYSCLOCK / 2000 / (val); \
	TCCR1A                = _BV(WGM11); \
	TCCR1B                = _BV(WGM13) | _BV(CS10); \
	ICR1                  = pwmval; \
	OCR1A                 = pwmval / 3; \
})

#define TIMER_CONFIG_NORMAL() ({ \
	TCCR1A = 0; \
	TCCR1B = _BV(WGM12) | _BV(CS10); \
	OCR1A  = SYSCLOCK * USECPERTICK / 1000000; \
	TCNT1  = 0; \
})

//-----------------

#define TIMER_PWM_PIN  9   // Arduino UNO (ATmega328)

*/

//---------------------------------------------------------
// Special carrier modulator timer
//
#if defined(IR_USE_TIMER_CMT)

#define TIMER_RESET ({     \
	uint8_t tmp __attribute__((unused)) = CMT_MSC; \
	CMT_CMD2 = 30;         \
})

#define TIMER_ENABLE_PWM  do {                                         \
	CORE_PIN5_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_DSE | PORT_PCR_SRE;  \
} while(0)

#define TIMER_DISABLE_PWM  do {                                        \
	CORE_PIN5_CONFIG = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;  \
} while(0)

#define TIMER_ENABLE_INTR   NVIC_ENABLE_IRQ(IRQ_CMT)
#define TIMER_DISABLE_INTR  NVIC_DISABLE_IRQ(IRQ_CMT)
#define TIMER_INTR_NAME     cmt_isr

//-----------------
#ifdef ISR
#	undef ISR
#endif
#define  ISR(f)  void f(void)

//-----------------
#define CMT_PPS_DIV  ((F_BUS + 7999999) / 8000000)
#if F_BUS < 8000000
#error IRremote requires at least 8 MHz on Teensy 3.x
#endif

//-----------------
#define TIMER_CONFIG_KHZ(val) ({ 	 \
	SIM_SCGC4 |= SIM_SCGC4_CMT;      \
	SIM_SOPT2 |= SIM_SOPT2_PTD7PAD;  \
	CMT_PPS    = CMT_PPS_DIV - 1;    \
	CMT_CGH1   = ((F_BUS / CMT_PPS_DIV / 3000) + ((val)/2)) / (val); \
	CMT_CGL1   = ((F_BUS / CMT_PPS_DIV / 1500) + ((val)/2)) / (val); \
	CMT_CMD1   = 0;                  \
	CMT_CMD2   = 30;                 \
	CMT_CMD3   = 0;                  \
	CMT_CMD4   = 0;                  \
	CMT_OC     = 0x60;               \
	CMT_MSC    = 0x01;               \
})

#define TIMER_CONFIG_NORMAL() ({  \
	SIM_SCGC4 |= SIM_SCGC4_CMT;   \
	CMT_PPS    = CMT_PPS_DIV - 1; \
	CMT_CGH1   = 1;               \
	CMT_CGL1   = 1;               \
	CMT_CMD1   = 0;               \
	CMT_CMD2   = 30;              \
	CMT_CMD3   = 0;               \
	CMT_CMD4   = (F_BUS / 160000 + CMT_PPS_DIV / 2) / CMT_PPS_DIV - 31; \
	CMT_OC     = 0;               \
	CMT_MSC    = 0x03;            \
})

#define TIMER_PWM_PIN  5

// defines for TPM1 timer on Teensy-LC
#elif defined(IR_USE_TIMER_TPM1)
#define TIMER_RESET          FTM1_SC |= FTM_SC_TOF;
#define TIMER_ENABLE_PWM     CORE_PIN16_CONFIG = PORT_PCR_MUX(3)|PORT_PCR_DSE|PORT_PCR_SRE
#define TIMER_DISABLE_PWM    CORE_PIN16_CONFIG = PORT_PCR_MUX(1)|PORT_PCR_SRE
#define TIMER_ENABLE_INTR    NVIC_ENABLE_IRQ(IRQ_FTM1)
#define TIMER_DISABLE_INTR   NVIC_DISABLE_IRQ(IRQ_FTM1)
#define TIMER_INTR_NAME      ftm1_isr
#ifdef ISR
#undef ISR
#endif
#define ISR(f) void f(void)
#define TIMER_CONFIG_KHZ(val) ({                     \
	SIM_SCGC6 |= SIM_SCGC6_TPM1;                 \
	FTM1_SC = 0;                                 \
	FTM1_CNT = 0;                                \
	FTM1_MOD = (F_PLL/2000) / val - 1;           \
	FTM1_C0V = (F_PLL/6000) / val - 1;           \
	FTM1_SC = FTM_SC_CLKS(1) | FTM_SC_PS(0);     \
})
#define TIMER_CONFIG_NORMAL() ({                     \
	SIM_SCGC6 |= SIM_SCGC6_TPM1;                 \
	FTM1_SC = 0;                                 \
	FTM1_CNT = 0;                                \
	FTM1_MOD = (F_PLL/40000) - 1;                \
	FTM1_C0V = 0;                                \
	FTM1_SC = FTM_SC_CLKS(1) | FTM_SC_PS(0) | FTM_SC_TOF | FTM_SC_TOIE; \
})
#define TIMER_PWM_PIN        16

// defines for timer_tiny0 (8 bits)
#elif defined(IR_USE_TIMER_TINY0)
#define TIMER_RESET
#define TIMER_ENABLE_PWM     (TCCR0A |= _BV(COM0B1))
#define TIMER_DISABLE_PWM    (TCCR0A &= ~(_BV(COM0B1)))
#define TIMER_ENABLE_INTR    (TIMSK |= _BV(OCIE0A))
#define TIMER_DISABLE_INTR   (TIMSK &= ~(_BV(OCIE0A)))
#define TIMER_INTR_NAME      TIMER0_COMPA_vect
#define TIMER_CONFIG_KHZ(val) ({ \
  const uint8_t pwmval = SYSCLOCK / 2000 / (val); \
  TCCR0A = _BV(WGM00); \
  TCCR0B = _BV(WGM02) | _BV(CS00); \
  OCR0A = pwmval; \
  OCR0B = pwmval / 3; \
})
#define TIMER_COUNT_TOP      (SYSCLOCK * USECPERTICK / 1000000)
#if (TIMER_COUNT_TOP < 256)
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR0A = _BV(WGM01); \
  TCCR0B = _BV(CS00); \
  OCR0A = TIMER_COUNT_TOP; \
  TCNT0 = 0; \
})
#else
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR0A = _BV(WGM01); \
  TCCR0B = _BV(CS01); \
  OCR0A = TIMER_COUNT_TOP / 8; \
  TCNT0 = 0; \
})
#endif

#define TIMER_PWM_PIN        1  /* ATtiny85 */

//---------------------------------------------------------
// Unknown Timer
//
//#else
//#	error "Internal code configuration error, no known IR_USE_TIMER# defined\n"
//#endif

#endif // ! boarddefs_h