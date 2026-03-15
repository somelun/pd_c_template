#include "arena.h"

#if TARGET_PLAYDATE
#include "pd_api.h"

#define pd_malloc(s) pd->system->realloc(NULL, (s))
#define pd_free(ptr) pd->system->realloc((ptr), 0)

extern PlaydateAPI* pd;

#define LOG_ERROR(fmt, ...) \
        pd->system->error(fmt, ##__VA_ARGS__)

#else
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define pd_malloc(s) malloc(s)
#define pd_free(ptr) free(ptr)

#define LOG_ERROR(fmt, ...) \
        printf(fmt "\n", ##__VA_ARGS__)

#endif // TARGET_PLAYDATE

void arena_init(MemoryArena* arena, size_t size) {
    arena->buffer = pd_malloc(size);
    if (arena->buffer == NULL) {
        LOG_ERROR("Failed to allocate memory arena of size %zu", size);
        return;
    }

    LOG_ERROR("Allocated %zu KB", size / 1024);

    arena->size = size;
    arena->used = 0;
    arena->high_mark = 0;
}

void arena_free(MemoryArena* arena) {
    if (arena->buffer) {
        pd_free(arena->buffer);
        arena->buffer = NULL;
    }

    LOG_ERROR("Deallocated %zu KB", arena->size / 1024);
    LOG_ERROR("Max used %zu KB", arena->high_mark / 1024);

    arena->size = 0;
    arena->used = 0;
    arena->high_mark = 0;
}

void* arena_alloc(MemoryArena* arena, size_t size) {
    // aligning to 4 bytes
    size_t aligned_size = (size + 3) & ~3;

    if (arena->used + aligned_size > arena->size) {
        LOG_ERROR("Memory arena out of space! Needed %zu bytes, only %zu available", aligned_size, arena->size - arena->used);
        return NULL;
    }

    void* ptr = (char*)arena->buffer + arena->used;
    arena->used += aligned_size;

    // Update high water mark if needed
    if (arena->used > arena->high_mark) {
        arena->high_mark = arena->used;
    }

    memset(ptr, 0, size);
    return ptr;
}

void arena_print(MemoryArena* arena, const char* name) {
    LOG_ERROR("%s Arena Stats:", name);
    LOG_ERROR("  Total Size: %zu bytes", arena->size);
    LOG_ERROR("  Used: %zu bytes (%.1f%%)", arena->used, (double)arena->used / arena->size * 100.0);
    LOG_ERROR("  High Mark: %zu bytes (%.1f%%)", arena->high_mark, (double)arena->high_mark / arena->size * 100.0);
}
