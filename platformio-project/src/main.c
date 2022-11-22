/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for number conversions
#include <uart.h>           // Peter Fleury's UART library

int main(void){
  TIM1_overflow_262ms();
  TIM1_overflow_interrupt_enable();
  sei();
}

ISR(TIMER1_OVF_vect){
  
}