/*
Pinout

PORTB:
PB0: Screen RS
PB1: Screen Enable
PB2: 
PB3: 
PB4: Software Interrupt pin PCINT4 (interrupt PCINT0)
PB5: 
PB6: 
PB7: 

PORTC:
PC0: Joystick X-Axis (ADC Channel 0)
PC1: Joystick Y-Axis (ADC Channel 1)
PC2: 
PC3: 
PC4: 
PC5: 
PC6: 
PC7:

PORTD:
PD0: Rotary Encoder Switch, pin PCINT16 (ext. interrupt PCINT2)
PD1: Rotary Encoder DT
PD2: Rotary Encoder CLK (External Interrupt INT0)
PD3: Joystick Switch (External Interrupt INT1)
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
/* uint8_t right_left = 0; // 0 : left | 1 : right
uint8_t right_left_modified = 0; // flag set after modification
uint8_t up_down = 1;  // 0 : down | 1 : up
uint8_t up_down_modified = 0; */

typedef enum {LEFT, RIGHT, DOWN, UP}direction;
direction joystick_direction;

typedef enum {TIMER, CLOCK, STOPWATCH, NONE}mode;
mode selection_mode = TIMER;
mode setting_mode = NONE;

typedef enum {ACTIVATED, DEACTIVATED}mode_state;
mode_state timer_state = DEACTIVATED;
mode_state stopwatch_state = DEACTIVATED;

typedef enum {HOURS, MINUTES, SECONDS}time_value;
time_value timer_time_value = MINUTES;
time_value clock_time_value = HOURS;
time_value stopwatch_time_value = MINUTES;

char s[4];

struct Timer_structure{
  int8_t minutes;
  int8_t seconds;
  int8_t tenths;
}timer_struct;

struct Clock_structure{
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint8_t tenths;
}clock_struct;

struct Stopwatch_structure{
  uint8_t minutes;
  uint8_t seconds;
  uint8_t tenths;
}stopwatch_struct;

void display_time(uint8_t value, uint8_t x, uint8_t y){
  char string[2];
  if(value <= 9){
    itoa(value, &string[1], 10);
    string[0]='0';
  }
  else itoa(value, string, 10);
  lcd_gotoxy(x,y);
  lcd_puts(string);
}

int main(){

  stopwatch_struct.minutes = 0;
  stopwatch_struct.seconds = 0;
  stopwatch_struct.tenths = 0;

  timer_struct.minutes = 0;
  timer_struct.seconds = 10;

  lcd_init(LCD_DISP_ON);

  GPIO_mode_input_nopull(&DDRD,PD1);
  GPIO_mode_output(&DDRB,PB4);
  GPIO_mode_input_pullup(&DDRD,PD0);
  GPIO_mode_input_pullup(&DDRD,PD3);

  ADC_voltage_ref_AVCC();
  ADC_channel_0();
  ADC_prescaler_128();
  ADC_enable();
  ADC_interrupt_enable();

  TIM1_overflow_262ms();
  TIM1_overflow_interrupt_enable();
  TIM2_overflow_16ms();
  TIM2_overflow_interrupt_enable();

  // Set external interrupt INT0 on rising edge (pin PD2 - rotary encoder clock)
  EICRA |= ((1<<ISC01) | (1<<ISC00));
  // Enable external interrupt INT0
  EIMSK |= (1<<INT0);

  // Set external interrupt INT1 on falling edge (pin PD3 - joystick button)
  EICRA |= (1<<ISC11); EICRA &= ~(1<<ISC10);
  // Enable external interrupt INT1
  EIMSK |= (1<<INT1); 

  // Set interrupt PCINT0
  PCICR |= (1<<PCIE0);
  // Enable interrupt on PIN PB4
  PCMSK0 |= (1<<PCINT4);

  // Set interrupt PCINT2
  PCICR |= (1<<PCIE2);
  // Enable interrupt on PIN PD0
  PCMSK2 |= (1<<PCINT16);

  sei();

  lcd_gotoxy(1,0);
  lcd_puts("00:00");
  lcd_gotoxy(8,0);
  lcd_puts("00:00:00");
  lcd_gotoxy(1,1);
  lcd_puts("00:00:00");

  while(1){}
  return 0;
}

