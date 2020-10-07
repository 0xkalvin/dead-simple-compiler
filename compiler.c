#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum TokenType
{
    ERROR = 0,
    IDENTIFIER,
    INTEGER_NUMBER,
    ASSIGNMENT,
    WHILE,
    EOS
} TokenType;

char *tokenTypesNames[] = {
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

int is_numeric_character(char c)
{
    if (c >= 48 && c <= 57)
    {
        return 1;
    }
    return 0;
}

int is_alpha_character(char c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    {
        return 1;
    }
    return 0;
}

char *read_file(const char *file_name)
{
    FILE *raw_file = fopen(file_name, "r");

    if (!raw_file)
    {
        printf("Failed to open file\n");
        exit(1);
    }

    fseek(raw_file, 0, SEEK_END); // shift to the end of the stream

    long size = ftell(raw_file); // tell current position of stream

    fseek(raw_file, 0, SEEK_SET); // shift to the beginning of the stream

    char *buffer = (char *)calloc(size + 1, sizeof(char));

    fread(buffer, sizeof(char), size, raw_file);

    fclose(raw_file);

    return buffer;
}

void process_identifier(char **buffer, Token *token)
{
    char *buffer_start = *buffer;

    while (is_alpha_character(**buffer) || is_numeric_character(**buffer))
    {
        *buffer += 1;
    }

    strncpy(token->attribute_for_identifier, buffer_start, *buffer - buffer_start);

    token->attribute_for_identifier[*buffer - buffer_start] = 0;

    if (strcasecmp(token->attribute_for_identifier, "WHILE") == 0)
    {

        token->type = WHILE;
    }
    else
    {
        token->type = IDENTIFIER;
    }
}

void process_number(char **buffer, Token *token)
{
    char *number_start = *buffer;

    while (is_numeric_character(**buffer))
    {
        *buffer += 1;
    }

    if (**buffer != '\n' && **buffer != '\r' && **buffer != '\t' && **buffer != ' ' && **buffer != 0)
    {
        return;
    }

    strncpy(token->attribute_for_identifier, number_start, *buffer - number_start);
    token->attribute_for_identifier[*buffer - number_start] = 0;
    token->attribute_for_number = atoi(token->attribute_for_identifier);
    token->type = INTEGER_NUMBER;
}

Token *get_token(char **buffer, int *current_line)
{
    Token *token = (Token *)malloc(sizeof(Token));

    while (**buffer == '\n' || **buffer == '\r' || **buffer == '\t' || **buffer == ' ')
    {
        if (**buffer == '\n')
        {
            *current_line += 1;
        }

        *buffer += 1;
    }

    if (**buffer == ':' && *(*buffer + 1) == '=')
    {
        *buffer += 2;
        token->type = ASSIGNMENT;
    }
    else if (is_numeric_character(**buffer))
    {
        process_number(buffer, token);
    }
    else if (is_alpha_character(**buffer))
    {
        process_identifier(buffer, token);
    }
    else if (**buffer == 0)
    {
        token->type = EOS;
    }
    else
    {
        token->type = ERROR;
    }

    return token;
}

int main(void)
{

    char *buffer = read_file("code.pas");
    int *current_line = (int *)malloc(sizeof(int));

    *current_line = 1;

    printf("Tokenization starting...\n");

    while (1)
    {
        Token *current_token = get_token(&buffer, current_line);

        printf("LINE %d | %s\t|", *current_line, tokenTypesNames[current_token->type]);

        if (current_token->type == INTEGER_NUMBER)
        {
            printf(" %d ", current_token->attribute_for_number);
        }
        else if (current_token->type == IDENTIFIER)
        {
            printf(" %s ", current_token->attribute_for_identifier);
        }
        else if (current_token->type == EOS || current_token->type == ERROR)
        {
            printf("\n");
            break;
        }
        printf("\n");
    }

    printf("Tokenization ending...\n");

    return 0;
}