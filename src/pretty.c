#include <stdio.h>
#include "pretty.h"

void prettyPROGRAM(PROGRAM *p) {
    if(p->variable_decls != 0) {
        prettyVAR_DECL(p->variable_decls);
    }
    printf("\n");

    if(p->statements != 0) {
        prettySTATEMENT(p->statements);
    }
}

void prettyTYPE(enum AllowedTypes type) {
    switch(type) {
        case t_typeInteger:
            printf("int"); break;
        case t_typeFloat:
            printf("float"); break;
        case t_typeBool:
            printf("bool"); break;
        case t_typeString:
            printf("string"); break;
    }
}

void prettyVAR_DECL(VAR_DECL *v) {
    if(v == 0) { return; }
    switch(v->kind) {
        case k_variableDeclKindDeclList:
            prettyVAR_DECL(v->val.var_decl_list.next);
            prettyVAR_DECL(v->val.var_decl_list.var_decl);
            return;
        case k_variableDeclKindDecl:
            printf("var ");
            prettyEXP(v->val.decl.identifier);
            printf(": ");
            prettyTYPE(v->val.decl.type);
            printf(" = ");
            prettyEXP(v->val.decl.value);
            break;
    }
    printf(";");
    printf("\n");
}
int ident = 0;
int currentIdent = 0;
void autoIdent() {
    for(int i = currentIdent; i < ident; i++) {
        printf("\t");
        currentIdent++;
    }
}
void newLine() {
    currentIdent = 0;
    printf("\n");
}
void prettySTATEMENT(STATEMENT *s) {
    if(s == 0) { return; }
    autoIdent();
    switch(s->kind) {
        case k_statementKIndStatementList:
            prettySTATEMENT(s->val.stmt_list.next);
            prettySTATEMENT(s->val.stmt_list.stmt);
            return;
        case k_statementKindRead:
            printf("read ");
            prettyEXP(s->val.unary_stmt);
            printf(";");
            break;
        case k_statementKindPrint:
            printf("print ");
            prettyEXP(s->val.unary_stmt);
            printf(";");
            break;
        case k_statementKindAssignment:
            prettyEXP(s->val.assignment.identifier);
            printf(" = ");
            prettyEXP(s->val.assignment.value);
            printf(";");
            break;
        case k_statementKindIfElse:
            printf("if (");
            prettyEXP(s->val.ifelseblock.expr);
            printf(") ");
            printf("{");
            newLine();
            ident++;
            prettySTATEMENT(s->val.ifelseblock.bodyblock);
            ident--;
            if(s->val.ifelseblock.elseblock != 0) {
                autoIdent();
                printf("} else ");
                printf("{");
                newLine();
                ident++;
                prettySTATEMENT(s->val.ifelseblock.elseblock);
                ident--;
                autoIdent();
                printf("}");
            } else {
                autoIdent();
                printf("}");
            }
            break;
        case k_statementKindWhile:
            printf("while (");
            prettyEXP(s->val.whileblock.expr);
            printf(") ");
            printf("{");
            newLine();
            ident++;
            prettySTATEMENT(s->val.whileblock.bodyblock);
            ident--;
            autoIdent();
            printf("}");
            break;
    }
    newLine();
}

void prettyEXP(EXP *e) {
    switch(e->kind) {
        case k_expressionKindIntLiteral:
            printf("%d", e->val.intLiteral);
            break;
        case k_expressionKindFloatLiteral:
            printf("%.5f", e->val.floatLiteral);
            break;
        case k_expressionKindStringLiteral:
            printf("\"%s\"", e->val.stringLiteral);
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
            printf("!");
            printf("(");
            prettyEXP(e->val.unary);
            printf(")");
            break;
        
        case k_expressionKindNumNegate:
            printf("-");
            printf("(");
            prettyEXP(e->val.unary);
            printf(")");
            break;
    }
}