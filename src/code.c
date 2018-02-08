#include "code.h"

void initializeCode(FILE * f) {
    codeFile = f;
}

void codePREAMBLE() {
    fprintf(codeFile, "#include <stdio.h>\n");
    fprintf(codeFile, "#define TRUE 1");
    fprintf(codeFile, "#define FALSE 0");
    fprintf(codeFile, "\n");
    fprintf(codeFile, "int main(int argc, char* argv[]) {\n");
}

void codeEPILOGUE() {
    fprintf(codeFile, "return 0;\n");
    fprintf(codeFile, "}");
}

void codePROGRAM(PROGRAM* p) {
    codePREAMBLE();
    if(p->variable_decls != 0) {
        codeVAR_DECL(p->variable_decls);
    }
    fprintf(codeFile, "\n"); // make it pretty boi

    if(p->statements != 0) {
        codeSTATEMENT(p->statements);
    }
    codeEPILOGUE();
}

void codeVAR_DECL(VAR_DECL* v) {
    if(v == 0) { return; }
    switch(v->kind) {
        case k_variableDeclKindDeclList:
            codeVAR_DECL(v->val.var_decl_list.var_decl);
            codeVAR_DECL(v->val.var_decl_list.next);
            break;
        case k_variableDeclKindDecl:
            fprintf(codeFile, "%s %s = ", typeToCStr(v->val.decl.type), v->val.decl.identifier->val.identifer);
            codeEXP(v->val.decl.value);
            break;
    }
    fprintf(codeFile, "\n");
}

void codeSTATEMENT(STATEMENT* s) {
    if(s == 0) { return; }
    switch(s->kind) {
        case k_statementKIndStatementList:
            codeSTATEMENT(s->val.stmt_list.stmt);
            codeSTATEMENT(s->val.stmt_list.next);
            break;
        case k_statementKindRead:
            fprintf(codeFile, "scanf(\"%s\", %s);", 
                    typeToCStrType(s->val.unary_stmt.e->inferredType), 
                    s->val.unary_stmt.e->val.identifer); // it must be the case that it is an identifier
            break;
        case k_statementKindPrint:
            fprintf(codeFile, "printf(\"%s\", ", typeToCStrType(s->val.unary_stmt.e->inferredType));
            codeEXP(s->val.unary_stmt.e);
            fprintf(codeFile, ");");
            break;
        case k_statementKindAssignment:
            fprintf(codeFile, "%s = ", s->val.assignment.identifier->val.identifer);
            codeEXP(s->val.assignment.value);
            fprintf(codeFile, ";");
            break;
        case k_statementKindIfElse:
            fprintf(codeFile, "if (");
            codeEXP(s->val.ifelseblock.expr);
            fprintf(codeFile, ") { \n");
            codeSTATEMENT(s->val.ifelseblock.bodyblock);
            fprintf(codeFile, "} ");
            if(s->val.ifelseblock.elseblock != 0) {
                fprintf(codeFile, "else { \n");
                codeSTATEMENT(s->val.ifelseblock.elseblock);
                fprintf(codeFile, "} ");
            }
            break;
        case k_statementKindWhile:
            fprintf(codeFile, "while (");
            codeEXP(s->val.whileblock.expr);
            fprintf(codeFile, ") { \n");
            codeSTATEMENT(s->val.whileblock.bodyblock);
            fprintf(codeFile, "} ");
            break;
            
    }
    fprintf(codeFile, "\n");
}

void codeEXP(EXP* e) {
    if(e == 0) { return; }
    switch(e->kind) {
        case k_expressionKindIntLiteral:
        case k_expressionKindFloatLiteral:
        case k_expressionKindBooleanLiteral:
        case k_expressionKindStringLiteral:
            fprintf(codeFile, typeToCStrType(e->inferredType), e->val.floatLiteral);
            break;
        case k_expressionKindIdentifier:
            fprintf(codeFile, "%s", e->val.identifer);
            break;
        case k_expressionKindAddition:
            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " + ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindSubtraction:
            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " - ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindMultiplication:
            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " * ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindDivision:
            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " / ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindEquals:
            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " == ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindNotEquals:
            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " != ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindLogicalAnd:
            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " && ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindLogicalOr:
            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " || ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindNegate:
            fprintf(codeFile, "!(");
            codeEXP(e->val.unary);
            fprintf(codeFile, ")");
            break;
    }
}

char* typeToCStr(enum AllowedTypes t) {
    switch(t) {
        case t_typeInteger: return "int";
        case t_typeFloat: return "float";
        case t_typeBool: return "int";
        case t_typeString: return "char*";
    }
    return "";
}

char* typeToCStrType(enum AllowedTypes t) {
    switch(t) {
        case t_typeInteger: return "%d";
        case t_typeFloat: return "%.5f";
        case t_typeBool: return "%d";
        case t_typeString: return "%s";
    }
    return "";
}