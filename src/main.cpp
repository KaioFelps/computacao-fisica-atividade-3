#include "arduino_impls.hpp"
#include "arduino_program_functions.hpp"
#include "arduino_utils.hpp"
#include "display.hpp"
#include "rotary_encoder.hpp"
#include "simple_timer.hpp"
#include <Arduino.h>

using tarefa3::StopwatchSettingsSwitch;
using tarefa3::arduino::ArduinoRotaryEncoderPinManager;
using tarefa3::arduino::ArduinoSevenSegmentsDisplayDriver;
using tarefa3::arduino::is_debounced;
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

// pino do buzzer
#define BUZZER 12

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

void handle_rotary_encoder_rotation(
    bool has_rotated_clockwise,
    StopwatchSettingsSwitch stopwatch_settings_state);

void handle_rotary_encoder_switch_press(
    bool switch_has_been_pressed, volatile bool *stopwatch_is_running,
    volatile StopwatchSettingsSwitch *stopwatch_settings_state);

void manage_arduino_timer_leds_display(
    StopwatchSettingsSwitch stopwatch_settings_state);

void loop()
{
  static volatile auto rotation_timer = millis();
  static volatile auto switch_timer = millis();
  static volatile auto tone_timer = millis();

  static volatile auto stopwatch_is_running = false;
  static volatile auto tone_is_playing = false;
  static volatile auto stopwatch_settings_state = StopwatchSettingsSwitch::NONE;

  const auto rotary_encoder_report = rotary_encoder.get_report();

  if (rotary_encoder_report.has_rotated && is_debounced(&rotation_timer))
  {
    handle_rotary_encoder_rotation(rotary_encoder_report.has_rotated_clockwise,
                                   stopwatch_settings_state);
  }

  if (rotary_encoder_report.switch_state_has_changed &&
      is_debounced(&switch_timer))
  {
    handle_rotary_encoder_switch_press(
        rotary_encoder_report.switch_has_been_pressed, &stopwatch_is_running,
        &stopwatch_settings_state);
  }

  if (stopwatch_is_running)
  {
    tarefa3::handle_stopwatch_counter(&stopwatch, &arduino_display_driver);

    if (stopwatch.is_zeroed())
    {
      stopwatch_is_running = false;
      tarefa3::start_buzzer(BUZZER, &tone_is_playing, &tone_timer);
    }
  }

  tarefa3::maybe_stop_buzzer(BUZZER, &tone_is_playing, &tone_timer);
  manage_arduino_timer_leds_display(stopwatch_settings_state);
}

void handle_rotary_encoder_rotation(
    bool has_rotated_clockwise,
    StopwatchSettingsSwitch stopwatch_settings_state)
{

  if (has_rotated_clockwise)
  {
    switch (stopwatch_settings_state)
    {
    case StopwatchSettingsSwitch::NONE:
      stopwatch.increment_seconds();
      break;
    case StopwatchSettingsSwitch::MINUTES:
      stopwatch.increment_minutes(false);
      break;
    case StopwatchSettingsSwitch::SECONDS:
      stopwatch.increment_seconds(false);
      break;
    }
  }
  else
  {
    switch (stopwatch_settings_state)
    {
    case StopwatchSettingsSwitch::NONE:
      stopwatch.decrement_seconds();
      break;
    case StopwatchSettingsSwitch::MINUTES:
      stopwatch.decrement_minutes(false);
      break;
    case StopwatchSettingsSwitch::SECONDS:
      stopwatch.decrement_seconds(false);
    }
  }
}

void handle_rotary_encoder_switch_press(
    bool switch_has_been_pressed, volatile bool *stopwatch_is_running,
    volatile StopwatchSettingsSwitch *stopwatch_settings_state)
{
  static volatile unsigned long rotary_encoder_switch_press_timer = millis();
  const auto press_time_for_enable_stopwatch_settings = 500;

  // quando pressionar o switch, inicia-se um contador para decidir qual
  // evento será executado quando o switch termianr de ser pressionado. Fazer
  // desse jeito evita conflitos de eventos (começar a contar assim que
  // apertado quando, na verdade, a intenção do usuário era entrar no modo de
  // configuração)
  if (switch_has_been_pressed)
  {
    rotary_encoder_switch_press_timer = millis();
  }
  // se pressionado por 1 segundo, entra (ou altera) no modo de configuração
  // do cronômetro
  else if (is_debounced(&rotary_encoder_switch_press_timer,
                        press_time_for_enable_stopwatch_settings))
  {
    // força o cronômetro a parar de rodar, caso o usuário entre no modo de
    // configuração enquanto ele está rodando
    *stopwatch_is_running = false;

    switch (*stopwatch_settings_state)
    {
    case StopwatchSettingsSwitch::NONE:
    case StopwatchSettingsSwitch::SECONDS:
      *stopwatch_settings_state = StopwatchSettingsSwitch::MINUTES;
      break;

    case StopwatchSettingsSwitch::MINUTES:
      *stopwatch_settings_state = StopwatchSettingsSwitch::SECONDS;
      break;
    }
  }
  else
  {
    // se o cronômetro estiver em seu estado normal, pressionar o switch deve
    // iniciar a contagem regressiva
    if (*stopwatch_settings_state == StopwatchSettingsSwitch::NONE)
    {
      *stopwatch_is_running = !(*stopwatch_is_running);
    }
    // se o cronômetro estiver em estado de configuração, então pressionar o
    // switch deve tirá-lo do estado de configuração
    else
    {
      *stopwatch_settings_state = StopwatchSettingsSwitch::NONE;
    }
  }
}

void manage_arduino_timer_leds_display(
    StopwatchSettingsSwitch stopwatch_settings_state)
{
  // todos os tempos se dão em milisegundos
  const auto blink_duration = 200;
  const unsigned int multiplex_delay = 5;

  switch (stopwatch_settings_state)
  {
  case StopwatchSettingsSwitch::NONE:
    tarefa3::display_every_digit(&stopwatch, &arduino_display_driver,
                                 multiplex_delay);
    break;

  case StopwatchSettingsSwitch::MINUTES:
    // mantém os dígitos dos minutos piscando
    tarefa3::partially_blink_half_digits(
        &stopwatch, &arduino_display_driver, blink_duration, multiplex_delay,
        SimpleTimer::TimeFragment::SecondsTen,
        SimpleTimer::TimeFragment::SecondsUnit);
    break;

  case StopwatchSettingsSwitch::SECONDS:
    // mantém os dígitos dos minutos segundos
    tarefa3::partially_blink_half_digits(
        &stopwatch, &arduino_display_driver, blink_duration, multiplex_delay,
        SimpleTimer::TimeFragment::MinutesTen,
        SimpleTimer::TimeFragment::MinutesUnit);
    break;
  }
}
