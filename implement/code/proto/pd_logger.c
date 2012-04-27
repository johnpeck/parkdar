/* pd_logger.c
 * 
 * Functions for handling log messages. 
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h> // Allows functions to accept an indefinite number of arguments
#include "pd_logger.h"

// Define a pointer to the logging configuration
logger_config_t logger_config;
logger_config_t *logger_config_ptr = &logger_config;

void logger_start( log_system_t logsys) {
    (logger_config_ptr -> only) = logsys; 
    
    switch( logsys ) {
        case log_system_RANGER:
            printf("Now logging ranger system\n");
            break;
        case log_system_COMMAND:
            printf("Now logging command system\n");
            break;
    }
}

void logger_msg( log_system_t logsys, log_level_t loglevel,char *logmsg, ... ) {
    va_list args; 
    int i; 
    char printbuffer[LOGGER_BUFFERSIZE]; 
    
    va_start (args, logmsg); 
    /* Make sure messages are never longer than printbuffer */
    i = vsprintf (printbuffer, logmsg, args); 
    va_end (args); 
    
    /* Check the level of the logging message.  The ALL loglevel should 
     * cause all messages to be printed */
    switch( loglevel ) {
        case log_level_ALL:
        case log_level_INFO:
            logger_system_filter( logsys, printbuffer );
        case log_level_WARNING:
        case log_level_ERROR:
        case log_level_NONE:
            break;
    }
}

void logger_system_filter( log_system_t logsys, char *logmsg ) {
    log_system_t logsys_iter = 0;
    while ( logsys_iter != log_system_ALL ) {
        printf("Checking system %i\n",logsys_iter);
        if ( logsys == logsys_iter ) {
            printf(logmsg);
        }
        logsys_iter++;
    }
}