ISR(TIMER1_OVF_vect){
  static uint8_t blink_flag = 0;
  ADC_start_conversion();
  
  switch(setting_mode){
    case TIMER:
      switch(timer_time_value){
        case HOURS:
          break;
        case MINUTES:
          if(blink_flag == 0){
            lcd_gotoxy(1,0);
            lcd_puts("  ");
            blink_flag = 1;
          }
          else{
            display_time(timer_struct.minutes,1,0);
            blink_flag = 0;
          }
          break;
        case SECONDS:
          if(blink_flag ==0){
            lcd_gotoxy(4,0);
            lcd_puts("  ");
            blink_flag = 1;
          }
          else{
            display_time(timer_struct.seconds,4,0);
            blink_flag = 0;
          }
          break;
      }
      break;
    // End of case TIMER

    case CLOCK:
      switch(clock_time_value){
        case HOURS:
          if(blink_flag == 0){
            lcd_gotoxy(8,0);
            lcd_puts("  ");
            blink_flag = 1;
          }
          else{
            display_time(clock_struct.hours,8,0);
            blink_flag = 0;
          }
          break;
        case MINUTES:
          if(blink_flag == 0){
            lcd_gotoxy(11,0);
            lcd_puts("  ");
            blink_flag = 1;
          }
          else{
            display_time(clock_struct.minutes,11,0);
            blink_flag = 0;
          }
          break;
        case SECONDS:
          if(blink_flag == 0){
            lcd_gotoxy(14,0);
            lcd_puts("  ");
            blink_flag = 1;
          }
          else{
            display_time(clock_struct.seconds,14,0);
            blink_flag = 0;
          }
          break;
      }
      break;
    //End of case CLOCK

    case STOPWATCH:
      if(blink_flag == 0){
        lcd_gotoxy(1,1);
        lcd_puts("  ");
        lcd_gotoxy(4,1);
        lcd_puts("  ");
        lcd_gotoxy(7,1);
        lcd_puts("  ");
        blink_flag = 1;
      }
      else{
        display_time(stopwatch_struct.minutes,1,1);
        display_time(stopwatch_struct.seconds,4,1);
        display_time(stopwatch_struct.tenths,7,1);
        blink_flag = 0;
      }
      break;
    case NONE:
      break;
  }// End of Switch setting mode
}

ISR(TIMER2_OVF_vect){
  static uint8_t no_of_overflows = 0;
  no_of_overflows++;

  if (no_of_overflows >= 6){
    // Do this every 6 x 16 ms = 100 ms
    no_of_overflows = 0;

    // CLOCK mode
    clock_struct.tenths++;
    if(clock_struct.tenths > 9){
      clock_struct.tenths = 0;
      clock_struct.seconds++;
      if(clock_struct.seconds > 59){
        clock_struct.seconds = 0;
        clock_struct.minutes++;
        if(clock_struct.minutes > 59){
          clock_struct.minutes = 0;
          clock_struct.hours++;
          if(clock_struct.hours > 23){
            clock_struct.hours = 0;
          }
          // Display hours after update
          display_time(clock_struct.hours,8,0); 
        }
        //Display minutes after update
        display_time(clock_struct.minutes,11,0);
      }
      //Display seconds after update
      display_time(clock_struct.seconds,14,0);
    }// End CLOCK mode

    // TIMER mode
    if(timer_state == ACTIVATED){
      timer_struct.tenths--;
      if(timer_struct.tenths < 0){
        timer_struct.tenths = 9;
        timer_struct.seconds--;
        if(timer_struct.seconds < 0){
          timer_struct.seconds = 59;
          timer_struct.minutes--;
          if(timer_struct.minutes < 0){
            timer_struct.minutes = 59;
          }
          //Display minutes after update
          display_time(timer_struct.minutes,1,0);
        }
        //Display seconds after update
        display_time(timer_struct.seconds,4,0);
      }// End TIMER mode
    }

    // STOPWATCH mode
    if(stopwatch_state == ACTIVATED){
      stopwatch_struct.tenths++;
      if(stopwatch_struct.tenths > 9){
        stopwatch_struct.tenths = 0;
        stopwatch_struct.seconds++;
        if(stopwatch_struct.seconds > 59){
          stopwatch_struct.seconds = 0;
          stopwatch_struct.minutes++;
          if(stopwatch_struct.minutes > 59){
            stopwatch_struct.minutes = 0;
          }
          //Display minutes after update
          display_time(stopwatch_struct.minutes,1,1);
        }
        //Display seconds after update
        display_time(stopwatch_struct.seconds,4,1);
      }
      //Display tenths after update
      display_time(stopwatch_struct.tenths,7,1);
    }
  }
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
      joystick_direction = LEFT;
    }
    else{
      joystick_direction = DOWN;
    }
    GPIO_write_toggle(&PORTB,PB4); // Interruption software
  }
  else if((x>1013) | (y>1013)){
    if(x>y){
      joystick_direction = RIGHT;
    }
    else{
      joystick_direction = UP;
    } 
    GPIO_write_toggle(&PORTB,PB4);
  }
}

