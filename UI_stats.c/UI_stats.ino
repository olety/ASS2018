#include <M5Stack.h>

unsigned int score = 0;
byte temp = 0;
byte sleepy = 40;
byte happy = 50;
byte hungry = 30;
byte messageIndex=0;
char* message[] = {
  "pet is eating",
  "pet is sleeping",
  "pet is playing",
  "pet is waiting",
  "pet is dead"
};

void displayTemp(){
  M5.Lcd.fillRect(235, 0, 100, 20, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(0xffff);
  M5.Lcd.setCursor(235,0);
  M5.Lcd.printf("Temp:%d", temp);
}

void displayMessage(){
  M5.Lcd.fillRect(0, 0, 200, 20, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(0xffff);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.print(message[messageIndex]);
}

void displayScore(){
  int x =0;
  int ts = score;
  while((ts = ts/10) > 0)
    x++;
  M5.Lcd.fillRect(235-x*12, 20, 200, 40, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(0xffff);
  M5.Lcd.setCursor(235-x*12,20);
  M5.Lcd.printf("Score:%d", score);
}

void displayFunctions(){
  M5.Lcd.setTextSize(1);  
  
  // button left
  M5.Lcd.setCursor(55,210);
  M5.Lcd.setTextColor(0x51d);
  M5.Lcd.print("sleep");
  
  // button mid
  M5.Lcd.setCursor(145,210);
  M5.Lcd.setTextColor(0x2589);
  M5.Lcd.print("hunger");
  
  // button right
  M5.Lcd.setCursor(240,210);
  M5.Lcd.setTextColor(0xfd79);
  M5.Lcd.print("happiness");

}

void displayStats(){
  M5.Lcd.setTextSize(2);

  // button left
  M5.Lcd.fillRect(55, 220, 50, 20, 0);
  M5.Lcd.setCursor(55,220);
  M5.Lcd.setTextColor(0x51d);
  M5.Lcd.printf("%d%%", sleepy);
  
  // button mid
  M5.Lcd.fillRect(145, 220, 50, 20, 0);
  M5.Lcd.setCursor(145,220);
  M5.Lcd.setTextColor(0x2589);
  M5.Lcd.printf("%d%%", hungry);
  
  // button right
  M5.Lcd.fillRect(240, 220, 50, 20, 0);
  M5.Lcd.setCursor(240,220);
  M5.Lcd.setTextColor(0xfd79);
  M5.Lcd.printf("%d%%", happy);
}

void updateUI(){
  // M5.Lcd.clear();
  displayTemp();
  displayMessage();
  displayScore();
  displayFunctions();
  displayStats();
}

// the setup routine runs once when M5Stack starts up
void setup(){
  // Initialize the M5Stack object
  M5.begin();
  M5.Lcd.clear();
}

// the loop routine runs over and over again forever
void loop() {
  updateUI();
  delay(1000);
  hungry=random(100);
  sleepy=random(100);
  happy=random(100);
  temp++;
  temp = temp % 50;
  score=score+15;
  messageIndex++;
  messageIndex= messageIndex%5;
}
