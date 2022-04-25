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
#define START 5
#define STOP 6
#define BACK 7
#define FORWARD 8
#define SPEED 9

extern uint8_t flagmenu2;
extern mailbox_t menu_mb;
extern msg_t menu_mb_buffer[BUFFER_SIZE];
extern mailbox_t holl_mb;
extern msg_t holl_mb_buffer[BUFFER_SIZE];

void Menu_Thread_Start(void);
#endif /* LCD_INC_MENUTHREAD_H_ */
