#ifndef RUN_TESTMODE
#include "simulator.h"

#include <stdint.h>
#include <windows.h>

#include "process.h"
#include "processmodel.h"
#include "queue.h"

static Context _ctx = {0 , 0};
static Context* ctx = &_ctx;

static const uint32_t MAX_PROCESS_COUNT = 10;
static const uint32_t STEP_DURATION = 5000;

// Forward declarations
void simulate_init();
void simulate_cleanup();
void simulate_sigint(int);

void simulate_init()
{
  ctx->ReadyQueue = (Queue*)calloc(1, sizeof(Queue));

  for (uint32_t n = 0; n < MAX_PROCESS_COUNT; ++n)
  {
    Process* process = (Process*)calloc(1, sizeof(Process));

    if (process)
    {
      process->p_id = n+1;
      process->p_state = ProcessState::READY;

      q_add(ctx->ReadyQueue, process);
    }
  }
}

void simulate_cleanup()
{
  if (ctx->Running)
  {
    free(ctx->Running);
    ctx->Running = NULL;
  }

  Process* process = q_remove(ctx->ReadyQueue);
  while (process)
  {
    free(process);
    process = q_remove(ctx->ReadyQueue);
  }

  free(ctx->ReadyQueue);
}

void simulate_sigint(int)
{
  simulate_cleanup();
  exit(0);
}

void simulate()
{
  simulate_init();

  printf("Init\n");
  ctx_print(ctx);
  printf("\n");

  printf("Running\n");
  while (true)
  {
    ctx_step(ctx);

    ctx_print(ctx);
    printf("\n");

    Sleep(STEP_DURATION);
  }
}

#endif
