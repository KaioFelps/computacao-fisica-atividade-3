#include "arduino_program_functions.hpp"
#include "arduino_utils.hpp"
#include <Arduino.h>

namespace tarefa3
{

using tarefa3::arduino::ArduinoSevenSegmentsDisplayDriver;
using tarefa3::arduino::is_debounced;
using tarefa3::timer::SimpleTimer;

void start_buzzer(const uint8_t buzzer_pin, volatile bool *tone_is_playing,
                  unsigned long *timer)
{
  tone(buzzer_pin, 300);
  *timer = millis();
  *tone_is_playing = true;
}

void maybe_stop_buzzer(const uint8_t buzzer_pin, volatile bool *tone_is_playing,
                       unsigned long *timer)
{
  if (*tone_is_playing && is_debounced(timer, 300))
  {
    noTone(buzzer_pin);
    *tone_is_playing = false;
  }
}

void handle_stopwatch_counter(
    SimpleTimer *stopwatch,
    const ArduinoSevenSegmentsDisplayDriver *display_driver)
{
  static auto seconds_timer = millis();
  static auto colon_timer = millis();

  const auto one_second_as_ms = 1000;
  const auto half_a_second_as_ms = one_second_as_ms / 2;

  if (is_debounced(&colon_timer, half_a_second_as_ms))
  {
    display_driver->toggle_colon();
  }

  if (is_debounced(&seconds_timer, one_second_as_ms))
  {
    stopwatch->decrement_seconds();
    seconds_timer = millis();
  }
}

void display_every_digit(SimpleTimer *stopwatch,
                         const ArduinoSevenSegmentsDisplayDriver *driver,
                         const uint16_t multiplex_delay)
{
  static auto display_multiplex_timer = millis();
  static uint8_t digit = 1;

  if (is_debounced(&display_multiplex_timer, multiplex_delay))
  {
    // obtém qual a unidade que vai ser acesa (dezena ou unidade de segundos ou
    // minutos)
    SimpleTimer::TimeFragment time_fragment;
    switch (digit)
    {
    case 1:
      time_fragment = SimpleTimer::TimeFragment::MinutesTen;
      break;

    case 2:
      time_fragment = SimpleTimer::TimeFragment::MinutesUnit;
      break;

    case 3:
      time_fragment = SimpleTimer::TimeFragment::SecondsTen;
      break;

    case 4:
      time_fragment = SimpleTimer::TimeFragment::SecondsUnit;
      break;
    }

    driver->turn_leds_off();
    stopwatch->display_time_fragment(time_fragment);

    // essa parte controla qual digito vai ser acendido a cada 5ms
    digit = digit == 4 ? 1 : digit + 1;
    display_multiplex_timer = millis();
  }
}

void partially_blink_half_digits(
    SimpleTimer *stopwatch, const ArduinoSevenSegmentsDisplayDriver *driver,
    const uint8_t half_digits_toggling_delay,
    uint8_t multiplex_delay_for_each_digit,
    SimpleTimer::TimeFragment fragment_1, SimpleTimer::TimeFragment fragment_2)
{
  static auto target_half_timer = millis();
  static auto left_half_timer = millis();

  static auto target_half_digits_should_be_off = false;

  if (is_debounced(&target_half_timer, half_digits_toggling_delay))
  {
    target_half_digits_should_be_off = !target_half_digits_should_be_off;
  }

  if (!target_half_digits_should_be_off)
  {
    display_every_digit(stopwatch, driver, multiplex_delay_for_each_digit);
    return;
  }

  if (is_debounced(&left_half_timer, multiplex_delay_for_each_digit))
  {
    // um booleano é o bastante pra alternar entre os dois únicos dígitos que
    // precisam estar visíveis neste caso
    static bool digit = false;
    const auto fragment = digit ? fragment_1 : fragment_2;
    digit = !digit;

    driver->turn_leds_off();
    stopwatch->display_time_fragment(fragment);
  }
}
} // namespace tarefa3
