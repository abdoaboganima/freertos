/**
 *
 * @file       12_taskNotification_ex1.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Wed Oct 19 07:14:59 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */


#include "Drivers/USART_interface.h"
#include "Drivers/DIO_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

#include <util/delay.h>

#define BIT0 ((1<<0))
#define BIT1 ((1<<1))


TaskHandle_t task1=NULL;
TaskHandle_t task2=NULL;

void vTask1(void *pv)
{
  while(1){
    /* Every call to this function increments the notification value in the TCB of task2 by one */
    xTaskNotifyGive(task2);
    vTaskDelay(1000);
  }
}


void vTask2(void *pv)
{
  while(1){
    uint32_t notification=ulTaskNotifyTake(pdFALSE, /*Don't clear the notification value on the exit*/
					   0       /*Don't wait*/
					   );
    printf("Got a notification value=%ld\n", notification);
    vTaskDelay(4000);
  }
}



int main(void)
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();

  
  if(pdPASS==xTaskCreate(vTask1, NULL, 200, NULL, 2, &task1))
    printf("Task1 task created successfully\n");

  if(pdPASS==xTaskCreate(vTask2, NULL, 200, NULL, 1, &task2))
    printf("Task2 task created successfully\n");

  
  
  vTaskStartScheduler();
  
  while(1);
  
  return 0;
}
