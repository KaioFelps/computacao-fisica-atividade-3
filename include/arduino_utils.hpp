#pragma once

namespace tarefa3::arduino
{
bool is_debounced(volatile unsigned long *debounce);
bool is_debounced(volatile unsigned long *debounce,
                  const unsigned int rate_limit_ms);
} // namespace tarefa3::arduino
