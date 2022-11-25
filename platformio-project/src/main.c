/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for number conversions
#include <uart.h>           // Peter Fleury's UART library
#include "adc.h"            // ADC library for AVR-GCC

int main(void){

  ADC_voltage_ref_AVCC();
  ADC_channel_0();
  ADC_enable();
  ADC_interrupt_enable();
  ADC_prescaler_128();
  ADC_auto_trigger_enable();
  ADC_auto_trigger_source_TIM1_OVF();

  TIM1_overflow_262ms();
  TIM1_overflow_interrupt_enable();
  sei();

  while(1){}
}

ISR(TIMER1_OVF_vect){
  static uint8_t flag = 0;
  switch(flag){
    case 0:
      ADC_channel_0(); flag = 1; break;
    case 1:
      ADC_channel_1(); flag = 0; break;
  }
}

ISR(ADC_vect){
  uint16_t value = ADC;
  char string[4];
  itoa(value, string, 10);
  uart_puts(string);
  uart_puts("\r\n");
}