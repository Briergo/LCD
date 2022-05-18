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
int16_t set_speed=20;

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
            input=PID_Reg(Reg1,set_speed,(int16_t)my_msg);
            sdWrite(uart3, (uint8_t *)&input,2);
            Motor_Speed((int16_t)input);
          }
    }
}

int16_t PID_Reg(struct regulator parm, int zadanie, int measure)
{
  int16_t input, e=0, I_Temp, D_Temp;
  e=zadanie-measure;
  if(parm.I*(parm.Summ_Error)<=parm.Max_Summ_Error)
  {
    I_Temp=parm.Summ_Error+e;
    parm.Summ_Error=I_Temp;
  }
  else
    I_Temp=parm.Summ_Error;
  if(I_Temp<0)
    palToggleLine(LINE_LED3);
  D_Temp=parm.Last_Process_Value-measure;
  parm.Last_Process_Value=measure;
  input=parm.P*e+parm.I*I_Temp+parm.D*D_Temp;
  parm.Last_Input=input;
  if(input>MAX_VALUE)
    input=MAX_VALUE;
  if(input<0)
  {
    palToggleLine(LINE_LED2);
    input*=-1;
  }
  return input;
}

void Init_PID_Reg(void)
{
  //П=260 Д=8 И=100
  //П=170 Д=80 И=350
  Reg1.P=500;
  Reg1.D=0 ;
  Reg1.I=0;
  Reg1.Summ_Error=0;
  Reg1.Max_Summ_Error=MAX_I_Reg;
  Reg1.Last_Process_Value=0;
  Reg1.Last_Input=0;
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
