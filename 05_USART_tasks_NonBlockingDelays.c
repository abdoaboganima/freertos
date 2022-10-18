/**
 *
 * @file       5_USART_tasks_NonBlockingDelays.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Fri Oct  7 18:23:13 2022
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
    vTaskDelay(1000);
  }
}



void vTask2(void *pv)
{
  while(1){
    USART_sendStream((char*) pv);
    USART_sendStream("\nHello From Task2\n\n");
    vTaskDelay(1000);
  }
}

int main()
{
  xTaskCreate(vTask1, Task1pcName, 100, (void *)Task1pcName, 1, NULL);
  xTaskCreate(vTask2, Task2pcName, 100, (void *)Task2pcName, 2, NULL);

  vTaskStartScheduler();
  while(1);
  return 0;
}
