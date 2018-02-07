#include "symbol.h"

int hash(char* str) {
    unsigned int hash = 0;
    while(*str) hash = (hash << 1) + *str++;
    return hash % HashSize;
}

SymbolTable* initSymbolTable() {
    SymbolTable* t = malloc(sizeof(SymbolTable));
    for(int i = 0; i < HashSize; i++) {
        t->table[i] = 0; // initialize the whole table to null
    }
    t->next = 0;

    return t;
}

SymbolTable* scopeSymbolTable(SymbolTable* s) {
    SymbolTable* newTable = initSymbolTable();
    newTable->next = s;
    return newTable;
}

SYMBOL* putSymbol(SymbolTable* t, char* name, enum SymbolKind kind) {
    int hashEntry = hash(name);
    SYMBOL* symbol;
    // traverse the whole symbols at this location for the name
    // return it if it exists
    for(symbol = t->table[hashEntry]; symbol; symbol = symbol->next) {
        if(strcmp(symbol->name, name) == 0) { return symbol; }
    }

    // does not exists, create a new symbol
    symbol = malloc(sizeof(SYMBOL));
    symbol->name = name;
    symbol->kind = kind;
    symbol->next = t->table[hashEntry];
    t->table[hashEntry] = symbol; // add to top of the list
    return symbol;
}

SYMBOL* getSymbol(SymbolTable* t, char* name) {
    int hashEntry = hash(name);
    SYMBOL* symbol;
    // traverse the whole symbols at this location for the name
    // return it if it exists
    for(symbol = t->table[hashEntry]; symbol; symbol = symbol->next) {
        if(strcmp(symbol->name, name) == 0) { return symbol; }
    }

    // symbol not found, recurse on outer scope symbol tables
    if(t->next == 0) { return 0; }
    return getSymbol(t->next, name);
}