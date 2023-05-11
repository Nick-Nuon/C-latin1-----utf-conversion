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


  return 0;
}
