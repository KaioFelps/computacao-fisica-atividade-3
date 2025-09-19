#pragma once

#include <stdint.h>

namespace tarefa3::core
{

enum class DisplayDigit
{
  ZERO,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
};

class DisplayDigitHelper
{
public:
  typedef struct
  {
    DisplayDigit next_digit;
    bool had_overflow;
  } DigitStep;

  static DisplayDigit from_uint8(uint8_t digit);
  static uint8_t to_uint8(DisplayDigit digit);
};
} // namespace tarefa3::core
