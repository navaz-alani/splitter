#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Internal representation of Target for the
// Splitter machine
struct Target
{
    char *name;
    char sym;
    double amt;
    struct Target *next;
    bool roundExemption;
};

struct Target *newTarget(char *name, char sym)
{
    struct Target *new = malloc(sizeof(struct Target));

    new->amt = 0.0;
    new->name = name;
    new->sym = sym;
    new->roundExemption = false;

    return new;
}

void destroyTarget(struct Target *t)
{
    free(t->name);
    free(t);
}

// Fixed-state splitter machine
struct Splitter
{
    int split_count;
    struct Target *people;
};

struct Splitter *newSplitter(void)
{
    struct Splitter *new = malloc(sizeof(struct Splitter));
    new->people = NULL;
    new->split_count = 0;

    return new;
}

bool addTarget(struct Splitter *s, char *name, char sym)
{
    if (s->split_count != 0)
    {
        // verify that they symbol is unique
        struct Target *current = s->people;
        struct Target *next = current->next;

        while (current != NULL)
        {
            if (current->sym == sym)
                return false;

            current = next;
            next = current->next;
        }
    }

    struct Target *new = newTarget(name, sym);
    new->next = s->people;
    s->people = new;
    s->split_count++;

    return true;
}

static bool isExempt(struct Target *p, const char *exempt)
{
    while (*exempt != '\0')
    {
        if (p->sym == *exempt)
            return true;

        exempt++;
    }

    return false;
}

bool addAmount(struct Splitter *s, const double amt, const char *exempt)
{
    if (s->split_count == 0)
        return false;

    int exemptCount = 0;
    struct Target *current = s->people;
    struct Target *next = current->next;

    // count and mark exemptions
    while (current != NULL)
    {
        if (isExempt(current, exempt))
        {
            exemptCount++;
            current->roundExemption = true;
        }

        current = next;
        next = current->next;
    }

    if (exemptCount == s->split_count)
        return false;

    double splitAmt = amt / (s->split_count - exemptCount);
    current = s->people;
    next = current->next;

    while (current != NULL)
    {
        if (!current->roundExemption)
            current->amt += splitAmt;

        // reset round exemption
        current->roundExemption = false;

        current = next;
        next = current->next;
    }

    return true;
}

void printSplitter(struct Splitter *s)
{
    if (s->split_count == 0)
        return;

    struct Target *current = s->people;
    struct Target *next = current->next;

    while (current != NULL)
    {
        printf("%s (%c): %.2f\n",
               current->name,
               current->sym,
               current->amt);

        current = next;
        next = current->next;
    }
}

// Frees the memory that was allocated for
// the splitter machine.
void destroySplitter(struct Splitter *s)
{
    if (s->split_count != 0)
    {
        struct Target *current = s->people;
        struct Target *next = current->next;

        while (current != NULL)
        {
            destroyTarget(current);
            current = next;
            next = current->next;
        }
    }

    free(s);
}
