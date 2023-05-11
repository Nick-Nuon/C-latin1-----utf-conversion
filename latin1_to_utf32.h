#include <stddef.h>
#include <stdint.h>
#include "helpers.h"
#pragma once


//this takes a latin1 string assumed to be in LITTLE format, and then output a UTF32 string,also in LITTLE FORMAT
template<endianness input_endianess>
void latin_to_UTF32(const char *latin_str, uint32_t *utf32_output, 
                    size_t utf32_len) {
  // Add BOM at the beginning of the UTF-32 string
  // utf32_output[0] = 0xFEFF;

  // Convert each character from Latin-1 to UTF-32
  for (size_t i = 0; i < utf32_len; i++) {
    unsigned char uchar_value = static_cast<unsigned char>(latin_str[i]);
    (!match_system(input_endianess)) ? utf32_output[i] = swap_bytes_32((uint32_t)uchar_value) : utf32_output[i] = (uint32_t)uchar_value; 
  }

/*
  for (size_t i = 0; i < utf32_len; i++) {
    printf("0x%08X ", utf32_output[i]);
  }*/
  
  printf("\n");
}

template<endianness input_endianess>
void UTF32_to_latin(const uint32_t *UTF32_str, size_t len, char *output) {
    uint32_t utf32_char;
    uint8_t latin_char;

    for (size_t i = 0; i < len; i++) {

        (!match_system(input_endianess)) ? utf32_char = swap_bytes_32((uint32_t)UTF32_str[i]) : utf32_char = (uint32_t)UTF32_str[i]; 
        /*printf("UTF32 hex: %08X\n", utf32_char);*/
        //printf("check: %X\n",(utf32_char & 0x00FFFFFF) == 0);

        
        //here you can use everything simply as hexadecimal without worrying about the switch C++ <=> memory
        if ((utf32_char & 0xFFFFFF00) == 0){ // Check if the character can be represented in Latin-1
        //if (utf32_char <= 0xFF) { 
            output[i] = (uint8_t)(utf32_char & 0xFF);
            /*printf("Output:%X\n", output[i]);*/
        } else {
            printf("caught a character not in range: %02x \n",utf32_char);
            output[i] = '*'; // Replace non-Latin-1 characters with a placeholder
        }
    }
}

