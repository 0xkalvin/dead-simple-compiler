#include <ctype.h>
#include <string.h>

#include "definitions.h"
#include "file_parser.h"
#include "helpers.h"

void process_identifier(char **buffer, Atom *atom)
{
    char *buffer_start = *buffer;

    while (is_alpha_character(**buffer) || is_numeric_character(**buffer) || is_underscore(**buffer))
    {
        *buffer += 1;
    }

    if (*buffer - buffer_start > 16)
    {
        atom->type = ERROR;
        return;
    }

    strncpy(atom->attribute_for_identifier, buffer_start, *buffer - buffer_start);

    atom->attribute_for_identifier[*buffer - buffer_start] = 0;

    for (int i = 0; i < TOTAL_NUMBER_OF_TOKENS - 1; i++)
    {
        if (strcasecmp(atom->attribute_for_identifier, tokenTypesNames[i]) == 0)
        {
            atom->type = i;
            return;
        }
    }

    atom->type = IDENTIFIER;
}

void process_number(char **buffer, Atom *atom)
{
    char *number_start = *buffer;

    while (is_numeric_character(**buffer))
    {
        *buffer += 1;
    }

    if (**buffer == '.' && (*(*buffer + 1) == 'e' || *(*buffer + 1) == 'E'))
    {
        *buffer += 3;

        if (**buffer == '+' || **buffer == '-')
        {
            *buffer += 1;
        }

        while (is_numeric_character(**buffer))
        {
            *buffer += 1;
        }

        if (**buffer != '\n' && **buffer != '\r' && **buffer != '\t' && **buffer != ' ' && **buffer != 0)
        {
            atom->type = ERROR;
            return;
        }

        strncpy(atom->attribute_for_identifier, number_start, *buffer - number_start);
        atom->attribute_for_identifier[*buffer - number_start] = 0;
        atom->attribute_for_number = atof(atom->attribute_for_identifier);
        atom->type = REAL_NUMBER;
    }
    else
    {

        strncpy(atom->attribute_for_identifier, number_start, *buffer - number_start);
        atom->attribute_for_identifier[*buffer - number_start] = 0;
        atom->attribute_for_number = atoi(atom->attribute_for_identifier);
        atom->type = INTEGER_NUMBER;
    }
}

