/**
 *
 * @file       7_delete_Tasks.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Sat Oct  8 17:05:09 2022
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
  uint8_t counter=0;
  while(1){
    USART_sendStream((char*) pv);
    USART_sendStream("\nHello From Task2\n\n");
    counter++;
    if(counter==2){
      USART_sendStream("Task2 is deleted\n");
      vTaskDelete(NULL); /* Execute this task twice, then delete it*/
      /*Anything after this line will not be executed .. Because the task is deleted */
    }
    vTaskDelay(1000); /*Non blocking delay*/
   
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
