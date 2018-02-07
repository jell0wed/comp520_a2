#include "tree.h"
#include "symbol.h"
#include "type.h"

char* typeToStr(enum AllowedTypes t) {
    switch(t) {
        t_typeInteger: return "int";
        t_typeFloat: return "float";
        t_typeBool: return "bool";
        t_typeString: return "string";
    }

    return "unknown";
}

int checkINT(enum AllowedTypes t, int lineno) {
    if(t != t_typeInteger) {
        reportError("integer type expected", lineno);
        return 0;
    }
    return 1;
}

int checkFLOAT(enum AllowedTypes t, int lineno) {
    if(t != t_typeFloat) {
        reportError("float type expected", lineno);
        return 0;
    }
    return 1;
}

int checkBOOL(enum AllowedTypes t, int lineno) {
    if(t != t_typeBool) {
        reportError("bool type expected", lineno);
        return 0;
    }
    return 1;
}

int checkSTRING(enum AllowedTypes t, int lineno) {
    if(t != t_typeString) {
        reportError("string type expected", lineno);
        return 0;
    }
    return 1;
}

int checkTypeEquals(enum AllowedTypes t1, enum AllowedTypes t2, int lineno) {
    if(t1 != t2) {
        reportError("unmatching types", lineno);
        return 0;
    }

    return 1;
}

int numericTypeImplicitCast(EXP* e) {
    EXP* e1 = e->val.binary.lhs;
    EXP* e2 = e->val.binary.rhs;
    if(e1->inferredType == t_typeFloat && e2->inferredType == t_typeInteger) {
        return 1;
    }

    if(e2->inferredType == t_typeFloat && e1->inferredType == t_typeInteger) {
        return 1;
    }

    return 0;
}

int numericTypeMatches(EXP* e) {
    EXP* e1 = e->val.binary.lhs;
    EXP* e2 = e->val.binary.rhs;

    if(e1->inferredType != e2->inferredType) {
        return 0;
    }

    return e1->inferredType == t_typeInteger ||
            e1->inferredType == t_typeFloat;
}

int logicalTypeMatches(EXP* e) {
    EXP* e1 = e->val.binary.lhs;
    EXP* e2 = e->val.binary.rhs;

    if(e1->inferredType != t_typeBool && e1->inferredType != t_typeInteger) {
        return 0;
    }

    if(e2->inferredType != t_typeBool && e2->inferredType != t_typeInteger) {
        return 0;
    }

    return 1;
}

int assignmentTypeMatches(VAR_DECL* decl, EXP* e) {
    switch(decl->val.decl.type) {
        case t_typeInteger:
            return checkINT(e->inferredType, e->lineno);
        case t_typeFloat:
            return (e->inferredType == t_typeInteger || checkFLOAT(e->inferredType, e->lineno));
        case t_typeString:
            return checkSTRING(e->inferredType, e->lineno);
        case t_typeBool:
            return checkBOOL(e->inferredType, e->lineno);
    }

    return 0;
}

int ifElseWhileTypeMatches(EXP* e) {
    if(e->inferredType == t_typeInteger || e->inferredType == t_typeBool) {
        return 1;
    }

    reportError("expr must be of type int or bool", e->lineno);
    return 0;
}

void typeImplementationPROGRAM(PROGRAM* p) {
    if(p->variable_decls != 0) {
        typeImplementationVAR_DECL(p->variable_decls);
    }

    if(p->statements != 0) {
        typeImplementationSTATEMENT(p->statements);
    }
}

void typeImplementationVAR_DECL(VAR_DECL* v) {
    if(v == 0) { return; }
    switch(v->kind) {
        case k_variableDeclKindDeclList:
            typeImplementationVAR_DECL(v->val.var_decl_list.var_decl);
            typeImplementationVAR_DECL(v->val.var_decl_list.next);
            break;
        case k_variableDeclKindDecl:
            // make sure the initial expression value matches the var type 
            getSymbol(mainSymbolTable, v->val.decl.identifier->val.identifer);
            
            break;
    }
}

