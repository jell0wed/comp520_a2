#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include "tree.h"

#define HashSize 317

typedef struct SymbolTable SymbolTable;
struct SymbolTable {
    SYMBOL *table[HashSize];
    SymbolTable *next;
};

SymbolTable* mainSymbolTable;

SymbolTable* initSymbolTable();
SymbolTable* scopeSymbolTable(SymbolTable* s);
SYMBOL* putSymbol(SymbolTable* t, char* name, enum SymbolKind kind);
SYMBOL* getSymbol(SymbolTable* t, char* name);
int defSymbol(SymbolTable* t, char* name);
void assertSymbolExists(SymbolTable* t, char* name);

void symPROGRAM(PROGRAM *p);

void symImplementationPROGRAM(SymbolTable* t, PROGRAM *p);

void symImplementationVAR_DECL(SymbolTable* t, VAR_DECL* v);

void symImplementationSTATEMENT(SymbolTable* t, STATEMENT* s);

void symImplementationEXP(SymbolTable* t, EXP* e);

void symTYPE(SymbolTable* t, int lineno, enum AllowedTypes);
void symTYPE_LITERAL_EXP(SymbolTable* t, enum AllowedTypes usedType, EXP* e);

void printSymbolTable(SymbolTable *t);
void printSymbol(SYMBOL* s);
char* symbolKindToStr(enum SymbolKind k);

#endif