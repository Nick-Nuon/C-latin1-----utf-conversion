#include <stdio.h>
#include <errno.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#include <assert.h>
#include "latin1_to_utf32.h"
#include "tests.h"
#include "helpers.h"




/* Printing stand-in variables:
%c: Prints a single character.
%d: Prints an integer as a signed decimal number.
%f: Prints a floating-point number in fixed-point notation.
%s: Prints a string of characters.
%u: Prints an unsigned integer as a decimal number.
%x or %X: Prints an unsigned integer as a hexadecimal number.
*/



int main(int argc, char **argv) {
  printf("Hello, World!\n");
  printf("This system's endianness is little endian: %d \n  \n", match_system(LITTLE));

  test_latin_to_UTF32BE();
  test_latin_to_UTF32LE();
  test_UTF32_to_latin();


  //::testing::InitGoogleTest(&argc, argv);
  //return RUN_ALL_TESTS();

  const char* examples[10] = {"\xf2\xb1\xa2q\xc1Lo\xca\xcf\xd6\xc0 F\xffT$%",
"G|\xb2i_}\xafk\xebQ\xb1q\xbf@A",
".\xba 8\xbc\xc6\xa1\xf0\xfa\xcd~\xfe\xaaLD\xc0\xe7",
"\xe1Mx\xcf)\xbd\xf0 1\xc7\\\xfe\xa5\xe2 3k",
"}\xc0\xc0&b\xf6 A,\xf2\xe8ge`\xb2G\xab",
"Y\xa1\xfbZ\xa9\xddy\xfb!G\xdb\xaf E\xec\xder",
"3kq|c\xb9~\xad\xfd\xda 3 \xd7WX\xc7",
"\xeb d\xf5*v\xca}\xe4 1\xa9Y\xc9w;\xeb\xae",
"!\xa3u\xba 0\xd4\xfd\xd6 DRL}=\xf6Z-",
"\xf8VU\xdb\xcc~\xfc\xcf\xf6\xddW\xf8\xac\xfd\xe6\xd1"};


    printf("\nTesting conversion back to back: Little Endianness\n");
    for (int i = 0; i < 10; i++) {
        test_conversion<LITTLE>(examples[i]);

    }

printf("\nTesting conversion back to back: Big Endianness\n");
    for (int i = 0; i < 10; i++) {
        test_conversion<BIG>(examples[i]);

    }

    printf("Test for UTF16_to_latin1: Little Endian:\n");
    test_utf16_to_latin<LITTLE>(u"Hello, World!");

    printf("Test for UTF16_to_latin1: Big Endian:\n");
    test_utf16_to_latin<BIG>(u"Hello, World!");


    printf("Test for UTF16_to_latin1: Big Endian, passing along surrogate (this should fail):\n");
    test_utf16_to_latin<BIG>(u"Hello,\xDC01 World!");

    printf("Test for UTF16_to_latin1: Little Endian, passing along surrogate (this should fail):\n");
    test_utf16_to_latin<BIG>(u"Hello,\xDC01 World!");

    printf("Test for latin_to_utf16: Big endian system\n");
    test_latin_to_utf16<BIG>();

        printf("Test for latin_to_utf16: Little Endian system\n");
    test_latin_to_utf16<LITTLE>();


    // test_conversion1<LITTLE>(examples[i],latin_to_UTF32,UTF32_to_latin);


/* 
printf("\nTesting conversion back to back: Big Endianness\n");
    for (int i = 0; i < 10; i++) {
        test_conversion1<BIG>(examples[i]);

    } */


/*
printf("\nTesting conversion back to back: Big Endianness\n");
    for (int i = 0; i < 10; i++) {
        test_conversion1<BIG,latin_to_UTF32,UTF32_to_latin>(examples[i],latin_to_UTF32,UTF32_to_latin);

    }*/


//This doesn't work, complains about type being wrong, no idea why
 /*
const char* test_string = "test string";
test_conversion_with_templates<BIG, char32_t>(test_string, strlen(test_string), latin_to_UTF32<BIG>, UTF32_to_latin<BIG>);
 */
  
    const char* example = "Hello, world!";
    test_conversion_utf16<LITTLE>(example);



    printf("\nTesting conversion Latin1 => UTF16 => Latin1: Little Endianness\n");
    for (int i = 0; i < 10; i++) {
        test_conversion_utf16<LITTLE>(examples[i]);

    }

        printf("\nTesting conversion Latin1 => UTF16 => Latin1: Big Endianness\n");
    for (int i = 0; i < 10; i++) {
        test_conversion_utf16<BIG>(examples[i]);

    }

    printf("\nTesting conversion UTF8 => Latin1: Pure ASCII \n");
        test_utf8_to_latin(u8"Hello Worlddf!!!!"); // pure ASCII should convert

    printf("\nTesting conversion UTF8 => Latin1: 2 byte Code point within acceptable range (129-255) \n");
        test_utf8_to_latin(u8"Hello World \xC3\x98"); // two bytes that denotes a code point within 129-255 range should convert, corresponds to character 0x00D8

    printf("\nTesting conversion UTF8 => Latin1: This should fail  \n");
        test_utf8_to_latin(u8"Hello World\xD8");

    printf("\nTesting conversion Latin1 => UTF8:  \n");
    test_utf8_to_latin("Hello World");


 /* 
printf("\nTesting conversion UTF8 back to back\n");
    for (int i = 0; i < 10; i++) {
        test_conversion_UTF8(examples[i]);

    }
 */

printf("\nTesting conversion UTF8 back to back\n");

test_conversion_UTF8("Hello world");

   return 0;
}
