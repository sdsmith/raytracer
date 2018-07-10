#pragma once

// Packed attribute
//
#if defined(__GNUC__) // __GNUG__
#   define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#else
#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_FULL_VER)
#   define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#else
#   error "Cannot determine packed attribute: unknown compiler"
#endif
#endif