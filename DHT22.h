/*****************************************************************************************************************************
**********************************    Author  : Ehab Magdy Abdullah                      *************************************
**********************************    Linkedin: https://www.linkedin.com/in/ehabmagdyy/  *************************************
**********************************    Youtube : https://www.youtube.com/@EhabMagdyy      *************************************
******************************************************************************************************************************/

#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "stm32f4xx_hal.h"

#define DHT22_PORT          GPIOA                 /* GPIO Port of your pin */
#define DHT22_PIN           GPIO_PIN_0            /* GPIO pin number */

void DHT22_Start(void);
uint8_t DHT22_Read(void);
uint8_t DHT22_Get_Reading(float* temp, float* humidity);

#endif /* INC_DHT22_H_ */
