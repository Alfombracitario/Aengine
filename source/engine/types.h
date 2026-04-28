#pragma once
#ifndef AENGINE_TYPES_H
#define AENGINE_TYPES_H
#include "engine/platform.h"

#if CURRENT_PLATFORM == PLATFORM_SNES
#include <snes.h>
#else
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   // NULL, size_t

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;

typedef float       f32;

#if platformHas64BitTypes
typedef uint64_t    u64;
typedef int64_t     s64;
#endif

#if platformHasDouble
typedef double      f64;
#endif
#endif
#endif