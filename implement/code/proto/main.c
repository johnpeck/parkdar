#include <stdio.h>
#include <string.h>
#include "pd_ranger.h"
#include "pd_command.h"




// Define a pointer to the received command state
recv_cmd_state_t  recv_cmd_state;
recv_cmd_state_t *recv_cmd_state_ptr = &recv_cmd_state;

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
