/* COPYRIHGT (C) HARRY CLARK 2024 */

/* HARRY CLARK'S COMMON UTILITY LIBRARY */

/* THIS FILE IS ABOUT COMMON DATA TYPES USED THROUGHOUT THE PROJECT */
/* CREATING TYPE DEFINED METHODS TO INSTANTIATE RAW POINTERS FOR MY */
/* METHODS, FUNCTIONS, ETC */

#ifndef COMMON
#define COMMON

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifndef UNSIGNED_TYPES
#define UNSIGNED_TYPES

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef size_t UNK;

typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned short USHORT;

#endif 

#ifndef SIGNED_TYPES
#define SIGNED_TYPES

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

#endif

#ifndef FLOATING_POINT
#define FLOATING_POINT

typedef float F32;
typedef double F64;
typedef volatile float VF32;
typedef volatile double VF64;

#endif

#define     STATIC      static
#define     INLINE      inline

#ifndef ASM_UNUSED
#define ASM_UNUSED __attribute__((unused))
#else
#define ASM_UNUSED
#endif

#define     PACKED_ALIGNED      __attribute__((packed, aligned(4)))

#define KB_TO_BYTES      1024
#define MB_TO_BYTES      (1024 * 1024)
#define GB_TO_BYTES      (1024 * 1024 * 1024)

#define FORMAT_SIZE(SIZE) \
    ((SIZE) >= GB_TO_BYTES ? (SIZE)/GB_TO_BYTES : \
     (SIZE) >= MB_TO_BYTES ? (SIZE)/MB_TO_BYTES : \
     (SIZE) >= KB_TO_BYTES ? (SIZE)/KB_TO_BYTES : (SIZE))

#define FORMAT_UNIT(SIZE) \
    ((SIZE) >= GB_TO_BYTES ? "GB" : \
     (SIZE) >= MB_TO_BYTES ? "MB" : \
     (SIZE) >= KB_TO_BYTES ? "KB" : "B")

#endif
