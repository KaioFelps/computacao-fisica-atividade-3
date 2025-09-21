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

  /// @brief  Opções para controlar o incremento/decremento dos minutos e
  /// segundos do timer
  class DeltaOptions
  {
  public:
    DeltaOptions() = default;
    DeltaOptions(bool should_bubble);
    DeltaOptions(bool should_bubble, bool skip_units);

    /// @brief Se a alteração deveria ou não ser propagada para a outra unidade
    /// de tempo (segundos -> minutos). Mudanças nos minutos nunca afetam os
    /// segundos.
    ///
    /// Se `false`, alterações nos segundos não afetam os minutos.
    /// Por exemplo: o cronômetro iria de 00:00 para 00:59 ao invés de 59:59.
    bool should_bubble = true;
    /// @brief Se o incremento deveria desconsiderar as unidades e alterar as
    /// dezenas diretamente. Por exemplo: se `false`, um exemplo de incrementos
    /// de segundos seria 00:13 → 00:14; se `true`, seria 00:13 → 00:19 → 00:29
    /// → ...
    bool skip_units = false;
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

  void increment_seconds();
  void increment_seconds(DeltaOptions options);

  void increment_minutes();
  void increment_minutes(DeltaOptions options);

  void decrement_seconds();
  void decrement_seconds(DeltaOptions options);

  void decrement_minutes();
  void decrement_minutes(DeltaOptions options);

  void reset(bool minutes, bool seconds);
  void maximize(bool minutes, bool seconds);

  bool is_zeroed() const;

private:
  const display::DisplayDriver *driver;

  core::DisplayDigit minutes_ten = core::DisplayDigit::ZERO;
  core::DisplayDigit minutes_unit = core::DisplayDigit::ZERO;
  core::DisplayDigit seconds_ten = core::DisplayDigit::ZERO;
  core::DisplayDigit seconds_unit = core::DisplayDigit::ZERO;

  void increment_minutes_by_ten();
  void decrement_minutes_by_ten();

  void increment_seconds_by_ten(bool should_bubble);
  void decrement_seconds_by_ten(bool should_bubble);

  void increment_minutes_by_unit();
  void decrement_minutes_by_unit();

  void increment_seconds_by_unit(bool should_bubble);
  void decrement_seconds_by_unit(bool should_bubble);

  SimpleTimer() = default;
};
} // namespace tarefa3::timer
