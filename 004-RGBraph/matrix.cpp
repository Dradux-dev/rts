#include "matrix.h"

#include <stdlib.h>

static inline int coordtoindex(Matrix* m, int row, int col) {
  if (!m) {
    return -1;
  }

  if (row > m->rows) {
    return -2;
  }

  if (col > m->cols) {
    return -3;
  }

  return ((row - 1) * m->cols) + (col - 1);
}

void m_init(Matrix* m) {
  if (!m) {
    return;
  }

  m->cols = m->rows = 0;
  m->data = NULL;
}

void m_create(Matrix* m, int rows, int cols) {
  if (!m) {
    return;
  }

  size_t size = cols * rows;
  m->rows = rows;
  m->cols = cols;
  m->data = (int*)calloc(size, sizeof(int));
}
int m_get(Matrix* m, int row, int col, int* value) {
  if (!m) {
    return -1;
  }

  int index = coordtoindex(m, row, col);
  if (index < 0) {
    return index;
  }

  if (!value) {
    return -4;
  }

  *value = m->data[index];
  return 0;
}

int m_set(Matrix* m, int row, int col, int value) {
  if (!m) {
    return -1;
  }

  int index = coordtoindex(m, row, col);
  if (index < 0) {
    return index;
  }

  m->data[index] = value;
  return 0;
}

int m_fill(Matrix* m, int value) {
  if (!m) {
    return -1;
  }

  if (!m->data) {
    return -2;
  }

  for(int i = 0; i < (m->rows * m->cols); ++i) {
    m->data[i] = value;
  }

  return 0;
}

void m_free(Matrix* m) {
  if (!m) {
    return;
  }

  if (!m->data) {
    return;
  }

  free(m->data);
  m->data = NULL;
}
