#ifndef MATRIX_H
#define MATRIX_H

struct Matrix {
  int rows;
  int cols;
  int* data;
};

void m_init(Matrix* m);
void m_create(Matrix* m, int rows, int cols);
int m_get(Matrix* m, int row, int col, int* value);
int m_set(Matrix* m, int row, int col, int value);
int m_fill(Matrix* m, int value);
void m_free(Matrix* m);

#endif // MATRIX_H