void typeImplementationSTATEMENT(STATEMENT* s) {
    if(s == 0) { return; }
    switch(s->kind) {
        case k_statementKIndStatementList:
            typeImplementationSTATEMENT(s->val.stmt_list.stmt);
            typeImplementationSTATEMENT(s->val.stmt_list.next);
            break;
        case k_statementKindAssignment:
            typeImplementationEXP(s->val.assignment.value);
            SYMBOL* identifierSymbol = getSymbol(mainSymbolTable, s->val.assignment.identifier->val.identifer);
            assignmentTypeMatches(identifierSymbol->val.var, s->val.assignment.value);
            break;
        case k_statementKindIfElse:
            typeImplementationEXP(s->val.ifelseblock.expr);
            ifElseWhileTypeMatches(s->val.ifelseblock.expr);
            break;
        case k_statementKindWhile:
            typeImplementationEXP(s->val.whileblock.expr);
            ifElseWhileTypeMatches(s->val.whileblock.expr);
            break;
        
    }
}

void typeImplementationEXP(EXP* e) {
    if(e == 0) { return; }
    switch(e->kind) {
        case k_expressionKindIntLiteral:
            e->inferredType = t_typeInteger;
            break;
        case k_expressionKindFloatLiteral:
            e->inferredType = t_typeFloat;
            break;
        case k_expressionKindBooleanLiteral:
            e->inferredType = t_typeBool;
            break;
        case k_expressionKindStringLiteral:
            e->inferredType = t_typeString;
            break;
        case k_expressionKindIdentifier: // nothing to do
            break;
        case k_expressionKindAddition:
        case k_expressionKindSubtraction:
        case k_expressionKindMultiplication:
        case k_expressionKindDivision:
        case k_expressionKindEquals:
        case k_expressionKindNotEquals:
        case k_expressionKindLogicalAnd:
        case k_expressionKindLogicalOr:
            typeImplementationEXP(e->val.binary.lhs);
            typeImplementationEXP(e->val.binary.rhs);
            typeImplementationBinaryEXP(e);
            break;
        case k_expressionKindNegate:
            typeImplementationEXP(e->val.unary);
            if(checkBOOL(e->val.unary->inferredType, e)) {
                e->inferredType = t_typeBool;
            }
    }
}

// takes in a binary expression and compute its final type
void typeImplementationBinaryEXP(EXP* e) {
    switch(e->kind) {
        case k_expressionKindAddition:
            if(e->val.binary.lhs->inferredType == t_typeString && e->val.binary.rhs->inferredType == t_typeString) {
                e->inferredType = t_typeString;
                break; // concatenation string + string
            }
        case k_expressionKindSubtraction:
        case k_expressionKindMultiplication:
            if(e->val.binary.lhs->inferredType == t_typeString && e->val.binary.rhs->inferredType == t_typeInteger) {
                e->inferredType = t_typeString;
                break; // string * int
            }
        case k_expressionKindDivision:
            if(numericTypeImplicitCast(e)) {
                e->inferredType = t_typeFloat;    
            } else if(numericTypeMatches(e)) {
                e->inferredType = e->val.binary.lhs->inferredType;
            } else {
                reportError("invalid numeric type match", e->lineno);
            }
            break;

        case k_expressionKindEquals:
        case k_expressionKindNotEquals:
            if(checkTypeEquals(e->val.binary.lhs->inferredType, e->val.binary.rhs->inferredType, e->lineno)) {
                e->inferredType = t_typeBool;    
            }
            break;
        case k_expressionKindLogicalAnd:
        case k_expressionKindLogicalOr:
            if(logicalTypeMatches(e)) {
                e->inferredType = t_typeBool;
            } else {
                reportError("invalid logical type match", e->lineno);
            }
            break;
    }
}