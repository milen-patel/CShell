#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Str.h"
#include "Scanner.h"

#define BUFF_SIZE 80 

/**
 * This program reads an input line from stdin and prints textual
 * representations of the tokens scanned from lines of input.
 */

// These three functions provide the basis of a REPL:
// Read-Evaluate-Print-Loop
size_t read(Str *line, FILE *stream);
Scanner eval(Str *input);
void print(Scanner scanner);

int main()
{
    Str line = Str_value(BUFF_SIZE);
    while (read(&line, stdin)) {
        print(eval(&line));
    }
    Str_drop(&line);
    return EXIT_SUCCESS;
}

size_t read(Str *line, FILE *stream) {
    printf("scanner> ");

    // Clear Str contents.
    Str_splice(line, 0, Str_length(line), NULL, 0);

    static char buffer[BUFF_SIZE];
    while (fgets(buffer, BUFF_SIZE, stream) != NULL) {
        Str_append(line, buffer);
        if (strchr(buffer, '\n') != NULL) {
            break;
        }
    }

    return Str_length(line);
}

Scanner eval(Str *line) {
    return Scanner_value(CharItr_of_Str(line));
}

void print(Scanner scanner) {
    while (Scanner_has_next(&scanner)) {
        Token next = Scanner_next(&scanner);
        switch (next.type) {
            case END_TOKEN: 
                printf("END\n");
                break;
            case WORD_TOKEN:
                printf("WORD(%s)\n", Str_cstr(&next.lexeme));
                break;
            case PIPE_TOKEN:
                printf("PIPE\n");
                break;
        }
        Str_drop(&next.lexeme);
    }
}
