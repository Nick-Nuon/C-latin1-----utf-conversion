#include <iconv.h>
#include <assert.h>
#include "helpers.h"
#include "latin1_to_utf32.h"
#include "utf16_to_latin1.h"
#include "utf8_to_latin1.h"

#pragma once

void test_latin_to_UTF32BE() {
  char latin_str[] = "Hello, world!";
  uint32_t utf32_output[13];
  latin_to_UTF32<BIG>(latin_str, 13,utf32_output );

  // Using iconv for conversion
  //assumes the string is in LATIN and output is UTF32BE (big endian)
  iconv_t cd = iconv_open("UTF-32BE", "ISO-8859-1");
  assert(cd != (iconv_t)-1);

  size_t in_bytes = strlen(latin_str);
  size_t out_bytes = in_bytes * sizeof(uint32_t);
  char *in_buf = latin_str;
  uint32_t *out_buf = (uint32_t *)malloc(out_bytes);
  uint32_t *iconv_output = out_buf;

  size_t result = iconv(cd, &in_buf, &in_bytes, (char **)&out_buf, &out_bytes);
  assert(result != (size_t)-1);

  iconv_close(cd);

  for (int i = 0; i < 14; i++) {
 /*printf("utf32_output[%d]: 0x%08X, iconv_output[%d]: 0x%08X\n", i,
           utf32_output[i], i, iconv_output[i]);*/
    assert(utf32_output[i] == iconv_output[i]);
  }

  free(iconv_output);
}

void test_latin_to_UTF32LE() {
  char latin_str[] = "Hello, world!";
  uint32_t utf32_output[13];
  latin_to_UTF32<LITTLE>(latin_str,13, utf32_output);

  // Using iconv for conversion
  //assumes the string is in LATIN and output is UTF32BE (big endian)
  iconv_t cd = iconv_open("UTF-32LE", "ISO-8859-1");
  assert(cd != (iconv_t)-1);

  size_t in_bytes = strlen(latin_str);
  size_t out_bytes = in_bytes * sizeof(uint32_t);
  char *in_buf = latin_str;
  uint32_t *out_buf = (uint32_t *)malloc(out_bytes);
  uint32_t *iconv_output = out_buf;

  size_t result = iconv(cd, &in_buf, &in_bytes, (char **)&out_buf, &out_bytes);
  assert(result != (size_t)-1);

  iconv_close(cd);

  for (int i = 0; i < 14; i++) {
 /*printf("utf32_output[%d]: 0x%08X, iconv_output[%d]: 0x%08X\n", i,
           utf32_output[i], i, iconv_output[i]);*/
    assert(utf32_output[i] == iconv_output[i]);
  }

  free(iconv_output);
}

