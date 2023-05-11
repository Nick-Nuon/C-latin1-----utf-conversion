#include <iconv.h>
#include <assert.h>
#include "helpers.h"
#include "latin1_to_utf32.h"

#pragma once

void test_latin_to_UTF32BE() {
  char latin_str[] = "Hello, world!";
  uint32_t utf32_output[13];
  latin_to_UTF32<BIG>(latin_str, utf32_output, 13);

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
  latin_to_UTF32<LITTLE>(latin_str, utf32_output, 13);

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
    //No, I did not type this horror: ChatGPT was kind enough to do it for me 
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
    UTF32BE_to_latin<BIG>(utf32be_input, utf32be_input_len, output);

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
    UTF32BE_to_latin<LITTLE>(utf32le_input, utf32le_input_len, output_le);

    // Compare the output with the expected output
    if (memcmp(output, expected_output,utf32le_input_len) != 0) {
        printf("Failed Little Endian UTF32BE => Latin test: expected \"%s\", got \"%s\"\n", expected_output, output_le);
        return false;
    }


    printf("Passed all tests!");
    return true;
}