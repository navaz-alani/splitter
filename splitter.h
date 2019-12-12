/*
 * SPLITTER MACHINE
 * © Navaz Alani 2019
 * Contact: nalani@uwaterloo.ca
 * License: MIT
 * 
 * This interface describes a fixed-state splitter
 * machine. The user defines targets and can then
 * split amounts between the targets, specifying
 * exemptions to splits.
 * 
 * A typical use case would be splitting a long
 * grocery bill amongst a group of people, where 
 * some of the people are exempt from the split 
 * for certain items.
 */

#pragma once
#include <stdbool.h>
#include "src/splitter.c"

// Fixed-state Splitter machine
struct Splitter;

// newSplitter() returns a pointer to heap-allocated
// memory for a freshly initialized Splitter machine.
struct Splitter *newSplitter(void);

// addTarget(s, name, sym) adds a target with the
// corresponding name and symbol to the Splitter
// machine
bool addTarget(struct Splitter *s, char *name, char sym);

// addAmount(s, amt, exemptions) splits amt amongst the
// targets in the Splitter machine s, which are not
// exempted from the split.
bool addAmount(struct Splitter *s, const double amt, const char *exemptions);

// printSplitter(s) prints out each of the targets
// with their symbols, and the amount split to them.
// Format used: targetName (symbol) :
void printSplitter(struct Splitter *s);

// destroySplitter(s) gracefully frees the memory
// allocated for the Splitter
void destroySplitter(struct Splitter *s);
