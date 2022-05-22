#include "ch.h"
#include "hal.h"
#include "menu.h"
#include "TFT_8080.h"
#include "Motor.h"
#include "debugger.h"
#include "pid.h"
#include "sensor.h"
#include "MenuThread.h"

#define WRITECMD 4
#define WRITEPID 7
#define GO 5
#define STOP 6


uint8_t flag_start=0;
uint8_t tim_flag=0;
uint8_t flag_ok=0;
uint8_t angle=0;
uint8_t flagmenu2=0;
extern node_t* menu3_1;
extern node_t* menu3_2;
extern node_t* menu3_3;

void cbgptfun3(GPTDriver *gptp);
GPTDriver *timer3 = &GPTD3;

mailbox_t uart_mb;
msg_t uart_mb_buffer[BUFFER_SIZE];
uint8_t buff[BUFFER_SIZE];


// Настраиваем частоту третьего таймера 50_000Гц (предделитель 4320, целое число, меньше чем 2^16) и указывает первую функцию как обработчик прерываний
GPTConfig gpt3_conf = {
    .frequency = 50000,
    .callback = cbgptfun3,
    .cr2 = 0,
    .dier = 0
};

int main(void)
{
    msg_t my_msg;
    uint16_t start=0;
    halInit();
    chSysInit();
    Uart_Init();
    chMBObjectInit(&holl_mb, holl_mb_buffer, BUFFER_SIZE);
    chMBObjectInit(&uart_mb, uart_mb_buffer, BUFFER_SIZE);
    palSetLineMode(LINE_LED1, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLineMode(LINE_LED2, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLineMode(LINE_LED3, PAL_MODE_OUTPUT_PUSHPULL);
    //запуск таймера 3
    gptStart(timer3, &gpt3_conf);
    Pid_Start();
    Menu_Thread_Start();

    //запуск в непрерывном режиме с периодом 0.1 с
    gptStartContinuous(timer3, 5000);
    while (1)
    {
      sdReadTimeout(uart3,buff,11,chTimeMS2I(100));
      start=buff[0];
        if(start==1)
        {
          palToggleLine(LINE_LED2);
          if(((buff[1]<<8)+buff[2])==WRITECMD)
          {
            palToggleLine(LINE_LED3);
            if(((buff[9]<<9)+buff[10])==GO)
            {
              Reg1.Summ_Error=0;
              Motor_Forward();
            }
            if(((buff[9]<<9)+buff[10])==STOP)
            {
              Reg1.Summ_Error=0;
              Motor_Stop();
            }
          }
          if(((buff[1]<<8)+buff[2])==WRITEPID)
          {
            menu3_1->menu_value=(buff[3]<<8)+buff[4];
            Reg1.P=menu3_1->menu_value;
            menu3_2->menu_value=(buff[5]<<8)+buff[6];
            Reg1.D= menu3_2->menu_value;
            menu3_3->menu_value=(buff[7]<<8)+buff[8];
            Reg1.I=menu3_3->menu_value;
          }
        }

      msg_t msg = chMBFetchTimeout(&uart_mb, &my_msg, chTimeMS2I(200));
      if (msg == MSG_OK)
        sdWrite(uart3, (uint8_t *)&my_msg,2);
    }
}



// callback функция таймера 3
void cbgptfun3(GPTDriver *gptp)
{
    (void)gptp;
    int16_t speed_sensor=0;
    palToggleLine(LINE_LED1);
    speed_sensor=Get_Holl_Sensor()*5;
    Clear_Holl_Sensor();
    if(flag_start)
      chMBPostI(&pid_mb, speed_sensor);
    angle++;
    if(angle>1)
    {
      chMBPostI(&uart_mb, speed_sensor);
      chMBPostI(&holl_mb, speed_sensor);
      angle=0;
    }
}




