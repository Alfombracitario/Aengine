#pragma once
#ifndef AENGINE_TYPES_H
#define AENGINE_TYPES_H

#include <stdint.h>

// ─── Enteros sin signo ───────────────────────────────────────────────────────
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

// ─── Enteros con signo ───────────────────────────────────────────────────────
typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

// ─── Punto flotante ──────────────────────────────────────────────────────────
typedef float       f32;
typedef double      f64;

// ─── Booleano ────────────────────────────────────────────────────────────────
#include <stdbool.h>  // bool, true, false (C99)

#endif