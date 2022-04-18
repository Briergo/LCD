/*
 * sensor.c
 *
 *  Created on: 18 ���. 2022 �.
 *      Author: Vania
 */
#include "hal.h"
#include "sensor.h"

int16_t holl_speed=0;
void Sensor_Gpio_Init(void)
{
  uint8_t arg = 5;
  //��������� ��� ������� �����
  palSetPadMode(GPIOB, 8u, PAL_MODE_INPUT);
  palEnablePadEvent(GPIOB, 8u, PAL_EVENT_MODE_BOTH_EDGES);
  palSetPadCallback(GPIOB, 8u, Holl, &arg);
  palSetPadMode(GPIOB, 9u, PAL_MODE_INPUT);

}
// callback �������, ������� ������ ��������� �� ������������ �������
void Holl(void* args)
{
    // �������������� ��������� � ���������� ����, � ������� ������ � uint8_t
    uint8_t arg = *((uint8_t*) args);
    // ��������, ��� �������� ��������� ��������
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


int16_t Get_Holl_Sensor(void)
{
  return holl_speed;
}

void Clear_Holl_Sensor(void)
{
  holl_speed=0;
}

