#include "rotary_encoder.hpp"

namespace tarefa3::rotary_encoder
{
RotaryEncoder::State::State(bool output_a_state_, bool output_b_state_)
    : output_a_state(output_a_state_), output_b_state(output_b_state_)
{
}

RotaryEncoder::~RotaryEncoder()
{
  if (this->previous_state)
  {
    delete this->previous_state;
  }

  if (this->current_state)
  {
    delete this->current_state;
  }
}

RotaryEncoder::RotaryEncoder(uint8_t output_a_pin_, uint8_t output_b_pin_,
                             uint8_t switch_pin_,
                             const RotaryEncoderPinsManager *pins_manager_)
    : output_a_pin(output_a_pin_), output_b_pin(output_b_pin_),
      switch_pin(switch_pin_), pins_manager(pins_manager_)
{
  this->current_state = new State(
      this->pins_manager->get_rotary_output_digital_level(this->output_a_pin),
      this->pins_manager->get_rotary_output_digital_level(this->output_b_pin));
}

bool RotaryEncoder::has_rotated()
{
  return this->previous_state->output_a_state !=
         this->current_state->output_a_state;
}

bool RotaryEncoder::check_if_rotation_is_clockwise()
{
  if (this->previous_state->output_a_state !=
      this->current_state->output_a_state)
  {
    return this->current_state->output_b_state ==
           this->current_state->output_a_state;
  }

  return false;
}

void RotaryEncoder::update()
{
  if (this->previous_state)
  {
    delete this->previous_state;
  }

  auto new_current_state = new State(
      this->pins_manager->get_rotary_output_digital_level(this->output_a_pin),
      this->pins_manager->get_rotary_output_digital_level(this->output_b_pin));

  this->previous_state = this->current_state;
  this->current_state = new_current_state;

  this->previous_switch_state = this->current_switch_state;
  this->current_switch_state =
      this->pins_manager->get_rotary_output_digital_level(this->switch_pin);
}

RotationReport RotaryEncoder::get_report()
{
  this->update();

  RotationReport report = {
      .has_rotated = this->has_rotated(),
      .has_rotated_clockwise = false,
      .switch_state_has_changed =
          this->previous_switch_state != this->current_switch_state,
      .switch_has_been_pressed = false,
  };

  if (report.has_rotated)
  {
    report.has_rotated_clockwise = this->check_if_rotation_is_clockwise();
  }

  if (report.switch_state_has_changed)
  {
    report.switch_has_been_pressed = !this->current_switch_state;
  }

  return report;
}
} // namespace tarefa3::rotary_encoder
