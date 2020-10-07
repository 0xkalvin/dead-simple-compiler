#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>

typedef enum TokenType
{
    ERROR = 0,
    IDENTIFIER,
    INTEGER_NUMBER,
    ASSIGNMENT,
    WHILE,
    EOS
} TokenType;

const char *tokenTypesNames[] = {
    "LEXICAL ERROR",
    "IDENTIFIER",
    "INTEGER NUMBER",
    "ASSIGNMENT",
    "WHILE",
    "END OF STRING"};

typedef struct Token
{
    TokenType type;
    int line;
    int attribute_for_number;
    char attribute_for_identifier[15];

} Token;

#endif