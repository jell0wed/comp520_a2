#ifndef TREE_H
#define TREE_H

#include "types.h"

typedef struct EXP EXP;
struct EXP {
    enum ExpressionKind kind;
    enum AllowedTypes inferredType;
    int lineno;
    union {
        char* identifer;
        int intLiteral;
        int boolLiteral;
        float floatLiteral;
        char* stringLiteral;
        struct { EXP* lhs; EXP* rhs; } binary;
        EXP* unary;
    } val;
};

typedef struct VAR_DECL VAR_DECL;
struct VAR_DECL {
    enum VariableDeclKind kind;
    int lineno;
    union {
        struct { EXP* identifier; enum AllowedTypes type; EXP* value; } decl;
        struct { VAR_DECL* var_decl; VAR_DECL* next; } var_decl_list;
    } val;
};

typedef struct STATEMENT STATEMENT;
struct STATEMENT {
    enum StatementKind kind;
    int lineno;
    union {
        EXP* unary_stmt;
        struct { EXP* identifier; EXP* value; } assignment;
        struct { EXP* expr; STATEMENT* bodyblock; STATEMENT* elseblock; } ifelseblock;
        struct { EXP* expr; STATEMENT* bodyblock; } whileblock;
        struct { STATEMENT* stmt; STATEMENT* next; } stmt_list;
    } val;
};

typedef struct PROGRAM PROGRAM;
struct PROGRAM {
    VAR_DECL* variable_decls;
    STATEMENT* statements;
};

typedef struct SYMBOL SYMBOL;
struct SYMBOL {
    char* name;
    enum SymbolKind kind; 
    union {
        VAR_DECL* var;
    } val;
    struct SYMBOL* next;
};

EXP *makeEXP_intLiteral(int intLiteral, int lineno);
EXP *makeEXP_floatLiteral(float floatLiteral, int lineno);
EXP *makeEXP_boolLiteral(int boolLiteral, int lineno);
EXP *makeEXP_stringLiteral(char* stringLiteral, int lineno);
EXP *makeEXP_identifier(char* identifier, int lineno);
EXP *makeEXP_binary(enum ExpressionKind k, EXP* lhs, EXP* rhs, int lineno);
EXP *makeEXP_negate(EXP* e, int lineno);
EXP *makeEXP_numNegate(EXP* e, int lineno);

VAR_DECL *makeVARDECL_varDeclaration(EXP* identifier, enum AllowedTypes type, EXP* val, int lineno);
VAR_DECL *makeVARDECL_varDeclarationList(VAR_DECL* vardec, VAR_DECL* next, int lineno);

STATEMENT *makeSTATEMENT_readStatement(EXP* identifier, int lineno);
STATEMENT *makeSTATEMENT_printStatement(EXP* e, int lineno);
STATEMENT *makeSTATEMENT_assignmentStatement(EXP* identifier, EXP* value, int lineno);
STATEMENT *makeSTATEMENT_ifElseStatement(EXP* expr, STATEMENT* bodyblock, STATEMENT* elseblock, int lineno);
STATEMENT *makeSTATEMENT_whileStatement(EXP* expr, STATEMENT* bodyblock, int lineno);
STATEMENT *makeSTATMENT_statementList(STATEMENT* stmt, STATEMENT* next, int lineno);

PROGRAM *makePROGRAM_programBody(VAR_DECL* var_decl_list, STATEMENT* stmt_list, int lineno);
#endif