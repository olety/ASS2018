#include <M5Stack.h>
#include <stdio.h>
#include <WiFi.h>
#include <HTTPClient.h>

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

const char* WIFI_SSID = "Tako";
const char* WIFI_PASS =  "12345678";

const char *SERVER_ADDRESS = "192.168.137.1";
const char *SERVER_REQUEST = "{\"name\":\"%s %s\",\"score\":\"%d\"}";
char send_req[200];
HTTPClient http;

void setup() {
  M5.begin();
  M5.Lcd.setBrightness(255);
  M5.Lcd.fillScreen(0xffff);


  // open serial connection to monitor the connection result
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // while the wifi is connectiong...
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.println("Connecting to WiFi..");
  }

  // successful connection
  M5.Lcd.println("Connected to the WiFi..");
  M5.Lcd.println(WiFi.localIP());

  
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
  M5.Lcd.println("Thank you for playing");
  }

void sendScore(){
  
  // Send a post request
  M5.Lcd.println("Connecting to the server");
  http.begin(SERVER_ADDRESS, 5656, "/" );
  M5.Lcd.println("successfully connect to server");
  http.addHeader("Content-Type", "application/json" );
  M5.Lcd.println("successfully added header");
  sprintf(send_req, SERVER_REQUEST, NamePart1[NameIndex[0]].c_str(), NamePart2[NameIndex[1]].c_str(), score);
  M5.Lcd.println("successfully sprintfed");
  M5.Lcd.println(send_req);
  http.POST((uint8_t*)send_req, strlen(send_req));
  M5.Lcd.println("post req sent");

}

void loop() {
  if (M5.BtnB.wasPressed())
  {
//   shuffle the name
      displayShuffleName();
      displayNameScore();
      displayThanks();
      sendScore();
      delay(10000);
      
      
    }
    M5.update();

}
