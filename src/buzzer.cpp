#include "buzzer.hpp"

namespace tarefa3::buzzer
{

Buzzer::Buzzer(const BuzzerPinManager *pin_manager_,
               const DebouncerFn debouncer_fn, const MillisFn millis_fn)
    : pin_manager(pin_manager_), is_debounced(debouncer_fn), millis(millis_fn)
{
}

void Buzzer::play_during(const unsigned long time_in_ms)
{
  static unsigned long timer = this->millis();
  if (!this->is_playing)
  {
    this->is_playing = true;
    this->pin_manager->set_signal_level(true);
  }

  if (this->is_debounced(&timer, time_in_ms))
  {
    this->is_playing = false;
    this->pin_manager->set_signal_level(false);
  }
}
} // namespace tarefa3::buzzer
