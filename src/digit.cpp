#include "digit.hpp"
#include <assert.h>

namespace tarefa3::core
{
DisplayDigit DisplayDigitHelper::from_uint8(uint8_t digit)
{
  // é pra lançar um erro se tentar converter um dígito que é maior que 9
  assert(digit <= 9);
  switch (digit)
  {
  case (0):
    return DisplayDigit::ZERO;
  case (1):
    return DisplayDigit::ONE;
  case (2):
    return DisplayDigit::TWO;
  case (3):
    return DisplayDigit::THREE;
  case (4):
    return DisplayDigit::FOUR;
  case (5):
    return DisplayDigit::FIVE;
  case (6):
    return DisplayDigit::SIX;
  case (7):
    return DisplayDigit::SEVEN;
  case (8):
    return DisplayDigit::EIGHT;
  case (9):
    return DisplayDigit::NINE;
  }
}

uint8_t DisplayDigitHelper::to_uint8(DisplayDigit digit)
{
  switch (digit)
  {
  case (DisplayDigit::ZERO):
    return 0;
  case (DisplayDigit::ONE):
    return 1;
  case (DisplayDigit::TWO):
    return 2;
  case (DisplayDigit::THREE):
    return 3;
  case (DisplayDigit::FOUR):
    return 4;
  case (DisplayDigit::FIVE):
    return 5;
  case (DisplayDigit::SIX):
    return 6;
  case (DisplayDigit::SEVEN):
    return 7;
  case (DisplayDigit::EIGHT):
    return 8;
  case (DisplayDigit::NINE):
    return 9;
  }
}
} // namespace tarefa3::core
