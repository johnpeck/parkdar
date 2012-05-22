/* pd_logger.h
 * 
 * Functions for handling log messages. 
 */
#include <stdint.h> // Defines uint8_t

/* Define the maximum log message size */
#define LOGGER_BUFFERSIZE 80
 
/* Each system_struct will describe one system.  Create an array of these
 * to define all systems recognized by the machine.  Each system can have
 * a unique bit in the logger configuration bitfield.  This bit controls
 * whether or not messages from that system will be printed. */
typedef struct system_struct {
    char *name; // The name of the system
    uint8_t bitshift; // The system's location in the enable bitfield
} logger_system_t;

/* Log levels recognized by the logger.  Log messages must be tagged with
 * one of these levels.  The messages will be sent to the output device
 * if their level is at or above the logger's threshold. 
 * 
 * Use logger_setlevel() to set the level threshold. 
 */
typedef enum log_level {
    log_level_INFO,
    log_level_WARNING,
    log_level_ERROR
} logger_level_t;

/* Logging configuration structure. 
 */
typedef struct logger_config_struct { 
    uint8_t enable; /* Bitfield in which each bit enables or disables
                      * log messages from the system defined by an array
                      * of system_struct */
    logger_level_t loglevel; // Only display messages at or above this level
} log_config_t;


/* Initialize the logging system to a set of defaults. 
 */  
void logger_init( void );

/* Set the log level 
 * Messages with loglevels at or above this setting will be sent to the
 *     output device.
 */
void logger_setlevel( logger_level_t loglevel );

/* Enable a system for logging.  This sets a bit in the logging configuration
 * structure's enable bitfield.  
 * 
 * To log multiple systems, call logger_disable(), then call this function 
 * for each system you'd like to log. 
 */
void logger_setsystem( char *logsys );

/* Turn off all logging. */
void logger_disable( void );

/* The interface to the logging system.  Use this function to send log
 * messages.  
 * 
 * logsys is a freeform string that must match one of the system name
 *     strings defined in an array of system structures.
 * 
 * loglevel is an enum defined above. 
 * 
 * logmsg is a format string -- the log message payload.
 */
void logger_msg( char *logsys, logger_level_t loglevel, char *logmsg, ... );

/* The same as logger_msg, but called with a string located in flash
 * memory.
 */
void logger_msg_p( char *logsys, logger_level_t loglevel,const char *logmsg, ... );

/* Called by logger_msg.  Filters out messages not enabled for logging.
*/
void logger_system_filter( char *logsys, char *logmsg );

/* Sends the final log message to the output device.  This function makes
 * the output device more modular.  The output chosen in the implementation
 * can be simply printf() for prototyping on a PC.
 */
void logger_output( char *logmsg );
