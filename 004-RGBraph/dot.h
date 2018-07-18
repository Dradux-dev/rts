#ifndef DOT_H
#define DOT_H

void dot_begin(const char* filename);
void dot_add_ressource(int index);
void dot_add_thread(int index);
void dot_occupy(int ressource, int thread);
void dot_wait(int thread, int ressource);
void dot_end();

#endif // DOT_H
