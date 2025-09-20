#include "arduino_impls.hpp"
#include "Arduino.h"

namespace tarefa3::arduino
{
////////////////////////////////////////////////
// `ArduinoRotaryEncoderPinManager`
////////////////////////////////////////////////
bool ArduinoRotaryEncoderPinManager::get_rotary_output_digital_level(
    uint8_t output_pin) const
{
  const uint8_t output = PINB & (1 << output_pin);
  return output != 0;
}

////////////////////////////////////////////////
// `ArduinoSevenSegmentsDisplayDriver`
////////////////////////////////////////////////
ArduinoSevenSegmentsDisplayDriver::ArduinoSevenSegmentsDisplayDriver(
    Pins pins_, Boards boards_)
    : pins(pins_), boards(boards_)
{
}

void ArduinoSevenSegmentsDisplayDriver::set_digit(
    display::DisplayDriver::DigitLed led, core::DisplayDigit digit) const
{
  using core::DisplayDigit;
  using display::DisplayDriver;

  // forma o digito nos segmentos de um dígito de 7 segmentos
  // mapa dos segmentos:
  // A - top
  // B - top right
  // C - bottom right
  // D - bottom
  // E - bottom left
  // F - top left
  // G - middle
  //
  // observe que para ativar um segmento é necessário colocá-lo em `LOW`,
  // pois o display está conectado no ânodo ou alguma coisa assim
  // veja: https://docs.wokwi.com/parts/wokwi-7segment
  uint8_t new_state;
  switch (digit)
  {
  case (DisplayDigit::ZERO):
    new_state = (1 << this->pins.segment_g);
    break;

  case (DisplayDigit::ONE):
    new_state = ~((1 << this->pins.segment_b) | (1 << this->pins.segment_c));
    break;

  case (DisplayDigit::TWO):
    new_state = (1 << this->pins.segment_f) | (1 << this->pins.segment_c);
    break;

  case (DisplayDigit::THREE):
    new_state = (1 << this->pins.segment_f) | (1 << this->pins.segment_e);
    break;

  case (DisplayDigit::FOUR):
    new_state = (1 << this->pins.segment_a) | (1 << this->pins.segment_e) |
                (1 << this->pins.segment_d);
    break;

  case (DisplayDigit::FIVE):
    new_state = (1 << this->pins.segment_b) | (1 << this->pins.segment_e);
    break;

  case (DisplayDigit::SIX):
    new_state = 1 << this->pins.segment_b;
    break;

  case (DisplayDigit::SEVEN):
    new_state = ~((1 << this->pins.segment_a) | (1 << this->pins.segment_b) |
                  (1 << this->pins.segment_c));
    break;

  case (DisplayDigit::EIGHT):
    new_state = 0;
    break;

  case (DisplayDigit::NINE):
    new_state = 1 << this->pins.segment_e;
    break;
  }

  uint8_t pin;
  switch (led)
  {
  case DisplayDriver::DigitLed::DIG1:
    pin = this->pins.digit_1;
    break;

  case DisplayDriver::DigitLed::DIG2:
    pin = this->pins.digit_2;
    break;

  case DisplayDriver::DigitLed::DIG3:
    pin = this->pins.digit_3;
    break;

  case DisplayDriver::DigitLed::DIG4:
    pin = this->pins.digit_4;
    break;
  }

  this->set_segments_board_state(new_state);
  this->set_digits_board_state(1 << pin);
}

void ArduinoSevenSegmentsDisplayDriver::turn_leds_off() const
{
  // desliga os 4 dígitos e todos os segmentos
  this->set_segments_board_state(0xFF);
  this->set_digits_board_state(0x00);
}

void ArduinoSevenSegmentsDisplayDriver::set_segments_board_state(
    uint8_t state) const
{
  *(this->boards.segments) = state;
}

void ArduinoSevenSegmentsDisplayDriver::set_digits_board_state(
    uint8_t state) const
{
  *(this->boards.digits) = state;
}

void ArduinoSevenSegmentsDisplayDriver::turn_colon_on() const
{
  *(this->boards.colon) |= (1 << this->pins.colon);
}

void ArduinoSevenSegmentsDisplayDriver::turn_colon_off() const
{
  *(this->boards.colon) &= ~(1 << this->pins.colon);
}

void ArduinoSevenSegmentsDisplayDriver::toggle_colon() const
{
  const auto colon_is_on = (*this->boards.colon) & (1 << this->pins.colon);
  if (colon_is_on)
  {
    this->turn_colon_off();
  }
  else
  {
    this->turn_colon_on();
  }
}

} // namespace tarefa3::arduino
