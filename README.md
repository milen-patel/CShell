# CShell 
                              
 The goal of this project was to create a basic shell using C. The initial part of this code is from an assignment, but I have modified it to remove memory-leaks and improve efficiency.
 
---

  # File Explanations and Project Structure
  ### Storage Functions        
  1. **Vec**: the underlying unit of memory storage in the program. A vec serves as a dynamically sized array whose content is stored in the heap. A vec has appropriate functions to modify and access the elements which it contains.
  2. **Str**: A str represents a string stored in dynamic memory that has associated accessor and mutator functions. Str's implementation builds upon Vec and automatically maintains a terminating null-character for every string. In the scope of this program, each word that is entered into the shell is stored as a unique Str.
  3. **StrVec**: A StrVec is a Vec composed of Str values that provides helpful accessor functions such as push and pop. In this program, the words composing each command are stored in a StrVec.
  
### Input Handling
1. **CharItr**: Provides easy methods for iterating over a sequence of characters in memory such as has next, next, and peek. Used to aid implementation of Scanner
2. **Scanner**: A scanner uses a CharItr to produce tokens that conform to the grammar of the shell. In the program, raw input is tokenized using the scanner.
3. **Node**: Used in the parsing of tokens. There exist error nodes, command nodes, and pipe nodes. Each node contains a type and its associated data.
4. **Parser**: Responsible for parsing thorugh all of the tokens created by a scanner. Derives a tree structure of nodes giving meaning to the user input.
5. **Exec**: Takes the root node of a parse tree and executes the appropriate shell commands while handling errors and piping.

---
