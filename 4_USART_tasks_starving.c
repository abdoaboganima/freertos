/**
 *
 * @file       3_USART_tasks_starving.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Fri Oct  7 13:24:02 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */

#include "Drivers/DIO_interface.h"
#include "Drivers/USART_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include <util/delay.h>


static const char *Task1pcName="Task1";
static const char *Task2pcName="Task2";

void vTask1(void *pv)
{
  while(1){
    USART_sendStream((char*) pv);
    USART_sendStream("\nHello From Task1\n\n");
    _delay_ms(1000);  /*Blocking delay*/
  }
}



void vTask2(void *pv)
{
  while(1){
    USART_sendStream((char*) pv);
    USART_sendStream("\nHello From Task2\n\n");
    _delay_ms(1000); /*Blocking delay*/
  }
}

int main()
{
  /*Making Task2 having greater priority than task1, gets task1 never executed*/
  xTaskCreate(vTask1, Task1pcName, 100, (void *)Task1pcName, 1, NULL);
  xTaskCreate(vTask2, Task2pcName, 100, (void *)Task2pcName, 2, NULL);

  vTaskStartScheduler();
  while(1);
  return 0;
}
