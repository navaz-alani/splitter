//
// Created by Navaz Alani on 2019-12-20.
//

// #include "splitter.h"
#pragma once
#include "target.c"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

enum split_by
{
    INCLUDE = 0,
    EXCLUDE = 1
};

enum mode
{
    TARGET = 0,
    ITEM = 1
};

struct splitter
{
    int target_count;
    struct target *targets;
    enum mode mode;
};

// mk_splitter() returns a pointer to a fresh splitter
//   machine which is initialized in TAREGET mode, which
//   is when the machine will only accept new TARGETS.
// splitter_chmod(s) may be used to change modes over to
//   ITEM mode which allows adding new items.
struct splitter *mk_splitter()
{
    struct splitter *new = malloc(sizeof(struct splitter));

    if (!new)
        return NULL;

    new->target_count = 0;
    new->targets = NULL;
    new->mode = TARGET;

    return new;
}

// free_splitter(s) frees all of the memory allocated for
//   the splitter machine s.
void free_splitter(struct splitter *s)
{
    if (!s)
        return;

    struct target *curr = s->targets;

    while (curr)
    {
        struct target *tmp = curr->next;
        free_target(curr);
        curr = tmp;
    }

    free(s);
}

enum mode splitter_mode(struct splitter *s)
{
    if (!s)
        return false;
    return s->mode;
}

// splitter_chmod(s) changes the mode of the splitter to
//   allow Items to be added.
// NOTE: this cannot be used to revert to TARGET mode
bool splitter_chmod(struct splitter *s)
{
    if (!s)
        return false;
    if (!s->target_count)
        return false;
    s->mode = ITEM;
    return true;
}

// splitter_target_count(s) returns the number of
//   targets in s.
int splitter_target_count(struct splitter *s)
{
    if (!s)
        return -1;
    return s->target_count;
}

// splitter_addt(s, name, sym) adds a target with the specified
//   name and symbol. Returns true if successful.
// If this function is not called in TARGET mode, no change is made.
bool splitter_addt(struct splitter *s, char *t_name, char t_sym)
{
    if (!s)
        return false;

    struct target *new = mk_target(t_name, t_sym, NULL);
    if (!new)
        return false;

    bool added = false;
    struct target *curr = s->targets;
    struct target *prev = NULL;

    if (curr == NULL)
    {
        s->targets = new;
        goto exit_seq;
    }

    while (curr)
    {
        if (curr->sym < t_sym)
        {
            if (curr->next)
            {
                prev = curr;
                curr = curr->next;
            }
            else
            {
                target_ch_next(curr, new);
                break;
            }
        }
        else
        {
            target_ch_next(new, curr);
            if (!prev)
                s->targets = new;
            else
                target_ch_next(prev, new);
            break;
        }
    }

exit_seq:
    ++s->target_count;
    return true;
}

static int sym_compare(const void *sym1, const void *sym2)
{
    return *(char *)sym1 - *(char *)sym2;
}

// spitter_addi(s, amt, split_by, t_syms) splits the amount amt
//   including/excluding targets mentioned in t_syms as specified
//   by the enum split_by. Returns true if successful.
// REQUIRES:
// NOTE: If unsuccessful, false is returned and the final state of
//   the splitter machine cannot be guaranteed to be valid.
//   When this happens, the user is advised to fix input and rerun.
// In such cases, the machine may only be considered intact when this
//   function is called when not in ITEM mode.
bool splitter_addi(struct splitter *s, double amt,
                   enum split_by split_by, const char *t_syms)
{
    printf("amt: %f Got syms: %s, enum: %d\n", amt, t_syms, split_by);
    if (!s)
        return false;

    int sym_count = (strcmp(t_syms, "-")) ? strlen(t_syms) : s->target_count;
    int diff = s->target_count - sym_count;

    if (diff < 0)
        return false;
    else if (diff == 0 && split_by == EXCLUDE)
        return true;

    int split_count = (split_by == INCLUDE)
                          ? sym_count
                          : diff;

    double per_target = amt / split_count;

    if (strcmp(t_syms, "-"))
        qsort((void *)t_syms, sym_count, sizeof(char), sym_compare);
    printf("sorted syms: %s\n", t_syms);

    struct target *curr = s->targets;
    int curr_sym = 0;

    while (curr)
    {
        if (split_by == INCLUDE)
        {
            if (diff == 0)
                curr->amt += per_target;
            else if (strcmp(t_syms, "-") && curr->sym == t_syms[curr_sym])
            {
                curr->amt += per_target;
                ++curr_sym;
            }
        }
        else
        {
            if (diff == 0 || curr->sym != t_syms[curr_sym])
                curr->amt += per_target;
            else if (curr->sym == t_syms[curr_sym])
                ++curr_sym;
        }

        curr = curr->next;
    }

    return true;
}

// splitter_print_targets(s) prints out the names and
//   symbols of the targets stored in s.
void splitter_print_targets(struct splitter *s)
{
    if (!s || !s->targets)
        return;

    struct target *curr = s->targets;
    int count = 1;

    while (curr)
    {
        printf("Target %d: %s, %c\n",
               count++, curr->name, curr->sym);
        curr = curr->next;
    }
}

// splitter_print_all(s) prints out names of targets stored
//   in s along with their symbols and individual amounts.
void splitter_print_all(struct splitter *s)
{
    if (!s || !s->targets)
        return;

    struct target *curr = s->targets;
    int count = 1;

    while (curr)
    {
        printf("%d. %s, %c: %lf\n",
               count++, curr->name, curr->sym, curr->amt);
        curr = curr->next;
    }
}