void process_comments(char **buffer, Atom *atom)
{

    if (**buffer == '{')
    {
        while (**buffer != '}' && **buffer != 0)
        {
            *buffer += 1;
        }

        if (**buffer == 0)
        {
            atom->type = ERROR;
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

    atom->type = COMMENT;
}

Atom *get_atom(char **buffer, int *current_line)
{
    Atom *atom = (Atom *)malloc(sizeof(Atom));

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
        atom->type = ASSIGNMENT;
    }
    else if (**buffer == '(')
    {
        *buffer += 1;
        atom->type = PARENTHESIS_START;
    }
    else if (**buffer == ')')
    {
        *buffer += 1;
        atom->type = PARENTHESIS_END;
    }
    else if (**buffer == '.')
    {
        *buffer += 1;
        atom->type = DOT;
    }
    else if (**buffer == ';')
    {
        *buffer += 1;
        atom->type = SEMICOLON;
    }
    else if (**buffer == ',')
    {
        *buffer += 1;
        atom->type = COLON;
    }
    else if (**buffer == '-')
    {
        *buffer += 1;
        atom->type = SUBTRACTION;
    }
    else if (**buffer == '+')
    {
        *buffer += 1;
        atom->type = ADDITION;
    }
    else if (**buffer == '/')
    {
        *buffer += 1;
        atom->type = DIVISION;
    }
    else if (**buffer == '*')
    {
        *buffer += 1;
        atom->type = MULTIPLICATION;
    }
    else if (**buffer == '<' && *(*buffer + 1) == '=')
    {
        *buffer += 1;
        atom->type = EQUAL_OR_LESS_THAN;
    }
    else if (**buffer == '>' && *(*buffer + 1) == '=')
    {
        *buffer += 1;
        atom->type = EQUAL_OR_GREATER_THAN;
    }
    else if (**buffer == '!' && *(*buffer + 1) == '=')
    {
        *buffer += 1;
        atom->type = DIFFERENT;
    }
    else if (**buffer == '<')
    {
        *buffer += 1;
        atom->type = LESS_THAN;
    }
    else if (**buffer == '>')
    {
        *buffer += 1;
        atom->type = GREATER_THAN;
    }
    else if (**buffer == '=')
    {
        *buffer += 1;
        atom->type = EQUAL;
    }
    else if (is_numeric_character(**buffer))
    {
        process_number(buffer, atom);
    }
    else if (is_alpha_character(**buffer))
    {
        process_identifier(buffer, atom);
    }
    else if (**buffer == '{' || **buffer == '#')
    {
        process_comments(buffer, atom);
    }
    else if (**buffer == 0)
    {
        atom->type = EOS;
    }
    else if (**buffer == 39 && is_ascii_character(*(*buffer + 1)) && *(*buffer + 2) == 39)
    {
        atom->type = ASCII_CHARACTER;
        atom->attribute_for_number = (int)*(*buffer + 1);
        *buffer += 3;
    }
    else
    {

        atom->type = ERROR;
    }

    return atom;
}

/*
Declaring Sintax functions
*/
void consume(char **buffer, int *current_line, AtomType *lookahead, AtomType current_atom_type);
void program(char **buffer, int *current_line, AtomType *lookahead);
void block(char **buffer, int *current_line, AtomType *lookahead);
void variable_declaration(char **buffer, int *current_line, AtomType *lookahead);
void type(char **buffer, int *current_line, AtomType *lookahead);
void variable(char **buffer, int *current_line, AtomType *lookahead);
void composed_command(char **buffer, int *current_line, AtomType *lookahead);
void command(char **buffer, int *current_line, AtomType *lookahead);
void assignment_command(char **buffer, int *current_line, AtomType *lookahead);
void if_command(char **buffer, int *current_line, AtomType *lookahead);
void while_command(char **buffer, int *current_line, AtomType *lookahead);
void read_command(char **buffer, int *current_line, AtomType *lookahead);
void write_command(char **buffer, int *current_line, AtomType *lookahead);
void expression(char **buffer, int *current_line, AtomType *lookahead);
void relational(char **buffer, int *current_line, AtomType *lookahead);
void simple_expression(char **buffer, int *current_line, AtomType *lookahead);
void add_operator(char **buffer, int *current_line, AtomType *lookahead);
void term(char **buffer, int *current_line, AtomType *lookahead);
void multiply_operator(char **buffer, int *current_line, AtomType *lookahead);
void factor(char **buffer, int *current_line, AtomType *lookahead);

int main(void)
{

    char *buffer = read_file("code.pas");
    int *current_line = (int *)malloc(sizeof(int));

    *current_line = 1;

    AtomType lookahead = get_atom(&buffer, current_line)->type;

    printf("Starting to process code...\n");

    program(&buffer, current_line, &lookahead);

    if (lookahead == EOS)
    {
        printf("Program was accepted\n");
    }
    else
    {
        printf("ERRORRRRR\n");
    }

    printf("Finished to process program\n");
    free(current_line);

    return 0;
}

void consume(char **buffer, int *current_line, AtomType *lookahead, AtomType current_atom_type)
{
    if (*lookahead == current_atom_type)
    {
        *lookahead = get_atom(buffer, current_line)->type;
    }
    else
    {
        printf("Syntactic error \n");
        exit(1);
    }
}

void program(char **buffer, int *current_line, AtomType *lookahead)
{
    consume(buffer, current_line, lookahead, PROGRAM);
    consume(buffer, current_line, lookahead, IDENTIFIER);
    consume(buffer, current_line, lookahead, SEMICOLON);
    block(buffer, current_line, lookahead);
    consume(buffer, current_line, lookahead, DOT);
}

void block(char **buffer, int *current_line, AtomType *lookahead)
{
    variable_declaration(buffer, current_line, lookahead);
    composed_command(buffer, current_line, lookahead);
}

void variable_declaration(char **buffer, int *current_line, AtomType *lookahead)
{
    type(buffer, current_line, lookahead);
    variable(buffer, current_line, lookahead);
    consume(buffer, current_line, lookahead, SEMICOLON);
}

void type(char **buffer, int *current_line, AtomType *lookahead)
{

    if (*lookahead == INTEGER)
    {

        consume(buffer, current_line, lookahead, INTEGER);
    }
    else if (*lookahead == REAL_NUMBER)
    {

        consume(buffer, current_line, lookahead, REAL_NUMBER);
    }
    else if (*lookahead == CHAR)
    {

        consume(buffer, current_line, lookahead, CHAR);
    }
    else if (*lookahead == BOOLEAN)
    {

        consume(buffer, current_line, lookahead, BOOLEAN);
    }
}

void variable(char **buffer, int *current_line, AtomType *lookahead)
{
    consume(buffer, current_line, lookahead, IDENTIFIER);
    while (*lookahead == COLON)
    {
        consume(buffer, current_line, lookahead, COLON);
        consume(buffer, current_line, lookahead, IDENTIFIER);
    }
}

void composed_command(char **buffer, int *current_line, AtomType *lookahead)
{

    consume(buffer, current_line, lookahead, BEGIN);
    command(buffer, current_line, lookahead);

    while (*lookahead == SEMICOLON)
    {

        consume(buffer, current_line, lookahead, SEMICOLON);
        command(buffer, current_line, lookahead);
    }

    consume(buffer, current_line, lookahead, END);
}

void command(char **buffer, int *current_line, AtomType *lookahead)
{
    if (*lookahead == IDENTIFIER)
    {
        assignment_command(buffer, current_line, lookahead);
    }

    if (*lookahead == IF)
    {
        if_command(buffer, current_line, lookahead);
    }

    if (*lookahead == WHILE)
    {
        while_command(buffer, current_line, lookahead);
    }

    if (*lookahead == READ)
    {
        read_command(buffer, current_line, lookahead);
    }

    if (*lookahead == WRITE)
    {
        write_command(buffer, current_line, lookahead);
    }

    if (*lookahead == BEGIN)
    {
        composed_command(buffer, current_line, lookahead);
    }
}

void assignment_command(char **buffer, int *current_line, AtomType *lookahead)
{
    consume(buffer, current_line, lookahead, IDENTIFIER);
    consume(buffer, current_line, lookahead, ASSIGNMENT);
    expression(buffer, current_line, lookahead);
}

void if_command(char **buffer, int *current_line, AtomType *lookahead)
{
    consume(buffer, current_line, lookahead, IF);
    consume(buffer, current_line, lookahead, PARENTHESIS_START);
    expression(buffer, current_line, lookahead);
    consume(buffer, current_line, lookahead, PARENTHESIS_END);
    consume(buffer, current_line, lookahead, THEN);
    command(buffer, current_line, lookahead);

    if (*lookahead == ELSE)
    {
        consume(buffer, current_line, lookahead, ELSE);
        command(buffer, current_line, lookahead);
    }
}

void while_command(char **buffer, int *current_line, AtomType *lookahead)
{
    consume(buffer, current_line, lookahead, WHILE);
    consume(buffer, current_line, lookahead, PARENTHESIS_START);
    expression(buffer, current_line, lookahead);
    consume(buffer, current_line, lookahead, PARENTHESIS_END);
    consume(buffer, current_line, lookahead, DO);
    command(buffer, current_line, lookahead);
}

void read_command(char **buffer, int *current_line, AtomType *lookahead)
{
    consume(buffer, current_line, lookahead, READ);
    consume(buffer, current_line, lookahead, PARENTHESIS_START);
    variable(buffer, current_line, lookahead);
    consume(buffer, current_line, lookahead, PARENTHESIS_END);
}

void write_command(char **buffer, int *current_line, AtomType *lookahead)
{
    consume(buffer, current_line, lookahead, WRITE);
    consume(buffer, current_line, lookahead, PARENTHESIS_START);
    expression(buffer, current_line, lookahead);

    while (*lookahead == COLON)
    {
        consume(buffer, current_line, lookahead, COLON);
        expression(buffer, current_line, lookahead);
    }
    consume(buffer, current_line, lookahead, PARENTHESIS_END);
}

void expression(char **buffer, int *current_line, AtomType *lookahead)
{
    simple_expression(buffer, current_line, lookahead);

    if (*lookahead == GREATER_THAN || *lookahead == EQUAL_OR_GREATER_THAN || *lookahead == LESS_THAN || *lookahead == EQUAL_OR_LESS_THAN || *lookahead == DIFFERENT)
    {

        relational(buffer, current_line, lookahead);
        simple_expression(buffer, current_line, lookahead);
    }
}

void relational(char **buffer, int *current_line, AtomType *lookahead)
{
    if (*lookahead == GREATER_THAN)
    {
        consume(buffer, current_line, lookahead, GREATER_THAN);
    }
    else if (*lookahead == EQUAL_OR_GREATER_THAN)
    {
        consume(buffer, current_line, lookahead, EQUAL_OR_GREATER_THAN);
    }
    else if (*lookahead == LESS_THAN)
    {
        consume(buffer, current_line, lookahead, LESS_THAN);
    }
    else if (*lookahead == EQUAL_OR_LESS_THAN)
    {
        consume(buffer, current_line, lookahead, EQUAL_OR_LESS_THAN);
    }
    else if (*lookahead == DIFFERENT)
    {
        consume(buffer, current_line, lookahead, DIFFERENT);
    }
}

void simple_expression(char **buffer, int *current_line, AtomType *lookahead)
{
    if (*lookahead == ADDITION)
    {
        consume(buffer, current_line, lookahead, ADDITION);
    }
    else if (*lookahead == SUBTRACTION)
    {
        consume(buffer, current_line, lookahead, SUBTRACTION);
    }

    term(buffer, current_line, lookahead);

    if (*lookahead == ADDITION || *lookahead == SUBTRACTION || *lookahead == OR)
    {
        add_operator(buffer, current_line, lookahead);
        term(buffer, current_line, lookahead);
    }
}

void add_operator(char **buffer, int *current_line, AtomType *lookahead)
{
    if (*lookahead == ADDITION)
    {
        consume(buffer, current_line, lookahead, ADDITION);
    }
    else if (*lookahead == SUBTRACTION)
    {
        consume(buffer, current_line, lookahead, SUBTRACTION);
    }
    else if (*lookahead == OR)
    {
        consume(buffer, current_line, lookahead, OR);
    }
}

void term(char **buffer, int *current_line, AtomType *lookahead)
{
    factor(buffer, current_line, lookahead);

    if (*lookahead == MULTIPLICATION || *lookahead == DIVISION || *lookahead == MOD || *lookahead == AND)
    {
        multiply_operator(buffer, current_line, lookahead);
        factor(buffer, current_line, lookahead);
    }
}

void multiply_operator(char **buffer, int *current_line, AtomType *lookahead)
{
    if (*lookahead == MULTIPLICATION)
    {
        consume(buffer, current_line, lookahead, MULTIPLICATION);
    }
    else if (*lookahead == DIVISION)
    {
        consume(buffer, current_line, lookahead, DIVISION);
    }
    else if (*lookahead == MOD)
    {
        consume(buffer, current_line, lookahead, MOD);
    }
    else if (*lookahead == AND)
    {
        consume(buffer, current_line, lookahead, AND);
    }
}

void factor(char **buffer, int *current_line, AtomType *lookahead)
{
    if (*lookahead == IDENTIFIER)
    {
        consume(buffer, current_line, lookahead, IDENTIFIER);
    }
    else if (*lookahead == INTEGER_NUMBER)
    {
        consume(buffer, current_line, lookahead, INTEGER_NUMBER);
    }
    else if (*lookahead == REAL_NUMBER)
    {
        consume(buffer, current_line, lookahead, REAL_NUMBER);
    }
    else if (*lookahead == CHAR)
    {
        consume(buffer, current_line, lookahead, CHAR);
    }
    else if (*lookahead == BOOLEAN)
    {
        consume(buffer, current_line, lookahead, BOOLEAN);
    }
    else if (*lookahead == NOT)
    {
        consume(buffer, current_line, lookahead, NOT);
        factor(buffer, current_line, lookahead);
    }
    else if (*lookahead == PARENTHESIS_START)
    {
        consume(buffer, current_line, lookahead, PARENTHESIS_START);
        expression(buffer, current_line, lookahead);
        consume(buffer, current_line, lookahead, PARENTHESIS_END);
    }
}
