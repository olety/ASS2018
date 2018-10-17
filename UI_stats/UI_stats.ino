#include <M5Stack.h>

// Game settings
#define STARTING_SCORE 999990
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

//Current state
#define STATE_IDLE 0
#define STATE_SLEEP 1
#define STATE_HUNGER 2
#define STATE_HAPPINESS 3
#define STATE_DEATH 4
#define STATE_WIN 5


unsigned int score = STARTING_SCORE;
int temp = 0;
int sleep_pts = STARTING_SLEEP_PTS;
int happiness_pts = STARTING_HAPPINESS_PTS;
int hunger_pts = STARTING_HUNGER_PTS;
int messageIndex = MSG_IDLE;
int currentState = STATE_IDLE;

String message[] = {
  "pet is waiting",
  "pet is sleeping",
  "pet is eating",
  "pet is playing",
  "YOU LOST",
  "YOU WON"
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
  M5.Lcd.printf("%d%%", sleep_pts);
  
  // button mid
  M5.Lcd.fillRect(145, 220, 50, 20, 0);
  M5.Lcd.setCursor(145,220);
  M5.Lcd.setTextColor(0x2589);
  M5.Lcd.printf("%d%%", hunger_pts);
  
  // button right
  M5.Lcd.fillRect(240, 220, 50, 20, 0);
  M5.Lcd.setCursor(240,220);
  M5.Lcd.setTextColor(0xfd79);
  M5.Lcd.printf("%d%%", happiness_pts);
}

void diplayPet() {
  M5.Lcd.setTextColor(0xfd79);
}

void updateUI(){
  // M5.Lcd.clear();
  displayTemp();
  displayMessage();
  displayScore();
  displayFunctions();
  displayStats();
//  displayPet();
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
    if (score > 999999) {
      win();
    }
    
    delay(5000);
  }
}

void displayFinalScreen() {
}

void lose() {
  // TODO(UI ppl): show the resume button and the score 
  messageIndex = MSG_DEATH;
  currentState = STATE_DEATH;
}

void win() {
  // TODO(UI ppl): show the resume button and the score 
  messageIndex = MSG_WIN;
  currentState = STATE_WIN;
  updateUI();
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
  };
}

void reset_game() {
  score = STARTING_SCORE;
  sleep_pts = STARTING_SLEEP_PTS;
  happiness_pts = STARTING_HAPPINESS_PTS;
  hunger_pts = STARTING_HUNGER_PTS;
  messageIndex = MSG_IDLE;
  currentState = STATE_IDLE;
  updateUI();
}
  
void processEvent(void *pvParameters) {
  // Handle a button press and update the score afterwards
  // We should also pause the updatePts activity during the event
  for (;;) {
    // TODO(Binou) - Make it look fancy
    if (currentState == STATE_DEATH || currentState == STATE_WIN) {
      if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {
        reset_game();
      } 
    } else {
      // Check if a button was pressed
      if (M5.BtnA.wasPressed()) {
        sleep_pts = sleep_pts + 5;
        messageIndex = MSG_SLEEP;
        currentState = STATE_SLEEP;
        checkDeath();
        delay(1000);
      }
      
      if (M5.BtnB.wasPressed()) {
        hunger_pts = hunger_pts - 5;
        messageIndex = MSG_HUNGER;
        currentState = STATE_HUNGER;
        checkDeath();
        delay(1000);
      }
      
      if (M5.BtnC.wasPressed()) {
        happiness_pts = happiness_pts + 5;
        messageIndex = MSG_HAPPINESS;
        currentState = STATE_HAPPINESS;
        checkDeath();
        delay(1000);
      }
      
      currentState = STATE_IDLE;
      messageIndex = MSG_IDLE;
      M5.update();
    }
  }
}


  
void updatePts() {
  // Every activity's points go down by 1 every second
  if (currentState == STATE_IDLE) {
    sleep_pts = sleep_pts - (int) 1;
    happiness_pts = happiness_pts - 1;
    hunger_pts = hunger_pts + 1;
  }
  // Check whether the pet has died
  checkDeath();
}

// the setup routine runs once when M5Stack starts up
void setup(){
  // Initialize the M5Stack object
  M5.begin();
  M5.Lcd.clear();
  // Task that updates the score every 5 seconds
  xTaskCreatePinnedToCore (
      updateScore,
      "updateScore",
      4096,
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
   
}


// the loop routine runs over and over again forever
void loop() {
  if (currentState != STATE_DEATH && currentState != STATE_WIN){
    updatePts();
    updateUI();
    delay(1000);    
  }
}
