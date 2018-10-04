#include <M5Stack.h>
#include <Wire.h>
#include "DHT12.h"

DHT12 dht12;
int t;
int old_t = -1000;


void setup() {
    M5.begin();
    Wire.begin();
    M5.Lcd.setTextSize(10);
    M5.Lcd.setTextColor(TFT_YELLOW);
}

void loop() {
    t = dht12.readTemperature();
    if (t != old_t)
    {
      if (t > 27)
      {
        M5.Lcd.setTextColor(0xe8e4);
      }
      else
      {
        M5.Lcd.setTextColor(0x2589);
      }
      M5.Lcd.fillScreen(RED);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.print(t);
    }
    old_t = t;
    M5.update();
}
