/**
 *
 * @file       9_timers_ex1.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Fri Oct 14 17:12:53 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */

#include "Drivers/USART_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/timers.h"

#include <util/delay.h>
TimerHandle_t oneShotTimer, autoReloadTimer;

void oneShotTimerCallback(TimerHandle_t timer)
{
  /*obtain the current tick time*/
  TickType_t xTimeNow=xTaskGetTickCount();
  printf("One shot timer, time now is: %u\n", xTimeNow);
}


void autoReloadTimerCallback(TimerHandle_t timer)
{
  static uint8_t autoReloadCount=0;
  
  /*obtain the current tick time*/
  TickType_t xTimeNow=xTaskGetTickCount();
  printf("Auto reload timer, time now is: %u\n", xTimeNow);
  autoReloadCount++;
  /*Count until 6, then stop the timer*/
  if(autoReloadCount==6)
    xTimerStop(autoReloadTimer, 0);
}

int main()
{

  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();

  /*One shot timer, the callback executed once*/
  oneShotTimer   =xTimerCreate("One shot timer ",
			       1000,
			       pdFALSE,
			       NULL,
			       oneShotTimerCallback);

  /*periodic timer, the callback executed periodically*/
  autoReloadTimer= xTimerCreate("Auto reload timer",
				3000,
				pdTRUE,
				NULL,
				autoReloadTimerCallback);

  BaseType_t status1 = xTimerStart(oneShotTimer, 0);
  BaseType_t status2 = xTimerStart(autoReloadTimer, 0);

  if(status1==pdPASS && status2==pdPASS){
    printf("Timers started successfully\n");
    
    /*Timer will not start unless the schedule is started*/
    vTaskStartScheduler();
  }
  
  while(1);
  return 0;
}
