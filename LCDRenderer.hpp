#ifndef LCDRENDERER_HPP
#define LCDRENDERER_HPP

#include <LiquidCrystal.h>
#include "Config.hpp"
#include "Menu.hpp"

class LCDRenderer {
public:
  LCDRenderer(LiquidCrystal& lcdRef)
    : lcd(lcdRef) {}

  void setup() {
    lcd.begin(NUM_COLS, NUM_ROWS);
    lcd.noCursor();
    lcd.noBlink();
  }

  void render(const MenuController& menu) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(menu.getTitle());

    switch (menu.getState()) {
      case MenuState::EDIT_TIME:
        renderTimeEditor(menu);
        break;

      case MenuState::EDIT_INTERVAL:
        renderIntervalEditor(menu);
        break;

      case MenuState::EDIT_TIME_COUNT:
        renderTimeCount(menu);
        break;

      default:
        renderMenu(menu);
        break;
    }

    blinkState = !blinkState;
  }

private:
  LiquidCrystal& lcd;
  bool blinkState = false;

  void renderMenu(const MenuController& menu) {
    lcd.setCursor(0, 1);
    lcd.print(">");
    lcd.print(menu.getCurrentText());
  }

  void renderTimeCount(const MenuController& menu) {
    lcd.setCursor(0, 1);
    lcd.print("Total: ");
    lcd.print(menu.getFixedCount());
  }

  void renderTimeEditor(const MenuController& menu) {
    TimeHMSS t = menu.currentEditTime();
    TimeField field = menu.getTimeField();

    lcd.setCursor(0, 1);

    if (field == TimeField::Hour && !blinkState) {
      lcd.print("  ");
    } else {
      print2(t.hour);
    }
    lcd.print(":");

    if (field == TimeField::Minute && !blinkState) {
      lcd.print("  ");
    } else {
      print2(t.minute);
    }
    lcd.print(":");

    if (field == TimeField::Second && !blinkState) {
      lcd.print("  ");
    } else {
      print2(t.second);
    }
    lcd.print(" ");

    if (field == TimeField::Meridiem && !blinkState) {
      lcd.print("  ");
    } else {
      lcd.print(t.meridiem == Meridiem::AM ? "AM" : "PM");
    }
  }

  void renderIntervalEditor(const MenuController& menu) {
    IntervalTime i = menu.currentEditInterval();
    IntervalField field = menu.getIntervalField();

    lcd.setCursor(0, 1);

    if (field == IntervalField::Hour && !blinkState) {
      lcd.print("  ");
    } else {
      print2(i.hours);
    }
    lcd.print(":");

    if (field == IntervalField::Minute && !blinkState) {
      lcd.print("  ");
    } else {
      print2(i.minutes);
    }
    lcd.print(":");

    if (field == IntervalField::Second && !blinkState) {
      lcd.print("  ");
    } else {
      print2(i.seconds);
    }
  }

  void print2(uint8_t v) {
    if (v < 10) lcd.print("0");
    lcd.print(v);
  }
};

#endif