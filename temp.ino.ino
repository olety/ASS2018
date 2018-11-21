#include <M5Stack.h>
#include "utility/DHT12.h"
#include <Wire.h>

DHT12 dht12;
int t;
int old_t = -1000;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  t = dht12.readTemperature();
  if (t != old_t)
  {
    if (t > 27)
    {
      M5.Lcd.print("Temperature is %d. It's too high", t);
    }
    else if 
    {
      M5.Lcd.print("Temperature is %d. It's too low", t);
    }
    old_t = t;
    M5.update();
  }
}
