#include <M5Stack.h>
#include <stdlib.h>

String NamePart1[] = {
  "Bland",
  "Clever",
  "Childish",
  "Energetic",
  "Frank",
  "Gentle",
  "Lazy",
  "Noisy",
  "Romantic",
  "Sensitive"
};

String NamePart2[] = {
  "Egg",
  "Kiwi",
  "Banana",
  "Potato",
  "Tomato",
  "Ginger",
  "Mushroom",
  "Peach",
  "Grape",
  "Cucumber"
  
};

int NameIndex[2];
unsigned int score = 10;

void setup() {
  M5.begin();
  M5.Lcd.setBrightness(255);
  M5.Lcd.fillScreen(0xffff);
}


void displayShuffleName(){
//  set the text
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(0x0000);
// shuffle the name
  int NameIndex1 = random(0,9);
  int NameIndex2 =random(0,9);
  NameIndex[0]= NameIndex1;
  NameIndex[1]= NameIndex2;
//  M5.Lcd.printf("%d""%d",NameIndex1,NameIndex2);
//  delay(800000000);
  
//  show your name is shuffle....
  M5.Lcd.setCursor(10,115);
  M5.Lcd.printf("%s","your name is shuffling..");
  delay(3000);
  M5.Lcd.fillScreen(0xffff);

//show the name
  M5.Lcd.setCursor(10,90);
  M5.Lcd.println("your name is:");
  M5.Lcd.setCursor(100,120);
  M5.Lcd.print(NamePart1[NameIndex[0]]);
  M5.Lcd.print(" ");
  M5.Lcd.print(NamePart2[NameIndex[1]]);
  delay(3000);
  
  }


void displayNameScore(){
  M5.Lcd.fillScreen(0xffff);
  M5.Lcd.fillRect(50,30,220,180,0x0000);
  M5.Lcd.setCursor(60,50);
  M5.Lcd.setTextColor(0xffff);
  M5.Lcd.print(NamePart1[NameIndex[0]]);
  M5.Lcd.print(" ");
  M5.Lcd.print(NamePart2[NameIndex[1]]);
  M5.Lcd.setCursor(75,120);
  M5.Lcd.print("You got");
  M5.Lcd.setCursor(85,140);
  M5.Lcd.printf("%d",score);
  delay(5000);
  }

void displayThanks(){
  M5.Lcd.fillScreen(0xffff);
  M5.Lcd.setCursor(20,115);
  M5.Lcd.setTextColor(0x0000);
  M5.Lcd.print("Thank you for playing");
  }

void loop() {
  if (M5.BtnB.wasPressed())
  {
//   shuffle the name
      displayShuffleName();
      displayNameScore();
      displayThanks();
      
    }
    M5.update();

}
