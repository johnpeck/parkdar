/* pd_numbers.h
 * 
 * Functions for handling numbers. 
 */
#include <stdint.h>
 
/* asc2num() -- Converts an ascii character representing a hexadecimal
 *              number into its integer equivalent.  Accepts both upper
 *              and lower case characters.  Returns 0 for non-numeric
 *              arguments.
 */
uint_8 asc2num(char n_asc);
    
