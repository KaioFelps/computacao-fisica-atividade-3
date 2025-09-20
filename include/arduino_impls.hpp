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
public:
  typedef struct
  {
    uint8_t segment_a;
    uint8_t segment_b;
    uint8_t segment_c;
    uint8_t segment_d;
    uint8_t segment_e;
    uint8_t segment_f;
    uint8_t segment_g;
    uint8_t digit_1;
    uint8_t digit_2;
    uint8_t digit_3;
    uint8_t digit_4;
    uint8_t colon;
  } Pins;

  typedef struct
  {
    volatile uint8_t *digits;
    volatile uint8_t *segments;
    volatile uint8_t *colon;
  } Boards;

  ArduinoSevenSegmentsDisplayDriver(Pins pins, Boards boards);

  void set_digit(display::DisplayDriver::DigitLed led,
                 core::DisplayDigit digit) const final;

  void turn_colon_on() const final;
  void turn_colon_off() const final;
  void toggle_colon() const final;

  void turn_leds_off() const;

private:
  ArduinoSevenSegmentsDisplayDriver() = default;

  Boards boards;
  Pins pins;

  void set_segments_board_state(uint8_t state) const;
  void set_digits_board_state(uint8_t state) const;
};
} // namespace tarefa3::arduino
