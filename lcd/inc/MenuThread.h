/*
 * MenuThread.h
 *
 *  Created on: 18 апр. 2022 г.
 *      Author: Vania
 */
#include "pid.h"

#ifndef LCD_INC_MENUTHREAD_H_
#define LCD_INC_MENUTHREAD_H_

#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4

extern mailbox_t menu_mb;
extern msg_t menu_mb_buffer[BUFFER_SIZE];

void Menu_Thread_Start(void);
#endif /* LCD_INC_MENUTHREAD_H_ */
