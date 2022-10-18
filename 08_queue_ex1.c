
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
  char buff[20];
  for (int i = 1; i <=10 ; ++i){
    sprintf(buff, "Message %d", i);
    if(pdPASS==xQueueSend(queue, (void *)buff, 10))
      printf("Message %d sent successfully\n", i);
    else
      printf("Error sending Message %d\n", i);
    
    printf("#elements in the queue is: %d\n ", uxQueueMessagesWaiting(queue));
  }
  /*Note if you don't delete the task, the scheduler will stuck in the loop*/
  vTaskDelete(NULL); /* Delete this task*/
  while(1);
}



void vTask2(void *pv)
{
  char buff[20];
  while(1){
    if(pdPASS==xQueueReceive(queue, (void *) buff, 10))
      printf("Received message is --------> %s\n", buff);
    else
      printf ("The queue is empty\n");
    vTaskDelay(1000);
  }
}


int main()
{
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();
 
  queue=xQueueCreate(10, 20*sizeof(char));

  if(queue)
    printf("The queue has been created successfully\n\n");
  else
    printf("Error creating the queue\n\n");
  
  xTaskCreate(vTask1, Task1pcName, 200, (void *)Task1pcName, 2, NULL);
  xTaskCreate(vTask2, Task2pcName, 200, (void *)Task2pcName, 1, NULL);

  vTaskStartScheduler();
  while(1);
  return 0;
}
