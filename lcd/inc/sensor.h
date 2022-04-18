/*
 * sensor.h
 *
 *  Created on: 18 апр. 2022 г.
 *      Author: Vania
 */

#ifndef LCD_INC_SENSOR_H_
#define LCD_INC_SENSOR_H_

void Holl(void* args);
int16_t Get_Holl_Sensor(void);
void Sensor_Gpio_Init(void);
void Clear_Holl_Sensor(void);

#endif /* LCD_INC_SENSOR_H_ */
