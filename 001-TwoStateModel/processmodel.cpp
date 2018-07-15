#include "processmodel.h"

#include <stdint.h>

#include "process.h"
#include "queue.h"

void ctx_print(Context *ctx)
{
  if (!ctx)
  {
    return ;
  }

  printf("{\n");

  printf("\tREADY=");
  uint32_t count = 0;
  QueueNode* node = ctx->ReadyQueue->start;

  while (node)
  {
    if (count > 0)
    {
      printf(", ");
    }

    printf("%d", node->process->p_id);

    // Next node
    ++count;
    node = node->next;
  }
  printf("\n");

  printf("\tRUNNING=");
  if (ctx->Running)
  {
    printf("%d", ctx->Running->p_id);
  }
  printf("\n");

  printf("}\n");
}

void ctx_step(Context *ctx)
{
  if (!ctx)
  {
    return;
  }

  if (ctx->Running)
  {
    q_add(ctx->ReadyQueue, ctx->Running);
    p_switch_state(ctx->Running);
    ctx->Running = NULL;
  }

  ctx->Running = q_remove(ctx->ReadyQueue);
  if (ctx->Running)
  {
    p_switch_state(ctx->Running);
  }
}
