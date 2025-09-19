#pragma once

#include "digit.hpp"
#include "display.hpp"
#include <stdint.h>

namespace tarefa3::timer
{

class SimpleTimer
{
public:
  enum class TimeFragment
  {
    MinutesTen,
    MinutesUnit,
    SecondsTen,
    SecondsUnit
  };

  SimpleTimer(const uint8_t minutes_ten_digit_pin,
              const uint8_t minutes_unit_digit_pin,
              const uint8_t seconds_ten_digit_pin,
              const uint8_t seconds_unit_digit_pin,
              const display::DisplayDriver *driver);

  core::DisplayDigit get_minutes_ten() const;
  void set_minutes_ten(core::DisplayDigit digit);

  core::DisplayDigit get_minutes_unit() const;
  void set_minutes_unit(core::DisplayDigit digit);

  core::DisplayDigit get_seconds_ten() const;
  void set_seconds_ten(core::DisplayDigit digit);

  core::DisplayDigit get_seconds_unit() const;
  void set_seconds_unit(core::DisplayDigit digit);

  void update_display(TimeFragment fragment);

  void increment_seconds();
  void increment_minutes();

  void decrement_seconds();
  void decrement_minutes();

  void reset();
  void maximize();

private:
  const display::DisplayDriver *driver;

  const uint8_t minutes_ten_digit_pin;
  const uint8_t minutes_unit_digit_pin;
  const uint8_t seconds_ten_digit_pin;
  const uint8_t seconds_unit_digit_pin;

  core::DisplayDigit minutes_ten = core::DisplayDigit::ZERO;
  core::DisplayDigit minutes_unit = core::DisplayDigit::ZERO;
  core::DisplayDigit seconds_ten = core::DisplayDigit::ZERO;
  core::DisplayDigit seconds_unit = core::DisplayDigit::ZERO;

  void set_time_fragment(TimeFragment fragment, core::DisplayDigit digit);

  SimpleTimer() = default;
};
} // namespace tarefa3::timer
