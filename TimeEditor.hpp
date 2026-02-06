#ifndef TIMEEDITOR_HPP
#define TIMEEDITOR_HPP

#include "Config.hpp"

enum class TimeField {
  Hour,
  Minute,
  Second,
  Meridiem
};

enum class IntervalField {
  Hour,
  Minute,
  Second
};

class TimeEditor {
public:
  void startEditing(TimeHMSS time) {
    editTime = time;
    field = TimeField::Hour;
  }

  void increment() {
    switch (field) {
      case TimeField::Hour:
        editTime.hour = editTime.hour % 12 + 1;
        break;
      case TimeField::Minute:
        editTime.minute = (editTime.minute + 1) % 60;
        break;
      case TimeField::Second:
        editTime.second = (editTime.second + 1) % 60;
        break;
      case TimeField::Meridiem:
        editTime.meridiem = editTime.meridiem == Meridiem::AM ? Meridiem::PM : Meridiem::AM;
        break;
    }
  }

  void decrement() {
    switch (field) {
      case TimeField::Hour:
        editTime.hour = editTime.hour == 1 ? 12 : editTime.hour - 1;
        break;
      case TimeField::Minute:
        editTime.minute = editTime.minute == 0 ? 59 : editTime.minute - 1;
        break;
      case TimeField::Second:
        editTime.second = editTime.second == 0 ? 59 : editTime.second - 1;
        break;
      case TimeField::Meridiem:
        editTime.meridiem = editTime.meridiem == Meridiem::AM ? Meridiem::PM : Meridiem::AM;
        break;
    }
  }

  bool nextField() {
    if (field == TimeField::Meridiem) {
      return true;
    }
    field = static_cast<TimeField>(static_cast<uint8_t>(field) + 1);
    return false;
  }

  TimeHMSS getTime() const {
    return editTime;
  }
  TimeField getField() const {
    return field;
  }

private:
  TimeHMSS editTime{};
  TimeField field = TimeField::Hour;
};

class IntervalEditor {
public:
  void startEditing(IntervalTime interval) {
    editInterval = interval;
    field = IntervalField::Hour;
  }

  void increment() {
    switch (field) {
      case IntervalField::Hour:
        editInterval.hours = (editInterval.hours + 1) % 24;
        break;
      case IntervalField::Minute:
        editInterval.minutes = (editInterval.minutes + 1) % 60;
        break;
      case IntervalField::Second:
        editInterval.seconds = (editInterval.seconds + 1) % 60;
        break;
    }
  }

  void decrement() {
    switch (field) {
      case IntervalField::Hour:
        editInterval.hours = editInterval.hours == 0 ? 23 : editInterval.hours - 1;
        break;
      case IntervalField::Minute:
        editInterval.minutes = editInterval.minutes == 0 ? 59 : editInterval.minutes - 1;
        break;
      case IntervalField::Second:
        editInterval.seconds = editInterval.seconds == 0 ? 59 : editInterval.seconds - 1;
        break;
    }
  }

  bool nextField() {
    if (field == IntervalField::Second) {
      return true;
    }
    field = static_cast<IntervalField>(static_cast<uint8_t>(field) + 1);
    return false;
  }

  IntervalTime getInterval() const {
    return editInterval;
  }
  IntervalField getField() const {
    return field;
  }

private:
  IntervalTime editInterval{};
  IntervalField field = IntervalField::Hour;
};

#endif