#include <stdarg.h> // Allows functions to accept an indefinite number of arguments

int usart_printf (const char *fmt, ...) { 
   va_list args; 
   uint i; 
   char printbuffer[64]; 

   va_start (args, fmt); 

   /* For this to work, printbuffer must be larger than 
    * anything we ever want to print. 
    */ 
   i = vsprintf (printbuffer, fmt, args); 
   va_end (args); 

   /* Print the string */ 
   print(printbuffer); 
   return 0; 
} 
