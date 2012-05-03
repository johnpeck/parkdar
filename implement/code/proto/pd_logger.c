/* pd_logger.c
 * 
 * Functions for handling log messages. 
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h> // Allows functions to accept an indefinite number of arguments
#include <stdint.h> // Defines uint8_t
#include "pd_logger.h"

// Define a pointer to the logging configuration
logger_config_t logger_config;
logger_config_t *logger_config_ptr = &logger_config;

/* An array of system_structs will contain our system definitions for
 * logging. */
struct system_struct system_array[] ={
    // The logger system
    {"logger", // Name
    0 // Bitshift -- bit location in logging configuration bitfields
    },
    // The command system
    {"command",
    1
    },
    // End of table indicator.  Must be last.
    {"",0}
};

void logger_init() {
    logger_config_ptr -> enable = 0xff; /* Logs from all systems enabled
                                         * by default. */
    logger_config_ptr -> loglevel = log_level_ALL;
}

void logger_setlevel( log_level_t loglevel ) {
    logger_config_ptr -> loglevel = loglevel;
    logger_msg( "logger", log_level_INFO,
                "Logging set to level %i\n",loglevel);
}

void logger_setsystem( char *logsys ) {
    struct system_struct *system_array_ptr = system_array;
    // Go through all systems looking for a match to the system name
    while (strcmp( system_array_ptr -> name, "" ) != 0) {
        if (strcmp( logsys, system_array_ptr -> name ) == 0) {
            // We've found a matching system
            (logger_config_ptr -> enable) |= (1<< (system_array_ptr -> bitshift));
            logger_msg("logger", log_level_INFO, 
                "Now logging system %s\n", system_array_ptr -> name);
            break;
        }
        system_array_ptr++;
    }
    return;
}

void logger_disable() {
    logger_config_ptr -> enable = 0;
    return;
}

void logger_msg( char *logsys, log_level_t loglevel,char *logmsg, ... ) {
    va_list args; 
    uint8_t i; 
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

void logger_system_filter( char *logsys, char *logmsg ) {
    char sysname[LOGGER_BUFFERSIZE];
    struct system_struct *system_array_ptr = system_array;
    // Go through all systems looking for a match to the system name
    while (strcmp( system_array_ptr -> name, "" ) != 0) {
        if (strcmp( logsys, system_array_ptr -> name ) == 0) {
            // We've found a matching system
            if ((logger_config_ptr -> enable) & 
                (1<< (system_array_ptr -> bitshift))) {
                /* The system is enabled for logging.  Send two strings
                 * to the logging device: 
                 * 1. (name of the system)
                 * 2. Log message */
                snprintf(sysname,LOGGER_BUFFERSIZE,"(%s) ",
                    system_array_ptr -> name);
                logger_output(sysname);
                logger_output(logmsg);
            break;
            }
        }
        system_array_ptr++;
    }
    return;
}


void logger_output( char *logmsg ) {
    printf("%s",logmsg);
}
