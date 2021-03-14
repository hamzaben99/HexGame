#ifndef SET_DYNAMIC_STRUC_H
#define SET_DYNAMIC_STRUC_H

#include <stddef.h>
#include "set_dynamic.h"

int find(const int s[], size_t size, int x);

void shift_right(int s[], size_t begin, size_t end);

void shift_left(int s[], size_t begin, size_t end);

#endif
