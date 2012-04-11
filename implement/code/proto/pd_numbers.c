#include <stdint.h>
#include "pd_numbers.h"

/* asc2num() -- Converts an ascii character representing a hexadecimal
 *              number into its integer equivalent.  Accepts both upper
 *              and lower case characters.  Returns 0 for non-numeric
 *              arguments.
 */
uint_8 asc2num(char n_asc) {
    uint_8 n_int = 0;
    n_int = n_asc - 0x30; // Subtract off non-numeric beginning of table
    if ( n_int > 0x09 ) {
        // Input number is 0xa or larger.  Need to subtract off the 
        // non-numeric interval between 9 and A in the table.
        n_int -= 0x07;
    }
    if ( n_int > 0x0f ) {
        // Input number is lower case.  Need to subtract off the difference
        // between lower and upper cases.
        n_int -= 0x20;
    }
    if ( n_int > 0xf ) {
        // If the integer is still too big, this is an invalid entry.
        n_int = 0x00;
    }
    return n_int;
}
        
