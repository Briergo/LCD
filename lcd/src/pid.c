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
int16_t set_speed=100;

static THD_WORKING_AREA(pidThread, 256);// 256 - stack size

static THD_FUNCTION(pid, arg)
{
    arg=arg;
    msg_t my_msg;
    int32_t input=0;
    while(1)
    {
      msg_t msg = chMBFetchTimeout(&pid_mb, &my_msg, chTimeMS2I(10));
      if (msg == MSG_OK)
          {
            if((int16_t)my_msg<0)
              my_msg*=-1;
            input=PID_Reg(set_speed,(int16_t)my_msg);
            Motor_Speed((int16_t)input);
          }
    }
}

int16_t PID_Reg(int zadanie, int measure)
{
  int16_t input, e=0, I_Temp, D_Temp;
  e=zadanie-measure;
  if(Reg1.I*(Reg1.Summ_Error)<=Reg1.Max_Summ_Error)
  {
    I_Temp=Reg1.Summ_Error+e;
    Reg1.Summ_Error=I_Temp;
  }
  else
    I_Temp=Reg1.Summ_Error;
  D_Temp=Reg1.Last_Process_Value-measure;
  Reg1.Last_Process_Value=measure;
  input=Reg1.P*e+Reg1.I*I_Temp+Reg1.D*D_Temp;
  if(input>MAX_VALUE)
    input=MAX_VALUE;
  if(input<0)
  {
    input*=-1;
  }
  return input;
}

void Init_PID_Reg(void)
{
  Reg1.P=20;
  Reg1.D=0 ;
  Reg1.I=2;
  Reg1.Summ_Error=0;
  Reg1.Max_Summ_Error=MAX_I_Reg;
  Reg1.Last_Process_Value=0;
}

void Pid_Start(void)
{
  Motor_Init();
  Motor_Forward();
  Init_PID_Reg();
  Sensor_Gpio_Init();
  chMBObjectInit(&pid_mb, pid_mb_buffer, BUFFER_SIZE);
  chThdCreateStatic(pidThread, sizeof(pidThread), NORMALPRIO, pid, NULL);
}
