#ifndef SET_H
#define SET_H

#include <stddef.h>

#define SUCCESS 0

struct set;

struct set* set__empty(void);

int set__is_empty(const struct set* se);

int set__find(const struct set* se, int c);

size_t set__size(const struct set* se);

int set__value(const struct set* se, size_t index);

int set__random_value(struct set* se);

int set__add(struct set* se, int c);

int set__remove(struct set* se, int c);

struct set *set__intersection(struct set *s1, struct set *s2);

void set__free(struct set* se);

#endif

