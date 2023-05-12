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
    test_utf16_to_latin<LITTLE>();

    printf("Test for UTF16_to_latin1: Big Endian:\n");
    test_utf16_to_latin<BIG>();
        


  return 0;
}
