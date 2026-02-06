#include <Arduino.h>
#include <LiquidCrystal.h>
#include "Config.hpp"
#include "FeederController.hpp"

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
FeederController controller(lcd);

void setup() {
  Serial.begin(9600);
  controller.setup();
}

void loop() {
  controller.update();
}
