#ifndef RTC_HPP
#define RTC_HPP

#include <Arduino.h>
#include <DS3231.h>
#include "Config.hpp"

class RTC {
public:
  RTC()
    : rtc(SDA, SCL) {}

  void begin() {
    rtc.begin();
  }

  void setTime(uint8_t hour, uint8_t minute, uint8_t second) {
    rtc.setTime(hour, minute, second);
  }

  void setDate(uint8_t day, uint8_t month, uint16_t year) {
    rtc.setDate(day, month, year);
  }

  TimeHMSS getCurrentTime() {
    Time t = rtc.getTime();
    TimeHMSS result;

    if (t.hour == 0) {
      result.hour = 12;
      result.meridiem = Meridiem::AM;
    } else if (t.hour < 12) {
      result.hour = t.hour;
      result.meridiem = Meridiem::AM;
    } else if (t.hour == 12) {
      result.hour = 12;
      result.meridiem = Meridiem::PM;
    } else {
      result.hour = t.hour - 12;
      result.meridiem = Meridiem::PM;
    }

    result.minute = t.min;
    result.second = t.sec;

    return result;
  }

  Time getRawTime() {
    return rtc.getTime();
  }

  bool isTimeMatch(const TimeHMSS& target) {
    TimeHMSS now = getCurrentTime();
    return now.hour == target.hour && 
    now.minute == target.minute && 
    now.second == target.second && 
    now.meridiem == target.meridiem;
  }

  unsigned long getSecondsSinceMidnight() {
    Time t = rtc.getTime();
    return (unsigned long)t.hour * 3600 + (unsigned long)t.min * 60 + (unsigned long)t.sec;
  }

  unsigned long getUnixTime() {
    Time t = rtc.getTime();
    return rtc.getUnixTime(t);
  }

  bool hasIntervalElapsed(unsigned long lastTime, const IntervalTime& interval) {
    unsigned long currentTime = getUnixTime();
    unsigned long intervalSeconds = (unsigned long)interval.hours * 3600 + (unsigned long)interval.minutes * 60 + (unsigned long)interval.seconds;

    return (currentTime - lastTime) >= intervalSeconds;
  }

private:
  DS3231 rtc;
};

#endif