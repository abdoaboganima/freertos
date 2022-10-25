/**
 *
 * @file       11_eventGroups_andingEvents.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Tue Oct 18 16:34:24 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */
#include "Drivers/USART_interface.h"
#include "Drivers/DIO_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/event_groups.h"

#include <util/delay.h>

#define BIT0 ((1<<0))
#define BIT1 ((1<<1))

EventGroupHandle_t twoEvents;

void vButton1(void *pv)
{
  /*Make B0 as input*/
  DIO_SetPinDirection(DIO_PORTB, DIO_PIN0, DIO_PIN_INPUT);
  /*Enable the internal pull-up resistance*/
  DIO_SetPinValue(DIO_PORTB, DIO_PIN0, DIO_PIN_HIGH);
  

  uint8_t pinValue;
  
  while(1){
  
    DIO_GetPinValue(DIO_PORTB, DIO_PIN0, &pinValue);

    vTaskDelay(200); /*For the de-bouncing*/

    if(pinValue==DIO_PIN_LOW){
      printf("button 1 pressed\n");
      xEventGroupSetBits(twoEvents, BIT0);
      
    }
  }
}


void vButton2(void *pv)
{
  /*Make B1 as input*/
  DIO_SetPinDirection(DIO_PORTB, DIO_PIN1, DIO_PIN_INPUT);
  /*Enable the internal pull-up resistance*/
  DIO_SetPinValue(DIO_PORTB, DIO_PIN1, DIO_PIN_HIGH);

  
  uint8_t pinValue;
  
  while(1){
  
    DIO_GetPinValue(DIO_PORTB, DIO_PIN1, &pinValue);

    vTaskDelay(200); /*For the de-bouncing*/
	
    
    if(pinValue==DIO_PIN_LOW){
      printf("button 2 pressed\n");
      xEventGroupSetBits(twoEvents, BIT1);
      vTaskDelay(30);
    }
  }
}

void vPrint(void *pv)
{
  
  while(1){
    xEventGroupWaitBits(twoEvents, 
			BIT0|BIT1,
			pdTRUE, /*Clear the two flags after reading*/
			pdTRUE, /*Wait for all the flags to happen */
			portMAX_DELAY);
    
    printf("The two events have happened\n");
  }
}



int main(void)
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();

  /*Creating an event group object*/
  twoEvents=xEventGroupCreate();
  
  if(pdPASS==xTaskCreate(vButton1, NULL, 200, NULL, 2, NULL))
    printf("Button1 task created successfully\n");

  if(pdPASS==xTaskCreate(vButton2, NULL, 200, NULL, 2, NULL))
    printf("Button2 task created successfully\n");

  
  if(pdPASS==xTaskCreate(vPrint, NULL, 200, NULL, 0, NULL))
    printf("Print task created successfully\n");
  
  
  vTaskStartScheduler();
  
  while(1);
  
  return 0;
}
