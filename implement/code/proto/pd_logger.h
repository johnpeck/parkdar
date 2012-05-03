/* pd_logger.h
 * 
 * Functions for handling log messages. 
 */
#include <stdint.h> // Defines uint8_t

/* Define the maximum log message size */
#define LOGGER_BUFFERSIZE 80
 
/* Each system_struct will describe one system */
struct system_struct {
    char *name; // The name of the system
    uint8_t bitshift; // The system's location in the enable bitfield
};

/* Log levels recognized by the logger.  Logging can be configured to
 * only output messages above a certain level, with ALL being the lowest. */
typedef enum log_level {
    log_level_ALL,
    log_level_INFO,
    log_level_WARNING,
    log_level_ERROR,
    log_level_NONE
} log_level_t;

/* Logging configuration structure. */
typedef struct logger_config_struct { 
    uint8_t enable; /* Bitfield in which each bit enables or disables
                      * log messages from the system defined by an array
                      * of system_struct */
    log_level_t loglevel; // Only display messages at or above this level
} logger_config_t;


/* Initialize the logging system to a set of defaults. */  
void logger_init();

/* Set the log level 
 * Messages with loglevels above this setting will be logged.
 * Set log_level_NONE to turn logging off completely. 
 * Set log level_ALL to log messages with all loglevels. */
void logger_setlevel( log_level_t loglevel );

/* Set the system to log.  Find the list of system names defined in an
 * array of system_struct.  To log multiple systems, call logger_disable(),
 * then call this function for each system you'd like to log. */
void logger_setsystem( char *logsys );

/* Turn off all logging. */
void logger_disable();

/* The interface to the logging system.  Programs should use this function
 * to send format strings to the logging device.  The actual logging device
 * should be chosen by modifying the logger_output() function. */
void logger_msg( char *logsys, log_level_t loglevel, char *logmsg, ... ); 

/* Only print messages meant for the specified system */
void logger_system_filter( char *logsys, char *logmsg );

/* Choose the logging device -- how the log messages will actually be
 * printed out or saved.  During prototyping on the PC, this can just
 * be printf(). */
void logger_output( char *logmsg );
