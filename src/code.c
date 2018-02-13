#include "code.h"

void initializeCode(FILE * f) {
    codeFile = f;
}

void codePREAMBLE() {
    fprintf(codeFile, "#include <stdio.h>\n");
    fprintf(codeFile, "#define TRUE 1\n");
    fprintf(codeFile, "#define FALSE 0\n");
    fprintf(codeFile, "char __READ_BOOL[5];\n");
    fprintf(codeFile, "char* strconcat(char* str, int num) { char buf[1024] = \"\"; for(int i = 0; i < num; i++) { strcat(buf, str); } return strdup(buf); }\n");
    fprintf(codeFile, "char* strplus(char* str1, char* str2) { char buf[1024] = \"\"; strcat(buf, str1); strcat(buf, str2); return strdup(buf); }\n");
    fprintf(codeFile, "void readBool(int* boolVal) { if (strcmp(__READ_BOOL, \"TRUE\") == 0) { *boolVal = TRUE; } else if (strcmp(__READ_BOOL, \"FALSE\") == 0) { *boolVal = FALSE; } else { fprintf(stderr, \"Error! Bool value readed is invalid\"); exit(1); } } \n");
    fprintf(codeFile, "char* printBool(int boolVal) { return boolVal == TRUE ? \"TRUE\" : \"FALSE\"; }\n");
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
            codeVAR_DECL(v->val.var_decl_list.next);
            codeVAR_DECL(v->val.var_decl_list.var_decl);
            break;
        case k_variableDeclKindDecl:
            fprintf(codeFile, "%s %s = ", typeToCStr(v->val.decl.type), v->val.decl.identifier->val.identifer);
            codeEXP(v->val.decl.value);
            fprintf(codeFile, ";");
            break;
    }
    fprintf(codeFile, "\n");
}

void codeSTATEMENT(STATEMENT* s) {
    if(s == 0) { return; }
    switch(s->kind) {
        case k_statementKIndStatementList:
            codeSTATEMENT(s->val.stmt_list.next);
            codeSTATEMENT(s->val.stmt_list.stmt);
            break;
        case k_statementKindRead:
            // bonus points 8) read bool as strings
            if(s->val.unary_stmt->inferredType == t_typeBool) {
                fprintf(codeFile, "scanf(\"%s\", __READ_BOOL);\n", "%s");
                fprintf(codeFile, "readBool(&%s);", s->val.unary_stmt->val.identifer); 
                break;
            }

            fprintf(codeFile, "scanf(\"%s\", %s);", 
                    typeToCStrType(s->val.unary_stmt->inferredType), 
                    s->val.unary_stmt->val.identifer); // it must be the case that it is an identifier
            break;
        case k_statementKindPrint:
            // bonus points 8) print bool as TRUE or FALSE
            if(s->val.unary_stmt->inferredType == t_typeBool) {
                fprintf(codeFile, "printf(\"%s\", printBool(", "%s");
                codeEXP(s->val.unary_stmt);
                fprintf(codeFile, "));");
                break;
            }

            fprintf(codeFile, "printf(\"%s\", ", typeToCStrType(s->val.unary_stmt->inferredType));
            codeEXP(s->val.unary_stmt);
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
            fprintf(codeFile, "%d", e->val.intLiteral);
            break;
        case k_expressionKindFloatLiteral:
            fprintf(codeFile, "%.5f", e->val.floatLiteral);
            break;
        case k_expressionKindBooleanLiteral:
            fprintf(codeFile, e->val.boolLiteral == 1 ? "TRUE" : "FALSE");
            break;
        case k_expressionKindStringLiteral:
            fprintf(codeFile, "\"%s\"", e->val.stringLiteral);
            break;
        case k_expressionKindIdentifier:
            fprintf(codeFile, "%s", e->val.identifer);
            break;
        case k_expressionKindAddition:
            // handle non std c case with string addition ;)
            if(e->val.binary.lhs->inferredType == t_typeString && 
                e->val.binary.rhs->inferredType == t_typeString) {
                fprintf(codeFile, "strplus(");
                codeEXP(e->val.binary.lhs);
                fprintf(codeFile, ", ");
                codeEXP(e->val.binary.rhs);
                fprintf(codeFile, ")");
                break;
            }
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
            // handle non std c case with string multiplication ;)
            if(e->val.binary.lhs->inferredType == t_typeString && 
                e->val.binary.rhs->inferredType == t_typeInteger) {
                int numRepeat = e->val.binary.rhs->val.intLiteral;
                fprintf(codeFile, "strconcat(");
                codeEXP(e->val.binary.lhs);
                fprintf(codeFile, ", %d)", numRepeat);
                break;
            } 

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
            // handle non std c case for string ;)
            if(e->val.binary.lhs->inferredType == t_typeString && 
                e->val.binary.rhs->inferredType == t_typeString) {
                fprintf(codeFile, "(strcmp(");
                codeEXP(e->val.binary.lhs);
                fprintf(codeFile, ", ");
                codeEXP(e->val.binary.rhs);
                fprintf(codeFile, ") == 0)");
                break;
            }

            fprintf(codeFile, "(");
            codeEXP(e->val.binary.lhs);
            fprintf(codeFile, " == ");
            codeEXP(e->val.binary.rhs);
            fprintf(codeFile, ")");
            break;
        case k_expressionKindNotEquals:
            // handle non std c case for string ;)
            if(e->val.binary.lhs->inferredType == t_typeString && 
                e->val.binary.rhs->inferredType == t_typeString) {
                fprintf(codeFile, "(strcmp(");
                codeEXP(e->val.binary.lhs);
                fprintf(codeFile, ", ");
                codeEXP(e->val.binary.rhs);
                fprintf(codeFile, ") != 0)");
                break;
            }

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
        case k_expressionKindNumNegate:
            fprintf(codeFile, "-(");
            codeEXP(e->val.unary);
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