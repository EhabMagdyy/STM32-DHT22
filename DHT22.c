/*****************************************************************************************************************************
**********************************    Author  : Ehab Magdy Abdullah                      *************************************
**********************************    Linkedin: https://www.linkedin.com/in/ehabmagdyy/  *************************************
**********************************    Youtube : https://www.youtube.com/@EhabMagdyy      *************************************
******************************************************************************************************************************/

#include "DHT22.h"

extern TIM_HandleTypeDef htim10;           /* extern the timer handler you are using */

uint8_t RH1, RH2, T1, T2, Sum, CheckSum;   /* Variables to store DHT22 data */

/*   Setting DHT22 Pin Input to receive data    */
static void Set_Pin_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT22_PORT, &GPIO_InitStruct);
}

/*   Setting DHT22 Pin Output to send data    */
static void Set_Pin_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT22_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT22_PORT, &GPIO_InitStruct);
}

/*   Provides delay in MicroSeconds   */
static void Delay_US(uint16_t delay)
{
    __HAL_TIM_SET_COUNTER(&htim10, 0);
    while (__HAL_TIM_GET_COUNTER(&htim10) < delay);
}

/*   Start Communication with DHT22   */
void DHT22_Start(void)
{
    Set_Pin_Output();
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, GPIO_PIN_RESET);
    Delay_US(1300);     // wait for > 1ms
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, GPIO_PIN_SET);
    Delay_US(30);       // wait for 20-40us
    Set_Pin_Input();

    while(!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)));    // wait for the pin to go high
    __HAL_TIM_SET_COUNTER(&htim10, 0);
    while((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)) && 100 > __HAL_TIM_GET_COUNTER(&htim10));    // wait for the pin to go low
}

/*   Reading 8 bits of data from DHT22   */
uint8_t DHT22_Read(void)
{
    uint8_t i, x = 0;

    for (i = 0 ; i < 8 ; i++)
    {
        while (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)));  // wait for the pin to go high
        Delay_US (40);   // wait for 40 us

        if (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)))    // if the pin is low - bit = 0
            x &= ~(1 << (7 - i));
        else											    // if the pin is low - bit = 1
            x |= (1 << (7 - i));

        __HAL_TIM_SET_COUNTER(&htim10, 0);
        while((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)) && 100 > __HAL_TIM_GET_COUNTER(&htim10));    // wait for the pin to go low
    }
    return x;
}

/*   Set the values of Temperature & Humidity if CheckSum is Correct   */
uint8_t DHT22_Get_Reading(float* temp, float* humidity)
{
	uint8_t isReadingCorrect = 0;

	DHT22_Start();

    RH1 = DHT22_Read();            // Most Significant 8bits of Humidity
    RH2 = DHT22_Read();            // Least Significant 8bits of Humidity
    T1 = DHT22_Read();             // Most Significant 8bits of Temperature
    T2 = DHT22_Read();             // Least Significant 8bits of Temperature
    CheckSum = DHT22_Read();       // Check Sum on last 8*4 bits
    Sum = RH1 + RH2 + T1 + T2;     // Actual Sum of RH1,RH2,T1,T2

    if(CheckSum == Sum)
    {
    	isReadingCorrect = 1;

        if((T1 >> 7) & 1)          // if Temperature Negative
        {
        	*temp = (float) T2 / 10 * (-1);
        }
        else
        {
        	*temp = (float)((T1 << 8) | T2) / 10;
        }

        *humidity = (float)((RH1 << 8) | RH2) / 10;
    }

    return isReadingCorrect;
}
