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
            // first make sure the initial expression is fine
            symImplementationEXP(t, v->val.decl.value);
                
            // make sure identifier is not already declared
            if(defSymbol(t, v->val.decl.identifier->val.identifer)) {
                reportStrError("\"%s\" is already declared",
                                v->val.decl.identifier->val.identifer,
                                v->lineno);
            } else { // put it in the symbol table
                SYMBOL* s = putSymbol(t, v->val.decl.identifier->val.identifer, st_symbolVariable);
                s->val.var = v; // link the variable with the symbol
            }
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
            symImplementationEXP(t, s->val.unary_stmt);
            break;
        case k_statementKindPrint:
            symImplementationEXP(t, s->val.unary_stmt);
            break;
        case k_statementKindAssignment:
            // make sure the idenfifier is declared
            symImplementationEXP(t, s->val.assignment.identifier);
            break;
        case k_statementKindIfElse:
            // make sure the identifier is declared
            symImplementationEXP(t, s->val.ifelseblock.expr);
            symImplementationSTATEMENT(t, s->val.ifelseblock.bodyblock);
            symImplementationSTATEMENT(t, s->val.ifelseblock.elseblock);
            break;
        case k_statementKindWhile:
            // make sure the identifier is declared
            symImplementationEXP(t, s->val.whileblock.expr);
            symImplementationSTATEMENT(t, s->val.whileblock.bodyblock);
            break;
    }
}

void symImplementationEXP(SymbolTable* t, EXP* e) {
    if(e == 0) { return; }
    switch(e->kind) {
        case k_expressionKindIntLiteral:
        case k_expressionKindFloatLiteral:
        case k_expressionKindStringLiteral:
        case k_expressionKindBooleanLiteral:
            break;
        case k_expressionKindIdentifier: // makes sure the identifier exists
            if(!defSymbol(t, e->val.identifer)) {
                reportStrError("unknown symbol \"%s\" (probably not declared)", e->val.identifer, e->lineno);
            }
            break;
        case k_expressionKindAddition:
        case k_expressionKindSubtraction:
        case k_expressionKindMultiplication:
        case k_expressionKindDivision:
        case k_expressionKindEquals:
        case k_expressionKindNotEquals:
        case k_expressionKindLogicalAnd:
        case k_expressionKindLogicalOr:
            symImplementationEXP(t, e->val.binary.lhs);
            symImplementationEXP(t, e->val.binary.rhs);
            break;
        case k_expressionKindNegate:
            symImplementationEXP(t, e->val.unary);
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

void printSymbolTable(SymbolTable *t) {
    if(t == 0) { return; }
    printSymbolTable(t->next);
    for(int i = 0; i < HashSize; i++) {
        printSymbol(t->table[i]);
    }
}

void printSymbol(SYMBOL* s) {
    if(s == 0) { return; }
    printSymbol(s->next);
    
    printf("Symbol<%s>(%s)\n", symbolKindToStr(s->kind), s->name);
}

char* symbolKindToStr(enum SymbolKind k) {
    switch(k) {
        case st_symbolVariable: return "st_symbolVariable";    
    }
    return "unknown";
}
