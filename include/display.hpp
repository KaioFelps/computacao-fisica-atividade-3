#pragma once

#include "digit.hpp"
#include <stdint.h>

namespace tarefa3::display
{

class DisplayDriver
{
public:
  enum DigitLed
  {
    DIG1,
    DIG2,
    DIG3,
    DIG4
  };

  virtual void set_digit(DigitLed pin,
                         tarefa3::core::DisplayDigit digit) const = 0;
  virtual void turn_colon_on() const = 0;
  virtual void turn_colon_off() const = 0;
};

} // namespace tarefa3::display
