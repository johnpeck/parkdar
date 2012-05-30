/* pd_ascii.c
 * 
 * Functions for handling ASCII characters. 
 */

#include <ctype.h>  // Provides tolower()
#include "pd_ascii.h"

void lowstring(char *mixstring) {
    while (*mixstring != '\0') {
        *mixstring = tolower(*mixstring);
        mixstring++;
    }
}
