#include <stdio.h>
#include "pretty.h"

void prettyPROGRAM(PROGRAM *p) {
    if(p->variable_decls != 0) {
        prettyVAR_DECL(p->variable_decls);
    }

    if(p->statements != 0) {
        prettySTATEMENT(p->statements);
    }
}

void prettyVAR_DECL(VAR_DECL *v) {
    switch(v->kind) {
        case k_variableDeclKindDecl:
            printf("var ");
            prettyEXP(v->val.decl.identifier);
            printf(": ");
            switch(v->val.decl.type) {
                case t_typeInteger:
                    printf("int"); break;
                case t_typeFloat:
                    printf("float"); break;
                case t_typeBool:
                    printf("bool"); break;
                case t_typeString:
                    printf("string"); break;
            }
            printf(" = ");
            prettyEXP(v->val.decl.value);
            printf("\n");
            break;
        case k_variableDeclKindDeclList:
            prettyVAR_DECL(v->val.var_decl_list.next);
            prettyVAR_DECL(v->val.var_decl_list.var_decl);
            break;
    }
}

void prettySTATEMENT(STATEMENT *s) {
    switch(s->kind) {
        case k_statementKindRead:
            printf("read ");
            prettyEXP(s->val.unary_stmt);
            printf("\n");
            break;
        case k_statementKindPrint:
            printf("print ");
            prettyEXP(s->val.unary_stmt);
            printf("\n");
            break;
        case k_statementKindAssignment:
            prettyEXP(s->val.assignment.identifier);
            printf(" = ");
            prettyEXP(s->val.assignment.value);
            printf("\n");
            break;
        case k_statementKindIfElse:
            printf("if (");
            prettyEXP(s->val.ifelseblock.expr);
            printf(") ");
            printf("{");
            printf("\n");
            prettySTATEMENT(s->val.ifelseblock.bodyblock);
            printf("}");
            if(s->val.ifelseblock.elseblock != 0) {
                printf(" else ");
                printf("{");
                printf("\n");
                prettySTATEMENT(s->val.ifelseblock.elseblock);
                printf("}");
            }
            break;
        case k_statementKindWhile:
            printf("while (");
            prettyEXP(s->val.whileblock.expr);
            printf(") ");
            printf("{");
            printf("\n");
            prettySTATEMENT(s->val.whileblock.bodyblock);
            printf("}");
            printf("\n");
            break;
        case k_statementKIndStatementList:
            prettySTATEMENT(s->val.stmt_list.next);
            prettySTATEMENT(s->val.stmt_list.stmt);
            break;
    }
}

void prettyEXP(EXP *e) {
    switch(e->kind) {
        case k_expressionKindIntLiteral:
            printf("%d", e->val.intLiteral);
            break;
        case k_expressionKindFloatLiteral:
            printf("%.f", e->val.floatLiteral);
            break;
        case k_expressionKindStringLiteral:
            printf("%s", e->val.stringLiteral);
            break;
        case k_expressionKindBooleanLiteral:
            printf("%d", e->val.boolLiteral);
            break;
        case k_expressionKindIdentifier:
            printf("%s", e->val.identifer);
            break;

        case k_expressionKindAddition:
            printf("(");
            prettyEXP(e->val.binary.lhs);
            printf("+");
            prettyEXP(e->val.binary.rhs);
            printf(")");
            break;
        case k_expressionKindSubtraction:
            printf("(");
            prettyEXP(e->val.binary.lhs);
            printf("-");
            prettyEXP(e->val.binary.rhs);
            printf(")");
            break;
        case k_expressionKindMultiplication:
            printf("(");
            prettyEXP(e->val.binary.lhs);
            printf("*");
            prettyEXP(e->val.binary.rhs);
            printf(")");
            break;
        case k_expressionKindDivision:
            printf("(");
            prettyEXP(e->val.binary.lhs);
            printf("/");
            prettyEXP(e->val.binary.rhs);
            printf(")");
            break;
        case k_expressionKindEquals:
            printf("(");
            prettyEXP(e->val.binary.lhs);
            printf("==");
            prettyEXP(e->val.binary.rhs);
            printf(")");
            break;
        case k_expressionKindNotEquals:
            printf("(");
            prettyEXP(e->val.binary.lhs);
            printf("!=");
            prettyEXP(e->val.binary.rhs);
            printf(")");
            break;
        case k_expressionKindLogicalAnd:
            printf("(");
            prettyEXP(e->val.binary.lhs);
            printf("&&");
            prettyEXP(e->val.binary.rhs);
            printf(")");
            break;
        break;
        case k_expressionKindLogicalOr:
            printf("(");
            prettyEXP(e->val.binary.lhs);
            printf("||");
            prettyEXP(e->val.binary.rhs);
            printf(")");
            break;

        case k_expressionKindNegate:
            printf("-");
            printf("(");
            prettyEXP(e->val.unary);
            printf(")");
            break;
    }
}