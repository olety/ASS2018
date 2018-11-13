#include <M5Stack.h>
#include <WiFi.h>
const char* ssid = "GIGA-029-2.4G";
const char* password =  "77077018";


void setup() {
  m5.begin();


  // open serial connection to monitor the connection result
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // while the wifi is connectiong...
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.println("Connecting to WiFi..");
  }

  // successful connection
  M5.Lcd.println("Connected to the WiFi..");
  M5.Lcd.println(WiFi.localIP());



}

void loop() {
  // put your main code here, to run repeatedly:

}
