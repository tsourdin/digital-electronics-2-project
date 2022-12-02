#ifndef ADC_H
#define ADC_H

/***********************************************************************
 * 
 * ADC library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2022 Thibault Sourdin
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/**
 * @file 
 * @defgroup sourdin_adc ADC Library <adc.h>
 * @code #include "adc.h" @endcode
 *
 * @brief ADC library for AVR-GCC.
 *
 * The library contains functions for controlling AVRs' ADC modules.
 *
 * @note Based on AVR Libc Reference Manual. Tested on ATmega328P 
 *       (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2.
 * @author Thibault Sourdin
 * @copyright (c) 2022 Thibault Sourdin, This work is licensed under 
 *                the terms of the MIT license
 * @{
 */


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>


/* Defines -----------------------------------------------------------*/

/** @brief  */
// ADMUX[7,6] = 00
#define ADC_voltage_ref_AREF()      ADMUX &= ~((1<<REFS1) | (1<<REFS0));
// ADMUX[7,6] = 01
#define ADC_voltage_ref_AVCC()      ADMUX &= ~(1<<REFS1); ADMUX |= (1<<REFS0);
// ADMUX[7,6] = 11
#define ADC_voltage_ref_internal()  ADMUX |= ((1<<REFS1) | (1<<REFS0));

// ADMUX[3..0] = 0000
#define ADC_channel_0()     ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
// ADMUX[3..0] = 0001
#define ADC_channel_1()     ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1)); ADMUX |= (1<<MUX0);
// defines for other channels optionals

// ADCSRA[7] = 1
#define ADC_enable()            ADCSRA |= (1<<ADEN);
// ADCSRA[6] = 1
#define ADC_start_conversion()  ADCSRA |= (1<<ADSC);
// ADCSRA[5] = 1
#define ADC_auto_trigger_enable()   ADCSRA |= (1<<ADATE);
// ADCSRA[3] = 1
#define ADC_interrupt_enable()  ADCSRA |= (1<<ADIE);
// ADCSRA[2..0] = 111
#define ADC_prescaler_128()     ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
// defines for other prescaler values optionals

// ADCSRB[2..0] = 000
#define ADC_auto_trigger_source_free_running()  ADCSRB &= ~((1<<ADTS2) | (1<<ADTS1) | (1<<ADCS0));
// ADCSRB[2..0] = 100
#define ADC_auto_trigger_source_TIM0_OVF()      ADCSRB |= (1<<ADTS2); ADCSRB &= ~((1<<ADTS1) | (1<<ADTS0));
// ADCSRB[2..0] = 110
#define ADC_auto_trigger_source_TIM1_OVF()      ADCSRB |= ((1<<ADTS2) | (1<<ADTS1)); ADCSRB &= ~(1<<ADTS0);


#endif