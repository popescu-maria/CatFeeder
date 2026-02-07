#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>

const uint8_t RS = 9, EN = 8, D4 = 7, D5 = 6, D6 = 5, D7 = 4;
const uint8_t NUM_COLS = 16;
const uint8_t NUM_ROWS = 2;

const uint8_t BUZZER_PIN = 11;
const uint8_t SERVO_PIN = 10;

const uint8_t UP_PIN = 2;
const uint8_t DOWN_PIN = 3;
const uint8_t ENTER_PIN = 12;

enum class InputEvent { None,
                        Up,
                        Down,
                        Enter };

enum class Meridiem : uint8_t {
  AM,
  PM
};

struct TimeHMSS {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  Meridiem meridiem;
};

struct IntervalTime {
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
};

enum class FeedMode : uint8_t {
  Manual,
  FixedTime,
  Interval
};

enum class PortionSize : uint8_t {
  Small = 0,   // 20g
  Medium = 1,  // 40g
  Large = 2    // 60g
};


struct FeederConfig {
  FeedMode mode;
  uint8_t fixedCount;
  TimeHMSS fixedTimes[5];
  IntervalTime interval;
  PortionSize quantity;
  unsigned long lastFeedingTime;
};



#endif
