#ifndef RTS001_QUEUE_H
#define RTS001_QUEUE_H

struct QueueNode
{
  struct Process* process;
  struct QueueNode* next;
};

struct Queue
{
  QueueNode* start;
  QueueNode* end;
};

void q_add(Queue* queue, struct Process* process);
Process* q_remove(Queue* queue);
void q_print(Queue* queue);

#endif // RTS001_QUEUE_H
