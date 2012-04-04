#include <stdio.h>
#include <string.h>
#include "recv_char_flow.h"


#define NUMCOMMANDS 5
// Define the size of the received character buffer
#define RECEIVE_BUFFER_SIZE 20
// Define the size of the parse buffer
#define PARSE_BUFFER_SIZE 20

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

/* An array of command_structs will contain our remote commands */
struct command_struct command_array[] ={
    // The junk function
    {"junk", // Name
    "hex", // Argument type
    2, // Maximum number of characters in argument
    &junkfunc,
    "Some junk"},
    // The crap function
    {"crap",
    "none",
    0,
    &crapfunc,
    "Some crap"},
    // End of table indicator.  Must be last.
    {"","",0,0,""}
};




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
// Define a pointer to the state
recv_cmd_state_t  recv_cmd_state, *recv_cmd_state_ptr = &recv_cmd_state;

void junkfunc(void) {
    printf("This is all junk!\r\n");
    return;
}

void crapfunc(void) {
    printf("This is all crap!\r\n");
    return;
}


/* Making this function explicitly take a pointer to the received command
 * state structure makes it clear that it modifies this structure.  This
 * function will ultimately also have to set up the USART hardware. */
void usart_init( recv_cmd_state_t *recv_cmd_state_ptr ) {
    memset((recv_cmd_state_ptr -> rbuffer),0,RECEIVE_BUFFER_SIZE);
    recv_cmd_state_ptr -> rbuffer_write_ptr =
        recv_cmd_state_ptr -> rbuffer; // Initialize write pointer
    memset((recv_cmd_state_ptr -> pbuffer),0,PARSE_BUFFER_SIZE);
    recv_cmd_state_ptr -> pbuffer_arg_ptr =
        recv_cmd_state_ptr -> pbuffer; // Initialize argument pointer
    recv_cmd_state_ptr -> rbuffer_count = 0;
    recv_cmd_state_ptr -> pbuffer_lock = 0; // Parse buffer unlocked
    return;
}

/* check_argsize( pointer to received command state,
 *                pointer to list of commands )
 * Returns 0 if the argument size is less than or equal to the number
 * of characters specified in the command list.  Returns -1 otherwise. */
int check_argsize(recv_cmd_state_t *recv_cmd_state_ptr ,
                    struct command_struct *command_array) {
    int isok = 0;
    printf("Checking argument of %s...\r\n", command_array -> name);
    int argsize = strlen(recv_cmd_state_ptr -> pbuffer_arg_ptr);
    printf("Argument size is %d\r\n", argsize);
    if (argsize > (command_array -> arg_max_chars)) {
        printf("Argument is bigger than the specified %d!\r\n",
               command_array -> arg_max_chars);
        isok = -1;
    }
    return isok;
}

/* Erases the received character buffer, resets the received character
 * number, and resets the write pointer. */
void rbuffer_erase( recv_cmd_state_t *recv_cmd_state_ptr ) {
    memset((recv_cmd_state_ptr -> rbuffer),0,RECEIVE_BUFFER_SIZE);
    recv_cmd_state_ptr -> rbuffer_write_ptr =
        recv_cmd_state_ptr -> rbuffer; // Initialize write pointer
    recv_cmd_state_ptr -> rbuffer_count = 0;
    return;
}

/* receive_isr_proto(char)
 * This mocks up the receive character interrupt of the AVR */
void receive_isr_proto( recv_cmd_state_t *recv_cmd_state_ptr,
                         char *inchar) {
    if (!strcmp(inchar,"\r")) {
        printf("Found a terminator\r\n");
        if ((recv_cmd_state_ptr -> rbuffer_count) == 0) {
            return;
        }
        else {
            if ((recv_cmd_state_ptr -> pbuffer_lock) == 1) {
                printf("Command process speed error!\r\n");
                rbuffer_erase(recv_cmd_state_ptr);
                return;
            }
            else {
                strcpy((recv_cmd_state_ptr -> pbuffer),
                    (recv_cmd_state_ptr -> rbuffer));
                recv_cmd_state_ptr -> pbuffer_lock = 1;
                printf("Parse buffer contains %s\r\n",
                    (recv_cmd_state_ptr -> pbuffer));
                return;
            }
        }
    }
    else {
        printf("  <-- Not a terminator.  Received count is %d\r\n",
            recv_cmd_state_ptr -> rbuffer_count);
        if ((recv_cmd_state_ptr -> rbuffer_count) >=
            (RECEIVE_BUFFER_SIZE-1)) {
            printf("Received character number above limit.\r\n");
            rbuffer_erase(recv_cmd_state_ptr);
            return;
        }
        else {
            // Write the character to the received character buffer
            strcpy((recv_cmd_state_ptr -> rbuffer_write_ptr),inchar);
            // Increment the received character count
            (recv_cmd_state_ptr -> rbuffer_count)++;
            // Increment the write pointer
            (recv_cmd_state_ptr -> rbuffer_write_ptr)++;
        }
    }
    return;
}

