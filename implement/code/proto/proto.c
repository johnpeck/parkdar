#include <stdio.h>
#include <string.h>



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
    char rxbuffer[20]; // This will be like my recieved character buffer
    memset(rxbuffer,0,20);
    char * rxStartPtr = rxbuffer; // Always points to start of buffer
    char * rxScanPtr = rxbuffer; // Walks through buffer
    char * rxWritePtr = rxbuffer; // Walks through buffer for writing
    char mystring[20] = "string"; // This is like chars from USART
    char rxToken[20];
    int count = 0;
    /* Write mystring to the command buffer */
    while ( mystring[count] != '\0' ) {
        *rxWritePtr = mystring[count];
        rxWritePtr++;
        count++;
    };
    puts(rxbuffer);
    /* Scan the command buffer for first occurance of \r */
    char * termPtr = strchr(rxbuffer,'\r'); // Finds command terminator
    if (termPtr != NULL) {
        count = 0;
        while (rxScanPtr != termPtr) {
            rxToken[count] = *rxScanPtr;
            rxScanPtr++;
            count++;
        };
        printf("The found token is %s\r\n",rxToken);
    };
 }
          




int main()
{
    int doAvgTest = 0; // Test averaging numbers in an array
    int doIdxTest = 1; // Test indexing in an array
    if (doAvgTest != 0) { 
        int myarray[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,\
                       20};
        puts("Hello World!");
        looper(myarray);
        printf("Average value is %d\n",getavg(myarray,2));
        printf("Five over 2 is %0.3f\n",(0.5*5));
    }; // End average test
    if (doIdxTest != 0) {
        idxtest();
    };
    return 0;
}
