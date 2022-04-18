#include "hal.h"

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





extern uint8_t flag_start;



void Motor_Forward(void);
void Motor_Back(void);
void Motor_GPIO_Init(void);
void Motor_Stop(void);
void Motor_Speed(int16_t speed);
void Motor_Init(void);
void Motor_Deinit(void);
void Motor_GPIO_Deinit(void);






