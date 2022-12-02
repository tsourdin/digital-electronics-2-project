/*
Pinout

PORTB:
PB0: Screen RS
PB1: Screen Enable
PB2: 
PB3: 
PB4: Software Interrupt PCINT4 => PCI0
PB5: 
PB6: 
PB7: 

PORTC:
PC0: Joystick X-Axis (ADC Channel 0)
PC1: Joystick Y-Axis (ADC Channel 1)
PC2: Joystick Switch
PC3: 
PC4: 
PC5: 
PC6: 
PC7:

PORTD:
PD0: Rotary Encoder Switch
PD1: Rotary Encoder DT
PD2: Rotary Encoder CLK (External Interrupt INT0)
PD3: 
PD4: Screen D4
PD5: Screen D5
PD6: Screen D6
PD7: Screen D7

*/

/* Includes */
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <uart.h>
#include <gpio.h>
#include <lcd.h>
#include <lcd_definitions.h>
#include "adc.h"
#include "timer.h"

/* Global Variables */
uint16_t x;
uint16_t y;
uint8_t right_left; // 0 : left | 1 : right
uint8_t right_left_modified; // flag set after modification
uint8_t up_down;  // 0 : down | 1 : up
uint8_t up_down_modified;
typedef enum {TIMER, CLOCK, STOPWATCH}mode;
mode selected_mode = TIMER;
char s[4];

struct Timer_structure{
  uint8_t minutes;
  uint8_t seconds;
}timer_struct;

struct Clock_structure{
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
}clock_struct;

struct Stopwatch_structure{
  uint8_t minutes;
  uint8_t seconds;
}stopwatch_struct;

int main(){

  lcd_init(LCD_DISP_ON);

  GPIO_mode_input_nopull(&DDRD,PD1);
  GPIO_mode_output(&DDRB,PB4);

  ADC_voltage_ref_AVCC();
  ADC_channel_0();
  ADC_prescaler_128();
  ADC_enable();
  ADC_interrupt_enable();

  TIM1_overflow_262ms();
  TIM1_overflow_interrupt_enable();

  // Set external interrupt INT0 on rising edge
  EICRA |= ((1<<ISC01) | (1<<ISC00));
  // Enable external interrupt INT0
  EIMSK |= (1<<INT0);

  // Set interrupt PCI0
  PCICR |= (1<<PCIE0);
  // Enable interrupt on pin PB4
  PCMSK0 |= (1<<PCINT4);

  sei();

  lcd_gotoxy(0,0);
  lcd_putc('*');
  lcd_puts("00:00");
  lcd_gotoxy(8,0);
  lcd_puts("00:00:00");
  lcd_gotoxy(1,1);
  lcd_puts("00:00");

  while(1){
    // Move the '*' to select between Timer, Stopwatch and Clock
    // State machine to switch between the 3 modes
    // If / else statement but could be more efficient

    if(selected_mode == TIMER){
      lcd_gotoxy(0,0);
      lcd_putc('*');
      lcd_gotoxy(0,0);
      if((right_left_modified == 1) && (right_left == 1)){
        selected_mode = CLOCK;
        right_left_modified = 0;
        lcd_putc(' ');
      }
      else if((up_down_modified == 1) && (up_down == 0)){
        selected_mode = STOPWATCH;
        up_down_modified = 0;
        lcd_putc(' ');
      }
    }

    else if(selected_mode == CLOCK){
      lcd_gotoxy(7,0);
      lcd_putc('*');
      lcd_gotoxy(7,0);
      if(right_left_modified == 1){
        if(right_left == 0){
          selected_mode = TIMER;
        }
        else{
          selected_mode = STOPWATCH;
        }
        right_left_modified = 0;
        lcd_putc(' ');
      }
    }

    else if(selected_mode == STOPWATCH){
      lcd_gotoxy(0,1);
      lcd_putc('*');
      lcd_gotoxy(0,1);
      if((right_left_modified == 1) && (right_left == 0)){
        selected_mode = CLOCK;
        right_left_modified = 0;
        lcd_putc(' ');
      }
      else if((up_down_modified == 1) && (up_down == 1)){
        selected_mode = TIMER;
        up_down_modified = 0;
        lcd_putc(' ');
      }
    }

    /* itoa(selected_mode,s,10);
    uart_puts(s);
    uart_puts("\n"); */
  }
  return 0;
}

ISR(INT0_vect){
  uint8_t dt = GPIO_read(&PIND,PD1);
  if(dt == 0) uart_puts("CW\n");
  else uart_puts("ACW\n");
}

ISR(TIMER1_OVF_vect){
  ADC_start_conversion();
}

ISR(ADC_vect){
  static uint8_t xy_flag = 0;
  char string[5];
  
  if(xy_flag == 0){
    x = ADC;
    itoa(x,string,10);
    /* uart_puts("x=");
    uart_puts(string);
    uart_puts("\n"); */
    xy_flag = 1;
    ADC_channel_1();
  }
  else{
    y = ADC;
    itoa(y,string,10);
    /* uart_puts("y=");
    uart_puts(string);
    uart_puts("\n"); */
    xy_flag = 0;
    ADC_channel_0();
  }

  if((x<10) | (y<10)){
    if(x<y){
      right_left = 0;
      right_left_modified = 1;
    }
    else{
      up_down = 0;
      up_down_modified = 1;
    }
    //GPIO_write_toggle(&PORTB,PB4); // Interruption software
  }
  else if((x>1013) | (y>1013)){
    if(x>y){
      right_left = 1;
      right_left_modified = 1;
    }
    else{
      up_down = 1;
      up_down_modified = 1;
    } 
    //GPIO_write_toggle(&PORTB,PB4);
  }
}

// For debug
ISR(PCINT0_vect){
  if(right_left == 0) uart_puts("left ");
  else uart_puts("right ");
  if(up_down == 0) uart_puts("down\n");
  else uart_puts("up\n");
}