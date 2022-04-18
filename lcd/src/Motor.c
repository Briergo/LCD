#include "Motor.h"



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

void Motor_GPIO_Deinit(void)
{
  palClearPad(IN2_PORT,IN2_PIN);
  palClearPad(IN1_PORT,IN1_PIN);
  palClearPad(ENA_PORT,ENA_PIN);
}

void Motor_Forward(void)
{
  IN1_HI;
  IN2_LO;
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

void Motor_Init(void)
{
  //Инициализация контактов
  Motor_GPIO_Init();
  // Запустим модуль в работу
  pwmStart( &PWMD1, &pwm1conf );
}

void Motor_Deinit(void)
{
  //Инициализация контактов
  Motor_GPIO_Deinit();
  // Запустим модуль в работу
  pwmStop( &PWMD1);
}

















