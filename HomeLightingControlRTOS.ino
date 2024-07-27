#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include "task.h"
#include "semphr.h"
#include <LiquidCrystal_I2C.h>
#define pir1 8
#define pir2 9
int qt1 = A0;
int qt2 = A1;
int pirstate1 = LOW;
int pirstate2 = LOW;
int lamp3 = 13;
int lamp2  = 12;
int lamp1 = 11;

TaskHandle_t xdieukhien1;
TaskHandle_t xdieukhien2;
TaskHandle_t xdieukhien3;
SemaphoreHandle_t xMutex;
LiquidCrystal_I2C lcd(0x27, 16, 2);
void Function1(void *pvParameter);
void Function2(void *pvParameter);
void Function2(void *pvParameter);

void setup()
{
Serial.begin(9600);
pinMode(qt1, INPUT);
pinMode(qt2, INPUT);

pinMode(lamp1, OUTPUT);
pinMode(lamp2, OUTPUT);
pinMode(lamp3, OUTPUT);

pinMode(pir1, INPUT);
pinMode(pir2, INPUT);
xMutex = xSemaphoreCreateMutex();
xTaskCreate(Function1, "Function1", 128,NULL,1, &xdieukhien1);
xTaskCreate(Function2, "Function2", 128,NULL,1, &xdieukhien2);
xTaskCreate(Function3, "Function3", 128,NULL,1, &xdieukhien3);
  
lcd.init();
lcd.backlight();

vTaskStartScheduler();
}

void loop()
{

}

void Function1(void *pvParameter)
{
while(1)
{
xSemaphoreTake(xMutex, portMAX_DELAY);
Serial.print("______Phong ngu_______ \n");
Serial.print("gia tri quang tro  1 : ");
Serial.println(analogRead(qt1));

Serial.print("gia tri cam bien cdong  1 : ");
Serial.println(digitalRead(pir1));

if(( (10 < analogRead(qt1) ) && (analogRead(qt1)  < 350 )) && (digitalRead(pir1)==HIGH) )
{
digitalWrite(lamp1, HIGH);
lcd.setCursor(0, 0);
lcd.print("ROOM:ON ");
delay(2000);
if (pirstate1 == LOW)
{
pirstate1 = HIGH;}
}
else
{
digitalWrite(lamp1, LOW);
lcd.setCursor(0, 0);
lcd.print("ROOM:OFF");
delay(2000);
if (pirstate1 == HIGH)
{
pirstate1 = LOW;}
}
delay(1000);
xSemaphoreGive(xMutex);
taskYIELD();
}
}

void Function2(void *pvParameter)
{
while(1)
{
xSemaphoreTake(xMutex, portMAX_DELAY);
Serial.print("______Ngoai san_______ \n");
Serial.print("gia tri quang tro 2: ");
Serial.println(analogRead(qt2) );

if(analogRead(qt2) < 350   )
{
digitalWrite(lamp3, HIGH);
lcd.setCursor(0,1);
lcd.print("DOOR:ON ");
delay(2000);
}
else
{
digitalWrite(lamp3, LOW);
lcd.setCursor(0,1);
lcd.print("DOOR:OFF");
delay(2000);  
}
delay(1000);
xSemaphoreGive(xMutex);
taskYIELD();
}
}

void Function3(void *pvParameter)
{
while(1)
{
xSemaphoreTake(xMutex, portMAX_DELAY);
Serial.print("______Cau thang_______ \n");
Serial.print("gia tri cam bien chuyen dong 2: ");
Serial.println(digitalRead(pir2) );

if(digitalRead(pir2)==HIGH   )
{
digitalWrite(lamp2, HIGH);
lcd.setCursor(9,0);
lcd.print("STAIR:");
lcd.setCursor(10,1);
lcd.print("ON ");
delay(2000);
if (pirstate2 == LOW)
{
pirstate2 = HIGH;
}

}
else
{
digitalWrite(lamp2, LOW);
lcd.setCursor(9,0);
lcd.print("STAIR:");

lcd.setCursor(10,1);
lcd.print("OFF");
delay(2000);
if (pirstate2 == HIGH)
{
pirstate2 = LOW;
}
}
delay(1000);
xSemaphoreGive(xMutex);
taskYIELD();
}
}
