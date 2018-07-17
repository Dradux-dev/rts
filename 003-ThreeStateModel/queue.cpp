#include "queue.h"

#include <stdlib.h>

#include "process.h"

void q_add(Queue *queue, Process *process)
{
  if (!queue || !process)
  {
    return;
  }

  QueueNode* node = (QueueNode*)calloc(1, sizeof(QueueNode));
  if (!node)
  {
    return;
  }

  // set process
  node->process = process;

  if (!queue->start)
  {
    // set start and end
    queue->start = node;
    queue->end = queue->start;

    return;
  }

  queue->end->next = node;
  queue->end = node;
}

Process* q_remove(Queue* queue)
{
  if (!queue || !queue->start)
  {
      return NULL;
  }

  // Get first node
  QueueNode* node = queue->start;

  // Set following node as new start
  queue->start = node->next;
  if (!queue->start)
  {
      // Queue is now empty
      queue->end = NULL;
  }

  Process* process = node->process;
  free(node);

  return process;
}

void q_print(Queue* queue)
{
  uint32_t count = 0;
  QueueNode* node = queue->start;

  while (node)
  {
    // print comma seperator for 2nd
    // and following nodes
    if (count > 0)
    {
      printf(", ");
    }

    // Print process in squared brackets
    // without trailing line feed
    printf("[");
    p_print(node->process, 0);
    printf("]");

    // Grab next entry
    node = node->next;
    ++count;
  }

  printf("\n");
}

