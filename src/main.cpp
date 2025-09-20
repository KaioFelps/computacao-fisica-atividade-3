#include "arduino_impls.hpp"
#include "arduino_utils.hpp"
#include "display.hpp"
#include "rotary_encoder.hpp"
#include "simple_timer.hpp"
#include <Arduino.h>

using tarefa3::arduino::ArduinoRotaryEncoderPinManager;
using tarefa3::arduino::ArduinoSevenSegmentsDisplayDriver;
using tarefa3::arduino::is_debounced;
using tarefa3::core::DisplayDigit;
using tarefa3::rotary_encoder::RotaryEncoder;
using tarefa3::timer::SimpleTimer;

// pinos do rotary encoder
#define SWITCH PB2
#define OUTPUT_A PB1
#define OUTPUT_B PB0

// pinos do display
#define DIG1 PC3
#define DIG2 PC4
#define DIG3 PC5
#define DIG4 PC2
#define COLON PB3

// variáveis utilizadas para manipular os componentes do simulador
static const auto arduino_pin_manager = ArduinoRotaryEncoderPinManager();
static const auto arduino_display_driver = ArduinoSevenSegmentsDisplayDriver(
    ArduinoSevenSegmentsDisplayDriver::Pins{
        .segment_a = PD0,
        .segment_b = PD1,
        .segment_c = PD2,
        .segment_d = PD3,
        .segment_e = PD4,
        .segment_f = PD5,
        .segment_g = PD6,
        .digit_1 = DIG1,
        .digit_2 = DIG2,
        .digit_3 = DIG3,
        .digit_4 = DIG4,
        .colon = COLON,
    },
    ArduinoSevenSegmentsDisplayDriver::Boards{
        .digits = &PORTC,
        .segments = &PORTD,
        .colon = &PORTB,
    });

static auto rotary_encoder =
    RotaryEncoder(OUTPUT_A, OUTPUT_B, SWITCH, &arduino_pin_manager);

static auto stopwatch = SimpleTimer(&arduino_display_driver);

void setup()
{
  Serial.end();
  // reforçando:
  // 1 = output
  // 0 = input
  DDRB = 0b00001000; // todos os pinos da seção B são entradas (inputs), exceto
                     // a saída dos dois pontos
  DDRD = 0xFF;       // todos os pinos na seção D são saídas (outputs) — para os
                     // segmentos do display
  DDRC = 0xFF; // os 4 primeiros bits da seção C são saídas (outputs) — para os
               // dígitos do display
  DDRB &= ~(1 << SWITCH); // torna o pino que recebe o SWITCH do rotary encoder
                          // em uma entrada
  PORTB |= (1 << SWITCH); // habilita o resistor de pull-up interno da entrada
  // do switch
}

void handle_multiplex_timer_display(
    SimpleTimer *stopwatch, const ArduinoSevenSegmentsDisplayDriver *driver);

void handle_stopwatch_counter(
    SimpleTimer *stopwatch,
    const ArduinoSevenSegmentsDisplayDriver *display_driver);

void loop()
{
  static volatile auto rotation_timer = millis();
  static volatile auto switch_timer = millis();
  static volatile auto stopwatch_is_running = false;
  const auto rotary_encoder_report = rotary_encoder.get_report();

  if (rotary_encoder_report.has_rotated && is_debounced(&rotation_timer))
  {
    if (rotary_encoder_report.has_rotated_clockwise)
    {
      stopwatch.increment_seconds();
    }
    else
    {
      stopwatch.decrement_seconds();
    }
  }

  if (rotary_encoder_report.switch_state_has_changed &&
      is_debounced(&switch_timer))
  {
    if (rotary_encoder_report.switch_has_been_pressed)
    {
      stopwatch_is_running = !stopwatch_is_running;
    }
  }

  if (stopwatch_is_running)
  {
    handle_stopwatch_counter(&stopwatch, &arduino_display_driver);
  }

  handle_multiplex_timer_display(&stopwatch, &arduino_display_driver);
}

void handle_stopwatch_counter(
    SimpleTimer *stopwatch,
    const ArduinoSevenSegmentsDisplayDriver *display_driver)
{
  static volatile auto seconds_timer = millis();
  static volatile auto colon_timer = millis();

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

void handle_multiplex_timer_display(
    SimpleTimer *stopwatch, const ArduinoSevenSegmentsDisplayDriver *driver)
{
  const unsigned int multiplex_delay = 5;
  static volatile auto display_multiplex_timer = millis();
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
