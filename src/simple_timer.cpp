#include "simple_timer.hpp"
#include "display.hpp"

using tarefa3::core::DisplayDigit;
using tarefa3::core::DisplayDigitHelper;

namespace tarefa3::timer
{
SimpleTimer::SimpleTimer(const uint8_t minutes_ten_digit_pin_,
                         const uint8_t minutes_unit_digit_pin_,
                         const uint8_t seconds_ten_digit_pin_,
                         const uint8_t seconds_unit_digit_pin_,
                         const display::DisplayDriver *driver_)
    : minutes_ten_digit_pin(minutes_ten_digit_pin_),
      minutes_unit_digit_pin(minutes_unit_digit_pin_),
      seconds_ten_digit_pin(seconds_ten_digit_pin_),
      seconds_unit_digit_pin(seconds_unit_digit_pin_), driver(driver_)
{
}

DisplayDigit SimpleTimer::get_minutes_ten() const { return this->minutes_ten; }

DisplayDigit SimpleTimer::get_minutes_unit() const
{
  return this->minutes_unit;
}

DisplayDigit SimpleTimer::get_seconds_ten() const { return this->seconds_ten; }

DisplayDigit SimpleTimer::get_seconds_unit() const
{
  return this->seconds_unit;
}

void SimpleTimer::set_minutes_ten(DisplayDigit digit)
{
  this->minutes_ten = digit;
}

void SimpleTimer::set_minutes_unit(DisplayDigit digit)
{
  this->minutes_unit = digit;
}

void SimpleTimer::set_seconds_ten(DisplayDigit digit)
{
  this->seconds_ten = digit;
}

void SimpleTimer::set_seconds_unit(DisplayDigit digit)
{
  this->seconds_unit = digit;
}

void SimpleTimer::increment_seconds()
{
  auto seconds_unit_as_int = DisplayDigitHelper::to_uint8(this->seconds_unit);
  auto will_overflow = seconds_unit_as_int == 9;
  if (!will_overflow)
  {
    this->set_seconds_unit(
        DisplayDigitHelper::from_uint8(seconds_unit_as_int + 1));
    return;
  }

  this->set_seconds_unit(DisplayDigit::ZERO);
  auto seconds_ten_as_int = DisplayDigitHelper::to_uint8(this->seconds_ten);
  // se chegou aqui é porque a unidade foi de 9 -> 0, então se for 5 iria
  // para 6 (60 segundos é o trigger pra virar o minuto)
  will_overflow = seconds_ten_as_int == 5;
  if (!will_overflow)
  {
    this->set_seconds_ten(
        DisplayDigitHelper::from_uint8(seconds_ten_as_int + 1));
    return;
  }

  this->set_seconds_ten(DisplayDigit::ZERO);
  this->increment_minutes();
}

void SimpleTimer::increment_minutes()
{
  auto minutes_unit_as_int = DisplayDigitHelper::to_uint8(this->minutes_unit);
  auto will_overflow = minutes_unit_as_int == 9;
  if (!will_overflow)
  {
    this->set_minutes_unit(
        DisplayDigitHelper::from_uint8(minutes_unit_as_int + 1));
    return;
  }

  this->set_minutes_unit(DisplayDigit::ZERO);
  auto minutes_ten_as_int = DisplayDigitHelper::to_uint8(this->minutes_ten);
  // vai ter overflow se os minutos forem 59 e houver um incremento
  will_overflow = minutes_ten_as_int == 5;
  if (!will_overflow)
  {
    this->set_minutes_ten(
        DisplayDigitHelper::from_uint8(minutes_ten_as_int + 1));
    return;
  }

  // manda de 59:59 -> 0
  this->reset();
}

void SimpleTimer::decrement_minutes()
{
  auto minutes_unit_as_int = DisplayDigitHelper::to_uint8(this->minutes_unit);
  auto will_underflow = minutes_unit_as_int == 0;
  if (!will_underflow)
  {
    this->set_minutes_unit(
        DisplayDigitHelper::from_uint8(minutes_unit_as_int - 1));
    return;
  }

  auto minutes_ten_as_int = DisplayDigitHelper::to_uint8(this->minutes_ten);
  will_underflow = minutes_ten_as_int == 0;
  if (!will_underflow)
  {
    this->set_minutes_ten(
        DisplayDigitHelper::from_uint8(minutes_ten_as_int - 1));
    return;
  }

  // se chegou em 00m00,
  // manda pra 59m59
  this->maximize();
}

void SimpleTimer::decrement_seconds()
{
  auto unit_as_int = DisplayDigitHelper::to_uint8(this->seconds_unit);
  auto will_underflow = unit_as_int == 0;
  if (!will_underflow)
  {
    this->set_seconds_unit(DisplayDigitHelper::from_uint8(unit_as_int - 1));
    return;
  }

  this->set_seconds_unit(DisplayDigit::ZERO);
  auto ten_as_int = DisplayDigitHelper::to_uint8(this->seconds_ten);
  will_underflow = ten_as_int == 0;

  if (!will_underflow)
  {
    this->set_seconds_ten(DisplayDigitHelper::from_uint8(ten_as_int - 1));
    return;
  }

  // se chegou em 00s, manda pra (m -1):59
  this->set_seconds_ten(DisplayDigit::FIVE);
  this->set_seconds_unit(DisplayDigit::NINE);
  this->decrement_minutes();
}

void SimpleTimer::reset()
{
  this->set_minutes_ten(DisplayDigit::ZERO);
  this->set_minutes_unit(DisplayDigit::ZERO);
  this->set_seconds_ten(DisplayDigit::ZERO);
  this->set_seconds_unit(DisplayDigit::ZERO);
}

void SimpleTimer::maximize()
{
  this->set_minutes_ten(DisplayDigit::NINE);
  this->set_minutes_unit(DisplayDigit::NINE);
  this->set_seconds_ten(DisplayDigit::NINE);
  this->set_seconds_unit(DisplayDigit::NINE);
}

void SimpleTimer::update_display(TimeFragment fragment)
{
  switch (fragment)
  {
  case TimeFragment::MinutesTen:
    this->driver->set_digit(this->minutes_ten_digit_pin, this->minutes_ten);
    break;

  case TimeFragment::MinutesUnit:
    this->driver->set_digit(this->minutes_unit_digit_pin, this->minutes_unit);
    break;

  case TimeFragment::SecondsTen:
    this->driver->set_digit(this->seconds_ten_digit_pin, this->seconds_ten);
    break;

  case TimeFragment::SecondsUnit:
    this->driver->set_digit(this->seconds_unit_digit_pin, this->seconds_unit);
    break;
  }
}

} // namespace tarefa3::timer
