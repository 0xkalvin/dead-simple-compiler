#include <ctype.h>
#include <string.h>

#include "definitions.h"
#include "file_parser.h"
#include "helpers.h"

void process_identifier(char **buffer, Token *token)
{
    char *buffer_start = *buffer;

    while (is_alpha_character(**buffer) || is_numeric_character(**buffer) || is_underscore(**buffer))
    {
        *buffer += 1;
    }

    if (*buffer - buffer_start > 16)
    {
        token->type = ERROR;
        return;
    }

    strncpy(token->attribute_for_identifier, buffer_start, *buffer - buffer_start);

    token->attribute_for_identifier[*buffer - buffer_start] = 0;

    for (int i = 0; i < TOTAL_NUMBER_OF_TOKENS - 1; i++)
    {
        if (strcasecmp(token->attribute_for_identifier, tokenTypesNames[i]) == 0)
        {
            token->type = i;
            return;
        }
    }

    token->type = IDENTIFIER;
}

void process_number(char **buffer, Token *token)
{
    char *number_start = *buffer;

    while (is_numeric_character(**buffer))
    {
        *buffer += 1;
    }

    if (**buffer == '.')
    {
        *buffer += 1;

        while (is_numeric_character(**buffer))
        {
            *buffer += 1;
        }

        if (**buffer != '\n' && **buffer != '\r' && **buffer != '\t' && **buffer != ' ' && **buffer != 0)
        {
            token->type = ERROR;
            return;
        }

        strncpy(token->attribute_for_identifier, number_start, *buffer - number_start);
        token->attribute_for_identifier[*buffer - number_start] = 0;
        token->attribute_for_number = atof(token->attribute_for_identifier);
        token->type = REAL_NUMBER;
    }
    else
    {
        if (**buffer != '\n' && **buffer != '\r' && **buffer != '\t' && **buffer != ' ' && **buffer != 0)
        {
            token->type = ERROR;
            return;
        }

        strncpy(token->attribute_for_identifier, number_start, *buffer - number_start);
        token->attribute_for_identifier[*buffer - number_start] = 0;
        token->attribute_for_number = atoi(token->attribute_for_identifier);
        token->type = INTEGER_NUMBER;
    }
}

void process_comments(char **buffer, Token *token)
{

    if (**buffer == '{')
    {
        while (**buffer != '}' && **buffer != 0)
        {
            *buffer += 1;
        }

        if (**buffer == 0)
        {
            token->type = ERROR;
            return;
        }

        *buffer += 1;
    }
    else
    {
        while (**buffer != '\n' && **buffer != 0)
        {
            *buffer += 1;
        }
        *buffer += 1;
    }

    token->type = COMMENT;
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
    else if (**buffer == '(')
    {
        *buffer += 1;
        token->type = PARENTHESIS_START;
    }
    else if (**buffer == ')')
    {
        *buffer += 1;
        token->type = PARENTHESIS_END;
    }
    else if (**buffer == '.')
    {
        *buffer += 1;
        token->type = DOT;
    }
    else if (**buffer == ';')
    {
        *buffer += 1;
        token->type = SEMICOLON;
    }
    else if (**buffer == '-')
    {
        *buffer += 1;
        token->type = SUBTRACTION;
    }
    else if (**buffer == '+')
    {
        *buffer += 1;
        token->type = ADDITION;
    }
    else if (**buffer == '/')
    {
        *buffer += 1;
        token->type = DIVISION;
    }
    else if (**buffer == '*')
    {
        *buffer += 1;
        token->type = MULTIPLICATION;
    }
    else if (**buffer == '<' && *(*buffer + 1) == '=')
    {
        *buffer += 1;
        token->type = EQUAL_OR_LESS_THAN;
    }
    else if (**buffer == '>' && *(*buffer + 1) == '=')
    {
        *buffer += 1;
        token->type = EQUAL_OR_GREATER_THAN;
    }
    else if (**buffer == '!' && *(*buffer + 1) == '=')
    {
        *buffer += 1;
        token->type = DIFFERENT;
    }
    else if (**buffer == '<')
    {
        *buffer += 1;
        token->type = LESS_THAN;
    }
    else if (**buffer == '>')
    {
        *buffer += 1;
        token->type = GREATER_THAN;
    }
    else if (**buffer == '=')
    {
        *buffer += 1;
        token->type = EQUAL;
    }
    else if (is_numeric_character(**buffer))
    {
        process_number(buffer, token);
    }
    else if (is_alpha_character(**buffer))
    {
        process_identifier(buffer, token);
    }
    else if (**buffer == '{' || **buffer == '#')
    {
        process_comments(buffer, token);
    }
    else if (**buffer == 0)
    {
        token->type = EOS;
    }
    else if (**buffer == 39 && is_ascii_character(*(*buffer + 1)) && *(*buffer + 2) == 39)
    {
        token->type = ASCII_CHARACTER;
        token->attribute_for_number = (int)*(*buffer + 1);
        *buffer += 3;
    }
    else
    {
        printf("PASSOU AQUI %c %d \n", **buffer, **buffer);

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
        else if (current_token->type == ASCII_CHARACTER)
        {
            printf(" %c ", current_token->attribute_for_number);
        }
        else if (current_token->type == COMMENT)
        {
            *current_line += 1;
        }
        else if (current_token->type == EOS || current_token->type == ERROR)
        {
            printf("\n");
            break;
        }
        printf("\n");
    }

    printf("Tokenization ending...\n");
    free(current_line);

    return 0;
}