/**
 *
 * @file       9_timers_ex2.c
 * @brief      Contains simple application for the SW timer,
 * @author     Abdulrahman Aboghanima
 * @date       Sat Oct 15 16:35:13 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */

#include "Drivers/USART_interface.h"
#include "Drivers/DIO_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/timers.h"

TimerHandle_t lightTimer;

void vButtonTask(void *pv)
{
  /*Make B0 as input*/
  DIO_SetPinDirection(DIO_PORTB, DIO_PIN0, DIO_PIN_INPUT);
  /*Enable the internal pull-up resistance*/
  DIO_SetPinValue(DIO_PORTB, DIO_PIN0, DIO_PIN_HIGH);
  
  /*Make A0 as output*/
  DIO_SetPinDirection(DIO_PORTA, DIO_PIN0, DIO_PIN_OUTPUT);
  
  uint8_t pinValue;
  
  while(1){
    DIO_GetPinValue(DIO_PORTB, DIO_PIN0, &pinValue);
    if(pinValue==DIO_PIN_LOW){
      /*If the button is pressed, turn on the light and start (or restart) the timer*/
      DIO_SetPinValue(DIO_PORTA, DIO_PIN0, DIO_PIN_HIGH);
      xTimerReset(lightTimer, 10);
      printf("Light is turned on\n");
    }
  }
}


void timerCallback(TimerHandle_t timer)
{
  /*The callback function turns off the light after the timeout*/
  DIO_SetPinValue(DIO_PORTA, DIO_PIN0, DIO_PIN_LOW);
  printf("Light is turned off\n");
}




int main(void)
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();
  
  
  lightTimer=xTimerCreate("Backlight Timer", /*Just a timer name*/
			  3000,          /*Every 3000 ticks, the timer callback will be executed*/
			  pdFALSE,       /*For creating one shot timer */
			  NULL,          /*There is no need for the timer id*/
			  timerCallback  /*The function that will be executed at the timeout*/
			  );

  if(pdPASS==xTimerStart(lightTimer, 10))
    printf("Timer started successfully\n");

  if(pdPASS==xTaskCreate(vButtonTask, NULL, 200, NULL, 0, NULL))
    printf("Button task created successfully\n");
  
  vTaskStartScheduler();
  
  while(1);
  
  return 0;
}
