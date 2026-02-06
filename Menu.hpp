#ifndef MENU_HPP
#define MENU_HPP

#include <Arduino.h>
#include <EEPROM.h>
#include "Config.hpp"
#include "TimeEditor.hpp"

class Menu {
public:
  Menu(const char** items, uint8_t count)
    : words(items), size(count) {}
  void up() {
    index = (index + size - 1) % size;
  }
  void down() {
    index = (index + 1) % size;
  }
  void reset() {
    index = 0;
  }
  uint8_t getIndex() const {
    return index;
  }
  const char* current() const {
    return words[index];
  }
private:
  const char** words;
  uint8_t size;
  uint8_t index = 0;
};

enum class MenuState {
  MAIN,
  CHANGE_MODE,
  SETTINGS,
  EDIT_TIME_COUNT,
  EDIT_TIME,
  EDIT_INTERVAL,
  EDIT_QUANTITY
};

class MenuController {
public:
  MenuController(FeederConfig& cfg)
    : config(cfg),
      mainMenu(mainItems, 3),
      modeMenu(modeItems, 3),
      manualMenu(manualItems, 2),
      fixedMenu(fixedItems, 3),
      intervalMenu(intervalItems, 3),
      quantityMenu(quantityItems, 3) {}

  void load() {
    EEPROM.get(0, config);
    if (config.fixedCount == 0 || config.fixedCount > 5) {
      config.mode = FeedMode::Manual;
      config.fixedCount = 1;
      config.quantity = PortionSize::Small;
      ;
    }
  }

  void save() {
    EEPROM.put(0, config);
  }

  void handleEvent(InputEvent e) {
    switch (state) {

      case MenuState::MAIN:
        handleMenu(mainMenu, e);
        if (e == InputEvent::Enter) {
          if (e == InputEvent::Enter) {
            if (mainMenu.getIndex() == 0) {
              feedNowTriggered = true;
            } else if (mainMenu.getIndex() == 1) {
              modeMenu.reset();
              state = MenuState::CHANGE_MODE;
            } else if (mainMenu.getIndex() == 2) {
              activeSettings()->reset();
              state = MenuState::SETTINGS;
            }
          }
        }
        break;

      case MenuState::CHANGE_MODE:
        handleMenu(modeMenu, e);
        if (e == InputEvent::Enter) {
          config.mode = static_cast<FeedMode>(modeMenu.getIndex());
          save();
          state = MenuState::MAIN;
        }
        break;

      case MenuState::SETTINGS:
        {
          Menu* m = activeSettings();
          handleMenu(*m, e);
          if (e == InputEvent::Enter) {
            if (config.mode == FeedMode::FixedTime && m->getIndex() == 0)
              state = MenuState::EDIT_TIME_COUNT;
            else if (config.mode == FeedMode::Interval && m->getIndex() == 0) {
              intervalEditor.startEditing(config.interval);
              state = MenuState::EDIT_INTERVAL;
            } else if (strcmp(m->current(), "Change quantity") == 0) {
              state = MenuState::EDIT_QUANTITY;
            } else if (strcmp(m->current(), "Back") == 0)
              state = MenuState::MAIN;
          }
          break;
        }

      case MenuState::EDIT_TIME_COUNT:
        if (e == InputEvent::Up && config.fixedCount < 5) config.fixedCount++;
        if (e == InputEvent::Down && config.fixedCount > 1) config.fixedCount--;
        if (e == InputEvent::Enter) {
          timeIndex = 0;
          timeEditor.startEditing(config.fixedTimes[0]);
          state = MenuState::EDIT_TIME;
        }
        break;

      case MenuState::EDIT_TIME:
        if (e == InputEvent::Up) timeEditor.increment();
        if (e == InputEvent::Down) timeEditor.decrement();
        if (e == InputEvent::Enter) {
          if (timeEditor.nextField()) {
            config.fixedTimes[timeIndex++] = timeEditor.getTime();
            if (timeIndex >= config.fixedCount) {
              save();
              state = MenuState::SETTINGS;
            } else {
              timeEditor.startEditing(config.fixedTimes[timeIndex]);
            }
          }
        }
        break;

      case MenuState::EDIT_INTERVAL:
        if (e == InputEvent::Up) intervalEditor.increment();
        if (e == InputEvent::Down) intervalEditor.decrement();
        if (e == InputEvent::Enter) {
          if (intervalEditor.nextField()) {
            config.interval = intervalEditor.getInterval();
            save();
            state = MenuState::SETTINGS;
          }
        }
        break;

      case MenuState::EDIT_QUANTITY:
        handleMenu(quantityMenu, e);
        if (e == InputEvent::Enter) {
          config.quantity = static_cast<PortionSize>(quantityMenu.getIndex());
          save();
          state = MenuState::MAIN;
        }
        break;

      default:
        break;
    }
  }

