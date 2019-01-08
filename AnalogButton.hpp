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

#ifndef __ANALOG_BUTTON_H__
#define __ANALOG_BUTTON_H__

#include <functional>

namespace analog_button
{
/**
 * Represents single analog button
 */
class AnalogButton
{
public:
  typedef std::function<void(void)> Handler;

  /**
   * C-tor
   * 
   * @param a_range_begin button value range start [0, a_range_end]
   * @param a_range_end button value range end [a_range_begin, 1024]
   * @param a_click_handler button click event handler
   * @param a_long_press_handler button long press event handler
   */
  AnalogButton(uint16_t a_range_begin, uint16_t a_range_end,
              Handler a_click_handler, Handler a_long_press_handler = 0)
  : range_begin(a_range_begin), range_end(a_range_end), pressed(false),
    click_handler(a_click_handler), long_press_handler(a_long_press_handler)
  {
  }

  /**
   * Checks if provided value is in value range of this button
   * 
   * @param button value in range [0, 1024]
   * @return true if value is in range of this button, false otherwise
   */
  bool is_in_range(uint16_t value)
  {
    return value >= range_begin && value <= range_end;
  }

  /**
   * Called when button is pressed
   */
  void on_pressed()
  {
    pressed = true;
  }

  /**
   * Called when button is pressed
   */
  void on_released()
  {
    pressed = false;
    long_pressed = false;
  }

  /**
   * Returns if button is pressed
   * 
   * @return true if button is currently pressed
   */
  bool is_pressed()
  {
    return pressed;
  }

  /**
   * Returns if button has been long pressed
   * 
   * @return true if button is currently long pressed
   */
  bool is_long_pressed()
  {
    return long_pressed;
  }

  /**
   * Called when button is clicked
   */
  void on_clicked()
  {
    if (click_handler)
    {
      click_handler();
    }
  }

  /**
   * Called when button is long pressed
   */
  void on_long_pressed()
  {
    if (long_press_handler)
    {
      long_press_handler();
    }
    long_pressed = true;
  }

private:
  uint16_t range_begin;
  uint16_t range_end;
  bool pressed;
  bool long_pressed;
  Handler click_handler;
  Handler long_press_handler;
};
}
#endif
