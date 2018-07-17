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

  // Ready
  uint32_t count = 0;
  QueueNode* node = ctx->ReadyQueue->start;
  printf("\tREADY=");
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

  // Blocked
  count = 0;
  node = ctx->BlockedQueue->start;
  printf("\tBLOCKED=");
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

void ctx_block(Context* ctx)
{
  if (!ctx)
  {
    return;
  }

  if (ctx->Running)
  {
    q_add(ctx->BlockedQueue, ctx->Running);
    p_block(ctx->Running);
    ctx->Running = NULL;
  }

  // get next running process
  ctx_step(ctx);
}

void ctx_unblock(Context* ctx)
{
  if (!ctx)
  {
    return;
  }

  Process* process = q_remove(ctx->BlockedQueue);
  if (process)
  {
    p_unblock(process);
    q_add(ctx->ReadyQueue, process);
  }
}
