#pragma once

#include "display.hpp"
#include "rotary_encoder.hpp"
#include <stdint.h>

namespace tarefa3::arduino
{
class ArduinoRotaryEncoderPinManager
    : public rotary_encoder::RotaryEncoderPinsManager
{
public:
  bool get_rotary_output_digital_level(uint8_t output_pin) const final;
};

class ArduinoSevenSegmentsDisplayDriver : public display::DisplayDriver
{
private:
  ArduinoSevenSegmentsDisplayDriver() = default;

  volatile uint8_t *segments_board;
  volatile uint8_t *digits_board;
  uint8_t segment_a_pin;
  uint8_t segment_b_pin;
  uint8_t segment_c_pin;
  uint8_t segment_d_pin;
  uint8_t segment_e_pin;
  uint8_t segment_f_pin;
  uint8_t segment_g_pin;

  void set_segments_board_state(uint8_t state) const;
  void set_digits_board_state(uint8_t state) const;

public:
  ArduinoSevenSegmentsDisplayDriver(
      volatile uint8_t *digits_board, volatile uint8_t *segments_board,
      uint8_t segment_a_pin, uint8_t segment_b_pin, uint8_t segment_c_pin,
      uint8_t segment_d_pin, uint8_t segment_e_pin, uint8_t segment_f_pin,
      uint8_t segment_g_pin);

  void set_digit(uint8_t pin, core::DisplayDigit digit) const final;
  void turn_leds_off() const;
};
} // namespace tarefa3::arduino
