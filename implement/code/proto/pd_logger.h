/* pd_logger.h
 * 
 * Functions for handling log messages. 
 */

/* Define the maximum log message size */
#define LOGGER_BUFFERSIZE 80
 
/* Internal systems recognized by the logger */
typedef enum log_system {
    log_system_RANGER,
    log_system_COMMAND,
    log_system_LOGGER,
    log_system_NONE,
    log_system_ALL
} log_system_t;

/* Log levels recognized by the logger.  Logging can be configured to
 * only output messages above a certain level, with ALL being the lowest. */
typedef enum log_level {
    log_level_ALL,
    log_level_INFO,
    log_level_WARNING,
    log_level_ERROR,
    log_level_NONE
} log_level_t;

/* Define fpointer_t to have the type "pointer to function," a return
 * value of void, and a parameter type of void. */
typedef void (*log_fptr_t)(void);

/* Logging configuration structure. */
typedef struct logger_config_struct { 
    log_system_t suppress; // Suppress messages from this system
    log_system_t only; // Only display messages from this system
    log_level_t loglevel; // Only display messages at or above this level
    log_level_t stopon; // Halt when a message is issued at this level
} logger_config_t;


/* Initialize the logging system to a set of defaults. */  
void logger_init();

/* Set the log level 
 * Messages with loglevels above this setting will be logged.
 * Set log_level_NONE to turn logging off completely. 
 * Set log level_ALL to log messages with all loglevels. */
void logger_setlevel( log_level_t );

/* Set the system or systems to log.  You can set this to log_system_ALL
 * to log all systems, or to one specific system to log that system
 * only. */
void logger_setsystem( log_system_t );

/* Set the system or systems from which to suppress log messages.  You can
 * set this to log_system_ALL to suppress all log messages, or to one
 * specific system to suppress that system only. */
void logger_blocksystem( log_system_t );

/* The interface to the logging system.  Programs should use this function
 * to send format strings to the logging device.  The actual logging device
 * should be chosen by modifying the logger_output() function. */
void logger_msg( log_system_t, log_level_t, char *logmsg, ... ); 

/* Only print messages meant for the specified system */
void logger_system_filter( log_system_t, char *logmsg );

/* Choose the logging device -- how the log messages will actually be
 * printed out or saved.  During prototyping on the PC, this can just
 * be printf(). */
void logger_output( char *logmsg );
