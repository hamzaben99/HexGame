
#include <stdlib.h>
#include <stdio.h>
#include "set_dynamic_struc.h"

////////////////////////////////////////////////////

int find(const int s[], size_t size, int x) {
  if ( (size == 0) || (x <= s[0]) )
    return 0;
  if ( x > s[size-1] )
    return size;
  size_t a = 0;
  size_t b = size-1;
  size_t m = (b+a)/2;
  while ( (a <= b) && (x != s[m]) ) {
    if (x < s[m])
      b = m - 1;
    else
      a = m + 1;
    m = (b+a)/2;
  }
  if (s[m] == x)
    return m;
  return m + 1;
}

void shift_right(int s[], size_t begin, size_t end) {
  for ( size_t k = end+1; k > begin ; --k )
    s[k] = s[k-1];
}

void shift_left(int s[], size_t begin, size_t end) {
  for ( size_t k = begin; k <= end ; ++k )
    s[k-1] = s[k];
}

////////////////////////////////////////////////////

struct set* set__empty() {
  struct set* s = (struct set*) malloc(sizeof(struct set));
  s->s = (int*) malloc(sizeof(int));
  s->s[0] = -1;
  s->size = 0;
  s->capacity = 1;
  return s;
}

int set__is_empty(const struct set* se) {
  return ( se->size == 0 ); 
}

int set__find(const struct set* se, int c) {
  if ( c < 0 )
    return 0;
  size_t k = find( se->s, se->size, c );
  return ( k < se->size && se->s[k] == c );
}

size_t set__size(const struct set* se) {
  return se->size;
}

int set__value(const struct set* se, size_t index) {
  return se->s[index];
}

int set__random_value(struct set* se) {
  if (set__is_empty(se))
    return -1;
  int random_index = rand()%set__size(se);
  return set__value(se,random_index);;
}

int set__add(struct set* se, int c) {
  if ( c < 0 )
    return 1;
  size_t spot = find( se->s, se->size, c );
  if ( spot < se->size && se->s[spot] == c )
    return 1;
  else if ( se->size == 0 ) {
    se->s[0] = c;
    se->size++;
  }
  else {
    if ( se->size == se->capacity ) {
      se->capacity = se->capacity * 2;
      se->s = realloc( se->s, se->capacity*sizeof(int) );
      if ( se->s == NULL )
	return 1;
    }
    shift_right( se->s, spot, se->size-1 );
    se->s[spot]=c;
    se->size++;
  }
  return 0;
}

int set__remove(struct set* se, int c) {
  if ( c < 0 )
    return 1;
  size_t spot = find( se->s, se->size, c );
  if ( ( spot >= se->size || se->s[spot] != c ) )
    return 1;
  shift_left( se->s, spot+1, se->size-1 );
  se->size--;
  if ( (2*se->size < se->capacity) && (se->capacity > 1) ) {  
    se->capacity = se->capacity / 2;
    se->s = realloc( se->s, se->capacity*sizeof(int) );
    if ( se->s == NULL )
      return 1;
  }
  return 0;
}

struct set *set__intersection(struct set *s1, struct set *s2) {
  struct set *set_inter = set__empty();
  int n = set__size(s1);
  int m = set__size(s2);
  for (int i = 0; i < n && i < m; i++) {
    if (n <= m && set__find(s2, s1->s[i])) {
      set__add(set_inter, s1->s[i]);
    }
    if (m < n && set__find(s1, s2->s[i])) {
      set__add(set_inter, s2->s[i]);
    }
  }
  return set_inter;
}

void set__free(struct set* se) {
  free(se->s);
  free(se);
}

////////////////////////////////////////////////////
