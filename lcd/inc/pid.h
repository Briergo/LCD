/*
 * pid.h
 *
 *  Created on: 18 апр. 2022 г.
 *      Author: Vania
 */

#ifndef LCD_INC_PID_H_
#define LCD_INC_PID_H_

#include "hal.h"

#define BUFFER_SIZE 100
#define MAX_I_Reg 10000
#define MAX_VALUE 10000

struct regulator {
  int16_t P,
  I,
  D;
  int32_t Summ_Error,
  Max_Summ_Error,
  Last_Process_Value;
} ;

extern int16_t set_speed;
extern struct regulator Reg1;
extern mailbox_t pid_mb;
extern msg_t pid_mb_buffer[BUFFER_SIZE];

int16_t PID_Reg( int zadanie, int measure);
void Init_PID_Reg(void);
void Pid_Start(void);

#endif /* LCD_INC_PID_H_ */
