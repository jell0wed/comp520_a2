#include <stdio.h>
#include "pretty.h"

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
            prettyEXP(e->val.refExp);
            printf(")");
            break;

        case k_expressionKindVarDecl:
            printf("var ");
            prettyEXP(e->val.var_decl.identifier);
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
            prettyEXP(e->val.var_decl.val);
            printf("\n");
            break;
        case k_expressionKindVarDeclarationList:
            prettyEXP(e->val.var_decl_list.next);
            prettyEXP(e->val.var_decl_list.var_decl);
            break;

        case k_expressionKindReadStatement:
            printf("read ");
            printf("(");
            prettyEXP(e->val.unary_stmt.identifier);
            printf(")");
            printf("\n");
            break;
        case k_expressionKindPrintStatement:
            printf("print ");
            printf("(");
            prettyEXP(e->val.unary_stmt.identifier);
            printf(")");
            printf("\n");
            break;
        case k_expressionKindAssignmentStatement:
            prettyEXP(e->val.assignment.identifier);
            printf(" = ");
            prettyEXP(e->val.assignment.value);
            printf("\n");
            break;
        case k_expressionKindIfElseStatement:
            printf("if (");
            prettyEXP(e->val.ifelsestmt.expr);
            printf(") ");
            printf("{");
            printf("\n");
            prettyEXP(e->val.ifelsestmt.bodyblock);
            printf("}");
            if(e->val.ifelsestmt.elseblock != 0) {
                printf(" else ");
                printf("{");
                printf("\n");
                prettyEXP(e->val.ifelsestmt.elseblock);
                printf("}");
            }
            break;
        case k_expressionKindWhileStatement:
            printf("while (");
            prettyEXP(e->val.whilestmt.expr);
            printf(")");
            printf("{");
            printf("\n");
            prettyEXP(e->val.whilestmt.bodyblock);
            printf("}");
            break;

        case k_expressionKindStatementList:
            prettyEXP(e->val.stmt_list.next);
            prettyEXP(e->val.stmt_list.stmt);
            break;

        case k_expressionProgramBody:
            prettyEXP(e->val.programbody.var_decl_list);
            printf("\n");
            prettyEXP(e->val.programbody.stmt_list);
            break;
    }
}