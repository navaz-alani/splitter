#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const int BUFFER_SIZE = 100;

const char *TARGET_BLOCK_START = ":targets";
const char *TARGET_BLOCK_END = ":end-targets";
const char *ITEMS_BLOCK_START = ":items";
const char *ITEMS_BLOCK_END = ":end-items";

void readLine(FILE *f, char *buff, int *lineLen)
{
    int written = 0;
    // Initially, ignore whitespace
    bool ignoreWs = true;

    while (written < BUFFER_SIZE)
    {
        char c = fgetc(f);

        if (c == ' ' && ignoreWs)
            continue;

        if (c == '\n' || c == EOF)
        {
            buff[written] = '\0';
            *lineLen = written;
            return;
        }

        buff[written++] = c;
        // Toggle whitespace off
        ignoreWs = (ignoreWs) ? false : ignoreWs;
    }
    buff[written] = '\0';
    *lineLen = written;
}

char *readWord(const char *str, int *len)
{
    int wordSize = 1;
    char *w = malloc(wordSize * sizeof(char));

    int writeCount = 0;
    int strCounter = 0;

    while (str[strCounter] != ' ' && str[strCounter] != '\0')
    {
        // Doubling stategy for buffer
        if (writeCount == wordSize - 1)
        {
            wordSize *= 2;
            char *tmp = realloc(w, wordSize * sizeof(char));

            if (tmp != NULL)
                w = tmp;
            else
            {
                free(w);
                printf("ERROR [MEM]: Memory reallocation failed.\n");
                return NULL;
            }
        }

        w[writeCount++] = str[strCounter++];
    }

    w[writeCount] = '\0';
    *len = writeCount;
    return w;
}

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

        // if (wordLen > len - 2)
        // {
        //     printf("ERROR [PARSING/ReadTarget]: Expected exemptions symbol.\n");
        //     goto panic;
        // }

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
    // A multi purpose buffer for the function
    char *buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));

    int len = 0;
    readLine(f, buffer, &len);

    // First line of document declares 'targets'
    if (strcmp(buffer, TARGET_BLOCK_START) != 0)
    {
        printf("ERROR [PARSING]: Expected '%s'\n", TARGET_BLOCK_START);
        goto panic;
    }

    readLine(f, buffer, &len);
    while (strcmp(buffer, TARGET_BLOCK_END) != 0)
    {
        // add the target
        int wordLen = 0;
        char *name = readWord(buffer, &wordLen);

        if (wordLen > len - 2)
        {
            printf("ERROR [PARSING/ReadTarget]: Expected target symbol.\n");
            goto panic;
        }

        char sym = buffer[wordLen + 1];
        if (!addTarget(s, name, sym))
        {
            printf("ERROR [PARSING/AddTarget]: Unable to add target '%s (%c)'\n",
                   name, sym);
            printf("                           Symbol not unique\n");
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
