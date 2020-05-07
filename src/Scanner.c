#include <ctype.h>

#include "Scanner.h"

static void fillnextPos(Scanner *self);

Scanner Scanner_value(CharItr char_itr)
{
    /* Create a meaningless token to start */
    Token next = {
        END_TOKEN,
        Str_from("")
    };

    /* Create an itr using the meaningless token */
    Scanner itr = {
        char_itr,
        next 
    };

    fillnextPos(&itr); /* Update the 'next' member of our token to be meaningful */
    Str_drop(&next.lexeme); /* Free the string from the meaningless token */
    return itr;
}

bool Scanner_has_next(const Scanner *self)
{
   if(self->next.type == END_TOKEN) {
      return false;
   } else {
      return true;
   } 
}

Token Scanner_peek(const Scanner *self)
{
    return self->next;
}

Token Scanner_next(Scanner *self)
{
    Token toGive = self->next;
    fillnextPos(self);
    return toGive;
}

/* Takes a scanner and deletes all chars that are meaningless to the grammar
 * until the first valid character is reac */
static void throwUselessInput(Scanner *self) {
    while(CharItr_has_next(&self->char_itr)) {
        if ((CharItr_peek(&self->char_itr)) == '\t') { 
            CharItr_next(&self->char_itr);
        } else if ((CharItr_peek(&self->char_itr)) == '\n') {
            CharItr_next(&self->char_itr);
        } else if ((CharItr_peek(&self->char_itr)) == ' ') { 
            CharItr_next(&self->char_itr);
        } else {
            break;
        }
    }
}

/* Ensures that self->next always contains the next available token, if one exists */
static void fillnextPos(Scanner *self) {
    /* Delete meaningless input */
    throwUselessInput(self);    

    /* If we have no more input in the char_itr than yield an END_TOKEN */
    if (!CharItr_has_next(&self->char_itr)) {
        Token next;
        next.type = END_TOKEN;
        self->next = next;
        return;
    }

    /* Check for a pipe token next */
    if (CharItr_peek(&self->char_itr) == '|') {
        Token next = {
            PIPE_TOKEN,
            Str_from("|") 
        };
        self->next = next;
        CharItr_next(&self->char_itr);
        return;
    }

    /* If we make it here, then we have a word to read */
    Str buildingWord = Str_value(1025);
    while ((CharItr_has_next(&self->char_itr)) && (CharItr_peek(&self->char_itr) != '|')) {
        if (((CharItr_peek(&self->char_itr)) == '|') | ((CharItr_peek(&self->char_itr)) == '\0')) {
            break;
        }

        /* Parse out the invalid characters */
        char peekedChar = CharItr_peek(&self->char_itr);
        if (peekedChar == '\t') { 
            break;
        } else if (peekedChar == '\n') {
            break;
        } else if (peekedChar == ' ') { 
            break;
        }

        /* If we make it here then we have a char that fits in our word */
        char nextChar = CharItr_next(&self->char_itr);
        Str_splice(&buildingWord, Str_length(&buildingWord), 0, &nextChar, 1);
    }

    /* buildingWord now contains the next word, so we can compose the word token */
    Token next = {
        WORD_TOKEN,
        buildingWord
    };

    self->next = next;
}
