#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern const int BUFFER_SIZE;

// readline(f, buff, lineLen) reads from f and writes
// to the buffer (there is a global buffer size).
// lineLen is set to the length of the line in characters.
// Initial whitespace is ignored.
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

// splitStr(s) returns a pointer to an array
// containing one word per index from s.
// len is then set to be the number of words in
// s (length of returned array...)
//
// - Will modify s provided!
char **splitStr(char *s, int *wordCount)
{
    int lineLen = strlen(s);
    int maxWords = 1;
    char **words = malloc(maxWords * sizeof(char *));
    int numSplit = 0;

    int i = 0;
    for ( i = 0; i < lineLen; i++)
    {
        // Double array size when needed...
        if (numSplit == maxWords)
        {
            maxWords *= 2;
            char **tmp = realloc(words, maxWords * sizeof(char *));

            if (tmp == NULL)
            {
                printf("ERROR: [PARSING/LineSplit/MEM]: Memory Error!\n");
                free(words);
                return NULL;
            }
            words = tmp;
        }
        
        if ((i == 0 && s[i] != ' ') ||
            (i > 0 && s[i] != ' ' && (s[i - 1] == ' ' || s[i - 1] == '\0')))
            words[numSplit++] = s + i;
        else if (s[i] == ' ') 
            s[i] = '\0';
    }

    *wordCount = numSplit;
    return words;
}