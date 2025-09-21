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

  SimpleTimer(const display::DisplayDriver *driver);

  core::DisplayDigit get_minutes_ten() const;
  void set_minutes_ten(core::DisplayDigit digit);

  core::DisplayDigit get_minutes_unit() const;
  void set_minutes_unit(core::DisplayDigit digit);

  core::DisplayDigit get_seconds_ten() const;
  void set_seconds_ten(core::DisplayDigit digit);

  core::DisplayDigit get_seconds_unit() const;
  void set_seconds_unit(core::DisplayDigit digit);

  void display_time_fragment(TimeFragment fragment);

  /// @brief Incrementa os segundos em 1 segundo
  void increment_seconds();
  /// @brief Incrementa os segundos em 1 segundo
  /// @param should_bubble indica se o incremento deveria afetar os minutos
  void increment_seconds(bool should_bubble);

  /// @brief Incrementa os minutos em 1 minuto
  void increment_minutes();
  /// @brief Incrementa os minutos em 1 minuto
  /// @param should_bubble Indica se o incremento deveria afetar os segundos
  void increment_minutes(bool should_bubble);

  /// @brief Decrementa os segundos em 1 segundo
  void decrement_seconds();
  /// @brief Decrementa os segundos em 1 segundo
  /// @param should_bubble Indica se o decremento deveria afetar os minutos
  void decrement_seconds(bool should_bubble);

  /// @brief Decrementa os minutos em 1 minuto
  void decrement_minutes();
  /// @brief Decrementa os minutos em 1 minuto
  /// @param should_bubble Indica se o decremento deveria afetar os segundos
  void decrement_minutes(bool should_bubble);

  void reset(bool minutes, bool seconds);
  void maximize(bool minutes, bool seconds);

  bool is_zeroed() const;

private:
  const display::DisplayDriver *driver;

  core::DisplayDigit minutes_ten = core::DisplayDigit::ZERO;
  core::DisplayDigit minutes_unit = core::DisplayDigit::ZERO;
  core::DisplayDigit seconds_ten = core::DisplayDigit::ZERO;
  core::DisplayDigit seconds_unit = core::DisplayDigit::ZERO;

  SimpleTimer() = default;
};
} // namespace tarefa3::timer
