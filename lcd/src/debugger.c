/*
 * debugger.c
 *
 *  Created on: 18 апр. 2022 г.
 *      Author: Vania
 */

#include "hal.h"
#include "debugger.h"
#include "chprintf.h"


SerialDriver *uart3 = &SD3;
static BaseSequentialStream *uart3_stream;

void Uart_Init(void)
{
  // запускаем драйвер в работу
  sdStart(uart3, &uart_conf);
  // Переводим ноги в состояние Rx, Tx
  palSetPadMode( GPIOD, 8, PAL_MODE_ALTERNATE(7) );
  palSetPadMode( GPIOD, 9, PAL_MODE_ALTERNATE(7) );
  // Переопределяем указатель на поток
  uart3_stream = (BaseSequentialStream *)uart3;
}

void Uart_Deinit(void)
{
  // останавливаем драйвер
  sdStop(uart3);
  // Ощищаем ноги в состояние Rx, Tx
  palClearPad( GPIOD, 8);
  palClearPad( GPIOD, 9);
}


// Функция отправки строки в терминал
void dbgprintf( const char* format, ... )
{
// Проверяем, что debug_stream_init() случился
    if ( !uart3_stream )
    return;

// Отправляем в chvprintf() данные и ждём чуда
    va_list ap;
    va_start(ap, format);
    chvprintf(uart3_stream, format, ap);
    va_end(ap);
}


