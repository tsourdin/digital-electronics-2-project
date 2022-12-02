 /*
 * LCD screen wiring from lab4 :
 *   16x2 character LCD with parallel interface
 *     VSS  - GND (Power supply ground)
 *     VDD  - +5V (Positive power supply)
 *     RS   - PB0 (Register Select: High for Data transfer, Low for Instruction transfer)
 *     RW   - GND (Read/Write signal: High for Read mode, Low for Write mode)
 *     E    - PB1 (Read/Write Enable: High for Read, falling edge writes data to LCD)
 *     D3:0 - NC (Data bits 3..0, Not Connected)
 *     D4   - PD4 (Data bit 4)
 *     D5   - PD5 (Data bit 5)
 *     D6   - PD6 (Data bit 6)
 *     D7   - PD7 (Data bit 7)
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for number conversions
#include <uart.h>           // Peter Fleury's UART library
#include "adc.h"            // ADC library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library

// Rotary Encoder Inputs
#define CLK PC5
#define DT PC4
#define SW PC3

uint8_t counter = 0;
uint8_t currentStateCLK;
uint8_t lastStateCLK;

uint8_t last_len;
char * currentDir ="";
unsigned long lastButtonPress = 0;

uint8_t pressed = 0;

int main(void){
  /*
  ADC_voltage_ref_AVCC();
  ADC_channel_0();
  ADC_enable();
  ADC_interrupt_enable();
  ADC_prescaler_128();
  ADC_auto_trigger_enable();
  ADC_auto_trigger_source_TIM0_OVF();

  TIM0_overflow_16ms();
  TIM0_overflow_interrupt_enable()

  sei();
  */
  
  // Set encoder pins as inputs
  GPIO_mode_input_nopull(DDRC,CLK);
  GPIO_mode_input_nopull(DDRC,DT);
  GPIO_mode_input_pullup(DDRC,SW);

  lcd_init(LCD_DISP_ON);

	// Setup Serial Monitor
  uart_init(UART_BAUD_SELECT(9600, F_CPU));

	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);

  while(1){
    lcd_gotoxy(1,2);
	  lcd_puts("Hello");
    uint8_t found = 0;
    currentStateCLK = GPIO_read(&PINC,CLK);
    char days[][6] = {"Mon","Tues","Wednes","Thurs","Fri","Satur","Sun"};
    char * day;

    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
      if (GPIO_read(&PINC,DT) != GPIO_read(&PINC,CLK)) {
        counter ++;
        currentDir ="CW";
        if (counter == 7){
          counter = 0;
        }
        uart_puts("CW ");
        uart_puts(counter);			
      } else {
        // Encoder is rotating CW so increment
        counter --;
        currentDir ="CCW";
        if (counter == -1){
          counter = 6;
        }
        uart_puts("CCW ");
        uart_puts(counter);
      }
      lcd_gotoxy(0,0);
      lcd_puts(days[counter]);
    }
    lastStateCLK = currentStateCLK;
    
    if (GPIO_read(&PINC,SW) == 0) {
      day = days[counter];
      lcd_clrscr();
      lcd_puts(day);
      lcd_gotoxy(6, 0);
      lcd_puts(day);
      lcd_gotoxy(0, 1);
      lcd_puts("Day selected");
      delay(2000);
      lcd_gotoxy(0, 1);
      lcd_puts("            ");
      found = 1;
      }

      // Remember last button press event
      lastButtonPress = millis();
    }


}


ISR(ADC_vect){
  uint16_t value = ADC;
  static uint8_t flag = 0;
  if(flag == 0){
    ADC_channel_0();
    flag = 1;
  }
  else{
    ADC_channel_1();
    flag = 0;
  }
  char string[4];
  itoa(value, string, 10);
  uart_puts(string);
  uart_puts("\r\n");
}