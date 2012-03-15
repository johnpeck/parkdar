#include <stdio.h>
#include <string.h>

#define NUMCOMMANDS 5
/* Define the size of the received character buffer */
#define RECEIVE_BUFFER_SIZE 20

/* This will be like my received character buffer */
char usart_receive_buffer[RECEIVE_BUFFER_SIZE]; 

/* Receive character buffer state structure. The idea is that I'll create
 * a structure to keep track of the state in every flow diagram I make.
 * I have a flow diagram for received characters, so I created this
 * structure. */
typedef struct { 
    char rbuffer[RECEIVE_BUFFER_SIZE]; // Received character buffer
    int charcount; // Number of chars sent to receive buffer
    int pbuffer_lock; // Parse buffer lock.  1 = locked
} rbuffer_state_t;
// Define a pointer to the state
rbuffer_state_t  rbuffer_state, *rbuffer_state_ptr = &rbuffer_state;


/* Making this function explicitly take a pointer to the receive buffer
 * state structure makes it clear that it modifies this structure.  This
 * function will ultimately also have to set up the USART hardware. */
void usart_init( rbuffer_state_t *rbuffer_state_ptr ) {
    memset((rbuffer_state_ptr -> rbuffer),0,RECEIVE_BUFFER_SIZE);
    rbuffer_state_ptr -> charcount = 0;
    rbuffer_state_ptr -> pbuffer_lock = 0; // Parse buffer unlocked
    return;
}

char * cmdListPtr[NUMCOMMANDS] = {
    "range?",
    "RANGE?"
};

int looper(int *array) {
    for (int count = 0;count < 5; count++) {
        printf("Array value is %d\n",array[count]);
        puts("My junk");
    };
    return 0;
}

/* getavg()
 * Averages n numbers from an array, where n is the closest power of 2
 * to avgnum */
int getavg(int *array,int avgnum) {
    int numshift = 0;
    int sum = 0;
    int average = 0;
    int powcount = 0;
    int avgshift = avgnum >> 1;
    if (avgshift == 0)
        average = array[0];
    else {
        while (avgshift != 0) {
            powcount++;
            avgshift = avgshift >> 1;
        };
        printf("Closest power of 2 is %d\n",powcount);
        for (int count = 0; count < (1 << powcount); count++) {
            sum += array[count];
        };
        average = sum >> powcount;
    };
    
    return average;

}

/* idxtest()
 * Builds up a character array until a \r is detected. */
int idxtest() {
    
    memset(usart_receive_buffer,0,20);
    char rxToken[20];
    memset(rxToken,0,20);
    char * rxStartPtr = usart_receive_buffer; // Always points to start of buffer
    char * rxScanPtr = usart_receive_buffer; // Walks through buffer
    char * rxWritePtr = usart_receive_buffer; // Walks through buffer for writing
    char mystring[20] = "stringstring"; // This is like chars from USART
    int count = 0;
    /* Write mystring to the command buffer */
    while ( mystring[count] != '\0' ) {
        *rxWritePtr = mystring[count];
        rxWritePtr++;
        count++;
    };
    puts(usart_receive_buffer);
    /* Scan the command buffer for first occurance of \r.  If there is
     * one, then everything leading up to it is part of the incoming
     * command.  If there's no \r and the buffer is more than half full,
     * there's no way a good command will be entered.  Print an error
     * message and clear the buffer. */
    char * termPtr = strchr(usart_receive_buffer,'\r'); // Finds first command terminator
    int rxCharnum = strlen(usart_receive_buffer); // Find characters in receive buffer
    if (termPtr != NULL) {
        count = 0;
        while (rxScanPtr != termPtr) {
            rxToken[count] = *rxScanPtr;
            rxScanPtr++;
            count++;
        };
        printf("The found token is %s\r\n",rxToken);
    }
    else if (rxCharnum > (20 >> 2)) {
        puts("Buffer overflow!");
        rxWritePtr = usart_receive_buffer;
        memset(usart_receive_buffer,0,20); // Reset the buffer
    };
 }
          
/* cmdIdent(*cmdString)
 * Checks to see if a command is recognized by the system.  Accepts
 * the entire command string returned by the command scanner.  Separates
 * the command and its argument.
 * --Commands and arguments are separated by spaces -- not generalized
 *   whitespace.  There may be more than one space after a command. 
 * --Commands are made of 5 characters + an optional question mark */
char * cmdIdent(char * recString) {
    char cmdStr[10]; // Will hold the command (not the argument)
    char * scanPtr = recString;
    memset(cmdStr,0,10);
    printf("I received the string %s\r\n",recString);
    /* Check for spaces -- is there an argument? */
    char * firstSpacePtr = strchr(recString,' '); // Finds first space 
    if (firstSpacePtr != NULL) {
        int count = 0;
        while (scanPtr != firstSpacePtr) {
            cmdStr[count] = *scanPtr;
            scanPtr++;
            count++;
        };
        printf("The command has a space and is %s\r\n",cmdStr);
    }
    else {
        strcpy(cmdStr,recString);
        printf("The command didn't have a space and is %s\r\n",cmdStr);
    };
    printf("The first command in the list is %s\r\n",cmdListPtr[0]);
    /* Now see if the command is identified.  Loop through all remote
     * commands and test. */
    int strDiff = 0;
    int cmdValid = 0;
    for ( int i = 0; i < NUMCOMMANDS; i++ ) {
        strDiff = strcmp( cmdStr, cmdListPtr[i] );
        if ( strDiff == 0 ) {
            printf("The command %s is recognized.\r\n",cmdStr);
            cmdValid = 1;
            break;
        };
    };
    if ( cmdValid != 1 ) {
        printf("The command %s is not recognized.\r\n",cmdStr);
    };
    return cmdStr;
}

/* receive_isr_proto(char)
 * This mocks up the receive character interrupt of the AVR */
void receive_isr_proto( rbuffer_state_t *rbuffer_state_ptr,
                         char *inchar) {
    if (!strcmp(inchar,"\r")) {
        printf("It's a terminator");
    };
    return;
}

 

int main()
{
    int doAvgTest = 0; // Test averaging numbers in an array
    int doIdxTest = 0; // Test indexing in an array
    int doIdent = 0; // Test identifying a string
    int do_isr_test = 1; // Test the isr prototype
    if (doAvgTest != 0) { 
        int myarray[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,\
                       20};
        puts("Hello World!");
        //looper(myarray);
        printf("Average value is %d\n",getavg(myarray,2));
        printf("Five over 2 is %0.3f\n",(0.5*5));
    }; // End average test
    if (doIdxTest != 0) {
        idxtest();
    };
    if (doIdent != 0) {
        cmdIdent("range? 5"); // Send the command I want to check
    };
    if (do_isr_test != 0) {
        usart_init( rbuffer_state_ptr );
        //receive_isr_proto(rbuffer_state_ptr, "r");
    };
    return 0;
}
