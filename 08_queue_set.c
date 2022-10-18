/**
 *
 * @file       8_queue_set.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Thu Oct 13 19:46:08 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * @note       configUSE_QUEUE_SETS 1  should be added into the FreeRTOSConfig.h
 */


#include "Drivers/DIO_interface.h"
#include "Drivers/USART_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/queue.h"
#include <util/delay.h>


static QueueHandle_t queue1=NULL;
static QueueHandle_t queue2=NULL;
static QueueSetHandle_t queueSet=NULL;

/*Common Messages Formats*/
const char *s1          = "Sender1";
const char *s2          = "Sender2";
const char *unknown     = "UNKNOWN SOURCE";
const char *error_msg   = "Error Sending Message %d\n";
const char *success_msg = "%s sent %c\n";
const char *received    = "Received from %s, %c\n";

void vSender1(void *pv)
{
  char c='A';
  while(1){
    c='A'+(c-'A')%26;
    if(pdPASS==xQueueSend(queue1, &c, 0))
      printf(success_msg, s1, c++);
    else
      printf(error_msg, 1);

    vTaskDelay(4000); /*Non-Blocking Delay*/
    
  }
}



void vSender2(void *pv)
{
  char c='0';
  while(1){
    c='0'+(c-'0')%10;
    
    if(pdPASS==xQueueSend(queue2, &c, 0))
      printf(success_msg, s2, c++);
    else
      printf(error_msg, 2);

   vTaskDelay(5000); /*Non-Blocking Delay*/
  }
}

void vReceiver(void *pv)
{
  QueueHandle_t queueThatContainsData;
  char c;
  while(1){
    queueThatContainsData= (QueueHandle_t)xQueueSelectFromSet(queueSet, portMAX_DELAY);

    xQueueReceive(queueThatContainsData, &c, 0);
	  
    if(queueThatContainsData==queue1)
      printf(received, s1, c);
    else if(queueThatContainsData==queue2)
      printf(received, s2, c);
    else
      printf("Received data from undefined queue\n");
    vTaskDelay(2000);
  }

}



int main()
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();

  /*If the value of the queue is less than 2, error appears*/
  queue1=xQueueCreate(3, sizeof(char));
  queue2=xQueueCreate(3, sizeof(char));

  queueSet=xQueueCreateSet(3*2);

  /*Add queue1 to the queue set*/
  xQueueAddToSet(queue1, queueSet);
  /*Add queue2 to the queue set*/
  xQueueAddToSet(queue2, queueSet);
  
  if(queue1 && queue2)
    printf("The queues have been created successfully\n\n");
  else
    printf("Error creating the queues\n\n");
  
  xTaskCreate(vSender1,  NULL, 200, NULL, 2, NULL);
  xTaskCreate(vSender2,  NULL, 200, NULL, 1, NULL);
  xTaskCreate(vReceiver, NULL, 200, NULL, 0, NULL);

  vTaskStartScheduler();
  while(1);

  return 0;
}
