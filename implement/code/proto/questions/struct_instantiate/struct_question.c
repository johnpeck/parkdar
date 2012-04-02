#include <stdio.h>
#include <string.h>


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
    char pbuffer[PARSE_BUFFER_SIZE]; // Parse buffer
    int rbuffer_count; // Number of chars sent to receive buffer
    int pbuffer_lock; // Parse buffer lock.  1 = locked
    int pbuffer_match; // Match found for command in pbuffer.  1 = match
} recv_cmd_state_t;
/* Define a pointer to the state -- here's my problem:
 * Creating the pointer with this line works: */
recv_cmd_state_t  recv_cmd_state, *recv_cmd_state_ptr = &recv_cmd_state;
// ...but creating the pointer with this line causes a segmentation fault:
// recv_cmd_state_t *recv_cmd_state_ptr;


/* Making this function explicitly take a pointer to the received command
 * state structure makes it clear that it modifies this structure.  This
 * function will ultimately also have to set up the USART hardware. */
void usart_init( recv_cmd_state_t *recv_cmd_state_ptr ) {
    memset((recv_cmd_state_ptr -> rbuffer),0,RECEIVE_BUFFER_SIZE);
    recv_cmd_state_ptr -> rbuffer_write_ptr =
        recv_cmd_state_ptr -> rbuffer; // Initialize write pointer
    memset((recv_cmd_state_ptr -> pbuffer),0,PARSE_BUFFER_SIZE);
    recv_cmd_state_ptr -> rbuffer_count = 0;
    recv_cmd_state_ptr -> pbuffer_lock = 0; // Parse buffer unlocked
    return;
}



int main() {
    usart_init( recv_cmd_state_ptr );
}
