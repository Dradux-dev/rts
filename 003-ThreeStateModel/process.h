#ifndef RTS001_PROCESS_H
#define RTS001_PROCESS_H

#include <stdio.h>
#include <stdint.h>

struct ProcessState {
  enum Values
  {
    READY = 0,
    RUNNING = 1,
    BLOCKED = 2
  };
};

typedef ProcessState::Values EProcessState;

struct Process
{
  uint32_t p_id;
  EProcessState p_state;
};


void p_switch_state(Process* process);
void p_block(Process* process);
void p_unblock(Process* process);
void p_print(Process* process, uint8_t UseLF = 1);


#endif // RTS001_PROCESS_H
