#include <stdio.h>



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




int main()
{
    int myarray[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,\
                       20};
    puts("Hello World!");
    looper(myarray);
    printf("Average value is %d\n",getavg(myarray,2));
    return 0;
}
