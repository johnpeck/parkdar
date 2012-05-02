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
    return;
}

void logger_blocksystem( log_system_t logsys ) {
    logger_config_ptr -> suppress = logsys;
    logger_msg( log_system_LOGGER, log_level_INFO,
                "Now suppressing system %i log messages.\n",logsys);
    return;
}





void logger_msg( log_system_t logsys, log_level_t loglevel,char *logmsg, ... ) {
    va_list args; 
    int i; 
    char printbuffer[LOGGER_BUFFERSIZE]; 
    
    va_start (args, logmsg); 
    /* Make sure messages are never longer than printbuffer */
    i = vsnprintf (printbuffer, LOGGER_BUFFERSIZE, logmsg, args); 
    va_end (args); 
    
    if (loglevel >= (logger_config_ptr -> loglevel)) {
        /* If this message's level is high enough to be logged, we send
         * it on to be filtered by system. */
        logger_system_filter( logsys, printbuffer );
    }
}

void logger_system_filter( log_system_t logsys, char *logmsg ) {
    log_system_t logsys_iter = 0;
    // Do we need to suppress this message?
    if ((logger_config_ptr -> suppress) != log_system_NONE) {
        // We have a system to suppress
        if ((logger_config_ptr -> suppress) == log_system_ALL) {
            // We need to suppress all messages, so just get out
            return;
        }
        else if ((logger_config_ptr -> suppress) == logsys) {
            // We need to suppress this system's logmessage, so get out
            return;
        }
    }
    // Are we just logging everything?
    if ((logger_config_ptr -> only) != log_system_ALL) {
    
    while ( logsys_iter != log_system_ALL ) {
        if ( logsys == logsys_iter ) {
            /* If the system specified by the log message matches the system
             * set by this iteration, send the message on to the output
             * device. */
            logger_output(logmsg);
        }
        logsys_iter++;
    }
}

void logger_output( char *logmsg ) {
    printf("%s",logmsg);
}
