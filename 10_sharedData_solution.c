/**
 *
 * @file       10_sharedData_solution.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Sun Oct 16 06:50:16 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */

#include "Drivers/USART_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

#include <util/delay.h>

uint8_t x=0, y=0;


void vCheckTask(void *pv)
{
  
  while(1){
    if(x!=y)
      printf("Problem!!!: The two variables should be equal\n");
    else
      printf("No problem\n");
    _delay_ms(2000); /*Blocking delay: To make the message noticeable*/
    vTaskDelay(40);
  }
}

void vModifyTask(void *pv)
{
  /* We need to modify (increment) x, y at the same time
   * But that may not happen because there may be in interrupt or a task that
   * may preempt this task
   */
  while(1){
    /*We consider the block of code that modifies them as a critical section*/
    taskENTER_CRITICAL();
    x++;
    printf("x is incremented\n");
    y++;
    printf("y is incremented\n");
    taskEXIT_CRITICAL();
  }
}



int main(void)
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();
  
  if(pdPASS==xTaskCreate(vCheckTask, NULL, 200, NULL, 1, NULL))
    printf("Check task created successfully\n");

  if(pdPASS==xTaskCreate(vModifyTask, NULL, 200, NULL, 0, NULL))
    printf("Modify task created successfully\n");
  
  
  vTaskStartScheduler();
  
  while(1);
  
  return 0;
}
