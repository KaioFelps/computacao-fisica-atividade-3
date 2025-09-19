#pragma once

#include "digit.hpp"
#include <stdint.h>

namespace tarefa3::display
{

class DisplayDriver
{
public:
  virtual void set_digit(uint8_t pin,
                         tarefa3::core::DisplayDigit digit) const = 0;
};

} // namespace tarefa3::display
