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

// timers pra usar nos debounces
volatile static auto rotation_timer = millis();

// variáveis utilizadas para manipular os componentes do simulador
static const auto arduino_pin_manager = ArduinoRotaryEncoderPinManager();
static const auto arduino_display_driver = ArduinoSevenSegmentsDisplayDriver(
    &PORTC, &PORTD, PD0, PD1, PD2, PD3, PD4, PD5, PD6);

static auto rotary_encoder =
    RotaryEncoder(OUTPUT_A, OUTPUT_B, SWITCH, &arduino_pin_manager);

static auto timer_display =
    SimpleTimer(DIG1, DIG2, DIG3, DIG4, &arduino_display_driver);

void setup()
{
  // reforçando:
  // 1 = output
  // 0 = input
  DDRB = 0;    // todos os pinos da seção B são entradas (inputs)
  DDRD = 0xFF; // todos os pinos na seção D são saídas (outputs) — para os
               // segmentos do display
  DDRC = 0xFF; // os 4 primeiros bits da seção C são saídas (outputs) — para os
               // dígitos do display
  DDRB &= ~(1 << SWITCH); // torna o pino que recebe o SWITCH do rotary encoder
                          // em uma entrada
  PORTB |= (1 << SWITCH); // habilita o resistor de pull-up interno da entrada
                          // do switch
}

void handle_multiplex_timer_display(
    const ArduinoSevenSegmentsDisplayDriver *driver, SimpleTimer *timer);

void loop()
{
  const auto rotary_encoder_report = rotary_encoder.get_report();

  if (rotary_encoder_report.has_rotated && is_debounced(&rotation_timer))
  {
    if (rotary_encoder_report.has_rotated_clockwise)
    {
      timer_display.increment_seconds();
    }
    else
    {
      timer_display.decrement_seconds();
    }
  }

  handle_multiplex_timer_display(&arduino_display_driver, &timer_display);
}

void handle_multiplex_timer_display(
    const ArduinoSevenSegmentsDisplayDriver *driver, SimpleTimer *timer)
{
  const unsigned int multiplex_delay = 500;
  static unsigned int display_multiplex_timer = millis();
  static uint8_t digit = 1;

  if (millis() - display_multiplex_timer >= multiplex_delay)
  {
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
    timer->display_time_fragment(time_fragment);
    digit = digit == 4 ? 1 : digit + 1;
    display_multiplex_timer = millis();
  }
}
