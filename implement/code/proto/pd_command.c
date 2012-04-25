#include <stdio.h>
#include <string.h>
#include "pd_command.h"
#include "pd_ranger.h"




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

void rbuffer_erase( recv_cmd_state_t *recv_cmd_state_ptr ) {
    memset((recv_cmd_state_ptr -> rbuffer),0,RECEIVE_BUFFER_SIZE);
    recv_cmd_state_ptr -> rbuffer_write_ptr =
        recv_cmd_state_ptr -> rbuffer; // Initialize write pointer
    recv_cmd_state_ptr -> rbuffer_count = 0;
    return;
}

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

/* Making this function explicitly take a pointer to the received command
 * state structure makes it clear that it modifies this structure.  This
 * function will ultimately also have to set up the USART hardware. */
void command_init( recv_cmd_state_t *recv_cmd_state_ptr ) {
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



