#include "ch.h"
#include "hal.h"
#include "menu.h"
#include "TFT_8080.h"
#include "Motor.h"
#include "debugger.h"
#include "pid.h"
#include "sensor.h"
#include "MenuThread.h"




uint8_t flag_start=0;
uint8_t tim_flag=0;
uint8_t flag_ok=0;
uint8_t angle=0;
uint8_t flagmenu2=0;

void cbgptfun3(GPTDriver *gptp);
GPTDriver *timer3 = &GPTD3;


// ����������� ������� �������� ������� 50_000�� (������������ 4320, ����� �����, ������ ��� 2^16) � ��������� ������ ������� ��� ���������� ����������
GPTConfig gpt3_conf = {
    .frequency = 50000,
    .callback = cbgptfun3,
    .cr2 = 0,
    .dier = 0
};

int main(void)
{
    int16_t speed_uart=0;
    halInit();
    chSysInit();
    Uart_Init();
    chMBObjectInit(&holl_mb, holl_mb_buffer, BUFFER_SIZE);
    palSetLineMode(LINE_LED1, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLineMode(LINE_LED2, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLineMode(LINE_LED3, PAL_MODE_OUTPUT_PUSHPULL);
    //������ ������� 3
    gptStart(timer3, &gpt3_conf);
    Pid_Start();
    Menu_Thread_Start();

    //������ � ����������� ������ � �������� 0.1 �
    gptStartContinuous(timer3, 5000);
    while (1)
    {
      chThdSleepMilliseconds(200);
      speed_uart=Get_Holl_Sensor();
      dbgprintf("%d",set_speed);
      //sdWrite(uart3, (uint8_t *)&speed,2);
    }
}



// callback ������� ������� 3
void cbgptfun3(GPTDriver *gptp)
{
    (void)gptp;
    int16_t speed_sensor=0;
    palToggleLine(LINE_LED1);
    speed_sensor=Get_Holl_Sensor();
    Clear_Holl_Sensor();
    chMBPostI(&pid_mb, speed_sensor);
    angle++;
    if(angle>1)
    {
      chMBPostI(&holl_mb, speed_sensor);
      angle=0;
    }
}




