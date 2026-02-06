#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>

class Button {
public:
  Button(uint8_t pin)
    : buttonPin(pin) {}

  void begin() {
    pinMode(buttonPin, INPUT_PULLUP);
  }

  bool update() {
    int reading = digitalRead(buttonPin);

    if (reading != lastReading) {
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
      if (reading == LOW && lastStableState == HIGH) {
        lastStableState = LOW;
        lastReading = reading;
        return true;
      }
      lastStableState = reading;
    }

    lastReading = reading;
    return false;
  }

private:
  uint8_t buttonPin;

  int lastReading = HIGH;
  int lastStableState = HIGH;
  unsigned long lastDebounceTime = 0;

  static constexpr unsigned long DEBOUNCE_DELAY = 50;
};

#endif
