#include <stddef.h>
#include "helpers.h"

inline size_t UTF8_to_latin(const char* buf, size_t len, char* latin_output) {
 const uint8_t *data = reinterpret_cast<const uint8_t *>(buf);
  size_t pos = 0;
  char* start{latin_output};

  while (pos < len) {
    // try to convert the next block of 16 ASCII bytes
    if (pos + 16 <= len) { // if it is safe to read 16 more bytes, check that they are ascii
      uint64_t v1;
      ::memcpy(&v1, data + pos, sizeof(uint64_t));
      uint64_t v2;
      ::memcpy(&v2, data + pos + sizeof(uint64_t), sizeof(uint64_t));
      uint64_t v{v1 | v2}; //We're only interested in these bits: 1000 1000 1000 1000, so it makes sense to concatenate everything
      if ((v & 0x8080808080808080) == 0) { //if NONE of these are set, e.g. all of them are zero, then everything is ASCII
        size_t final_pos = pos + 16;
        while(pos < final_pos) {
          *latin_output++ = char(buf[pos]);
          pos++;
        }
        continue;
      }
    }
    //suppose it isn't an all ASCII byte
    uint8_t leading_byte = data[pos]; // leading byte
    if (leading_byte < 0b10000000) {
      // converting one ASCII byte !!!
      *latin_output++ = char(leading_byte);
      pos++;
    } else if ((leading_byte & 0b11100000) == 0b11000000) {//the first three bits indicate:
      // We have a two-byte UTF-8
      if(pos + 1 >= len) { return 0; } // minimal bound checking
      if ((data[pos + 1] & 0b11000000) != 0b10000000) { return 0; }// checks if the next byte is a valid continuation byte in UTF-8. A valid continuation byte starts with 10.
      // range check -
      uint32_t code_point = (leading_byte & 0b00011111) << 6 | (data[pos + 1] & 0b00111111);//assembles the Unicode code point from the two bytes. It does this by discarding the leading 110 and 10 bits from the two bytes, shifting the remaining bits of the first byte, and then combining the results with a bitwise OR operation.
      if (code_point < 0x80 || 0xFF < code_point) { return 0; } //We only care about the range 129-255 which is Non-ASCII latin1 characters
      *latin_output++ = char(code_point); 
      pos += 2;
    } else if ((leading_byte & 0b11110000) == 0b11100000) {
      // We have a three-byte UTF-8
      if(pos + 2 >= len) { return 0; } // minimal bound checking

      if ((data[pos + 1] & 0b11000000) != 0b10000000) { return 0; }
      if ((data[pos + 2] & 0b11000000) != 0b10000000) { return 0; }
      // range check
/*       uint32_t code_point = (leading_byte & 0b00001111) << 12 |
                   (data[pos + 1] & 0b00111111) << 6 |
                   (data[pos + 2] & 0b00111111);
      if (code_point < 0x800 || 0xffff < code_point ||
          (0xd7ff < code_point && code_point < 0xe000)) {
        return 0;
      }
      *latin_output++ = char(code_point); */
      pos += 3;
    } else if ((leading_byte & 0b11111000) == 0b11110000) { // 0b11110000
      // we have a 4-byte UTF-8 word.
      if(pos + 3 >= len) { return 0; } // minimal bound checking
      if ((data[pos + 1] & 0b11000000) != 0b10000000) { return 0; }
      if ((data[pos + 2] & 0b11000000) != 0b10000000) { return 0; }
      if ((data[pos + 3] & 0b11000000) != 0b10000000) { return 0; }

      // range check
/* 
      uint32_t code_point =
          (leading_byte & 0b00000111) << 18 | (data[pos + 1] & 0b00111111) << 12 |
          (data[pos + 2] & 0b00111111) << 6 | (data[pos + 3] & 0b00111111);
      if (code_point <= 0xffff || 0x10ffff < code_point) { return 0; }
      *latin_output++ = char(code_point); */
      pos += 4;
    } else {
      return 0;
    }
  }
  return latin_output - start;
}


/* size_t latin_to_UTF8(const char *latin_str,  size_t len, char *utf8_output) {
    size_t output_index = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char uchar_value = static_cast<unsigned char>(latin_str[i]);
        if (uchar_value <= 0x7F) {
            // Characters in the range 0x00-0x7F are identical in Latin-1 and UTF-8.
            utf8_output[output_index++] = uchar_value;
        } else {
            // Characters in the range 0x80-0xFF need to be converted to two-byte UTF-8 sequences.
            utf8_output[output_index++] = 0xC0 | (uchar_value >> 6);  // First byte: 110xxxxx
            utf8_output[output_index++] = 0x80 | (uchar_value & 0x3F); // Second byte: 10xxxxxx
        }
    }
    return output_index; // Return the length of the output string
} */

inline size_t latin_to_UTF8(const char* buf, size_t len, char* utf8_output) {
  const uint32_t *data = reinterpret_cast<const uint32_t *>(buf);
  size_t pos = 0;
  char* start{utf8_output};
  while (pos < len) {
    // try to convert the next block of 2 ASCII characters
    if (pos + 2 <= len) { // if it is safe to read 8 more bytes, check that they are ascii
      uint64_t v;
      ::memcpy(&v, data + pos, sizeof(uint64_t));
      if ((v & 0xFFFFFF80FFFFFF80) == 0) {
        *utf8_output++ = char(buf[pos]);
				*utf8_output++ = char(buf[pos+1]);
        pos += 2;
        continue;
      }
    }
    uint32_t word = data[pos];
    if((word & 0xFFFFFF80)==0) {
      // will generate one UTF-8 bytes
      *utf8_output++ = char(word);
      pos++;
    } else if((word & 0xFFFFFF00)==0) {
      // will generate two UTF-8 bytes
      // we have 0b110XXXXX 0b10XXXXXX
      //Example: suppose we have 
      *utf8_output++ = char((word>>6) | 0b11000000); //
      *utf8_output++ = char((word & 0b111111) | 0b10000000); //
      pos++;
    } else {
        //nothing happens

    }

    /* if((word & 0xFFFF0000)==0) {
      // will generate three UTF-8 bytes
      // we have 0b1110XXXX 0b10XXXXXX 0b10XXXXXX
			if (word >= 0xD800 && word <= 0xDFFF) { return 0; }
      *utf8_output++ = char((word>>12) | 0b11100000);
      *utf8_output++ = char(((word>>6) & 0b111111) | 0b10000000);
      *utf8_output++ = char((word & 0b111111) | 0b10000000);
      pos++;
    } else {
      // will generate four UTF-8 bytes
      // we have 0b11110XXX 0b10XXXXXX 0b10XXXXXX 0b10XXXXXX
			if (word > 0x10FFFF) { return 0; }
      *utf8_output++ = char((word>>18) | 0b11110000);
      *utf8_output++ = char(((word>>12) & 0b111111) | 0b10000000);
      *utf8_output++ = char(((word>>6) & 0b111111) | 0b10000000);
      *utf8_output++ = char((word & 0b111111) | 0b10000000);
      pos ++;
    } */
  }
  return utf8_output - start;
}