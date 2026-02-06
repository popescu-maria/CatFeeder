#ifndef MENUBUTTONS_HPP
#define MENUBUTTONS_HPP

#include "Button.hpp"
#include "Config.hpp"

class MenuButtons {
public:
  MenuButtons()
    : upButton(UP_PIN),
      downButton(DOWN_PIN),
      enterButton(ENTER_PIN) {}

  void begin() {
    upButton.begin();
    downButton.begin();
    enterButton.begin();
  }

  void update() {
    lastEvent = InputEvent::None;
    if (upButton.update()) lastEvent = InputEvent::Up;
    else if (downButton.update()) lastEvent = InputEvent::Down;
    else if (enterButton.update()) lastEvent = InputEvent::Enter;
  }

  InputEvent getEvent() {
    InputEvent e = lastEvent;
    lastEvent = InputEvent::None;
    return e;
  }

private:
  Button upButton, downButton, enterButton;
  InputEvent lastEvent = InputEvent::None;
};
#endif