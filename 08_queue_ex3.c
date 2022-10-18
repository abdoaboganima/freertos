/**
 *
 * @file       8_queue_ex3.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Tue Oct 11 18:48:58 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */

#include "Drivers/DIO_interface.h"
#include "Drivers/USART_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/queue.h"
#include <util/delay.h>

/*Defines IDs for the senders*/
typedef enum
  {
   eSender1,
   eSender2
  } DataSource_t;

/*Defines a structure for a message*/
typedef struct
{
  uint8_t ucValue;
  DataSource_t eDataSource;
} Data_t ;

static const Data_t messages[4]={
				 {100, eSender1},
				 {200, eSender1},
				 {30,  eSender2},
				 {55,  eSender2}};

QueueHandle_t queue=NULL;

/*Common Messages Formats*/
const char *s1          = "Sender1";
const char *s2          = "Sender2";
const char *unknown     = "UNKNOWN SOURCE";
const char *error_msg   = "Error Sending Message %d\n";
const char *success_msg = "%s sent %d\n";

void vSender1(void *pv)
{
  while(1){
    
    if(pdPASS==xQueueSend(queue, &messages[0], 0))
      printf(success_msg, s1, messages[0].ucValue);
    else
      printf(error_msg, 1);

    vTaskDelay(4000); /*Non-Blocking Delay*/

    if(pdPASS==xQueueSend(queue, &messages[1], 0))
      printf(success_msg, s1, messages[1].ucValue);
    else
      printf(error_msg, 2);

    vTaskDelay(4000); /*Non-Blocking Delay*/
    
  }
}



void vSender2(void *pv)
{
  while(1){
    
    if(pdPASS==xQueueSend(queue, &messages[2], 0))
      printf(success_msg, s2, messages[2].ucValue);
    else
      printf(error_msg, 3);
    vTaskDelay(4000); /*Non-Blocking Delay*/
    if(pdPASS==xQueueSend(queue, &messages[3], 0))
      printf(success_msg, s2, messages[3].ucValue);
    else
      printf(error_msg, 4);
    vTaskDelay(4000); /*Non-Blocking Delay*/
  }
}

void vReceiver(void *pv)
{
  Data_t buff;
  const char *sender;
  while(1){
    if(pdPASS==xQueueReceive(queue, &buff, 0)){
      if(buff.eDataSource==eSender1)
	sender=s1;
      else if(buff.eDataSource==eSender2)
	sender=s2;
      else
	sender=unknown;
      printf("Received from %s, %d\n", sender, buff.ucValue);
    }
    else
      printf("Error Receiving\n");

    vTaskDelay(2000);
  }

}



int main()
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();

  /*If the value of the queue is less than 2, error appears*/
  queue=xQueueCreate(3, sizeof(Data_t));

  if(queue)
    printf("The queue has been created successfully\n\n");
  else
    printf("Error creating the queue\n\n");
  
  xTaskCreate(vSender1,  NULL, 150, NULL, 2, NULL);
  xTaskCreate(vSender2,  NULL, 150, NULL, 1, NULL);
  xTaskCreate(vReceiver, NULL, 150, NULL, 0, NULL);

  vTaskStartScheduler();
  while(1);

  return 0;
}
