# Compiladores
### (Compilers)
This repository contains all the class projects for Compilers. 

----

## **mypas**

This is a implementation of a basic Pascal compiler that transforms source code into a representation suitable for analysis and execution. The compiler is divided into three main stages: **lexical analysis**, **syntactic analysis**, and **symbol management**.

#### Components

- **Lexical Analysis (`lexer.c`)**
The `lexer.c` file performs lexical analysis, which is the first step of the compiler. It reads the source code character by character, identifies lexical units (tokens), and classifies elements such as:  
  - **Keywords** (e.g., `PROGRAM`, `VAR`, etc.)  
  - **Identifiers** (e.g., variable and function names)  
  - **Numbers**  
  - **Operators** (e.g., `:=`, `<`, `>`)  

  It also ignores whitespace and comments.  

  - Key Functions:
    - **`gettoken`**: Returns the next token from the source code.  
    - **Auxiliary functions**:
      - `isID`: Identifies valid identifiers.
      - `isDEC`: Identifies decimal numbers.
      - `skipspaces`: Skips whitespace and comments.  

  Tokens are stored in the `lexeme` array and returned for use in the next stage.

- **Syntactic Analysis (`parser.c`)**
The `parser.c` file implements syntactic analysis, which checks if the sequence of tokens follows the grammar rules of the Pascal language.  

  - Key Functionality:
    - The main function, **`mypas`**, analyzes a complete program starting with the keyword `PROGRAM` and ending with `.`.  
    - It calls other functions to process specific parts of the program:
      - **`block`**: Processes code blocks.
      - **`vardef`**: Defines variables.
      - **`sbprgdef`**: Processes subprograms like functions and procedures.
      - **`stmtlist`**: Handles lists of statements.  

- **Error Handling:**
The function **`match`** validates expected tokens and moves to the next one, reporting errors if grammar rules are violated. The parser also manages expressions (`expr`, `term`) and control structures like conditional (`IF`) or loop (`WHILE`, `REPEAT`) statements.


- **Symbol Table Management (`symtab.c`)**
The `symtab.c` file manages the symbol table, a structure that stores information about variables, functions, and procedures defined in the program.  

  - Symbol Attributes:
  Each entry in the symbol table includes attributes such as:
    - Name
    - Type
    - Lexical level (scope)
    - Parameter flag  

  - Key Functions:
    - **`symtab_lookup`**: Searches for symbols by name and lexical level.
    - **`symtab_append`**: Adds new symbols to the table, checking for duplicates.
    - **`symtab_release`**: Removes symbols when exiting a scope.

The symbol table supports multiple lexical levels, enabling management of both local and global scopes.

---

#### Workflow Overview

- **Lexical Analysis (`lexer.c`)**:
   - Transforms source code into tokens.

- **Syntactic Analysis (`parser.c`)**:
   - Validates the program structure based on Pascal grammar rules.

- **Symbol Table Management (`symtab.c`)**:
   - Stores semantic information such as types and scopes.

These components work together to efficiently interpret Pascal code while identifying lexical, syntactic, and semantic errors.

--- 


---
## **mybc**
This project is a simple interpreter for a programming language, similar to an advanced calculator. It consists of three main files: `lexer.c`, `parser.c`, and `main.c`. Below is an overview of the project:

#### Main Components

- **Lexical Analyzer (`lexer.c`)**
The lexical analyzer is responsible for identifying and classifying tokens from the input. It recognizes various types of tokens, including:  
  - **Identifiers (ID)**  
  - **Decimal numbers (DEC)**  
  - **Octal numbers (OCT)**  
  - **Hexadecimal numbers (HEX)**  
  - **Floating-point numbers (FLT)**  
  - **Assignment operator (ASGN)**  

  - **Key Features:**
    - Implements functions to recognize each type of token.
    - Converts the input into a sequence of tokens for further processing.

- **Syntactic Analyzer (`parser.c`)**
The syntactic analyzer implements the logic to analyze the structure of the language. It uses a simple grammar for arithmetic expressions.

  - Key Functions:
    - **Evaluate expressions (`E`)**  
    - **Analyze terms (`T`)**  
    - **Analyze factors (`F`)**  

  - Additional Features:
    - Manages a stack for calculations.
    - Uses a symbol table to handle variables.

---

#### Features

1. Performs basic arithmetic operations:  
   - Addition (`+`), subtraction (`-`), multiplication (`*`), and division (`/`).  

2. Supports parentheses for grouping expressions.

3. Allows the definition and use of variables.

4. Recognizes different number formats:  
   - Decimal, octal, hexadecimal, and floating-point.

5. Prints the results of evaluated expressions.

---

#### Data Structures

1. **Stack**:  
   - Used to store values during calculations.

2. **Symbol Table**:  
   - A simple implementation to store variables and their corresponding values.

---

#### Purpose

This project demonstrates fundamental concepts of compilers and interpreters, including:  
- Lexical analysis,  
- Syntactic analysis, and  
- Expression evaluation.

It serves as an educational tool for understanding how interpreters process programming languages.


