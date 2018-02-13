#ifndef PRETTY_H
#define PRETTY_H

#include "tree.h"

void prettyPROGRAM(PROGRAM *p);
void prettyEXP(EXP *e);
void prettyTYPE(enum AllowedTypes type);
void prettyVAR_DECL(VAR_DECL *v);
void prettySTATEMENT(STATEMENT *s);

#endif