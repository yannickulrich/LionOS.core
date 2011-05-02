/*
 * LOSPaL - LionOS Programming Language
 * Interpreter Header
 */
#ifndef LOSPaL_H
#define LOSPaL_H 1
#include "strings.h"
#include "array.h"
void execute(char* start);
bool validateSyntax(char start[]);
bool validateSyntaxLine(char start[]);
#endif
