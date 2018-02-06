#ifndef TREE_H
#define TREE_H

#include "types.h"

typedef struct EXP EXP;
struct EXP {
    enum ExpressionKind kind;
    enum ExpressionFamily family;
    union {
        char *identifer;
        int intLiteral;
        int boolLiteral;
        float floatLiteral;
        char* stringLiteral;
        struct { EXP *lhs; EXP *rhs; } binary;
        EXP* refExp;

        struct { EXP *identifier; enum AllowedTypes type; EXP *val; } var_decl;
        struct { EXP *var_decl; EXP *next; } var_decl_list;

        struct { EXP *identifier; } unary_stmt;
        struct { EXP *identifier; EXP *value; } assignment;

        struct { EXP *stmt; EXP *next; } stmt_list;

        struct { EXP *expr; EXP *bodyblock; EXP *elseblock; } ifelsestmt;
        struct { EXP *expr; EXP *bodyblock; } whilestmt;

        struct { EXP *var_decl_list; EXP *stmt_list; } programbody;
    } val;
};

EXP *makeEXP_intLiteral(int intLiteral);
EXP *makeEXP_floatLiteral(float floatLiteral);
EXP *makeEXP_boolLiteral(int boolLiteral);
EXP *makeEXP_stringLiteral(char* stringLiteral);
EXP *makeEXP_identifier(char* identifier);

EXP *makeEXP_binary(enum ExpressionKind k, EXP* lhs, EXP* rhs);
EXP *makeEXP_negate(EXP* e);

EXP *makeEXP_varDeclaration(EXP* identifier, enum AllowedTypes type, EXP* val);
EXP *makeEXP_varDeclarationList(EXP* vardec, EXP* next);

EXP *makeEXP_readStatement(EXP* identifier);
EXP *makeEXP_writeStatement(EXP* identifier);
EXP *makeEXP_assignmentStatement(EXP* identifier, EXP* value);
EXP *makeEXP_ifElseStatement(EXP* expr, EXP* bodyblock, EXP* elseblock);
EXP *makeEXP_whileStatement(EXP* expr, EXP* bodyblock);

EXP *makeEXP_statementList(EXP* stmt, EXP* next);

EXP *makeEXP_programBody(EXP* var_decl_list, EXP* stmt_list);
#endif