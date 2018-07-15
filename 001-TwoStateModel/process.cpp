#include "process.h"

static inline const char* p_state_name(EProcessState state)
{
  static const char* names[] = {
    "READY",
    "RUNNING"
  };

  return names[(int)state];
}

void p_switch_state(Process* process)
{
  if (!process)
  {
    return;
  }

  process->p_state = (process->p_state == ProcessState::READY) ? ProcessState::RUNNING : ProcessState::READY;
}

void p_print(Process* process, uint8_t UseLF)
{
  if (!process)
  {
    return;
  }

  printf("PID=%d, STATE=%s%s", process->p_id, p_state_name(process->p_state), (UseLF) ? "\n" : "");
}
