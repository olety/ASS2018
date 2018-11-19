#include <HTTPClient.h>
#include <M5Stack.h>
#include <WiFi.h>

const char* WIFI_SSID = "GIGA-029-2.4G";
const char* WIFI_PASS =  "77077018";
const char *SERVER_ADDRESS = "enterthewebsiteaddress.com";
const char *SERVER_REQUEST = "{\"username\":\"777SLAYER777\",\"score\":\"100\"}";

void setup() {
  m5.begin();


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

  // Send a post request
  M5.Lcd.println("Connecting to the server");
  http.begin(SERVER_ADDRESS, 443, "/services/XXXX/XXXX/XXXX" );
  http.addHeader("Content-Type", "application/json" );
  http.POST((uint8_t*)SERVER_REQUEST, strlen(SERVER_REQUEST));
  M5.Lcd.println("post req sent");


}

void loop() {
  // put your main code here, to run repeatedly:

}
