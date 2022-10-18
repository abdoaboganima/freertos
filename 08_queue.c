/**
 *
 * @file       8_queue.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Mon Oct 10 15:04:39 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * @note       I must use semaphores for using shared resources like USART
 *             if the two tasks have the same priority.
 * @bug        Changing the stack depth causes data corruption.
 */

#include "Drivers/DIO_interface.h"
#include "Drivers/USART_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/queue.h"
#include <util/delay.h>


QueueHandle_t queue=NULL;
static const char *Task1pcName="Task1\n";
static const char *Task2pcName="Task2\n";


void vTask1(void *pv)
{
  char c='A';
  while(1){
    c='A'+(c-'A')%26;    /*For repeating 'A'-'Z' continuously */
    USART_sendStream((char*) pv);    
    if (pdPASS==xQueueSend(queue, &c, 10))
      printf("Sending %c to the queue\n", c++);
    else
      printf("****Error sending to the queue*****\n");
    printf("Tx: # elements in the queue is: %d\n\n", uxQueueMessagesWaiting(queue));
    vTaskDelay(300); /*Non-blocking delay*/
  }
}

void vTask2(void *pv)
{

  char c;
  while(1){
    USART_sendStream((char*) pv);
    if (pdPASS==xQueueReceive(queue, &c, 10))
      printf("Receiving %c from queue\n", c);
    else
      printf("*****Error Receiving from the queue******\n");
    printf("Rx: # elements in the queue is: %d\n\n",uxQueueMessagesWaiting(queue));
    vTaskDelay(300); /*Non blocking delay*/   
  }
}



int main()
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();
 
  queue=xQueueCreate(1, sizeof(char));

  if(queue)
    printf("The queue has been created successfully\n\n");
  else
    printf("Error creating the queue\n\n");
  
  xTaskCreate(vTask1, Task1pcName, 150, (void *)Task1pcName, 2, NULL);
  xTaskCreate(vTask2, Task2pcName, 150, (void *)Task2pcName, 1, NULL);

  vTaskStartScheduler();
  while(1);

  return 0;
}
