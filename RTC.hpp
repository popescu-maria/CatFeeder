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

  bool isTimeMatch(const TimeHMSS& target) {
    TimeHMSS now = getCurrentTime();
    return now.hour == target.hour && 
    now.minute == target.minute && 
    now.second == target.second && 
    now.meridiem == target.meridiem;
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
