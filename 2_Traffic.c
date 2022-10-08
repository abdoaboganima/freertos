/**
 *
 * @file       2_Traffic.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Thu Oct  6 06:30:37 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */


#include "DIO_interface.h"
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

#include <util/delay.h>
void LED1(void *pv);
void LED2(void *pv);
void LED3(void *pv);

xTaskHandle task1;
xTaskHandle task2;
xTaskHandle task3;

int main()
{

  DIO_SetPortDirection(DIO_PORTA, DIO_PORT_HIGH);
  DIO_SetPinValue(DIO_PORTA, DIO_PIN0, DIO_PIN_LOW);
  DIO_SetPinValue(DIO_PORTA, DIO_PIN1, DIO_PIN_LOW);
  DIO_SetPinValue(DIO_PORTA, DIO_PIN2, DIO_PIN_LOW);

  xTaskCreate(&LED1, "The red state Task", 100, NULL, 3, &task1);/*Red*/
  xTaskCreate(&LED3, "The yellow state Task", 100, NULL, 2, &task3);/*yellow*/
  xTaskCreate(&LED2, "The green state Task", 100, NULL, 1, &task2);/*Green*/
  
  vTaskStartScheduler();
  while(1){
    /*Do nothing*/
  }
  return 0;
}


void LED1(void *pv)
{
  while(1){
    DIO_SetPortValue(DIO_PORTA, 0x01);
    _delay_ms(5000);
    vTaskDelay(4000);
  }
}

void LED2(void *pv)
{
   while(1){
    DIO_SetPortValue(DIO_PORTA, 0x02);
    _delay_ms(1500);
    vTaskDelay(6000);
  }
}

void LED3(void *pv)
{
  while(1){
    DIO_SetPortValue(DIO_PORTA, 0x04);
    _delay_ms(1000);
    vTaskDelay(8100);
  }
}


