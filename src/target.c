//
// Created by Navaz Alani on 2019-12-20.
//

// #include "target.h"
#pragma once
#include <stdlib.h>
#include <string.h>

struct target
{
    char *name;
    char sym;
    double amt;
    struct target *next;
};

struct target *mk_target(const char *name, char sym, struct target *next)
{
    int name_len = strlen(name);

    if (name_len == 0)
    {
        return NULL;
    }

    struct target *new = malloc(sizeof(struct target));

    if (!new)
    {
        return NULL;
    }

    new->name = malloc((name_len + 1) * sizeof(char));
    strcpy(new->name, name);
    new->sym = sym;
    new->amt = 0.0;
    new->next = next;
    return new;
}

void free_target(struct target *t)
{
    if (!t)
    {
        return;
    }

    free(t->name);
    free(t);
}

void target_add(struct target *t, double amt)
{
    if (!t)
    {
        return;
    }

    t->amt += amt;
}

void target_ch_next(struct target *t, struct target *new_next)
{
    if (!t)
    {
        return;
    }

    t->next = new_next;
}