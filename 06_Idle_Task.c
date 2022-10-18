/**
 *
 * @file       6_Idle_Task.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Sat Oct  8 14:27:13 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * @note       configUSE_IDLE_HOOK must be 1
 */

#include "Drivers/DIO_interface.h"
#include "Drivers/USART_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include <util/delay.h>

static const char *Task1pcName="Task1";
xTaskHandle task1;

/*Will be called while the idle state*/
void vApplicationIdleHook(void);


void vTask1(void *pv)
{
  while(1){
    USART_sendStream((char*) pv);
    USART_sendStream("\nHello From Task1\n\n");
    vTaskDelay(2000);
  }
}



int main()
{
  xTaskCreate(vTask1, Task1pcName, 100, (void *)Task1pcName, 1, &task1);

  vTaskStartScheduler();
  while(1);
  return 0;
}

void vApplicationIdleHook(void)
{
  USART_sendStream("IDLE\n");
  _delay_ms(100);
}
