/**************************************************************
  Copyright (c) 2019 Mobsome
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
**************************************************************/

#ifndef __ANALOG_BUTTONS_H__
#define __ANALOG_BUTTONS_H__

#include "AnalogButton.hpp"
#include <vector>
#include <memory>

namespace analog_button
{
static const uint32_t SAMPLING_INTERVAL = 20;

/**
 * Represents group of buttons connected to single analog input
 */
class AnalogButtons
{
public:

  /**
   * C-tor
   * 
   * @param a_pin input pin
   * @param a_debounce button debounce value
   */
  AnalogButtons(uint8_t a_pin, uint16_t a_debounce = 20, uint16_t a_long_press_duration = 1000)
  : pin(a_pin), debounce(a_debounce), long_press_duration(a_long_press_duration), 
    last_check(0), press_start(0)
  {
    
  }

  void add_button(std::shared_ptr<AnalogButton> a_button)
  {
    buttons.push_back(a_button);
  }

  /**
   * Updates buttons state
   */
  void check()
  {
    if ((millis() - last_check) < SAMPLING_INTERVAL)
    {
      return;
    }

    last_check = millis();
    uint16_t value = analogRead(pin);

    for (BUTTONS_LIST::iterator iter = buttons.begin(); iter != buttons.end(); iter++)
    {
      std::shared_ptr<AnalogButton> button = *iter;
      if (is_debouncing(button))
      {
        // if debouncing then skip
        return;
      }
      
      if (button->is_in_range(value))
      {
        if (!button->is_pressed())
        {
          press_start = millis();
          button->on_pressed();
        }
        else if (!button->is_long_pressed() && (millis() - press_start) >= long_press_duration)
        {
          button->on_long_pressed();
        }
      }
      else if (button->is_pressed())
      {
        button->on_released();
        if ((millis() - press_start) < long_press_duration)
        {
          button->on_clicked();
        }
        press_start = 0;
      }
    }
  }

private:
  bool is_debouncing(std::shared_ptr<AnalogButton> button)
  {
    return button->is_pressed() && (millis() - press_start) < debounce;
  }

  typedef std::vector<std::shared_ptr<AnalogButton>> BUTTONS_LIST;
  uint8_t pin;
  uint16_t debounce;
  uint32_t long_press_duration;
  uint32_t last_check;
  uint32_t press_start;
  BUTTONS_LIST buttons;
};
}
#endif
