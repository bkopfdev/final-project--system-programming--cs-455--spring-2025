#include <stdio.h>
#include <stdlib.h>

int main() {
    char *string = (char *)malloc(1024);
    
    if (string == NULL) {
        perror("Couldn't allocate memory");

        return 1;
    }

    extern char etext; //end of program text
    extern char edata; //end of initialized data
    extern char end;   //end of uninitialized data

    printf("End of program text: %p\n", &etext);
    printf("End of initialized data segment: %p\n", &edata);
    printf("End of uninitialized data segment: %p\n", &end);

    free(string);

    return 0;
}