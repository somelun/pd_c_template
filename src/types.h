#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef int8_t    i8;   // signed 8-bit integer
typedef int16_t   i16;  // signed 16-bit integer
typedef int32_t   i32;  // signed 32-bit integer
typedef int64_t   i64;  // signed 64-bit integer
typedef uint8_t   u8;   // unsigned 8-bit integer
typedef uint16_t  u16;  // unsigned 16-bit integer
typedef uint32_t  u32;  // unsigned 32-bit integer

#define FIXED_SHIFT 16
#define FLOAT_TO_FIXED(x) ((i32)((x) * (1 << FIXED_SHIFT)))
#define FIXED_TO_INT(x) ((x) >> FIXED_SHIFT)
#define FIXED_MUL(a, b) (((i64)(a) * (b)) >> FIXED_SHIFT)

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define BYTES_PER_ROW 52

#define CENTER_X SCREEN_WIDTH / 2;
#define CENTER_Y SCREEN_HEIGHT / 2;

#endif // _TYPES_H_
