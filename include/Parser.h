#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include "Node.h"

/**
 * Given a Scanner, returns a pointer to the `Node` at
 * the root of the parse tree. The caller of `parse`
 * owns the resulting `Node*` and is responsible for
 * calling `Node_drop` to free its allocated memory.
 */
Node* parse(Scanner *s);

#endif