// Software interrupt
// Triggered by any change on PB4 (if a direction is detected)
ISR(PCINT0_vect){
  // Move the '*' to select between Timer, Stopwatch and Clock
  // State machine
  switch(joystick_direction){

    case RIGHT:
      switch(setting_mode){
        case TIMER:
          if(timer_time_value == MINUTES){
            timer_time_value = SECONDS;
            // Make sure that the minutes were not erased by the blinking
            display_time(timer_struct.minutes,1,0);
          }
          break;
        case CLOCK:
          switch(clock_time_value){
            case HOURS:
              clock_time_value = MINUTES;
              display_time(clock_struct.hours,8,0);
              break;
            case MINUTES:
              clock_time_value = SECONDS;
              display_time(clock_struct.minutes,11,0);
              break;
            case SECONDS:
              display_time(clock_struct.seconds,14,0);
              break;
          }
          break;
        case STOPWATCH:
          break;
        case NONE: // selection mode
          switch(selection_mode){
            case TIMER:
              lcd_gotoxy(0,0);
              lcd_putc(' ');
              lcd_gotoxy(7,0);
              lcd_putc('*');
              selection_mode = CLOCK;
              break;
            case CLOCK:
              break;
            case STOPWATCH:
              break;
            case NONE:
              break;
          }
          break;
      }
      break;
    // End of case: RIGHT

    case LEFT:
      switch(setting_mode){
        case TIMER:
          if(timer_time_value == SECONDS){
            timer_time_value = MINUTES;
            display_time(timer_struct.seconds,4,0);
          }
          break;
        case CLOCK:
          switch(clock_time_value){
            case HOURS:
              break;
            case MINUTES:
              clock_time_value = HOURS;
              display_time(clock_struct.minutes,11,0);
              break;
            case SECONDS:
              clock_time_value = MINUTES;
              display_time(clock_struct.seconds,14,0);
              break;
          }
          break;
        case STOPWATCH:
          break;
        case NONE: // selection mode
          switch(selection_mode){
            case TIMER:
              break;
            case CLOCK:
              lcd_gotoxy(7,0);
              lcd_putc(' ');
              lcd_gotoxy(0,0);
              lcd_putc('*');
              selection_mode = TIMER;
              break;
            case STOPWATCH:
              break;
            case NONE:
              break;
          }
          break;
      }
      break;
    // End of case LEFT

    case UP:
      if(selection_mode == STOPWATCH){
        lcd_gotoxy(0,1);
        lcd_putc(' ');
        lcd_gotoxy(0,0);
        lcd_putc('*');
        selection_mode = TIMER;
      }
      break;
    // End of case UP

    case DOWN:
      if(selection_mode == TIMER){
        lcd_gotoxy(0,0);
        lcd_putc(' ');
        lcd_gotoxy(0,1);
        lcd_putc('*');
        selection_mode = STOPWATCH;
      }
      break;
  }
}

