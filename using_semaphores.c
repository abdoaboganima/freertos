
#include "DIO_interface.h"
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "CLCD_interface.h"
#include "USART_interface.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include <stdio.h>


static FILE USART_stream=FDEV_SETUP_STREAM(USART_putchar, USART_getchar, _FDEV_SETUP_RW);

/*
//Without using semaphores
void LCD1(void *pv)
{
	while(1){
		USART_sendStream("TASK1111111111\n");
		vTaskDelay(100);
	}
}

void LCD2(void *pv)
{
	while(1){
		USART_sendStream("TASK2222222222\n");
		vTaskDelay(100);
	}
}
*/



typedef struct {
	char y;
	int x;
}x;


//using semaphores
//Srmaphore handle
xSemaphoreHandle uart;

void LCD1(void *pv)
{
	while(1){
		if(xSemaphoreTake(uart, 0)==pdPASS){
			USART_sendStream("TASK1111111111\n");
			xSemaphoreGive(uart);
		}
		vTaskDelay(1);
	}
}
void LCD2(void *pv)
{
	while(1){
		if(xSemaphoreTake(uart, 0)==pdPASS){
			USART_sendStream("TASK2222222222\n");
			xSemaphoreGive(uart);
		}
		vTaskDelay(1);
	}
}


int main()
{

  DIO_SetPortDirection(DIO_PORTA, DIO_PORT_HIGH);
  DIO_SetPortDirection(DIO_PORTB, DIO_PORT_HIGH);
  stdout = &USART_stream;
  USART_init();

  xTaskCreate(&LCD1, NULL, 250, NULL, 0, NULL);
  xTaskCreate(&LCD2, NULL, 250, NULL, 0, NULL);
  uart=xSemaphoreCreateCounting(1,1);


  vTaskStartScheduler();
  while(1){
    /*Do nothing*/
  }
  return 0;
}


