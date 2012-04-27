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



void logger_init() {
    logger_config_ptr -> suppress = log_system_NONE;
    logger_config_ptr -> only = log_system_NONE;
    logger_config_ptr -> stopon = log_level_NONE;
}

void logger_setlevel( log_level_t loglevel ) {
    logger_config_ptr -> loglevel = loglevel;
    logger_msg( log_system_LOGGER, log_level_INFO,
                "Logging set to level %i\n",loglevel);
}

void logger_setsystem( log_system_t logsys ) {
    logger_config_ptr -> only = logsys;
    logger_msg( log_system_LOGGER, log_level_INFO,
                "Now logging system %i\n",logsys);
}





void logger_msg( log_system_t logsys, log_level_t loglevel,char *logmsg, ... ) {
    va_list args; 
    int i; 
    char printbuffer[LOGGER_BUFFERSIZE]; 
    
    va_start (args, logmsg); 
    /* Make sure messages are never longer than printbuffer */
    i = vsprintf (printbuffer, logmsg, args); 
    va_end (args); 
    
    if (loglevel >= (logger_config_ptr -> loglevel)) {
        logger_system_filter( logsys, printbuffer );
    }
    
    
  
}

void logger_system_filter( log_system_t logsys, char *logmsg ) {
    log_system_t logsys_iter = 0;
    while ( logsys_iter != log_system_ALL ) {
        printf("Checking system %i\n",logsys_iter);
        if ( logsys == logsys_iter ) {
            printf("%s",logmsg);
        }
        logsys_iter++;
    }
}