bool test_UTF32_to_latin() {
    // Test data
    
    /*uint32_t utf32le_input[] = {
        0x00000048, 0x00000065, 0x0000006C, 0x0000006C, 0x0000006F,
        0x0000002C, 0x00000020, 0x00000057, 0x0000006F, 0x00000072,
        0x0000006C, 0x00000064, 0x00000021, 0x12300021 , 0x00000048
    };*/

    //this ensure this is represented *as is* in memory. 
    //No, I did not type this manually: ChatGPT was kind enough to do it for me 
    uint8_t utf8be_input[] = {
        0x00, 0x00, 0x00, 0x48,
        0x00, 0x00, 0x00, 0x65,
        0x00, 0x00, 0x00, 0x6C, 
        0x00, 0x00, 0x00, 0x6C,
        0x00, 0x00, 0x00, 0x6F, 
        0x00, 0x00, 0x00, 0x2C,
        0x00, 0x00, 0x00, 0x20,
        0x00, 0x00, 0x00, 0x57,
        0x00, 0x00, 0x00, 0x6F,
        0x00, 0x00, 0x00, 0x72,
        0x00, 0x00, 0x00, 0x6C, 
        0x00, 0x00, 0x00, 0x64,
        0x00, 0x00, 0x00, 0x21,
        0x12, 0x30, 0x00, 0x21,
        0x00, 0x00, 0x00, 0x48
    };



    //But the underlying architecture does another swap so at the memory level it is:
    //0x48000000 , 0x65000000, etc.
    //size_t len = sizeof(utf32be_input) / sizeof(uint32_t);

    size_t utf8be_input_len = sizeof(utf8be_input) / sizeof(uint8_t);
    size_t utf32be_input_len = utf8be_input_len / 4;
    uint32_t utf32be_input[utf32be_input_len];

    memcpy(utf32be_input, utf8be_input, sizeof(utf8be_input));

    // Expected output
    const char expected_output[] = "Hello, World!*H";

    // Call the UTF32BE_to_latin function
    char output[sizeof(expected_output)] = {0};
    UTF32_to_latin<BIG>(utf32be_input, utf32be_input_len, output);

    // Compare the output with the expected output
    if (memcmp(output, expected_output,utf32be_input_len) != 0) {
        printf("Failed Big Endian UTF32BE => Latin test: expected \"%s\", got \"%s\"\n", expected_output, output);
        return false;
    }

    
    uint8_t utf8le_input[] = {
          0x48, 0x00, 0x00, 0x00,
          0x65, 0x00, 0x00, 0x00,
          0x6C, 0x00, 0x00, 0x00,
          0x6C, 0x00, 0x00, 0x00,
          0x6F, 0x00, 0x00, 0x00,
          0x2C, 0x00, 0x00, 0x00,
          0x20, 0x00, 0x00, 0x00,
          0x57, 0x00, 0x00, 0x00,
          0x6F, 0x00, 0x00, 0x00,
          0x72, 0x00, 0x00, 0x00,
          0x6C, 0x00, 0x00, 0x00,
          0x64, 0x00, 0x00, 0x00,
          0x21, 0x00, 0x00, 0x00,
          0x21, 0x00, 0x30, 0x12,//this should output * to indicate a non-latin character. Placeholder for now.
          0x48, 0x00, 0x00, 0x00
    };

    size_t utf8le_input_len = sizeof(utf8le_input) / sizeof(uint8_t);
    size_t utf32le_input_len = utf8le_input_len / 4;
    uint32_t utf32le_input[utf32le_input_len];

    memcpy(utf32le_input, utf8le_input, sizeof(utf8le_input));

    // Call the UTF32BE_to_latin function
    char output_le[sizeof(expected_output)] = {0};
    UTF32_to_latin<LITTLE>(utf32le_input, utf32le_input_len, output_le);

    // Compare the output with the expected output
    if (memcmp(output, expected_output,utf32le_input_len) != 0) {
        printf("Failed Little Endian UTF32BE => Latin test: expected \"%s\", got \"%s\"\n", expected_output, output_le);
        return false;
    }


    printf("Passed all tests!");
    return true;
}


template<endianness input_endianess>
void test_conversion(const char *example) {
    // Step 1: Convert the original Latin-1 string to UTF-32
    const size_t latin_len = strlen(example);
    uint32_t utf32_output[latin_len];

    latin_to_UTF32<input_endianess>(example, latin_len, utf32_output);

/*
    // Print the content of utf32_output
    printf("utf32_output: [");
    for (size_t i = 0; i < latin_len; i++) {
        printf("U+%04X", utf32_output[i]);
        if (i + 1 < latin_len) {
            printf(", ");
        }
    }
    printf("]\n");*/

    // Step 2: Convert the UTF-32 string back to Latin-1
    char latin_output[latin_len];  
    UTF32_to_latin<input_endianess>(utf32_output, latin_len, latin_output);

    // Step 3: Compare the original Latin-1 string and the converted back string
    //bool success = strcmp(example, latin_output) == 0; <--- I believe this does not work because example is null terminated while latin_output isn't.
    bool success = true;
    for (size_t i = 0; i < strlen(example); i++) {
        
        if (example[i] != latin_output[i]) {
            success = false;
            printf("Mismatch at position %zu: original 0x%02X, converted 0x%02X\n", i, (unsigned char)example[i], (unsigned char)latin_output[i]);
        } /*else {
        printf("Match at position %zu: original 0x%02X, converted 0x%02X\n", i, (unsigned char)example[i], (unsigned char)latin_output[i]);
    }*/
    }

    printf("Test %s: Original: %s, Converted back: %s\n", success ? "PASSED" : "FAILED", example, latin_output);
}

/* template <endianness input_endianess>
void test_utf16_to_latin() {
    // Here is a string in UTF-16 format.
    // For this example, let's use a string without surrogate pairs
    char16_t utf16_str[] = u"Hello,\xD801 World!";

    // The length of the string
    size_t len = sizeof(utf16_str) / sizeof(utf16_str[0]) - 1;  // subtract 1 to exclude the null terminator


    if (!match_system(input_endianess)){
        for (size_t i = 0; i < len; i++) 
              { utf16_str[i] = swap_bytes(utf16_str[i]); }}

    // The buffer to store the output Latin-1 string
    char latin_str[50];

    // Call the function to convert the string
    size_t converted_len = utf16_to_latin<input_endianess>(utf16_str, len, latin_str);

    // Print out the result
    if (converted_len == 0) {
        printf("Conversion failed.\n");
    } else {
        printf("Converted string: %s\n", latin_str);
    }
}
 */

