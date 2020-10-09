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
    EOS,
    AND,
    BEGIN,
    BOOLEAN,
    CHAR,
    DO,
    ELSE,
    END,
    FALSE,
    IF,
    INTEGER,
    MOD,
    NOT,
    OR,
    PROGRAM,
    READ,
    REAL,
    THEN,
    TRUE,
    WRITE,
    PARENTHESIS_START,
    PARENTHESIS_END,
    DOT,
    SEMICOLON,
    ADDITION,
    SUBTRACTION,
    DIVISION,
    MULTIPLICATION,
    LESS_THAN,
    EQUAL_OR_LESS_THAN,
    EQUAL,
    DIFFERENT,
    GREATER_THAN,
    EQUAL_OR_GREATER_THAN,
    REAL_NUMBER,
    ASCII_CHARACTER,
    COMMENT,

    TOTAL_NUMBER_OF_TOKENS
} TokenType;

const char *tokenTypesNames[] = {
    "LEXICAL ERROR",
    "IDENTIFIER",
    "INTEGER NUMBER",
    "ASSIGNMENT",
    "WHILE",
    "END OF STRING",
    "AND",
    "BEGIN",
    "BOOLEAN",
    "CHAR",
    "DO",
    "ELSE",
    "END",
    "FALSE",
    "IF",
    "INTEGER",
    "MOD",
    "NOT",
    "OR",
    "PROGRAM",
    "READ",
    "REAL",
    "THEN",
    "TRUE",
    "WRITE",
    "PARENTHESIS_START",
    "PARENTHESIS_END",
    "DOT",
    "SEMICOLON",
    "ADDITION",
    "SUBTRACTION",
    "DIVISION",
    "MULTIPLICATION",
    "LESS_THAN",
    "EQUAL_OR_LESS_THAN",
    "EQUAL",
    "DIFFERENT",
    "GREATER_THAN",
    "EQUAL_OR_GREATER_THAN",
    "REAL_NUMBER",
    "ASCII_CHARACTER",
    "COMMENT"};

typedef struct Token
{
    TokenType type;
    int line;
    int attribute_for_number;
    char attribute_for_identifier[15];
} Token;

#endif