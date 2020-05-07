#include <stdio.h>
#include <stdlib.h>

#include "Vec.h"
#include "Str.h"

int main()
{
    int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    Str myString = Str_value(BUFFER_SIZE);

    /* Keep Reading Input Until We Reach EOF */
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        Str_append(&myString, buffer);

        /* If the line length is greater than the buffer, we need to read in again before printing */
        if(*Str_ref(&myString, Str_length(&myString)-1) == '\n') {
            printf("%s", Str_cstr(&myString));
            Str_splice(&myString, 0, Str_length(&myString), buffer, 0);
        }
    }
    
    Str_drop(&myString);
    return EXIT_SUCCESS;
} 
