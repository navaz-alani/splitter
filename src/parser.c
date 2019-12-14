#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "support.c"

const int BUFFER_SIZE = 100;

const char *TARGET_BLOCK_START = ":targets";
const char *TARGET_BLOCK_END = ":end-targets";
const char *ITEMS_BLOCK_START = ":items";
const char *ITEMS_BLOCK_END = ":end-items";

bool parseItems(struct Splitter *s, FILE *f)
{
    // A multi purpose buffer for the function
    char *buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));

    int len = 0;
    readLine(f, buffer, &len);

    if (strcmp(buffer, ITEMS_BLOCK_START) != 0)
    {
        printf("ERROR [PARSING]: Expected '%s'\n", ITEMS_BLOCK_START);
        goto panic;
    }

    readLine(f, buffer, &len);
    while (strcmp(buffer, ITEMS_BLOCK_END) != 0)
    {
        // add the item
        int wordLen = 0;
        char *amt_str = readWord(buffer, &wordLen);

        char *exemptions = buffer + wordLen + 1;
        if (!addAmount(s, atof(amt_str), exemptions))
        {
            printf("ERROR [PARSING/AddTarget]: Unable to add item '%.02f %s'\n",
                   atof(amt_str), exemptions);
            printf("                           All targets exempt!\n");
            goto panic;
        }

        readLine(f, buffer, &len);
    }

    free(buffer);
    return true;

panic:
    free(buffer);
    return false;
}

bool parseTargets(struct Splitter *s, FILE *f)
{
    char *buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));

    int len = 0;
    readLine(f, buffer, &len);

    // First line of the document declares targets block
    if (strcmp(buffer, TARGET_BLOCK_START) != 0)
    {
        printf("ERROR [PARSING]: Expected '%s'\n", TARGET_BLOCK_START);
        goto panic;
    }

    readLine(f, buffer, &len);
    while (strcmp(buffer, TARGET_BLOCK_END) != 0)
    {
        int wordCount = 0;
        char **lineSplit = splitStr(buffer, &wordCount);

        if (wordCount == 0)
            goto nextIteration;
        else if (wordCount == 1)
        {
            printf("ERROR [PARSING/readTargets]: Expected target symbol\n");
            goto panic;
        }

        char *name = lineSplit[0];
        char sym = lineSplit[1][0];

        if (!addTarget(s, name, sym))
        {
            printf("ERROR [PARSING/AddTarget]: Unable to add target '%s (%c)'\n",
                   name, sym);
            printf("                           Symbol not unique\n");
            goto panic;
        }

    nextIteration:
        readLine(f, buffer, &len);
    }

    free(buffer);
    return true;

panic:
    free(buffer);
    return false;
}

void parse(const char *filename)
{
    // Sure that the file exists
    FILE *f = fopen(filename, "r");

    struct Splitter *s = newSplitter();

    if (!parseTargets(s, f) || s->split_count == 0)
    {
        printf("> Failed to parse targets! Exiting...\n");
        goto panic;
    }

    if (!parseItems(s, f))
    {
        printf("> Failed to parse items! Exiting...\n");
        goto panic;
    }

    printf("\nFinal Splitter Machine State:\n");
    printSplitter(s);

panic:
    destroySplitter(s);

    if (fclose(f) != 0)
        printf("ERROR: Failed to close file '%s'\n", filename);
}
