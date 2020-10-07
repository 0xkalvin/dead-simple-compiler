#include "helpers.h"

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
