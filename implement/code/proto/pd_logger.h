/* pd_logger.h
 * 
 * Functions for handling log messages. 
 */
 
 
 /* Logging configuration structure. */
typedef struct logging_config_struct { 
    enum enable {OFF, ON}; // Global logging switch
} logging_config_t;

/* Internal systems recognized by the logger */
typedef enum {
    RANGER,
    COMMAND
} log_system_t;
 
/* Define fpointer_t to have the type "pointer to function," a return
 * value of void, and a parameter type of void. */
typedef int (*log_fptr_t)(); 

/* Turn logging on. */
void startlogging();

/* Turn logging off */
void stoplogging();
 
 
