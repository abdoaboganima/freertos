/**
 *
 * @file       proj01_smartWatch.c
 * @brief      
 * @author     Abdulrahman Aboghanima
 * @date       Thu Oct 20 06:26:36 2022
 * @copyright  Copyright (c) 2022
 * @version    0.1
 * 
 */

#include "Drivers/USART_interface.h"
#include "Drivers/DIO_interface.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/semphr.h"
#include "FreeRTOS/event_groups.h"
#include <util/delay.h>


typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
} time_t;


typedef struct {
  uint16_t year;
  uint8_t month;
  uint8_t day;
} date_t;


typedef struct {
  uint8_t heart_rate;
  uint8_t temp;
} human_data_t;


static time_t curr={0, 0, 0};
static date_t date={1970, 1, 1};
static human_data_t human={120 , 36};

xSemaphoreHandle time_set_semphr;


void LCD_Task(void *pv)
{
  
  printf("%d:%d:%d\n", curr.hours, curr.minutes, curr.seconds);
  
  while(1){
    printf("%d:%d:%d\n%d:%d:%d\n",
	   curr.hours, curr.minutes, curr.seconds,
	   date.day, date.month, date.year);
    
    vTaskDelay(1000);
  }
}

void rescale_time(time_t * const t)
{
  if(t->seconds >= 60){
    t->seconds=0;
    t->minutes++;
  }
  if(t->minutes >= 60){
    t->minutes=0;
    t->hours++;
  }
  if(t->hours>=24)
    t->hours%=24;
}

void Increment_Task(void *pv)
{
  
  while(1){
    curr.seconds++;
    rescale_time(&curr); /*Check if the seconds exceed 60*/
    
    vTaskDelay(1000); /*Delay for 1000 ms*/
  }
}

void Control_Task(void *pv)
{

  while(1){
    
    uint8_t features_btn_value, SetTime_btn_value;
    DIO_GetPinValue(DIO_PORTB, DIO_PIN0, &features_btn_value);
    DIO_GetPinValue(DIO_PORTB, DIO_PIN1, &SetTime_btn_value);
    
    vTaskDelay(200); /*For the de-bouncing*/
    if(features_btn_value==DIO_PIN_LOW)
      printf("Temp=%d\nHeart Rate=%d\n",
	     human.temp,
	     human.heart_rate);
    if(SetTime_btn_value==DIO_PIN_LOW)
      xSemaphoreGive(time_set_semphr);
  }
}


static inline int confirmIsPressed(void)
{
  uint8_t confirm_btn_value;
  DIO_GetPinValue(DIO_PORTD, DIO_PIN2, &confirm_btn_value);
  vTaskDelay(150); /*For the de-bouncing*/
  return confirm_btn_value==DIO_PIN_LOW;
}

static inline int incrementISPresses(void)
{
  uint8_t increment_btn_value;
  DIO_GetPinValue(DIO_PORTD, DIO_PIN3, &increment_btn_value);
  vTaskDelay(150); /*For the de-bouncing*/
  return increment_btn_value==DIO_PIN_LOW;
}

EventGroupHandle_t button_events;

void Date_and_TimeSetTask(void *pv)
{

  while(1){
    xSemaphoreTake(time_set_semphr, portMAX_DELAY);
    printf("Setting time\n");
    
    while(1){
      
      /*if increment btn is pressed*/
      if(incrementISPresses()){
	date.year++;
      }
      else{
      
	if(confirmIsPressed()){
	  //vTaskDelay(200);

	  if(incrementISPresses())
	    date.month++;

	  else{
	    while(incrementISPresses()){
	      date.day++;
	      if(confirmIsPressed())
		break;
	      
	      //break;
	    }
	  }
	}
	
      }
    }
      
  }
  
}

int main(void)
{
  
  //USART_init(); /*Doesn't work with simulide*/
  USART_redirect_stream_to_stdout();
  /*Make B0 as input*/
  DIO_SetPinDirection(DIO_PORTB, DIO_PIN0, DIO_PIN_INPUT);
  DIO_SetPinDirection(DIO_PORTB, DIO_PIN1, DIO_PIN_INPUT);
  DIO_SetPinDirection(DIO_PORTB, DIO_PIN2, DIO_PIN_INPUT);
  DIO_SetPinDirection(DIO_PORTB, DIO_PIN3, DIO_PIN_INPUT);

  /*Enable the internal pull-up resistance*/
  DIO_SetPinValue(DIO_PORTB, DIO_PIN0, DIO_PIN_HIGH);
  DIO_SetPinValue(DIO_PORTB, DIO_PIN1, DIO_PIN_HIGH);
  DIO_SetPinValue(DIO_PORTD, DIO_PIN2, DIO_PIN_HIGH);
  DIO_SetPinValue(DIO_PORTD, DIO_PIN3, DIO_PIN_HIGH);
  
  time_set_semphr=xSemaphoreCreateCounting(1,0);
  button_events=xEventGroupCreate();
  
  if(pdPASS==xTaskCreate(LCD_Task, NULL, 200, NULL, 0, NULL))
    printf("LCD_Task task created successfully\n");

  if(pdPASS==xTaskCreate(Increment_Task, NULL, 200, NULL, 2, NULL))
    printf("Increment_Task task created successfully\n");
  
  
  if(pdPASS==xTaskCreate(Control_Task, NULL, 200, NULL, 4, NULL))
    printf("Control_Task task created successfully\n");

  
  if(pdPASS==xTaskCreate(Date_and_TimeSetTask, NULL, 200, NULL, 4, NULL))
    printf("Date_and_TimeSetTask task created successfully\n");
  
  
  vTaskStartScheduler();
  
  while(1);
  
  return 0;
}
