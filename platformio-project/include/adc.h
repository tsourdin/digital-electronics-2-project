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

/**
 * @brief  
 */
#define ADC_voltage_ref_AREF()     ADMUX &= ~((1<<REFS0) | (<<REFS1));

#define ADC_voltage_ref_AVCC()      ADMUX &= ~(1<<REFS1); ADMUX |= (1<<REFS0);

#define ADC_voltage_ref_internal()  ADMUX |= ((1<<REFS0) | ())



#endif