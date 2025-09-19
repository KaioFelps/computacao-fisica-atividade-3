#pragma once

#include <stdint.h>
#include "rotary_encoder.hpp"

namespace tarefa3::arduino
{
  class ArduinoRotaryEncoderPinManager : public rotary_encoder::RotaryEncoderPinsManager
  {
  public:
    bool get_rotary_output_digital_level(uint8_t output_pin) const final;
  };
}
