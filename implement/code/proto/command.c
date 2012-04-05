#include <stdio.h>
#include <string.h>
#include "command.h"

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
