#ifndef _ARENA_H_
#define _ARENA_H_

#include "types.h"

typedef struct MemoryArena MemoryArena;
struct MemoryArena {
  void* buffer;       // the actual memory buffer
  size_t size;        // total size of the buffer
  size_t used;        // how much has been used so far
  size_t high_mark;   // high water mark for stats/debugging
};

void arena_init(MemoryArena* arena, size_t size);
void arena_free(MemoryArena* arena);

void* arena_alloc(MemoryArena* arena, size_t size);

void arena_print(MemoryArena* arena, const char* name);

#endif // _ARENA_H_
