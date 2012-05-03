#include <stdio.h>
#include <string.h>
#include "pd_ranger.h"


/* pd_command.h contains the extern declaration of command_array --
 * an array containing all the commands understood by the system. */
#include "pd_command.h"

/* pd_logger.h sets up logging */
#include "pd_logger.h"


// Define a pointer to the received command state
recv_cmd_state_t  recv_cmd_state;
recv_cmd_state_t *recv_cmd_state_ptr = &recv_cmd_state;




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
        logger_msg("command",log_level_INFO,
            "  <-- Not a terminator.  Received count is %d\r\n",
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
    logger_init(); 
    logger_disable();
    logger_setsystem( "logger" );
    logger_setsystem( "command" );
    logger_msg( "command",log_level_INFO,"My log message\n" );
    command_init( recv_cmd_state_ptr );
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
