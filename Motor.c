#include "Motor.h"
#include "TFT_8080.h"
#include "chprintf.h"

SerialDriver *uart3 = &SD3;
static BaseSequentialStream *uart3_stream;

PWMConfig pwm1conf = {
    // Укажем частоту 500кГц (предделитель равен 436, так что значение допустимое)
    .frequency = 300000,
    // Период равен 10000 тактам таймера, то есть 0,02 секунды
    .period    = 10000,
    // Не будем использовать прерывание по окончанию периода
    .callback  = NULL,
    // Настроим только первый кнал в состояние ACTIVE_HIGH и так же не будем использовать прерывание
    .channels  = {
                  {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
                  {.mode = PWM_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL}
                  },
    // Регистры оставим в покое. Обратите внимание, advanced функции таймера выключены, поэтому регистра bdtr нет
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
  // Запустим модуль в работу
  pwmStart( &PWMD1, &pwm1conf );
  // Запустим канал с коэффициентом заполнения 50% (здесь третий аргумент это количество тактов, 5000 / 10000 = 0,5)
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
  // запускаем драйвер в работу
  sdStart(uart3, &uart_conf);
  // Переводим ноги в состояние Rx, Tx
  palSetPadMode( GPIOD, 8, PAL_MODE_ALTERNATE(7) );
  palSetPadMode( GPIOD, 9, PAL_MODE_ALTERNATE(7) );
  // Переопределяем указатель на поток
  uart3_stream = (BaseSequentialStream *)uart3;
}


// Функция отправки строки в терминал
void dbgprintf( const char* format, ... )
{
// Проверяем, что debug_stream_init() случился
    if ( !uart3_stream )
    return;

// Отправляем в chvprintf() данные и ждём чуда
    va_list ap;
    va_start(ap, format);
    chvprintf(uart3_stream, format, ap);
    va_end(ap);
}


// callback функция, которая должна сработать по настроенному событию
void holl(void* args)
{
    // Преобразование аргумента к требуемому типу, в данному случае к uint8_t
    uint8_t arg = *((uint8_t*) args);
    // Проверка, что передача аргумента работает
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












