#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "splitter.h"
#include "parser.h"

int main(int argc, const char **argv)
{
    // No input files; exit
    if (argc == 1)
    {
        printf("ERROR: No input files.\n");
        return 1;
    }

    // check that all files are valid
    for (int i = 1; i < argc; ++i)
    {
        if (access(argv[i], F_OK) == -1)
        {
            printf("ERROR: File '%s' does not exist.\n", argv[i]);
            return 1;
        }
    }

    // run the machine on each file
    for (int i = 1; i < argc; ++i)
    {
        const char *filename = argv[i];

        printf("\n*** Now Processing '%s' ***\n", filename);
        parse(filename);
        printf("\n*** Done Processing '%s' ***\n", filename);
    }
}
