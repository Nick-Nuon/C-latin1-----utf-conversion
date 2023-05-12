#include <stddef.h>
#include <stdint.h>
#include <string.h>
#pragma once

#define SIMDUTF_IS_BIG_ENDIAN (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)

enum encoding_type {
        UTF8 = 1,       // BOM 0xef 0xbb 0xbf
        UTF16_LE = 2,   // BOM 0xff 0xfe
        UTF16_BE = 4,   // BOM 0xfe 0xff
        UTF32_LE = 8,   // BOM 0xff 0xfe 0x00 0x00
        UTF32_BE = 16,   // BOM 0x00 0x00 0xfe 0xff

        unspecified = 0
};

enum endianness {
        LITTLE,
        BIG
};


bool match_system(enum endianness e) {
#if SIMDUTF_IS_BIG_ENDIAN
    return e == endianness::BIG;
#else
    return e == endianness::LITTLE;
#endif
}


uint16_t swap_bytes(uint16_t val) {
  return (val << 8) | (val >> 8);
}

uint32_t swap_bytes_32(uint32_t val) {
  return ((val << 24) & 0xFF000000) |
         ((val << 8) & 0x00FF0000) |
         ((val >> 8) & 0x0000FF00) |
         ((val >> 24) & 0x000000FF);
}

