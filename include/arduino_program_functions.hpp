#pragma once

#include "arduino_impls.hpp"
#include "simple_timer.hpp"

namespace tarefa3
{

enum class StopwatchSettingsSwitch
{
  NONE,
  MINUTES,
  SECONDS
};

/// @brief Liga cada led do display de sete segmentos para exibir o dígito de
/// acordo com o timer. Liga cada led individualmente a cada 5ms para realizar
/// um multiplexing, conforme requisitado pela atividade.
/// @param stopwatch
/// @param driver
void display_every_digit(
    timer::SimpleTimer *stopwatch,
    const arduino::ArduinoSevenSegmentsDisplayDriver *driver,
    const uint16_t multiplex_delay);

/// @brief Mantém o cronômetro decrementando e satisfaz os requisitos
/// da atividade (piscar os dois pontos)
/// @param stopwatch
/// @param display_driver
void handle_stopwatch_counter(
    timer::SimpleTimer *stopwatch,
    const arduino::ArduinoSevenSegmentsDisplayDriver *display_driver);

/// @brief Acende parcialmente os dígitos do display do cronômetro, mantendo 2
/// dos digitos piscando a cada `blink_duration`ms.
/// @param stopwatch
/// @param driver
/// @param blink_duration
/// @param multiplex_delay_for_each_digit
/// @param fragment_1
/// @param fragment_2
void partially_blink_half_digits(
    timer::SimpleTimer *stopwatch,
    const arduino::ArduinoSevenSegmentsDisplayDriver *driver,
    const uint8_t half_digits_toggling_delay,
    uint8_t multiplex_delay_for_each_digit,
    timer::SimpleTimer::TimeFragment fragment_1,
    timer::SimpleTimer::TimeFragment fragment_2);

/// @brief Se o buzzer estiver tocando, interrompe-o caso ele já tenha tocado
/// por 300ms.
/// @param tone_is_playing
/// @param timer
void maybe_stop_buzzer(const uint8_t buzzer_pin, volatile bool *tone_is_playing,
                       unsigned long *timer);

/// @brief Faz o buzzer começar a tocar uma nota.
/// @param tone_is_playing
/// @param timer
void start_buzzer(const uint8_t buzzer_pin, volatile bool *tone_is_playing,
                  unsigned long *timer);
} // namespace tarefa3
