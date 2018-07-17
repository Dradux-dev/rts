#ifdef RUN_TESTMODE

#include "test.h"

static inline void title(const char* s)
{
  printf("%s\n", s);
}

static inline void end()
{
  printf("\n");
}

void test()
{
  Process a = {7, ProcessState::READY};
  Process b = {4, ProcessState::RUNNING};

  title("Initial states");
  p_print(&a);
  p_print(&b);
  end();

  title("Switch state of Process A");
  p_switch_state(&a);
  p_print(&a);
  end();

  title("Switch again state of Process A");
  p_switch_state(&a);
  p_print(&a);
  end();


  Queue queue = {0, 0};
  q_add(&queue, &a);
  q_add(&queue, &b);

  title("Initial Queue");
  q_print(&queue);
  end();

  title("Remove A");
  q_remove(&queue);
  q_print(&queue);
  end();

  title("Push A to end");
  q_add(&queue, &a);
  q_print(&queue);
  end();

  title("Remove B");
  q_remove(&queue);
  q_print(&queue);
  end();

  title("Remove A again");
  q_remove(&queue);
  q_print(&queue);
  end();

  title("Remove from empty queue");
  Process* p = q_remove(&queue);
  if (p)
  {
    printf("Found a strange process! ERROR!\n");
  }
  else
  {
    printf("No element returned\n");
  }
  end();
}

#endif // RUN_TESTMODE
