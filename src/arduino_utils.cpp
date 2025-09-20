#include "arduino_utils.hpp"
#include "Arduino.h"

namespace tarefa3::arduino
{
bool is_debounced(volatile unsigned long *debounce)
{
  const size_t delay_resistance = 25;

  if ((millis() - *debounce) <= delay_resistance)
  {
    return false;
  }

  *debounce = millis();
  return true;
}
} // namespace tarefa3::arduino
