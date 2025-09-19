
#include "Arduino.h"
#include "arduino_utils.hpp"

namespace tarefa2::arduino
{
  bool is_debounced(volatile unsigned long *debounce)
  {
    const size_t delay_resistance = 125;

    if ((millis() - *debounce) <= delay_resistance)
    {
      return false;
    }

    *debounce = millis();
    return true;
  }
}
