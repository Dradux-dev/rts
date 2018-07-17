#ifndef RUN_TESTMODE
#include "simulator.h"

#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "process.h"
#include "processmodel.h"
#include "queue.h"

static Context _ctx = {0 , 0, 0};
static Context* ctx = &_ctx;

static const uint32_t MAX_PROCESS_COUNT = 10;
static const uint32_t MIN_RANGE = 9;
static const uint32_t MAX_RANGE = 27;
static uint32_t blockRange = 10;
static uint32_t unblockRange = 10;

#ifdef WIN32
static const uint32_t STEP_DURATION = 5000;
#else
static const uint32_t STEP_DURATION = 5;
#endif

// Forward declarations
void simulate_init();
void simulate_cleanup();
void simulate_sigint(int);
void sigusr1Handler(int);
void sigusr2Handler(int);

static inline uint32_t randomNumber(uint32_t min, uint32_t max)
{
  return (rand() % (max - min)) + min;
}

void simulate_init()
{
  srand(time(NULL));

  signal(SIGUSR1, sigusr1Handler);
  signal(SIGUSR2, sigusr2Handler);

  blockRange = randomNumber(MIN_RANGE, MAX_RANGE);
  unblockRange = randomNumber(MIN_RANGE, MAX_RANGE);

  printf("Block chance %d%%\n", blockRange);
  printf("Unblock chance %d%%\n", unblockRange);

  ctx->ReadyQueue = (Queue*)calloc(1, sizeof(Queue));
  ctx->BlockedQueue = (Queue*)calloc(1, sizeof(Queue));

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

  process = q_remove(ctx->BlockedQueue);
  while (process)
  {
    free(process);
    process = q_remove(ctx->BlockedQueue);
  }

  free(ctx->ReadyQueue);
  free(ctx->BlockedQueue);
}

void simulate_sigint(int)
{
  simulate_cleanup();
  exit(0);
}


void sigusr1Handler(int)
{
  ctx_block(ctx);
}

void sigusr2Handler(int)
{
  ctx_unblock(ctx);
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
    uint32_t roll = randomNumber(1, 99);
    printf("Roll=%d\n", roll);
    if (roll <= blockRange)
    {
      // block
      raise(SIGUSR1);
    }
    else if (roll <= (blockRange + unblockRange))
    {
      // unblock
      raise(SIGUSR2);
    }

    ctx_step(ctx);

    ctx_print(ctx);
    printf("\n");

    #ifdef WIN32
    Sleep(STEP_DURATION);
    #else
    sleep(STEP_DURATION);
    #endif
  }
}

#endif
