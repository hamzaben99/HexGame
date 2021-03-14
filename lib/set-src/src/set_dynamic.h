#ifndef SET_DYNAMIC_H
#define SET_DYNAMIC_H

struct set {
  int* s;
  size_t capacity;
  size_t size;
};

#include "../include/set.h"

#endif
