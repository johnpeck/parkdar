#include <stdio.h>

int looper() {
    for (int count = 0;count < 5; count++) {
        puts("My junk");
    };
    return 0;
}


main()
{
    puts("Hello World!");
    looper();
    return 0;
}
