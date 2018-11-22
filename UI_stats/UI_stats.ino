#include <M5Stack.h>
#include <stdio.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT12.h"
#include <Wire.h>


//Graphics
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

// Game settings
#define STARTING_SCORE 0
#define WINNING_SCORE 100
#define STARTING_SLEEP_PTS 50
#define STARTING_HAPPINESS_PTS 50 
#define STARTING_HUNGER_PTS 50

// Message array indexes
#define MSG_IDLE 0
#define MSG_SLEEP 1
#define MSG_HUNGER 2
#define MSG_HAPPINESS 3
#define MSG_DEATH 4
#define MSG_WIN 5

// Current state
#define STATE_IDLE 0
#define STATE_SLEEP 1
#define STATE_HUNGER 2
#define STATE_HAPPINESS 3
#define STATE_DEATH 4
#define STATE_WIN 5

// Colors
#define DefaultTextColor 0x0000
#define FillColor 0xFFFF

// Images
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


// Game variables
unsigned int score = STARTING_SCORE;
int temp = 27;
int sleep_pts = STARTING_SLEEP_PTS;
int happiness_pts = STARTING_HAPPINESS_PTS;
int hunger_pts = STARTING_HUNGER_PTS;
int messageIndex = MSG_IDLE;
int currentState = STATE_IDLE;

int NameIndex[2];

// Wi-fi stuff
const char* WIFI_SSID = "Tako";
const char* WIFI_PASS =  "12345678";

const char *SERVER_ADDRESS = "192.168.137.1";
const char *SERVER_REQUEST = "{\"name\":\"%s %s\",\"score\":\"%d\"}";
char send_req[200];
HTTPClient http;

//temperature
DHT12 dht12;

// Tasks
TaskHandle_t idleHandle;

// Display messages
String message[] = {
  "Pet is waiting",
  "Pet is sleeping",
  "Pet is eating",
  "Pet is playing",
  "YOU LOST",
  "YOU WON"
};

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



void displayShuffleName(){
  int NameIndex1 = random(0,9);
  int NameIndex2 = random(0,9);
  NameIndex[0] = NameIndex1;
  NameIndex[1] = NameIndex2;

//  M5.Lcd.clear();
//  M5.Lcd.fillScreen(FillColor);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.setCursor(10,190);
  M5.Lcd.printf("%s","your name is shuffling..");
  delay(2000);
  
  M5.Lcd.fillScreen(FillColor);

//show the name
  M5.Lcd.setCursor(10,190);
  M5.Lcd.println("your name is:");
  M5.Lcd.setCursor(100,210);
  M5.Lcd.print(NamePart1[NameIndex[0]]);
  M5.Lcd.print(" ");
  M5.Lcd.print(NamePart2[NameIndex[1]]);
  delay(3000);
  
  }

void displayNameScore(){
  M5.Lcd.fillScreen(FillColor);
  
  M5.Lcd.setCursor(65,30);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.print(NamePart1[NameIndex[0]]);
  M5.Lcd.print(" ");
  M5.Lcd.print(NamePart2[NameIndex[1]]);
  M5.Lcd.setCursor(75,190);
  M5.Lcd.print("You got");
  M5.Lcd.setCursor(85,210);
  M5.Lcd.printf("%d",score);
  delay(5000);
  
  M5.Lcd.clear();
  M5.Lcd.fillScreen(FillColor);
  
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.setCursor(20,190);
  M5.Lcd.println("Your score is uploading");
  delay(500);
  }

void displayThanks(){
  M5.Lcd.fillScreen(FillColor);
  M5.Lcd.setCursor(20,190);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.println("Thank you for playing");
  delay(100);
}

void sendScore(){
  
  // Send a post request
//  M5.Lcd.println("Connecting to the server");
  http.begin(SERVER_ADDRESS, 5656, "/" );
//  M5.Lcd.println("successfully connect to server");
  http.addHeader("Content-Type", "application/json" );
//  M5.Lcd.println("successfully added header");
  sprintf(send_req, SERVER_REQUEST, NamePart1[NameIndex[0]].c_str(), NamePart2[NameIndex[1]].c_str(), score);
//  M5.Lcd.println("successfully sprintfed");
//  M5.Lcd.println(send_req);
  http.POST((uint8_t*)send_req, strlen(send_req));
//  M5.Lcd.println("post req sent");

}

void displayTemp(){
  M5.Lcd.fillRect(235, 20, 100, 20, FillColor);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.setCursor(235,20);
  M5.Lcd.printf("Temp:%d", temp);
}

void displayMessage(){
  M5.Lcd.fillRect(0, 0, 200, 20, FillColor);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.print(message[messageIndex]);
  M5.Lcd.fillRect(0, 20, 227, 40, FillColor);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.setCursor(0,20);
  M5.Lcd.print(temp == 27 ? "Temperature is good" : temp > 27 ? "Temperature is high" : "Temperature is low");
}

