#include <stdio.h>
#include "pretty.h"

void autoIndent(int indent) {
    for(int i = 0; i < indent; i++) {
        printf("\t");
    }
}

void prettyEXP(EXP *e, int indent) {
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
            prettyEXP(e->val.binary.lhs, indent);
            printf("+");
            prettyEXP(e->val.binary.rhs, indent);
            printf(")");
            break;
        case k_expressionKindSubtraction:
            printf("(");
            prettyEXP(e->val.binary.lhs, indent);
            printf("-");
            prettyEXP(e->val.binary.rhs, indent);
            printf(")");
            break;
        case k_expressionKindMultiplication:
            printf("(");
            prettyEXP(e->val.binary.lhs, indent);
            printf("*");
            prettyEXP(e->val.binary.rhs, indent);
            printf(")");
            break;
        case k_expressionKindDivision:
            printf("(");
            prettyEXP(e->val.binary.lhs, indent);
            printf("/");
            prettyEXP(e->val.binary.rhs, indent);
            printf(")");
            break;
        case k_expressionKindEquals:
            printf("(");
            prettyEXP(e->val.binary.lhs, indent);
            printf("==");
            prettyEXP(e->val.binary.rhs, indent);
            printf(")");
            break;
        case k_expressionKindNotEquals:
            printf("(");
            prettyEXP(e->val.binary.lhs, indent);
            printf("!=");
            prettyEXP(e->val.binary.rhs, indent);
            printf(")");
            break;
        case k_expressionKindLogicalAnd:
            printf("(");
            prettyEXP(e->val.binary.lhs, indent);
            printf("&&");
            prettyEXP(e->val.binary.rhs, indent);
            printf(")");
            break;
        break;
        case k_expressionKindLogicalOr:
            printf("(");
            prettyEXP(e->val.binary.lhs, indent);
            printf("||");
            prettyEXP(e->val.binary.rhs, indent);
            printf(")");
            break;

        case k_expressionKindNegate:
            printf("-");
            printf("(");
            prettyEXP(e->val.refExp, indent);
            printf(")");
            break;

        case k_expressionKindVarDecl:
            printf("var ");
            prettyEXP(e->val.var_decl.identifier, indent);
            printf(": ");
            switch(e->val.var_decl.type) {
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
            prettyEXP(e->val.var_decl.val, indent);
            printf("\n");
            break;
        case k_expressionKindVarDeclarationList:
            prettyEXP(e->val.var_decl_list.next, indent);
            prettyEXP(e->val.var_decl_list.var_decl, indent);
            break;

        case k_expressionKindReadStatement:
            autoIndent(indent);
            printf("read ");
            printf("(");
            prettyEXP(e->val.unary_stmt.identifier, indent);
            printf(")");
            printf("\n");
            break;
        case k_expressionKindPrintStatement:
            autoIndent(indent);
            printf("print ");
            printf("(");
            prettyEXP(e->val.unary_stmt.identifier, indent);
            printf(")");
            printf("\n");
            break;
        case k_expressionKindAssignmentStatement:
            autoIndent(indent);
            prettyEXP(e->val.assignment.identifier, indent);
            printf(" = ");
            prettyEXP(e->val.assignment.value, indent);
            printf("\n");
            break;
        case k_expressionKindIfElseStatement:
            autoIndent(indent);
            printf("if (");
            prettyEXP(e->val.ifelsestmt.expr, indent);
            printf(") ");
            printf("{");
            printf("\n");
            autoIndent(indent);
            prettyEXP(e->val.ifelsestmt.bodyblock, indent + 1);
            printf("}");
            if(e->val.ifelsestmt.elseblock != 0) {
                printf(" else ");
                printf("{");
                printf("\n");
                autoIndent(indent);
                prettyEXP(e->val.ifelsestmt.elseblock, indent + 1);
                printf("}");
            }
            break;
        case k_expressionKindWhileStatement:
            printf("while (");
            prettyEXP(e->val.whilestmt.expr, indent);
            printf(") ");
            printf("{");
            printf("\n");
            autoIndent(indent);
            prettyEXP(e->val.whilestmt.bodyblock, indent + 1);
            printf("}");
            printf("\n");
            break;

        case k_expressionKindStatementList:
            prettyEXP(e->val.stmt_list.next, indent);
            prettyEXP(e->val.stmt_list.stmt, indent);
            break;

        case k_expressionProgramBody:
            prettyEXP(e->val.programbody.var_decl_list, indent);
            printf("\n");
            prettyEXP(e->val.programbody.stmt_list, indent);
            break;
    }
}