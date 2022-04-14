#include "Motor.h"
#include "TFT_8080.h"
#include "chprintf.h"

SerialDriver *uart3 = &SD3;
static BaseSequentialStream *uart3_stream;

PWMConfig pwm1conf = {
    // ������ ������� 500��� (������������ ����� 436, ��� ��� �������� ����������)
    .frequency = 300000,
    // ������ ����� 10000 ������ �������, �� ���� 0,02 �������
    .period    = 10000,
    // �� ����� ������������ ���������� �� ��������� �������
    .callback  = NULL,
    // �������� ������ ������ ���� � ��������� ACTIVE_HIGH � ��� �� �� ����� ������������ ����������
    .channels  = {
                  {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
                  {.mode = PWM_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL}
                  },
    // �������� ������� � �����. �������� ��������, advanced ������� ������� ���������, ������� �������� bdtr ���
    .cr2        = 0,
    .dier       = 0
};


void Motor_GPIO_Init(void)
{
  palSetPadMode(IN2_PORT,IN2_PIN,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(IN1_PORT,IN1_PIN,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(ENA_PORT,ENA_PIN,  PAL_MODE_ALTERNATE(1) );
}

void Motor_Forward(void)
{
  IN1_HI;
  IN2_LO;
  Motor_PWMD();
  flag_start=1;

}
void Motor_Back(void)
{
  IN1_LO;
  IN2_HI;
}
void Motor_Stop(void)
{
  IN1_LO;
  IN2_LO;
  flag_start=0;
}

void Motor_Speed(int16_t speed)
{
  pwmEnableChannel( &PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1,speed) );

}

void Motor_PWMD (void)
{
  // �������� ������ � ������
  pwmStart( &PWMD1, &pwm1conf );
  // �������� ����� � ������������� ���������� 50% (����� ������ �������� ��� ���������� ������, 5000 / 10000 = 0,5)
}

void PID_Reg(struct regulator parm, int zadanie, int measure)
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
  Motor_Speed(input);
}


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


// callback �������, ������� ������ ��������� �� ������������ �������
void holl(void* args)
{
    // �������������� ��������� � ���������� ����, � ������� ������ � uint8_t
    uint8_t arg = *((uint8_t*) args);
    // ��������, ��� �������� ��������� ��������
    if (arg == 5)
    {
      if(palReadPad(GPIOB,8u)==1)
      {
        if(palReadPad(GPIOB,9u)==1)
        {
          holl_speed++;
        }
        if(palReadPad(GPIOB,9u)==0)
        {
          holl_speed--;
        }
      }

      if(palReadPad(GPIOB,8u)==0)
      {
        if(palReadPad(GPIOB,9u)==0)
        {
          holl_speed++;
        }
        if(palReadPad(GPIOB,9u)==1)
        {
          holl_speed--;
        }
      }
    }
}












