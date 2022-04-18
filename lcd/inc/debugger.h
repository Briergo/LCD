/*
 * debugger.h
 *
 *  Created on: 18 апр. 2022 г.
 *      Author: Vania
 */

#ifndef LCD_INC_DEBUGGER_H_
#define LCD_INC_DEBUGGER_H_

extern SerialDriver *uart3;

static const SerialConfig uart_conf = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};

void Uart_Init(void);
void Uart_Deinit(void);
void dbgprintf( const char* format, ... );

#endif /* LCD_INC_DEBUGGER_H_ */
