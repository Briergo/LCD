/*
 * debugger.c
 *
 *  Created on: 18 ���. 2022 �.
 *      Author: Vania
 */

#include "hal.h"
#include "debugger.h"
#include "chprintf.h"


SerialDriver *uart3 = &SD3;
static BaseSequentialStream *uart3_stream;

void Uart_Init(void)
{
  // ��������� ������� � ������
  sdStart(uart3, &uart_conf);
  // ��������� ���� � ��������� Rx, Tx
  palSetPadMode( GPIOD, 8, PAL_MODE_ALTERNATE(7) );
  palSetPadMode( GPIOD, 9, PAL_MODE_ALTERNATE(7) );
  // �������������� ��������� �� �����
  uart3_stream = (BaseSequentialStream *)uart3;
}

void Uart_Deinit(void)
{
  // ������������� �������
  sdStop(uart3);
  // ������� ���� � ��������� Rx, Tx
  palClearPad( GPIOD, 8);
  palClearPad( GPIOD, 9);
}


// ������� �������� ������ � ��������
void dbgprintf( const char* format, ... )
{
// ���������, ��� debug_stream_init() ��������
    if ( !uart3_stream )
    return;

// ���������� � chvprintf() ������ � ��� ����
    va_list ap;
    va_start(ap, format);
    chvprintf(uart3_stream, format, ap);
    va_end(ap);
}


