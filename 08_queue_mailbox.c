/**
 *
 * @file       8_queue_mailbox.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Fri Oct 14 05:53:16 2022
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




/*Common Messages Formats*/
const char *s1          = "Sender1";
const char *s2          = "Sender2";
const char *unknown     = "UNKNOWN SOURCE";
const char *error_msg   = "Error Sending Message %d\n";
const char *success_msg = "%s sent %c\n";
const char *received    = "Received from %s, %c\n";

QueueHandle_t xMailbox;


typedef struct {
  TickType_t time_stamp;
  uint16_t value;
} Data;

void vSender1(void *pv)
{
  Data d;
  uint8_t c='A';
  while(1){
    c='A'+(c-'A')%26;
    d.time_stamp=xTaskGetTickCount();
    d.value=c++;
    xQueueOverwrite(xMailbox, &d);
    vTaskDelay(4000); /*Non-Blocking Delay*/
  }
  
}

void vReceiver1(void *pv)
{
  Data d;
  while(1){
    xQueuePeek(xMailbox, &d, 0);
    printf("R1: The time Stamp is %d, the data is %c\n", d.time_stamp, d.value);
    vTaskDelay(2000);
  }

}


void vReceiver2(void *pv)
{
  Data d;
  while(1){
    xQueuePeek(xMailbox, &d, 0);
    printf("R2: The time Stamp is %d, the data is %c\n", d.time_stamp, d.value);
    vTaskDelay(2000);
  }

}




int main()
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();

  /*The mailbox is a queue that contains one element*/
  xMailbox=xQueueCreate(1, sizeof(Data));

 if(xMailbox)
    printf("The queues have been created successfully\n\n");
  else
    printf("Error creating the queues\n\n");
  
  xTaskCreate(vSender1,    NULL, 200, NULL, 2, NULL);
  xTaskCreate(vReceiver1,  NULL, 200, NULL, 0, NULL);
  xTaskCreate(vReceiver2,  NULL, 200, NULL, 1, NULL);

  vTaskStartScheduler();
  while(1);

  return 0;
}