template <endianness input_endianess>
void test_utf16_to_latin(const char16_t *test_str) {
    // Determine the length of the string
    size_t len = 0;
    while (test_str[len] != u'\0') { ++len; }

    // Create a copy of the string to avoid modifying the original string
    char16_t* utf16_str = new char16_t[len + 1];
    memcpy(utf16_str, test_str, (len + 1) * sizeof(char16_t));

    if (!match_system(input_endianess)) {
        for (size_t i = 0; i < len; i++) {
            utf16_str[i] = swap_bytes(utf16_str[i]);
        }
    }

    // The buffer to store the output Latin-1 string
    char latin_str[50];

    // Call the function to convert the string
    size_t converted_len = utf16_to_latin<input_endianess>(utf16_str, len, latin_str);

    // Print out the result
    if (converted_len == 0) {
        printf("Conversion failed.\n");
    } else {
        printf("Converted string: %s\n", latin_str);
    }

    // Free the dynamically allocated memory
    delete[] utf16_str;
}

template <endianness is_big_endian>
void test_latin_to_utf16() {
    // Here is a string in Latin-1 format
    char latin_str[] = "Hello, World!";

    // The length of the string
    size_t len = strlen(latin_str);

    // The buffer to store the output UTF-16 string
    // Note that we need 2 bytes for each Latin-1 character, plus 2 bytes for the null terminator
    char16_t utf16_str[50];

    // Call the function to convert the string
    size_t converted_len = latin_to_utf16<is_big_endian>(latin_str, len, utf16_str);

    // Print out the result
    if (converted_len == 0) {
        printf("Conversion failed.\n");
    } else {
        printf("Converted string (in hex): ");
        for (size_t i = 0; i < converted_len; ++i) {
            printf("%04x ", utf16_str[i]);
        }
        printf("\n");
    }
}





//This doesn't seem to work work as intended
template<endianness endian, typename T>
void test_conversion_with_templates(const char* input, size_t input_len,
                                    size_t (*encode)(const char*, size_t, T*), 
                                    size_t (*decode)(const T*, size_t, char*)) {                                        
    T buffer[input_len * 4];  // Assuming worst case UTF-32

    // Encode
    size_t encoded_size = encode(input, input_len, buffer);

    // Prepare buffer for decoded data
    char decoded_buffer[input_len];

    // Decode
    size_t decoded_size = decode(buffer, encoded_size, decoded_buffer);
}


template<endianness input_endianess>
void test_conversion_utf16(const char *example) {
    // Step 1: Convert the original Latin-1 string to UTF-16
    const size_t latin_len = strlen(example);
    char16_t utf16_output[latin_len];

    latin_to_utf16<input_endianess>(example, latin_len, utf16_output);

    // Step 2: Convert the UTF-16 string back to Latin-1
    char latin_output[latin_len];
    utf16_to_latin<input_endianess>(utf16_output, latin_len, latin_output);

    // Step 3: Compare the original Latin-1 string and the converted back string
    bool success = true;
    for (size_t i = 0; i < strlen(example); i++) {
        if (example[i] != latin_output[i]) {
            success = false;
            printf("Mismatch at position %zu: original 0x%02X, converted 0x%02X\n", i, (unsigned char)example[i], (unsigned char)latin_output[i]);
        }
    }

    printf("Test %s: Original: %s, Converted back: %s\n", success ? "PASSED" : "FAILED", example, latin_output);
}

void test_utf8_to_latin(const char *test_str) {
    // Determine the length of the string
    size_t len = strlen(test_str);

    // Create a copy of the string to avoid modifying the original string
    char* utf8_str = new char[len + 1];
    memcpy(utf8_str, test_str, (len + 1) * sizeof(char));

    // The buffer to store the output Latin-1 string
    char latin_str[50];

    // Call the function to convert the string
    size_t converted_len = UTF8_to_latin(utf8_str, len, latin_str);

    // Print out the result
    if (converted_len == 0) {
        printf("Conversion failed.\n");
    } else {
        printf("Converted string: %s\n", latin_str);
    }

    // Free the dynamically allocated memory
    delete[] utf8_str;
}

