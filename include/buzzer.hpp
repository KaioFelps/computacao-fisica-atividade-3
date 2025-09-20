#pragma once

#include <stdint.h>

namespace tarefa3::buzzer
{
using DebouncerFn = bool (*)(volatile unsigned long *debounce,
                             const unsigned int rate_limit_ms);

using MillisFn = unsigned long (*)(void);

class BuzzerPinManager
{
public:
  void virtual set_signal_level(bool signal_level) const = 0;
};

class Buzzer
{
public:
  Buzzer(const BuzzerPinManager *pin_manager, const DebouncerFn debouncer_fn,
         const MillisFn millis_fn);
  void play_during(const unsigned long time_in_ms);

private:
  bool is_playing = false;
  const DebouncerFn is_debounced;
  const MillisFn millis;
  const const BuzzerPinManager *pin_manager;
};
} // namespace tarefa3::buzzer
