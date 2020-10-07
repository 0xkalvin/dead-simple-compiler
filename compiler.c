#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum AtomType
{
    ERROR = 0,
    IDENTIFIER,
    INTEGER_NUMBER,
    ASSIGNMENT,
    WHILE,
    EOS
} AtomType;

char *atomTypesNames[] = {
    "LEXICAL ERROR",
    "IDENTIFIER",
    "INTEGER NUMBER",
    "ASSIGNMENT",
    "WHILE",
    "END OF STRING"};

typedef struct Atom
{
    AtomType type;
    int line;
    int attribute_for_number;
    char attribute_for_identifier[15];

} Atom;

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

void process_identifier(char **buffer, Atom *atom)
{
    char *start_of_identifier = *buffer;

    while (is_alpha_character(**buffer) || is_numeric_character(**buffer))
    {
        *buffer += 1;
    }

    strncpy(atom->attribute_for_identifier, start_of_identifier, *buffer - start_of_identifier);

    atom->attribute_for_identifier[*buffer - start_of_identifier] = 0;

    if (strcasecmp(atom->attribute_for_identifier, "WHILE") == 0)
    {

        atom->type = WHILE;
    }
    else
    {
        atom->type = IDENTIFIER;
    }
}

void process_number(char **buffer, Atom *atom)
{
    char *start_of_number = *buffer;

    while (is_numeric_character(**buffer))
    {
        *buffer += 1;
    }

    if (**buffer != '\n' && **buffer != '\r' && **buffer != '\t' && **buffer != ' ' && **buffer != 0)
    {
        return;
    }

    strncpy(atom->attribute_for_identifier, start_of_number, *buffer - start_of_number);
    atom->attribute_for_identifier[*buffer - start_of_number] = 0;
    atom->attribute_for_number = atoi(atom->attribute_for_identifier);
    atom->type = INTEGER_NUMBER;
}

Atom *get_atom(char **buffer, int* current_line)
{
    Atom *atom = (Atom *)malloc(sizeof(Atom));

    while (**buffer == '\n' || **buffer == '\r' || **buffer == '\t' || **buffer == ' ')
    {
        if(**buffer == '\n'){
            *current_line += 1;
        }

        *buffer += 1;
    }

    if (**buffer == ':' && *(*buffer + 1) == '=')
    {
        *buffer += 2;
        atom->type = ASSIGNMENT;
    }
    else if (is_numeric_character(**buffer))
    {
        process_number(buffer, atom);
    }
    else if (is_alpha_character(**buffer))
    {
        process_identifier(buffer, atom);
    }
    else if (**buffer == 0)
    {
        atom->type = EOS;
    }
    else
    {
        atom->type = ERROR;
    }

    return atom;
}

int main(void)
{

    char *buffer = read_file("code.pas");
    int *current_line = (int*)malloc(sizeof(int));

    *current_line = 1;

    while (1)
    {
        Atom *current_atom = get_atom(&buffer, current_line);

        printf("LINE %d | %s\t|", *current_line, atomTypesNames[current_atom->type]);

        if (current_atom->type == INTEGER_NUMBER)
        {
            printf(" %d ", current_atom->attribute_for_number);
        }
        else if (current_atom->type == IDENTIFIER)
        {
            printf(" %s ", current_atom->attribute_for_identifier);
        }
        else if (current_atom->type == EOS || current_atom->type == ERROR)
        {
            printf("\n");
            break;
        }
        printf("\n");
    }

    return 0;
}