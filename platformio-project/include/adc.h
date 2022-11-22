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
#define ADC_voltage_ref_AREF()      ADMUX &= ~((1<<REFS0) | (<<REFS1));
#define ADC_voltage_ref_AVCC()      ADMUX &= ~(1<<REFS1); ADMUX |= (1<<REFS0);
#define ADC_voltage_ref_internal()  ADMUX |= ((1<<REFS0) | (1<<REFS1));

#define ADC_channel_0()     ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
#define ADC_channel_1()     ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1)); ADMUX |= (1 << MUX0);

// define for other channels optional

#define ADC_enable()            ADCSRA |= (1 << ADEN);
#define ADC_interrupt_enable()  ADCSRA |= (1 << ADIE);
#define ADC_prescaler_128()     ADCSRA |= ((1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));

// define for other prescaler values optional

#define ADC_start_conversion()  ADCSRA |= (1 << ADSC);

#endif