void displayScore(){
  int x =0;
  int ts = score;
  while((ts = ts/10) > 0)
    x++;
  M5.Lcd.fillRect(235-x*12, 0, 200, 20, FillColor);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.setCursor(235-x*12,0);
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
  M5.Lcd.fillRect(55, 220, 50, 20, FillColor);
  M5.Lcd.setCursor(55,220);
  M5.Lcd.setTextColor(0x51d);
  M5.Lcd.printf("%d%%", sleep_pts);
  
  // button mid
  M5.Lcd.fillRect(145, 220, 50, 20, FillColor);
  M5.Lcd.setCursor(145,220);
  M5.Lcd.setTextColor(0x2589);
  M5.Lcd.printf("%d%%", hunger_pts);
  
  // button right
  M5.Lcd.fillRect(240, 220, 50, 20, FillColor);
  M5.Lcd.setCursor(240,220);
  M5.Lcd.setTextColor(0xfd79);
  M5.Lcd.printf("%d%%", happiness_pts);
}

void updateUI(){
  if (currentState != STATE_DEATH || currentState != STATE_WIN) {
    displayTemp();
    displayMessage();
    displayScore();
    displayFunctions();
    displayStats();
  }
}

void updateScore(void *pvParameters) {
  // Update the score based on the following formula (every 5 sec):
  // score = score + 1 + 1 if h in range(20,80) + 1 if hp in range(20,80) + 1 if s in range(20,80)
  // If the score is > 999,999 you win the game
  for (;;) {
    if (currentState == STATE_DEATH || currentState == STATE_WIN) {
      continue;
    }
    // Update the score
    if (sleep_pts >= 20 && sleep_pts<= 80) {
      score = score + 1;
    }
    
    if (hunger_pts >= 20 && hunger_pts<= 80) {
      score = score + 1;
    }
    
    if (happiness_pts >= 20 && happiness_pts<= 80) {
      score = score + 1;
    }
    
    score = score + 1 ;
    if (score > WINNING_SCORE) {
      win();
    }
    
    delay(5000);
  }
}

void processScore(){
  displayShuffleName();
  displayNameScore();
//  sendScore();
  displayThanks();
}

void handleReset(){
  M5.Lcd.fillScreen(FillColor);
  M5.Lcd.setCursor(20,190);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.println("Press any button to \n\t\trestart");
  delay(500);
////  while(1){
//    if (M5.BtnB.wasPressed()) {
//        M5.Lcd.fillScreen(FillColor);
//        M5.Lcd.setCursor(20,190);
//        M5.Lcd.setTextColor(DefaultTextColor);
//        M5.Lcd.println("Resetting the game...");
//        reset_game();
//        break;
//      } 
//  }
}

void lose() {
  // TODO(UI ppl): show the resume button and the score 
  messageIndex = MSG_DEATH;
  currentState = STATE_DEATH;
  
//  M5.Lcd.fillScreen(FillColor);
  processScore();
  M5.Lcd.fillScreen(FillColor);
  M5.Lcd.setCursor(20,190);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.println("Press any button to \n\t\trestart");
  delay(500);
}

void win() {
  // TODO(UI ppl): show the resume button and the score 
  messageIndex = MSG_WIN;
  currentState = STATE_WIN; 
  
//  M5.Lcd.fillScreen(FillColor);
  processScore();
  handleReset();
  
}

void checkDeath() {
  if (sleep_pts <= 0 || sleep_pts >= 100 || hunger_pts <= 0 || hunger_pts >= 100  || happiness_pts <= 0 || happiness_pts >= 100 ) { 
      // Some points are out of the boundaries
      lose();
  }
  
  if ((sleep_pts > 0 && sleep_pts < 20) || (sleep_pts > 80 && sleep_pts < 100)) {
      // Sleep is in the "danger zone"
      happiness_pts = happiness_pts - 1;
  } 
  
  if ((hunger_pts > 0 && hunger_pts < 20) || (hunger_pts > 80 && hunger_pts < 100)) {
      // Hunger is in the "danger zone"
      happiness_pts = happiness_pts - 1;
  }
}

void reset_game() {
  M5.Lcd.clear();
  M5.Lcd.fillScreen(FillColor);
  M5.Lcd.setTextColor(DefaultTextColor);
  M5.Lcd.setCursor(20,190);
  M5.Lcd.println("RESETTING THE GAME");
  delay(1000);
  
  M5.Lcd.clear();
  M5.Lcd.fillScreen(FillColor);
  M5.Lcd.setTextColor(DefaultTextColor);
  score = STARTING_SCORE;
  sleep_pts = STARTING_SLEEP_PTS;
  happiness_pts = STARTING_HAPPINESS_PTS;
  hunger_pts = STARTING_HUNGER_PTS;
  messageIndex = MSG_IDLE;
  currentState = STATE_IDLE;
  updateUI();
  M5.update();
}

void animation_idle(){
  updateUI();
  M5.Lcd.drawBitmap(96,56,128,128,living1);
  delay(500);
  M5.Lcd.drawBitmap(96,56,128,128,living2);
  delay(500);
  updateUI();
  }

