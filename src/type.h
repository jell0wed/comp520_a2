#ifndef TYPE_H
#define TYPE_H

#include "tree.h"
#include "error.h" 

char* typeToStr(enum AllowedTypes t);

int checkINT(enum AllowedTypes t, int lineno);
int checkFLOAT(enum AllowedTypes t, int lineno);
int checkBOOL(enum AllowedTypes t, int lineno);
int checkSTRING(enum AllowedTypes t, int lineno);
int checkTypeEquals(enum AllowedTypes t1, enum AllowedTypes t2, int lineno);
int numericTypeImplicitCast(EXP* e);
int numericTypeMatches(EXP* e);
int logicalTypeMatches(EXP* e);
int assignmentTypeMatches(VAR_DECL* decl, EXP* e);
int ifElseWhileTypeMatches(EXP* e);

void typeImplementationPROGRAM(PROGRAM* p);
void typeImplementationVAR_DECL(VAR_DECL* v);
void typeImplementationSTATEMENT(STATEMENT* s);
void typeImplementationEXP(EXP* e);
void typeImplementationBinaryEXP(EXP* e);

#endif