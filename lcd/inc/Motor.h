#include "hal.h"
#include "hal_pal.h"

#define IN2_PORT    GPIOF
#define IN2_PIN    13u
#define IN1_PORT    GPIOE
#define IN1_PIN    9u
#define ENA_PORT    GPIOE
#define ENA_PIN    11u
#define IN2_HI    palSetPad(IN2_PORT, IN2_PIN)
#define IN2_LO    palClearPad(IN2_PORT, IN2_PIN)
#define IN1_HI    palSetPad(IN1_PORT, IN1_PIN)
#define IN1_LO    palClearPad(IN1_PORT, IN1_PIN)

#define MAX_I_Reg 2147483647
#define MAX_VALUE 10000

extern SerialDriver *uart3;


extern uint8_t flag_start;
extern int16_t holl_speed;

static const SerialConfig uart_conf = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};

struct regulator {
  int16_t P,
  I,
  D;
  int32_t Summ_Error,
  Max_Summ_Error,
  Last_Process_Value;
} ;


void Motor_Forward(void);
void Motor_Back(void);
void Motor_GPIO_Init(void);
void Motor_Stop(void);
void Motor_PWMD (void);
void Motor_Speed(int16_t speed);
void PID_Reg(struct regulator parm, int zadanie, int measure);
void Uart_Init(void);
void dbgprintf( const char* format, ... );





