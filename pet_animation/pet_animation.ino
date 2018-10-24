#include <M5Stack.h>

#define living1 spider_living1_map
#define living2 spider_living2_map
#define playing1 spider_playing1_map
#define playing2 spider_playing2_map
#define eating1 spider_eating1_map
#define eating2 spider_eating2_map
#define sleeping1 spider_sleeping1_map
#define sleeping2 spider_sleeping2_map
#define dead1 spider_dead1_map
#define dead2 spider_dead2_map


#define livingArray1 extern unsigned char
#define livingArray2 extern unsigned char
#define playingArray1 extern unsigned char
#define playingArray2 extern unsigned char
#define eatingArray1 extern unsigned char
#define eatingArray2 extern unsigned char
#define sleepingArray1 extern unsigned char
#define sleepingArray2 extern unsigned char
#define deadArray1 extern unsigned char
#define deadArray2 extern unsigned char

livingArray1 living1[];
livingArray2 living2[];
playingArray1 playing1[];
playingArray2 playing2[];
eatingArray1 eating1[];
eatingArray2 eating2[];
sleepingArray1 sleeping1[];
sleepingArray2 sleeping2[];
deadArray1 dead1[];
deadArray2 dead2[];

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(0x000);
  M5.Lcd.setBrightness(255);
  

}

void loop() {

  int t;
  for (t=0;t<5;t++){
  animation_living();
  }
  for (t=0;t<5;t++){
  animation_playing();
  }
  for (t=0;t<5;t++){
  animation_eating();
  }
  for (t=0;t<5;t++){
  animation_sleeping();
  }
  for (t=0;t<5;t++){
  animation_dead();
  }
}

void animation_living(){
  M5.Lcd.drawBitmap(96,56,128,128,living1);
  delay(400);
  M5.Lcd.drawBitmap(96,56,128,128,living2);
  delay(400);
  }

void animation_playing(){
  M5.Lcd.fillRect(160,56,64,128,0xffff);
  M5.Lcd.drawBitmap(96,56,64,128,playing1);
  delay(400);
  M5.Lcd.fillRect(96,56,64,128,0xffff);
  M5.Lcd.drawBitmap(160,56,64,128,playing2);
  delay(400);
  
  } 

void animation_eating(){
  M5.Lcd.drawBitmap(96,56,128,128,eating1);
  delay(400);
  M5.Lcd.drawBitmap(96,56,128,128,eating2);
  delay(400);
  }

void animation_sleeping(){
  M5.Lcd.drawBitmap(96,56,128,128,sleeping1);
  delay(400);
  M5.Lcd.drawBitmap(96,56,128,128,sleeping2);
  delay(400);
  }

void animation_dead(){
  M5.Lcd.drawBitmap(96,56,128,128,dead1);
  delay(400);
  M5.Lcd.drawBitmap(96,56,128,128,dead2);
  delay(400);
  }
