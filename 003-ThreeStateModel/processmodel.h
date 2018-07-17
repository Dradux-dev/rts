#ifndef RTS001_PROCESSMODEL_H
#define RTS001_PROCESSMODEL_H

struct Context
{
  struct Queue* ReadyQueue;
  struct Queue* BlockedQueue;
  struct Process* Running;
};

void ctx_print(Context* ctx);
void ctx_block(Context* ctx);
void ctx_unblock(Context* ctx);
void ctx_step(Context* ctx);

#endif // RTS001_PROCESSMODEL_H
