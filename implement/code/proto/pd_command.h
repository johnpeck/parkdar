/* pd_command.h
 * 
 * Used to set up the parkdar command interface. */
 #include "pd_ranger.h"


// Define the size of the received character buffer
#define RECEIVE_BUFFER_SIZE 20
// Define the size of the parse buffer
#define PARSE_BUFFER_SIZE 20

/* Received command state structure. The idea is that I'll create
 * a structure to keep track of the state in every flow diagram I make.
 * I have a flow diagram for received characters, so I created this
 * structure. */
typedef struct { 
    char rbuffer[RECEIVE_BUFFER_SIZE]; // Received character buffer
    // rbuffer_write_ptr will always point to the next write location
    char *rbuffer_write_ptr;
    // Parse buffer.  pbuffer will point to the beginning of the parse
    // buffer, which will also be the beginning of the command to be
    // processed.
    char pbuffer[PARSE_BUFFER_SIZE];
    char *pbuffer_arg_ptr; // Points to the beginning of the argument
    int rbuffer_count; // Number of chars sent to receive buffer
    int pbuffer_lock; // Parse buffer lock.  1 = locked
} recv_cmd_state_t;


/* Define fpointer_t to have the type "pointer to function," a return
 * value of void, and a parameter type of void. */
typedef void (*fpointer_t)(void);
/* Each command_struct will describe one command */
struct command_struct {
    char *name; // The name of the command
    char *arg_type; // A string representing the type of argument expected
    int arg_max_chars; // The maximum number of characters in the argument
    fpointer_t execute; // The function to execute
    char *help;
};
/* The array of command structures will have global scope.  The variable
 * command_array should be initialized in pd_command.c */
extern struct command_struct command_array[];


/* Making this function explicitly take a pointer to the received command
 * state structure makes it clear that it modifies this structure.  This
 * function will ultimately also have to set up the USART hardware. */
void command_init( recv_cmd_state_t *recv_cmd_state_ptr );





/* check_argsize( pointer to received command state,
 *                pointer to list of commands )
 * Returns 0 if the argument size is less than or equal to the number
 * of characters specified in the command list.  Returns -1 otherwise. */
int check_argsize(recv_cmd_state_t *recv_cmd_state_ptr ,
                  struct command_struct *command_array);


/* process_pbuffer( recv_cmd_state_t *recv_cmd_state_ptr,
 *                  command_struct *commands )
 * Process the command (if there is one) in the parse buffer. */
void process_pbuffer( recv_cmd_state_t *recv_cmd_state_ptr ,
                    struct command_struct *command_array);
                    
/* Erases the received character buffer, resets the received character
 * number, and resets the write pointer. */
void rbuffer_erase( recv_cmd_state_t *recv_cmd_state_ptr );
