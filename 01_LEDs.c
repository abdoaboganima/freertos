/*
 * main.c
 *
 *  Created on: May 5, 2022
 *      Author: Abdulrahman Aboghanima
 */

#include "Drivers/DIO_interface.h"
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include <stdio.h>

void LED1(void *pv);
void LED2(void *pv);
void LED3(void *pv);

int main()
{

  DIO_SetPortDirection(DIO_PORTA, DIO_PORT_HIGH);
  DIO_SetPinValue(DIO_PORTA, DIO_PIN0, DIO_PIN_LOW);
  DIO_SetPinValue(DIO_PORTA, DIO_PIN1, DIO_PIN_LOW);
  DIO_SetPinValue(DIO_PORTA, DIO_PIN2, DIO_PIN_LOW);

  xTaskCreate(&LED1, NULL, 100, NULL, 0, NULL);
  xTaskCreate(&LED2, NULL, 100, NULL, 1, NULL);
  xTaskCreate(&LED3, NULL, 100, NULL, 2, NULL);

  
 
  
  vTaskStartScheduler();
  while(1){
    /*Do nothing*/
  }
  return 0;
}


void LED1(void *pv)
{
  static uint8_t state=0;
  while(1){
	  TOGGLE_BIT(state, 0);
	  DIO_SetPinValue(DIO_PORTA, DIO_PIN0, state);
	  vTaskDelay(1000);
  }
}

void LED2(void *pv)
{
  static uint8_t state=0;
  while(1){
	  TOGGLE_BIT(state, 0);
	  DIO_SetPinValue(DIO_PORTA, DIO_PIN1, state);
	  vTaskDelay(2000);
  }
}

void LED3(void *pv)
{
  static uint8_t state=0;
  while(1){
	  TOGGLE_BIT(state, 0);
	  DIO_SetPinValue(DIO_PORTA, DIO_PIN2, state);
	  vTaskDelay(3000);
  }
}
