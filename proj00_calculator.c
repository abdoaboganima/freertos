/**
 *
 * @file       calculator.c
 * @brief      Contains simple calculator functions
 * @author     Abdulrahman Aboghanima
 * @date       Sat Oct 15 05:20:54 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */

#include "Drivers/USART_interface.h"
#include "Drivers/KEYPAD_interface.h"
#include "Drivers/CLCD_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/semphr.h"

#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define BUFF_LEN 100

/* 66*77+66 */
char buffer[BUFF_LEN];
const char *ops="*-+/";
xSemaphoreHandle operation;

void vGet(void *pv)
{
  uint8_t curr=0;
  while(1){
    uint8_t c = KEYPAD_getPressedKey();
    
    if(c != NO_KEY_PRESSED){
      USART_send(c);
      if(c=='='){
	buffer[curr]=0;
	curr=0;
	xSemaphoreGive(operation);
	vTaskDelay(50);
      }
      else
	buffer[curr++]=c;
    }
  }
}

int doCalc(int num1, int num2, char op)  
{                                        
  int result;                            
  switch(op){                     
  case '+': result=num1+num2; break;                               
  case '-': result=num1-num2; break;                               
  case '*': result=num1*num2; break;                               
  case '/': result=num1/num2; break;                               
  default : result=0; break;                               
  }                                      
  return result;                         
}                                        


void vCalc(void *pv)
{
  
  while(1){   
    /*This Task cannot begin until the previous one gives the semaphore
     *Because the initial value of the semaphore is 0
     */
    /*Make this task waits indefinitely time waiting for the semaphore*/
    xSemaphoreTake(operation, portMAX_DELAY);

    /*points to first operation +-/ */          
    const char *ptr=strpbrk(buffer, ops);    
    
    /*Stores the first num*/               
    int result=atoi(buffer);                 
    /*55+66+77*/
    while(ptr!=NULL){                      
      char op=*ptr++;                           
      int num2=atoi(ptr);                      
      ptr=strpbrk(ptr, ops);               
      result=doCalc(result, num2, op);     
    }                                      
    printf("%d\n", result);
  }
}





int main(void)
{
  USART_init(); /*Doesn't work with simulide*/
  KEYPAD_init();
  USART_redirect_stream_to_stdout();
  
  /*Create a binary semaphore that has initial value of 0*/
  operation =xSemaphoreCreateCounting(1, 0);
  
  xTaskCreate(vGet, NULL, 150, NULL, 2, NULL);
  xTaskCreate(vCalc, NULL, 150, NULL, 1, NULL);

  vTaskStartScheduler();

  while(1);

  return 0;
}

