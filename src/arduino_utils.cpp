#include "arduino_utils.hpp"
#include "Arduino.h"

namespace tarefa3::arduino
{
bool is_debounced(unsigned long *debounce)
{
  const size_t delay_resistance = 25;
  return is_debounced(debounce, delay_resistance);
}

bool is_debounced(unsigned long *debounce, const unsigned int rate_limit_ms)
{
  if ((millis() - *debounce) <= rate_limit_ms)
  {
    return false;
  }

  *debounce = millis();
  return true;
}
} // namespace tarefa3::arduino