// Triggered by a push on the joystick button (falling edge on PD3)
ISR(INT1_vect){
  if(setting_mode == NONE) setting_mode = selection_mode;
  else{
    // Before switching to selection mode,
    // Re-display all the values in case they were erased by the blinking
    switch(setting_mode){
      case TIMER:
        display_time(timer_struct.minutes,1,0);
        display_time(timer_struct.seconds,4,0);
        break;
      case CLOCK:
        display_time(clock_struct.hours,8,0);
        display_time(clock_struct.minutes,11,0);
        display_time(clock_struct.seconds,14,0);
        break;
      case STOPWATCH:
        display_time(stopwatch_struct.minutes,1,1);
        display_time(stopwatch_struct.seconds,4,1);
        display_time(stopwatch_struct.tenths,7,1);
        break;
      case NONE:
        break;
    }
    setting_mode = NONE;
  }
}

// Triggered by a rising edge of the rotary encoder's CLOCK output
ISR(INT0_vect){
  uint8_t dt = GPIO_read(&PIND,PD1);
  if(setting_mode == TIMER){
    if(timer_time_value == MINUTES){
      if(dt == 0){
        if(timer_struct.minutes > 59) timer_struct.minutes = 0;
        else timer_struct.minutes++;
      }
      else{
        if(timer_struct.minutes < 1) timer_struct.minutes = 59;
        else timer_struct.minutes--;
      }
    }
    else if(timer_time_value == SECONDS){
      if(dt == 0){
        if(timer_struct.seconds > 59) timer_struct.seconds = 0;
        else timer_struct.seconds++;
      }
      else{
        if(timer_struct.seconds < 1) timer_struct.seconds = 59;
        else timer_struct.seconds--;
      }
    }
  }
  else if(setting_mode == CLOCK){
    if(clock_time_value == HOURS){
      if(dt == 0){
        if(clock_struct.hours > 23) clock_struct.hours = 0;
        else clock_struct.hours++;
      }
      else{
        if(clock_struct.minutes < 1) clock_struct.minutes = 23;
        else clock_struct.minutes--;
      }
    }
    else if(clock_time_value == MINUTES){
      if(dt == 0){
        if(clock_struct.minutes > 59) clock_struct.minutes = 0;
        else clock_struct.minutes++;
      }
      else{
        if(clock_struct.minutes < 1) clock_struct.minutes = 59;
        else clock_struct.minutes--;
      }
    }
    else if(clock_time_value == SECONDS){
      if(dt == 0){
        if(clock_struct.seconds > 59) clock_struct.seconds = 0;
        else clock_struct.seconds++;
      }
      else{
        if(clock_struct.seconds < 1) clock_struct.seconds = 59;
        else clock_struct.seconds--;
      }
    }
  }
}

// Triggered by any change on pin PD0 (rotary encoder push button)
ISR(PCINT2_vect){
  uint8_t data = GPIO_read(&PIND,PD0);
  if(data == 0){
    if(setting_mode == NONE){
      if(selection_mode == TIMER){
        if(timer_state == DEACTIVATED) timer_state = ACTIVATED;
        else timer_state = DEACTIVATED;
      }
      else if(selection_mode == STOPWATCH){
        if(stopwatch_state == DEACTIVATED) stopwatch_state = ACTIVATED;
        else stopwatch_state = DEACTIVATED;
      }
    }
    else if(setting_mode == TIMER){
      // Reset Timer
      timer_struct.minutes = 0;
      timer_struct.seconds = 0;
      timer_struct.tenths = 0;
    }
    else if(setting_mode == CLOCK){
      // Reset Clock
      clock_struct.hours = 0;
      clock_struct.minutes = 0;
      clock_struct.seconds = 0;
      clock_struct.tenths = 0;
    }
    else if(setting_mode == STOPWATCH){
      // Reset Stopwatch
      stopwatch_struct.minutes = 0;
      stopwatch_struct.seconds = 0;
      stopwatch_struct.tenths  = 0;
    }
    
  }
}