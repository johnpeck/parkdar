/* pd_logger.h
 * 
 * Functions for handling log messages. 
 */
 
 
/* Internal systems recognized by the logger */
typedef enum log_system {
    log_system_RANGER,
    log_system_COMMAND,
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
    log_level_t stopon; // Halt when a message is issued at this level
    log_fptr_t execute; // What to execute
} logger_config_t;

/* Turn logging on. */
void logger_start( log_system_t );

/* Turn logging off */
void logger_stop( log_system_t );

/* Initialize the logging system */ 
void logger_init( logger_config_t ); 

/* Set the log level */
void logger_level( log_level_t );

/* Allow for format strings in the log message */
void logger_msg( log_system_t, log_level_t, char *logmsg, ... ); 
