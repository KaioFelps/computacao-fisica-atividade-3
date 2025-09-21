#include "simple_timer.hpp"
#include "display.hpp"

using tarefa3::core::DisplayDigit;
using tarefa3::core::DisplayDigitHelper;

namespace tarefa3::timer
{
SimpleTimer::SimpleTimer(const display::DisplayDriver *driver_)
    : driver(driver_)
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

void SimpleTimer::increment_seconds() { this->increment_seconds(true); }
void SimpleTimer::increment_seconds(bool should_bubble)
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
  if (should_bubble) this->increment_minutes();
}

void SimpleTimer::increment_minutes() { this->increment_minutes(true); }
void SimpleTimer::increment_minutes(bool should_bubble)
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
  this->reset(true, should_bubble);
}

void SimpleTimer::decrement_minutes() { this->decrement_minutes(true); }
void SimpleTimer::decrement_minutes(bool should_bubble)
{
  auto minutes_unit_as_int = DisplayDigitHelper::to_uint8(this->minutes_unit);
  auto will_underflow = minutes_unit_as_int == 0;
  if (!will_underflow)
  {
    this->set_minutes_unit(
        DisplayDigitHelper::from_uint8(minutes_unit_as_int - 1));
    return;
  }

  this->set_minutes_unit(DisplayDigit::NINE);
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
  // só altera os segundos se `should_bubble` for `true`
  this->maximize(true, should_bubble);
}

void SimpleTimer::decrement_seconds() { this->decrement_seconds(true); }
void SimpleTimer::decrement_seconds(bool should_bubble)
{
  auto unit_as_int = DisplayDigitHelper::to_uint8(this->seconds_unit);
  auto will_underflow = unit_as_int == 0;
  if (!will_underflow)
  {
    this->set_seconds_unit(DisplayDigitHelper::from_uint8(unit_as_int - 1));
    return;
  }

  this->set_seconds_unit(DisplayDigit::NINE);
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
  if (should_bubble) this->decrement_minutes();
}

void SimpleTimer::reset(bool minutes, bool seconds)
{
  if (minutes)
  {
    this->set_minutes_ten(DisplayDigit::ZERO);
    this->set_minutes_unit(DisplayDigit::ZERO);
  }

  if (seconds)
  {
    this->set_seconds_ten(DisplayDigit::ZERO);
    this->set_seconds_unit(DisplayDigit::ZERO);
  }
}

void SimpleTimer::maximize(bool minutes, bool seconds)
{
  if (minutes)
  {
    this->set_minutes_ten(DisplayDigit::FIVE);
    this->set_minutes_unit(DisplayDigit::NINE);
  }

  if (seconds)
  {
    this->set_seconds_ten(DisplayDigit::FIVE);
    this->set_seconds_unit(DisplayDigit::NINE);
  }
}

void SimpleTimer::display_time_fragment(TimeFragment fragment)
{
  using display::DisplayDriver;
  switch (fragment)
  {
  case TimeFragment::MinutesTen:
    this->driver->set_digit(DisplayDriver::DIG1, this->minutes_ten);
    break;

  case TimeFragment::MinutesUnit:
    this->driver->set_digit(DisplayDriver::DIG2, this->minutes_unit);
    break;

  case TimeFragment::SecondsTen:
    this->driver->set_digit(DisplayDriver::DIG3, this->seconds_ten);
    break;

  case TimeFragment::SecondsUnit:
    this->driver->set_digit(DisplayDriver::DIG4, this->seconds_unit);
    break;
  }
}

bool SimpleTimer::is_zeroed() const
{
  return (this->minutes_ten == DisplayDigit::ZERO &&
          this->minutes_unit == DisplayDigit::ZERO &&
          this->seconds_ten == DisplayDigit::ZERO &&
          this->seconds_unit == DisplayDigit::ZERO);
}

} // namespace tarefa3::timer
