#include "tree.h"

EXP *makeEXP_intLiteral(int intLiteral, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Literal;
    e->kind = k_expressionKindIntLiteral;
    e->val.intLiteral = intLiteral;
    return e;
}

EXP *makeEXP_floatLiteral(float floatLiteral, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Literal;
    e->kind = k_expressionKindFloatLiteral;
    e->val.floatLiteral = floatLiteral;
    return e;
}

EXP *makeEXP_stringLiteral(char* stringLiteral, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Literal;
    e->kind = k_expressionKindStringLiteral;
    e->val.stringLiteral = stringLiteral;
    return e;
}

EXP *makeEXP_boolLiteral(int boolLiteral, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Literal;
    e->kind = k_expressionKindBooleanLiteral;
    e->val.boolLiteral = boolLiteral;
    return e;
}

EXP *makeEXP_identifier(char* identifier, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Literal;
    e->kind = k_expressionKindIdentifier;
    e->val.identifer = identifier;
    return e;
}

EXP *makeEXP_binary(enum ExpressionKind k, EXP* lhs, EXP* rhs, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Expression;
    e->kind = k;
    e->val.binary.lhs = lhs;
    e->val.binary.rhs = rhs;
    return e;
}

EXP *makeEXP_negate(EXP* ref, int lineno) {
    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Expression;
    e->kind = k_expressionKindNegate;
    e->val.refExp = ref;
    return e;
}

EXP *makeEXP_varDeclaration(EXP* identifier, enum AllowedTypes type, EXP* val, int lineno) {
    // make sure identifier is an identifier
    if(identifier->kind != k_expressionKindIdentifier) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->family = ef_VarDeclarations;
    e->kind = k_expressionKindVarDecl;
    e->val.var_decl.identifier = identifier;
    e->val.var_decl.type = type;
    e->val.var_decl.val = val;

    return e;
}

EXP *makeEXP_varDeclarationList(EXP* vardec, EXP* next, int lineno) {
    // make sure vardec is a variable declaration
    if(vardec->kind != k_expressionKindVarDecl) {
        // signal error
    }

    if(next->kind != k_expressionKindVarDeclarationList || next->kind != k_expressionKindVarDecl) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->family = ef_VarDeclarations;
    e->kind = k_expressionKindVarDeclarationList;
    e->val.var_decl_list.var_decl = vardec;
    e->val.var_decl_list.next = next;

    return e;
}

EXP *makeEXP_readStatement(EXP* identifier, int lineno) {
    if(identifier->kind != k_expressionKindIdentifier) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Statements;
    e->kind = k_expressionKindReadStatement;
    e->val.unary_stmt.identifier = identifier;
    return e;
}

EXP *makeEXP_printStatement(EXP* identifier, int lineno) {
    if(identifier->kind != k_expressionKindIdentifier) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Statements;
    e->kind = k_expressionKindPrintStatement;
    e->val.unary_stmt.identifier = identifier;
    return e;
}

EXP *makeEXP_assignmentStatement(EXP* identifier, EXP* value, int lineno) {
    if(identifier->kind != k_expressionKindIdentifier) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->kind = k_expressionKindAssignmentStatement;
    e->val.assignment.identifier = identifier;
    e->val.assignment.value = value;
    return e;
}

EXP *makeEXP_statementList(EXP* stmt, EXP* next, int lineno) {
    // make sure vardec is a variable declaration
    if(stmt->kind != k_expressionKindVarDecl) {
        // signal error
    }

    if(next->family != ef_Statements) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Statements;
    e->kind = k_expressionKindStatementList;
    e->val.stmt_list.stmt = stmt;
    e->val.stmt_list.next = next;

    return e;
}

EXP *makeEXP_ifElseStatement(EXP* expr, EXP* bodyblock, EXP* elseblock, int lineno) {
    if(expr->family != ef_Expression) {
        // signal  error
    }

    if(bodyblock->family != ef_Statements) {
        // signal error
    }

    if(elseblock != 0 && elseblock->family != ef_Statements) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Statements;
    e->kind = k_expressionKindIfElseStatement;
    e->val.ifelsestmt.expr = expr;
    e->val.ifelsestmt.bodyblock = bodyblock;
    if(elseblock != 0) {
        e->val.ifelsestmt.elseblock = elseblock;
    }

    return e;
}

EXP *makeEXP_whileStatement(EXP* expr, EXP* bodyblock, int lineno) {
    if(expr->family != ef_Expression) {
        // signal  error
    }

    if(bodyblock->family != ef_Statements) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Statements;
    e->kind = k_expressionKindWhileStatement;
    e->val.whilestmt.expr = expr;
    e->val.whilestmt.bodyblock = bodyblock;

    return e;
}

EXP *makeEXP_programBody(EXP* var_decl_list, EXP* stmt_list, int lineno) {
    if(stmt_list->family != ef_Statements) {
        // signal error
    }

    if(var_decl_list != 0 && var_decl_list->family != ef_VarDeclarations) {
        // signal error
    }

    EXP *e = malloc(sizeof(EXP));
    e->family = ef_Root;
    e->kind = k_expressionProgramBody;
    e->val.programbody.var_decl_list = var_decl_list;
    e->val.programbody.stmt_list = stmt_list;

    return e;
}