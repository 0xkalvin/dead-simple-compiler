#include "file_parser.h"

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