  const char* getTitle() const {
    switch (state) {
      case MenuState::MAIN:
        switch (config.mode) {
          case FeedMode::Manual: return "Mode Manual";
          case FeedMode::FixedTime: return "Mode Fixed Times";
          case FeedMode::Interval: return "Mode Interval";
          default: return "";
        }
      case MenuState::CHANGE_MODE: return "Change Mode";
      case MenuState::SETTINGS: return "Settings";
      case MenuState::EDIT_TIME_COUNT: return "How many times?";
      case MenuState::EDIT_TIME: return "Set Time";
      case MenuState::EDIT_INTERVAL: return "Set Interval";
      case MenuState::EDIT_QUANTITY: return "Choose portion:";
      default: return "";
    }
  }

  const char* getCurrentText() const {
    switch (state) {
      case MenuState::MAIN: return mainMenu.current();
      case MenuState::CHANGE_MODE: return modeMenu.current();
      case MenuState::SETTINGS: return activeSettings()->current();
      case MenuState::EDIT_QUANTITY: return quantityMenu.current();
      default: return "";
    }
  }

  bool shouldFeedNow() {
    if (feedNowTriggered == true) {
      bool result = feedNowTriggered;
      feedNowTriggered = false;
      return result;
    }
    return false;
  }

  TimeHMSS currentEditTime() const {
    return timeEditor.getTime();
  }
  IntervalTime currentEditInterval() const {
    return intervalEditor.getInterval();
  }
  MenuState getState() const {
    return state;
  }
  TimeField getTimeField() const {
    return timeEditor.getField();
  }
  IntervalField getIntervalField() const {
    return intervalEditor.getField();
  }
  uint8_t getFixedCount() const {
    return config.fixedCount;
  }
  PortionSize getQuantity() const {
    return config.quantity;
  }

private:
  FeederConfig& config;
  MenuState state = MenuState::MAIN;
  uint8_t timeIndex = 0;

  TimeEditor timeEditor;
  IntervalEditor intervalEditor;

  const char* mainItems[3] = { "Feed now", "Change mode", "Settings" };
  const char* modeItems[3] = { "Manual", "Fixed times", "Interval" };
  const char* manualItems[2] = { "Change quantity", "Back" };
  const char* fixedItems[3] = { "Change time", "Change quantity", "Back" };
  const char* intervalItems[3] = { "Set interval", "Change quantity", "Back" };
  const char* quantityItems[3] = { "Small(20g)", "Medium(40g)", "Big(60g)" };

  Menu mainMenu;
  Menu modeMenu;
  Menu manualMenu;
  Menu fixedMenu;
  Menu intervalMenu;
  Menu quantityMenu;

  bool feedNowTriggered = false;

  Menu* activeSettings() {
    if (config.mode == FeedMode::Manual) return &manualMenu;
    if (config.mode == FeedMode::FixedTime) return &fixedMenu;
    return &intervalMenu;
  }

  void handleMenu(Menu& m, InputEvent e) {
    if (e == InputEvent::Up) m.up();
    if (e == InputEvent::Down) m.down();
  }
};

#endif