void animation_playing(){
  updateUI();
  M5.Lcd.fillRect(160,56,64,128,FillColor);
  M5.Lcd.drawBitmap(96,56,64,128,playing1);
  delay(250);
  M5.Lcd.fillRect(96,56,64,128,FillColor);
  M5.Lcd.drawBitmap(160,56,64,128,playing2);
  delay(250);
  updateUI();
  } 

void animation_eating(){
  updateUI();
  M5.Lcd.drawBitmap(96,56,128,128,eating1);
  delay(250);
  M5.Lcd.drawBitmap(96,56,128,128,eating2);
  delay(250);
  updateUI();
  }

void animation_sleeping(){
  updateUI();
  M5.Lcd.drawBitmap(96,56,128,128,sleeping1);
  delay(250);
  M5.Lcd.drawBitmap(96,56,128,128,sleeping2);
  delay(250);
  updateUI();
  }

void animation_dead(){
  M5.Lcd.drawBitmap(96,56,128,128,dead1);
  delay(250);
  M5.Lcd.drawBitmap(96,56,128,128,dead2);
  delay(250);
  }
  
void processEvent(void *pvParameters) {
  // Handle a button press and update the score afterwards
  // We should also pause the updatePts activity during the event
  for (;;) {
    if (currentState != STATE_DEATH && currentState != STATE_WIN) {
      // Check if a button was pressed
      if (M5.BtnA.wasPressed()) {
        vTaskSuspend( idleHandle );
        sleep_pts = sleep_pts + 5;
        messageIndex = MSG_SLEEP;
        currentState = STATE_SLEEP;
        checkDeath();
        animation_sleeping();
        animation_sleeping();
        
        vTaskResume( idleHandle );
      }
      
      if (M5.BtnB.wasPressed()) {
        vTaskSuspend( idleHandle );
        hunger_pts = hunger_pts - 5;
        messageIndex = MSG_HUNGER;
        currentState = STATE_HUNGER;
        checkDeath();
        animation_eating();
        animation_eating();
        vTaskResume( idleHandle );
        M5.update();
      }
      
      if (M5.BtnC.wasPressed()) {
        vTaskSuspend( idleHandle );
        happiness_pts = happiness_pts + 5;
        messageIndex = MSG_HAPPINESS;
        currentState = STATE_HAPPINESS;
        checkDeath();
        animation_playing();
        animation_playing();
        vTaskResume( idleHandle );
      }
      
      currentState = STATE_IDLE;
      messageIndex = MSG_IDLE;
      M5.update();
    }
  }
}

void animateIdle(void *pvParameters) {
  for (;;) {
    if (currentState == STATE_DEATH || currentState == STATE_WIN) {
      animation_dead();
    } else {
      animation_idle();
    }
  }
}
  
void updatePts() {
  // Check whether we won
  if (score > WINNING_SCORE) {
    win();
  }
  // Every activity's points go down by 1 every second
  if (currentState == STATE_IDLE) {
    sleep_pts = sleep_pts - (int) 1 - (int)abs(27-temp)*2;
    happiness_pts = happiness_pts - 1 - (int)abs(27-temp)*2;
    hunger_pts = hunger_pts + 1;
  }
  // Check whether the pet has died
  checkDeath();
}

void tempThread(void *p){
  for(;;){
    temp = dht12.readTemperature();
    delay(1000);
  }
}

// the setup routine runs once when M5Stack starts up
void setup(){
  // Initialize the M5Stack object
  M5.begin();
  Wire.begin();
  M5.Lcd.clear();
  M5.Lcd.fillScreen(FillColor);
  // Wi-Fi setup start
  // open serial connection to monitor the connection result
//  Serial.begin(115200);
//  WiFi.begin(WIFI_SSID, WIFI_PASS);
//
//  // while the wifi is connectiong...
//  M5.Lcd.setTextColor(DefaultTextColor);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    M5.Lcd.println("Connecting to WiFi..");
//  }
//
//  // successful connection
//  M5.Lcd.println("Connected to the WiFi..");
//  M5.Lcd.println(WiFi.localIP());
//  M5.Lcd.clear();
//  M5.Lcd.fillScreen(FillColor);
//  // Wi-fi setup end
  
  // Task that updates the score every 5 seconds
  xTaskCreatePinnedToCore (
      updateScore,
      "updateScore",
      2048,
      NULL,
      2,
      NULL,
      0
    );
    
  // Task for processing the button press
  xTaskCreatePinnedToCore (
      processEvent,
      "processEvent",
      4096,
      NULL,
      1,
      NULL,
      0
    );

    xTaskCreatePinnedToCore (
      animateIdle,
      "animateIdle",
      4096,
      NULL,
      3,
      &idleHandle,
      0
    );

    xTaskCreatePinnedToCore (
      tempThread,
      "tempThread",
      4096,
      NULL,
      3,
      &idleHandle,
      0
    );
    
   
}


// the loop routine runs over and over again forever
void loop() {
  if (currentState != STATE_DEATH && currentState != STATE_WIN){
    updatePts();
    delay(1000);    
  }
}
