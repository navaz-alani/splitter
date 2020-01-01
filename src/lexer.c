#include "splitter.c"

struct splitter *s;

void add_target(char *yytext, int yyleng)
{
    if (!s)
    {
        printf("ERROR [ADD-t]: Expected ':targets'\n");
        exit(1);
    }

    if (splitter_mode(s) == ITEM)
    {
        printf("ERROR [ADD-t]: Expected items!\n");
        exit(1);
    }

    char *name = NULL;
    char sym = '\0';

    for (int i = 0; i < yyleng; ++i)
    {
        if (yytext[i] == ' ' ||
            yytext[i] == '\t' ||
            yytext[i] == '\n')
        {
            yytext[i] = '\0';
            continue;
        }

        if (!name)
            name = (yytext + i);
        else if (!sym)
        {
            if (yytext[i - 1])
                continue;

            sym = *(yytext + i);
            break;
        }
    }

    if (!name)
        printf("ERROR [ADD-t]: Expected target name!\n");
    else if (!sym)
        printf("ERROR [ADD-t]: Expected target symbol!\n");
    else
    {
        if (!splitter_addt(s, name, sym))
        {
            printf("ERROR [ADD-t]: Failed to add target '%s, %c'!\n", name, sym);
            exit(1);
        }
    }
}

void add_item(char *yytext, int yyleng)
{
    if (!s)
    {
        printf("ERROR [ADD-i]: Expected ':targets'\n");
        exit(1);
    }

    if (splitter_mode(s) == TARGET)
    {
        printf("ERROR [ADD-i]: Expected targets!\n");
        exit(1);
    }

    char *amt = NULL;
    char *syms = NULL;
    char inc_exc = '\0';

    for (int i = 0; i < yyleng; ++i)
    {
        if (yytext[i] == ' ' ||
            yytext[i] == '\t' ||
            yytext[i] == '\n')
        {
            yytext[i] = '\0';
            continue;
        }

        if (!amt)
            amt = (yytext + i);
        else if (!syms)
        {
            if (yytext[i - 1])
                continue;

            syms = (yytext + i);
        }
        else if (!inc_exc)
        {
            if (yytext[i - 1])
                continue;

            inc_exc = *(yytext + i);
            break;
        }
    }

    if (!amt)
        printf("ERROR [ADD-i]: Expected item name!\n");
    else
    {
        double add_amt = atof(amt);

        enum split_by split;

        if (!inc_exc || inc_exc == 'e')
            split = EXCLUDE;
        else if (inc_exc == 'i')
            split = INCLUDE;
        else
        {
            printf("ERROR [ADD-i]: Expected split by 'e' or 'i'; got '%c'\n", inc_exc);
            exit(1);
        }

        if (!splitter_addi(s, add_amt, split, syms))
        {
            printf("ERROR [ADD-i]: Failed to add item '%f, %s'!\n", add_amt, syms);
            exit(1);
        }
    }
}
