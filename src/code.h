#ifndef CODE_H
#define CODE_H

#include <stdio.h>
#include "tree.h"

FILE * codeFile = 0;

void initializeCode(FILE * f);

void codePROGRAM(PROGRAM* p);
void codePREAMBLE();
void codeEPILOGUE();
void codeVAR_DECL(VAR_DECL* v);
void codeSTATEMENT(STATEMENT* s);
void codeEXP(EXP* e);

char* typeToCStr(enum AllowedTypes t);
char* typeToCStrType(enum AllowedTypes t);
#endif