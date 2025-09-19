#pragma once

#include <stdint.h>

namespace tarefa3::rotary_encoder
{
  typedef struct
  {
    bool has_rotated;
    /// @brief  Essa opção é verdadeira se, e somente se,
    /// `has_rotated`=`true` e a rotação estiver no sentido horário.
    bool has_rotated_clockwise;
    /// @brief Indica se o switch teve alteração de estado.
    bool switch_state_has_changed;
    /// @brief Se `switch_state_has_changed` for `true`, então
    /// essa opção será `true` se o switch foi pressionado ou `false``
    /// se ele foi solto (pull up).
    /// Se `switch_state_has_changed` for `false`, essa opção não
    /// indica qualquer coisa relevante.
    bool switch_has_been_pressed;
  } RotationReport;

  class RotaryEncoderPinsManager
  {
  public:
    virtual bool get_rotary_output_digital_level(uint8_t output_pin) const = 0;
  };

  class RotaryEncoder
  {
  private:
    class State
    {
    public:
      // para os outputs: 0 = low; 1 = high
      bool output_a_state;
      bool output_b_state;

      State(bool output_a_state, bool output_b_state);
    };

    uint8_t output_a_pin;
    uint8_t output_b_pin;
    uint8_t switch_pin;
    State *previous_state;
    State *current_state;
    bool previous_switch_state = false;
    bool current_switch_state = false;

    const RotaryEncoderPinsManager *pins_manager;

    bool has_rotated();
    bool switch_has_been_pressed();
    bool check_if_rotation_is_clockwise();
    void update();

  public:
    ~RotaryEncoder();
    RotaryEncoder(uint8_t output_a_pin, uint8_t output_b_pin, uint8_t switch_pin, const RotaryEncoderPinsManager *pins_manager);
    RotationReport get_report();
  };
}
