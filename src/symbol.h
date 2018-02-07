#ifndef SYMBOL_H
#define SYMBOL_H

#include "tree.h"

#define HashSize 317

typedef struct SymbolTable SymbolTable;
struct SymbolTable {
    SYMBOL *table[HashSize];
    SymbolTable *next;
};

SymbolTable* initSymbolTable();
SymbolTable* scopeSymbolTable(SymbolTable* s);
SYMBOL* putSymbol(SymbolTable* t, char* name, enum SymbolKind kind);
SYMBOL* getSymbol(SymbolTable* t, char* name);

#endif