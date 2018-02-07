#include "tree.h"

EXP *makeEXP_intLiteral(int intLiteral, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->lineno = lineno;
    e->kind = k_expressionKindIntLiteral;
    e->val.intLiteral = intLiteral;
    return e;
}

EXP *makeEXP_floatLiteral(float floatLiteral, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->lineno = lineno;
    e->kind = k_expressionKindFloatLiteral;
    e->val.floatLiteral = floatLiteral;
    return e;
}

EXP *makeEXP_stringLiteral(char* stringLiteral, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->lineno = lineno;
    e->kind = k_expressionKindStringLiteral;
    e->val.stringLiteral = stringLiteral;
    return e;
}

EXP *makeEXP_boolLiteral(int boolLiteral, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->lineno = lineno;
    e->kind = k_expressionKindBooleanLiteral;
    e->val.boolLiteral = boolLiteral;
    return e;
}

EXP *makeEXP_identifier(char* identifier, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->lineno = lineno;
    e->kind = k_expressionKindIdentifier;
    e->val.identifer = identifier;
    return e;
}

EXP *makeEXP_binary(enum ExpressionKind k, EXP* lhs, EXP* rhs, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->lineno = lineno;
    e->kind = k;
    e->val.binary.lhs = lhs;
    e->val.binary.rhs = rhs;
    return e;
}

EXP *makeEXP_negate(EXP* ref, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->lineno = lineno;
    e->kind = k_expressionKindNegate;
    e->val.unary = ref;
    return e;
}

VAR_DECL *makeVARDECL_varDeclaration(EXP* identifier, enum AllowedTypes type, EXP* val, int lineno) {
    // make sure identifier is an identifier
    if(identifier->kind != k_expressionKindIdentifier) {
        // signal error
    }

    VAR_DECL *v = malloc(sizeof(VAR_DECL));
    v->lineno = lineno;
    v->kind = k_variableDeclKindDecl;
    v->val.decl.identifier = identifier;
    v->val.decl.type = type;
    v->val.decl.value = val;

    return v;
}

VAR_DECL *makeVARDECL_varDeclarationList(VAR_DECL* vardec, VAR_DECL* next, int lineno) {
    // make sure vardec is a variable declaration
    if(vardec->kind != k_variableDeclKindDecl) {
        // signal error
    }

    if(next->kind != k_variableDeclKindDecl || next->kind != k_variableDeclKindDeclList) {
        // signal error
    }

    VAR_DECL *v = malloc(sizeof(VAR_DECL));
    v->lineno = lineno;
    v->kind = k_variableDeclKindDeclList;
    v->val.var_decl_list.var_decl = vardec;
    v->val.var_decl_list.next = next;

    return v;
}

STATEMENT *makeSTATEMENT_readStatement(EXP* identifier, int lineno) {
    if(identifier->kind != k_expressionKindIdentifier) {
        // signal error
    }

    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = k_statementKindRead;
    s->val.unary_stmt.identifier = identifier;
    return s;
}

STATEMENT *makeSTATEMENT_printStatement(EXP* identifier, int lineno) {
    if(identifier->kind != k_expressionKindIdentifier) {
        // signal error
    }

    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = k_statementKindPrint;
    s->val.unary_stmt.identifier = identifier;
    return s;
}

STATEMENT *makeSTATEMENT_assignmentStatement(EXP* identifier, EXP* value, int lineno) {
    if(identifier->kind != k_expressionKindIdentifier) {
        // signal error
    }

    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = k_statementKindAssignment;
    s->val.assignment.identifier = identifier;
    s->val.assignment.value = value;
    return s;
}

STATEMENT *makeSTATEMENT_ifElseStatement(EXP* expr, STATEMENT* bodyblock, STATEMENT* elseblock, int lineno) {
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = k_statementKindIfElse;
    s->val.ifelseblock.expr = expr;
    s->val.ifelseblock.bodyblock = bodyblock;
    if(elseblock != 0) {
        s->val.ifelseblock.elseblock = elseblock;
    }

    return s;
}

STATEMENT *makeSTATEMENT_whileStatement(EXP* expr, STATEMENT* bodyblock, int lineno) {
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = k_statementKindWhile;
    s->val.whileblock.expr = expr;
    s->val.whileblock.bodyblock = bodyblock;

    return s;
}

STATEMENT *makeSTATMENT_statementList(STATEMENT* stmt, STATEMENT* next, int lineno) {
    STATEMENT *s = malloc(sizeof(STATEMENT));
    s->lineno = lineno;
    s->kind = k_statementKIndStatementList;
    s->val.stmt_list.stmt = stmt;
    s->val.stmt_list.next = next;

    return s;
}

PROGRAM *makePROGRAM_programBody(VAR_DECL* var_decl_list, STATEMENT* stmt_list, int lineno) {
    PROGRAM *p = malloc(sizeof(PROGRAM));
    p->variable_decls = var_decl_list;
    p->statements = stmt_list;

    return p;
}