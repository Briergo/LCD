#include "ch.h"
#include "hal.h"
#include "menu.h"
#include "TFT_8080.h"
#include "Motor.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>

uint8_t flag_start=0;

uint8_t tim_flag=0;
int16_t holl_speed=0;

int16_t speed = 5;
uint8_t tft_flag=0;
uint8_t flag_ok=0;

#define BUFFER_SIZE 100

mailbox_t tft_mb;
msg_t tft_mb_buffer[BUFFER_SIZE];

struct regulator Reg1;

void Uart_Init(void);
void dbgprintf( const char* format, ... );
void cbgptfun3(GPTDriver *gptp);
void cbgptfun4(GPTDriver *gptp);
void holl(void* args);
void Init_PID_Reg(void);

GPTDriver *timer3 = &GPTD3;
GPTDriver *timer4 = &GPTD4;


// Настраиваем частоту третьегоы таймера 50_000Гц (предделитель 4320, целое число, меньше чем 2^16) и указывает первую функцию как обработчик прерываний
GPTConfig gpt3_conf = {
    .frequency = 50000,
    .callback = cbgptfun3,
    .cr2 = 0,
    .dier = 0
};

GPTConfig gpt4_conf = {
    .frequency = 50000,
    .callback = cbgptfun4,
    .cr2 = 0,
    .dier = 0
};

static THD_WORKING_AREA(tftThread,512);// 512 - stack size

//static THD_WORKING_AREA(tftspeedThread, 2048);// 512 - stack size

static THD_WORKING_AREA(menuThread, 512);// 512 - stack size

static THD_FUNCTION(tft, arg)
{
    arg=arg;
    char String[15];
    msg_t my_msg;
    msg_t msg =chMBFetchI (&tft_mb, &my_msg);
    if (msg == MSG_OK)
    {
       sprintf(String,"%d",(int16_t)my_msg);
       TFT_Fill_Screen(160,200,200,220,WHITE);
       TFT_Draw_String(160,200,RED,WHITE,String,2);
    }
}
/*static THD_FUNCTION(tft_speed, arg)
{
    arg=arg;
    char String[15];
    if(right_flag)
      speed+=1;
    if(left_flag)
       speed-=1;
    sprintf(String,"%d",speed);
    TFT_Fill_Screen(0,320,0,240,WHITE);
    //TFT_Fill_Screen(160,200,150,170,WHITE);
    TFT_Draw_String(160,150,RED,WHITE,String,2);
    flag_ok=0;
}*/

static THD_FUNCTION(menu, arg)
{
    arg=arg;
    Menu_Disp();
    Cursor();
}

int main(void)
{
    uint8_t arg = 5;

    halInit();
    chSysInit();
    Uart_Init();
    palSetPadMode( GPIOC, 6, PAL_MODE_ALTERNATE(2) );
    palSetLineMode(LINE_LED1, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLineMode(LINE_LED2, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLineMode(LINE_LED3, PAL_MODE_OUTPUT_PUSHPULL);
    //настройка дисплея и инициализация меню
    TFT_Init();

    chMBObjectInit(&tft_mb, tft_mb_buffer, BUFFER_SIZE);

    Menu_Create();
    Menu_GPIO_Init();
    Motor_GPIO_Init();

    //запуск таймера 3
    gptStart(timer3, &gpt3_conf);

    //запуск таймера 3
    gptStart(timer4, &gpt4_conf);
    //запуск в непрерывном режиме с периодом 0.1 с
    gptStartContinuous(timer3, 5000);
    //Настройка ног датчика холла
    palSetPadMode(GPIOB, 8u, PAL_MODE_INPUT);
    palEnablePadEvent(GPIOB, 8u, PAL_EVENT_MODE_BOTH_EDGES);
    palSetPadCallback(GPIOB, 8u, holl, &arg);
    palSetPadMode(GPIOB, 9u, PAL_MODE_INPUT);

    Init_PID_Reg();

    while (1)
    {

      if(up_flag==1 )
      {
        /*if(flag_start==0)
        {
          Motor_Forward();
        }*/
        if(current->prev!=NULL)
        {
          current=current->prev;
          chThdCreateStatic(menuThread, sizeof(menuThread), NORMALPRIO+2, menu, NULL);
        }
        up_flag=0;
      }

      if(down_flag==1 )
      {
         /*if(flag_start==1)
         {
           Motor_Stop();
         }*/
        if(current->next!=NULL)
        {
          current=current->next;
          chThdCreateStatic(menuThread, sizeof(menuThread), NORMALPRIO+2, menu, NULL);
        }
        down_flag=0;
      }

      if(right_flag==1 )
      {
        /*if(flag_start==1 && speed<20)
        {
          if(flag_ok==0)
          {
            flag_ok=1;
            chThdCreateStatic(tftspeedThread, sizeof(tftThread), NORMALPRIO+2, tft_speed, NULL);
          }
        }*/
        if(current->child!=NULL)
        {
          current=current->child;
          chThdCreateStatic(menuThread, sizeof(menuThread), NORMALPRIO+2, menu, NULL);

        }
        right_flag=0;
      }

      if(left_flag==1)
      {
        /*if(flag_start==1 && speed>1)
        {
          if(flag_ok==0)
          {
            flag_ok=1;
            chThdCreateStatic(tftspeedThread, sizeof(tftThread), NORMALPRIO+2, tft_speed, NULL);
          }
        }*/
        if(current->parent!=NULL)
        {
          current=current->parent;
          chThdCreateStatic(menuThread, sizeof(menuThread), NORMALPRIO+2, menu, NULL);
        }
        left_flag=0;
      }

      if(tim_flag==1)
      {
        sdWrite(uart3, (uint8_t *)&holl_speed, 2);
        PID_Reg(Reg1,speed*2,holl_speed);
        if(tft_flag)
          chThdCreateStatic(tftThread, sizeof(tftThread), NORMALPRIO+1, tft, NULL);
        holl_speed=0;
        tim_flag=0;
      }

     }


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


// callback функция таймера 3
void cbgptfun3(GPTDriver *gptp)
{
    (void)gptp;
    palToggleLine(LINE_LED1);
    msg_t msg = chMBPostI(&tft_mb, holl_speed);
    if (msg == MSG_OK)
      tft_flag=1;
    else
      tft_flag=0;
    tim_flag=1;
}




