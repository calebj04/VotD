#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Arduino.h>
#include <WiFi.h>
#include "api.h"
#include "clock.h"
#include "secrets.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  Wire.begin(21, 22);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // begin serial communication and connect to wifi
  Serial.begin(115200);
  Serial.print("Connecting to ");
  // Serial.println(ssid);

  // WiFi.begin(ssid, password);

  WiFi.begin("Wokwi-GUEST", "", 6);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  // set the clock to est and get the current time
  struct tm currentTime = setClock();

  display.setCursor(0, 0);
  display.println(getVerse(currentTime.tm_yday + 1));

  display.display();

  //

  // sleep until 6:30am
  // UNCOMMENT
  // esp_sleep_enable_timer_wakeup(sleepTime(currentTime) * 1000000ULL);
  // esp_deep_sleep_start();
}

void loop()
{
}