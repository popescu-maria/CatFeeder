#ifndef FEEDERCONTROLLER_HPP
#define FEEDERCONTROLLER_HPP

#include <Servo.h>
#include "LCDRenderer.hpp"
#include "MenuButtons.hpp"
#include "Menu.hpp"
#include "Config.hpp"
#include "RTC.hpp"
#include "Buzzer.hpp"

class FeederController {
public:
  FeederController(LiquidCrystal& lcd)
    : renderer(lcd),
      menu(config) {}

  void setup() {
    rtc.begin();
    renderer.setup();

    servo.attach(SERVO_PIN);
    angle = CLOSED_ANGLE;
    servo.write(angle);

    buttons.begin();
    menu.load();
    renderer.render(menu);
  }

  void update() {
    updateServo();
    updateFeedingTiming();
    buttons.update();
    buzzer.handleBuzzer();

    InputEvent e = buttons.getEvent();

    checkFeedingSchedule();

    if (e != InputEvent::None) {
      menu.handleEvent(e);
      if (menu.shouldFeedNow()) {
        triggerFeeding();
      }
      needsRender = true;
    }

    if (isEditingState()) {
      unsigned long now = millis();
      if (now - lastRenderTime >= RENDER_INTERVAL) {
        lastRenderTime = now;
        needsRender = true;
      }
    }

    if (needsRender) {
      renderer.render(menu);
      needsRender = false;
    }
  }

private:
  FeederConfig config{};
  LCDRenderer renderer;
  RTC rtc;
  MenuButtons buttons;
  MenuController menu;

  Buzzer buzzer;

  enum class DoorState {
    Idle,
    Opening,
    Closing
  };
  DoorState doorState = DoorState::Idle;

  unsigned long lastServoUpdate = 0;
  static constexpr unsigned long SERVO_STEP_INTERVAL = 5;

  Servo servo;
  int angle = 0;
  const int OPEN_ANGLE = 60;
  const int CLOSED_ANGLE = 0;

  unsigned long lastRenderTime = 0;
  bool needsRender = true;
  static constexpr unsigned long RENDER_INTERVAL = 400;

  uint8_t lastFeedSecond = 255;

  static constexpr unsigned long SMALL_PORTION_MS = 400;
  static constexpr unsigned long MEDIUM_PORTION_MS = 600;
  static constexpr unsigned long LARGE_PORTION_MS = 1000;

  unsigned long feedStartTime = 0;
  unsigned long openDuration = 0;
  bool feedingActive = false;

  unsigned long getOpenDuration() const {
    switch (config.quantity) {
      case PortionSize::Small: return SMALL_PORTION_MS;
      case PortionSize::Medium: return MEDIUM_PORTION_MS;
      case PortionSize::Large: return LARGE_PORTION_MS;
      default: return SMALL_PORTION_MS;
    }
  }


  void checkFeedingSchedule() {
    if (config.mode == FeedMode::FixedTime) {
      TimeHMSS currentTime = rtc.getCurrentTime();

      for (uint8_t i = 0; i < config.fixedCount; i++) {
        if (rtc.isTimeMatch(config.fixedTimes[i])) {
          if (currentTime.second != lastFeedSecond) {
            triggerFeeding();
            lastFeedSecond = currentTime.second;
          }
        }
      }
    } else if (config.mode == FeedMode::Interval) {
      if (config.lastFeedingTime == 0) {
        config.lastFeedingTime = rtc.getUnixTime();
        menu.save();
        triggerFeeding();
      } else if (rtc.hasIntervalElapsed(config.lastFeedingTime, config.interval)) {
        config.lastFeedingTime = rtc.getUnixTime();
        menu.save();
        triggerFeeding();
      }
    }
  }

  void triggerFeeding() {
    openDuration = getOpenDuration();
    feedStartTime = millis();
    feedingActive = true;

    startOpeningDoor();
    buzzer.playAlarm();
    Serial.println("Feeding now!");
  }

  void startOpeningDoor() {
    doorState = DoorState::Opening;
  }

  void startClosingDoor() {
    doorState = DoorState::Closing;
  }

  void updateServo() {
    if (doorState == DoorState::Idle) return;

    unsigned long now = millis();
    if (now - lastServoUpdate < SERVO_STEP_INTERVAL) return;
    lastServoUpdate = now;

    if (doorState == DoorState::Opening) {
      if (angle < OPEN_ANGLE) {
        angle++;
        servo.write(angle);
      } else {
        doorState = DoorState::Idle;
      }
    } else if (doorState == DoorState::Closing) {
      if (angle > CLOSED_ANGLE) {
        angle--;
        servo.write(angle);
      } else {
        doorState = DoorState::Idle;
      }
    }
  }

  void updateFeedingTiming() {
    if (!feedingActive) return;

    unsigned long now = millis();

    if (now - feedStartTime >= openDuration) {
      feedingActive = false;
      startClosingDoor();
    }
  }

  bool isEditingState() const {
    MenuState state = menu.getState();
    return state == MenuState::EDIT_TIME || state == MenuState::EDIT_INTERVAL || state == MenuState::EDIT_TIME_COUNT;
  }
};


#endif
