#include "symbol.h"
#include "error.h"

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

// Checks if a given symbol is declared within the scope
int defSymbol(SymbolTable* t, char* name) {
    int hashEntry = hash(name);
    SYMBOL* symbol;
    for(symbol = t->table[hashEntry]; symbol; symbol = symbol->next) {
        if(strcmp(symbol->name, name) == 0) { return 1; }
    }

    return 0;
}

void symPROGRAM(PROGRAM* p) {
    mainSymbolTable = initSymbolTable(); // initialize new symtable
    // we normally have 3 passes
    // symInterfacePROGRAM : define classes and their interfaces;
    // symInterfaceTypesPROGRAM : build hierarchy and analyze interface types; and
    // symImplementationPROGRAM : define locals and analyze method bodies.
    // because the mini language does not allow for class definition,
    // references or even method, the only required phase in the 
    // symbol table construction is the implementation one
    symImplementationPROGRAM(mainSymbolTable, p);
}

void symImplementationPROGRAM(SymbolTable* t, PROGRAM *p) {
    if(p->variable_decls != 0) {
        symImplementationVAR_DECL(t, p->variable_decls);
    }

    if(p->statements != 0) {
        symImplementationSTATEMENT(t, p->statements);
    }
}

void symImplementationVAR_DECL(SymbolTable* t, VAR_DECL* v) {
    if(v == 0) { return; }
    switch(v->kind) {
        case k_variableDeclKindDeclList: // recursively iterate
            symImplementationVAR_DECL(t, v->val.var_decl_list.var_decl);
            symImplementationVAR_DECL(t, v->val.var_decl_list.next);
            break; 
        case k_variableDeclKindDecl:
            // make sure identifier is not already declared
            if(defSymbol(t, v->val.decl.identifier->val.identifer)) {
                reportStrError("\"%s\" is already declared",
                                v->val.decl.identifier->val.identifer,
                                v->val.decl.identifier->lineno);
            }
            // make sure it is a valid type
            symTYPE(t, v->lineno, v->val.decl.type);
            break;
    }
}

void symImplementationSTATEMENT(SymbolTable* t, STATEMENT* s) {
    if(s == 0) { return; }
    switch(s->kind) {
        case k_statementKIndStatementList: // recursively iterate
            symImplementationSTATEMENT(t, s->val.stmt_list.stmt);
            symImplementationSTATEMENT(t, s->val.stmt_list.next);
            break;
        case k_statementKindRead:
            // make sure the identifier is declared
            symImplementationEXP(t, s->val.unary_stmt.identifier);
            break;
        case k_statementKindPrint:
            // make sure the identifier is declared
            symImplementationEXP(t, s->val.unary_stmt.identifier);
            break;
        case k_statementKindAssignment:
            // make sure the idenfifier is declared
            symImplementationEXP(t, s->val.assignment.identifier);
            break;
        case k_statementKindIfElse:
            // make sure the identifier is declared
            symImplementationEXP(t, s->val.ifelseblock.expr);
            break;
        case k_statementKindWhile:
            // make sure the identifier is declared
            symImplementationEXP(t, s->val.whileblock.expr);
            break;
    }
}

void symImplementationEXP(SymbolTable* t, EXP* e) {
    if(e == 0) { return; }
    switch(e->kind) {
        case k_expressionKindIdentifier: // makes sure the identifier exists
            if(!defSymbol) {
                reportStrError("\"%s\" is not declared", e->val.identifer, e->lineno);
            }
            break;
    }
}

// although mini does not support custom type definition
// we use this function to validate the basic types supported
// in the language (ints, floats, bools, strings)
// since this is an enum, this should in theory never trigger an error...
void symTYPE(SymbolTable* t, int lineno, enum AllowedTypes usedType) {
    switch(usedType) {
        case t_typeInteger:
        case t_typeFloat:
        case t_typeBool:
        case t_typeString: 
            break;
    }

    reportStrError("Invalid type %d", (int) usedType, lineno);    
}

void symTYPE_LITERAL_EXP(SymbolTable* t, enum AllowedTypes usedType, EXP* e) {
    
}