/* process_pbuffer( recv_cmd_state_t *recv_cmd_state_ptr,
 *                  command_struct *commands )
 * Process the command (if there is one) in the parse buffer. */
void process_pbuffer( recv_cmd_state_t *recv_cmd_state_ptr ,
                    struct command_struct *command_array) {
    
    if ((recv_cmd_state_ptr -> pbuffer_lock) == 1) {
        // Parse buffer is locked -- there's a command to process
        printf("Parse buffer is locked\r\n");
        recv_cmd_state_ptr -> pbuffer_arg_ptr = strchr(recv_cmd_state_ptr -> pbuffer,' ');
        if (recv_cmd_state_ptr -> pbuffer_arg_ptr != NULL) {
            // Parse buffer contains a space -- there's an argument
            printf("The command contains a space\r\n");
            *(recv_cmd_state_ptr -> pbuffer_arg_ptr) = '\0'; // Terminate the command string
            (recv_cmd_state_ptr -> pbuffer_arg_ptr)++;
            while (*(recv_cmd_state_ptr -> pbuffer_arg_ptr) == ' ') {
                (recv_cmd_state_ptr -> pbuffer_arg_ptr)++; // Move to first non-space character
            }
            // arg_ptr now points to the beginning of the parameter
            printf("The parameter is %s\r\n",(recv_cmd_state_ptr -> pbuffer_arg_ptr));
        }
        // Look through the command list for a match
        int pbuffer_match = 0;
        while ((command_array -> execute) != 0) {
            printf("Looking at command %s\r\n",
                command_array -> name);
            if (strcmp( recv_cmd_state_ptr -> pbuffer,
                command_array -> name ) == 0) {
                // We've found a matching command
                printf("Match found \r\n");
                pbuffer_match = 1;
                if (strcmp( command_array -> arg_type, "none") != 0) {
                    // The command is specified to have an argument
                    int arg_ok = check_argsize(recv_cmd_state_ptr,command_array);
                    if (arg_ok != 0) {
                        printf("ERROR: Argument to %s is out of range.\r\n",
                               command_array -> name);
                        }
                    else {
                        command_array -> execute();
                    }
                }
                else  {
                    // There's no argument specified
                    if (recv_cmd_state_ptr -> pbuffer_arg_ptr != NULL) {
                        // There's an argument, but we didn't expect one
                        printf("WARNING: Ignoring argument\r\n");
                    }
                    command_array -> execute();
                }
                recv_cmd_state_ptr -> pbuffer_lock = 0;
                break;
            }
            command_array++;
        }
        // If we didn't find a match, send an error message
        if (pbuffer_match == 0) {
            printf("Unrecognized command: %s\r\n",
                   recv_cmd_state_ptr -> pbuffer); 
            recv_cmd_state_ptr -> pbuffer_lock = 0;
        }
    }
    return;
}



 

int main() {
    usart_init( recv_cmd_state_ptr );
    char teststr[] = "junk  13\r"; 
    char *teststr_ptr = teststr;
    char individ_char[2] = "0"; // Individual character pulled from string
    for ( int index = 0; index < strlen(teststr); index++) {
        strncpy(individ_char,teststr_ptr,1);
        printf("%s",individ_char);
        receive_isr_proto(recv_cmd_state_ptr,individ_char);
        teststr_ptr++;
    };
    process_pbuffer( recv_cmd_state_ptr, command_array );
    printf("\r\n");
    return 0;
}
