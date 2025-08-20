#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

void runProgram(char* src)
{
    Scanner scn;

    scanTokens(&scn, src);

    for (size_t i = 0; i < scn.list->i; i++)
        printf("[%s] ", scn.list->data[i].lexeme);

    scannerFree(&scn);
}

void runFile(const char* fname)
{
    FILE *fp = fopen(fname, "rb");
    if (!fp)
    {
        printf("Unable to access file %s\n", fname);
        // return 1;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char *buf = malloc(size + 1);

    fread(buf, 1, size, fp);
    buf[size] = '\0';

    fclose(fp);

    runProgram(buf);
    free(buf);
}

int runInteractive()
{
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Usage: ./cclox [file]\n");
        return 1;
    }
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        return runInteractive();
    }
}