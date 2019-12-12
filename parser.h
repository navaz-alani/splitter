/*
 * split PARSER
 * © Navaz Alani 2019
 * Contact: nalani@uwaterloo.ca
 * License: MIT
 * 
 * This interface provides a single function 
 * to parse the simple splitter syntax (example 
 * in test.splitter). It parses the file and 
 * uses the Splitter machine to split the 
 * items amongst the targets.
 */

#include "src/parser.c"

// parse(filename) will parse the content of
// the file and interpret it as splitter syntax.
//
// Output is sent to stdout.
void parse(const char *filename);
