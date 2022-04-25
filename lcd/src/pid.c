/*
 * pid.c
 *
 *  Created on: 18 апр. 2022 г.
 *      Author: Vania
 */
#include "ch.h"
#include "pid.h"
#include "Motor.h"
#include "sensor.h"
#include "debugger.h"

struct regulator Reg1;

mailbox_t pid_mb;
msg_t pid_mb_buffer[BUFFER_SIZE];
int16_t set_speed=0;

static THD_WORKING_AREA(pidThread, 256);// 256 - stack size

static THD_FUNCTION(pid, arg)
{
    arg=arg;
    msg_t my_msg;
    int32_t input=0;
    while(1)
    {
      msg_t msg = chMBFetchTimeout(&pid_mb, &my_msg, chTimeMS2I(50));
      if (msg == MSG_OK)
          {
            input=PID_Reg(Reg1,set_speed,(int16_t)my_msg);
            Motor_Speed((int16_t)input);
          }
    }
}

int32_t PID_Reg(struct regulator parm, int zadanie, int measure)
{
  int32_t input, e=0, I_Temp, D_Temp;
  e=zadanie-measure;
  I_Temp=parm.Summ_Error+e;
  if(I_Temp>parm.Max_Summ_Error)
    I_Temp=parm.Max_Summ_Error;
  parm.Summ_Error=I_Temp;
  D_Temp=parm.Last_Process_Value-measure;
  parm.Last_Process_Value=measure;
  input=parm.P*e+parm.I*I_Temp+parm.D*D_Temp;
  if(input>MAX_VALUE)
    input=MAX_VALUE;
  if(input<0)
    input*=-1;
  return input;
}

void Init_PID_Reg(void)
{
  Reg1.P=72;
  Reg1.D=20 ;
  Reg1.I=210;
  Reg1.Summ_Error=0;
  Reg1.Max_Summ_Error=MAX_I_Reg;
  Reg1.Last_Process_Value=0;
}

void Pid_Start(void)
{
  Motor_Init();
  Init_PID_Reg();
  Sensor_Gpio_Init();
  chMBObjectInit(&pid_mb, pid_mb_buffer, BUFFER_SIZE);
  chThdCreateStatic(pidThread, sizeof(pidThread), NORMALPRIO, pid, NULL);
}
