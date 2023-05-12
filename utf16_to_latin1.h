#include <stddef.h>
#include "helpers.h"

template <endianness big_endian>
inline size_t utf16_to_latin(const char16_t* buf, size_t len, char* latin_output) {
 
 const uint16_t *data = reinterpret_cast<const uint16_t *>(buf);
  size_t pos = 0;
  char* start{latin_output};
  
  while (pos < len) {
    //uint16_t word = !match_system(big_endian) ? utf16::swap_bytes(data[pos]) : data[pos]; --original
        uint16_t word = !match_system(big_endian) ? swap_bytes(data[pos]) : data[pos];
        if((word &0xF800 ) != 0xD800) { //check if the first top five bits
        // No surrogate pair, we just need to make sure that it will fit into 8 bit:
            if((word & 0xFF00) == 0) {
                *latin_output++ = char(word);
            }
            else {
                return 0;
            }
        pos++;
    } else {
      // must be a surrogate pair
/*       uint16_t diff = uint16_t(word - 0xD800);//a surrogate pair's MSB will always be 11011 --- we're only interested in what comes after. 
      if(diff > 0x3FF) { return 0; } //a sanity check to see if its indeed a high surrogate
      if(pos + 1 >= len) { return 0; } // minimal bound checking
      uint16_t next_word = !match_system(big_endian) ? utf16::swap_bytes(data[pos + 1]) : data[pos + 1];
      uint16_t diff2 = uint16_t(next_word - 0xDC00); //check if its indeed a low surrogate
      if(diff2 > 0x3FF) { return 0; } //if not a low surrogate, return
    //char value = (diff << 10) + diff2 + 0x10000; form a  
    //   *latin_output++ = char(value);
      pos += 2; */
      return 0; //based on other code, we just return 0
    }
  }
  return latin_output - start;
}

template <endianness output_endianess>
inline size_t latin_to_utf16(const char* buf, size_t len, char16_t* utf16_output) {
    const uint8_t* data = reinterpret_cast<const uint8_t*>(buf);
    size_t pos = 0;
    char16_t* start{ utf16_output };

    while (pos < len) {
        uint16_t word = uint16_t(data[pos]); // extend Latin-1 char to 16-bit Unicode code point
        *utf16_output++ = match_system(output_endianess) ? word : swap_bytes(word);
        pos++;
    }

    return utf16_output - start;
}
