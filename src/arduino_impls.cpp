#include "arduino_impls.hpp"
#include "Arduino.h"

namespace tarefa3::arduino
{
bool ArduinoRotaryEncoderPinManager::get_rotary_output_digital_level(
    uint8_t output_pin) const
{
  const uint8_t output = PINB & (1 << output_pin);
  return output != 0;
}

ArduinoSevenSegmentsDisplayDriver::ArduinoSevenSegmentsDisplayDriver(
    volatile uint8_t *digits_board_, volatile uint8_t *segments_board_,
    uint8_t segment_a_pin_, uint8_t segment_b_pin_, uint8_t segment_c_pin_,
    uint8_t segment_d_pin_, uint8_t segment_e_pin_, uint8_t segment_f_pin_,
    uint8_t segment_g_pin_)
    : digits_board(digits_board_), segments_board(segments_board_),
      segment_a_pin(segment_a_pin_), segment_b_pin(segment_b_pin_),
      segment_c_pin(segment_c_pin_), segment_d_pin(segment_d_pin_),
      segment_e_pin(segment_e_pin_), segment_f_pin(segment_f_pin_),
      segment_g_pin(segment_g_pin_)
{
}

void ArduinoSevenSegmentsDisplayDriver::set_digit(
    uint8_t pin, core::DisplayDigit digit) const
{
  using tarefa3::core::DisplayDigit;

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
    new_state = (1 << this->segment_g_pin);
    break;

  case (DisplayDigit::ONE):
    new_state = ~((1 << this->segment_b_pin) | (1 << this->segment_c_pin));
    break;

  case (DisplayDigit::TWO):
    new_state = (1 << this->segment_f_pin) | (1 << this->segment_c_pin);
    break;

  case (DisplayDigit::THREE):
    new_state = (1 << this->segment_f_pin) | (1 << this->segment_e_pin);
    break;

  case (DisplayDigit::FOUR):
    new_state = (1 << this->segment_a_pin) | (1 << this->segment_e_pin) |
                (1 << this->segment_d_pin);
    break;

  case (DisplayDigit::FIVE):
    new_state = (1 << this->segment_b_pin) | (1 << this->segment_e_pin);
    break;

  case (DisplayDigit::SIX):
    new_state = 1 << this->segment_b_pin;
    break;

  case (DisplayDigit::SEVEN):
    new_state = ~((1 << this->segment_a_pin) | (1 << this->segment_b_pin) |
                  (1 << this->segment_c_pin));
    break;

  case (DisplayDigit::EIGHT):
    new_state = 0;
    break;

  case (DisplayDigit::NINE):
    new_state = 1 << this->segment_e_pin;
    break;
  }

  this->set_segments_board_state(new_state);
  this->set_digits_board_state(~(1 << pin));
}

void ArduinoSevenSegmentsDisplayDriver::turn_leds_off() const
{
  // desliga os 4 dígitos e todos os segmentos
  this->set_digits_board_state(0x0F);
  this->set_segments_board_state(1);
}

void ArduinoSevenSegmentsDisplayDriver::set_segments_board_state(
    uint8_t state) const
{
  *(this->segments_board) = state;
}

void ArduinoSevenSegmentsDisplayDriver::set_digits_board_state(
    uint8_t state) const
{
  *(this->digits_board) = state;
}
} // namespace tarefa3::arduino
