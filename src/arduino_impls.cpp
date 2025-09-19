#include "Arduino.h"
#include "arduino_impls.hpp"

bool tarefa3::arduino::ArduinoRotaryEncoderPinManager::get_rotary_output_digital_level(uint8_t output_pin) const
{
  const uint8_t output = PINB & (1 << output_pin);
  return output != 0;
}
