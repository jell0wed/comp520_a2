#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

FILE* stderr;

int errors = 0;

int lineno;

void yyerror(const char *s);

void reportError(char *s, int lineno);

void reportStrError(char *s, char *name, int lineno);

void reportGlobalError(char *s);

void reportStrGlobalError(char *s, char *name);

void noErrors();

extern int lineno;

#endif