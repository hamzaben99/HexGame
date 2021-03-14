#ifndef SET_H
#define SET_H

#define SUCCESS 0

struct set;

void cur_start(const struct set* se);
void cur_fwd(const struct set* se);
int cur_val(const struct set* se);
int cur_is_start(const struct set* se);
int cur_is_end(const struct set* se);

struct set* set__empty(void);

int set__is_empty(const struct set* se);

int set__find(const struct set* se, int c);

size_t set__size(const struct set* se);

int set__add(struct set* se, int c);

int set__remove(struct set* se, int c);

int set__rand(struct set* se);

int set__rand_pop(struct set* se);

struct set* set__filter(const struct set* se, int (*filter)(int));

void set__debug_data(const struct set* se, char c);

void set__free(struct set* se);

#endif

