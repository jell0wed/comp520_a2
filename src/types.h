#ifndef TYPES_H
#define TYPES_H

enum SymbolKind {
    st_symbolVariable
};

enum AllowedTypes {
    t_typeUnknown,
    t_typeInteger,
    t_typeFloat,
    t_typeBool,
    t_typeString
    
};

enum ExpressionKind {
    k_expressionKindIntLiteral,
    k_expressionKindFloatLiteral,
    k_expressionKindStringLiteral,
    k_expressionKindBooleanLiteral,
    k_expressionKindIdentifier,
    k_expressionKindAddition,
    k_expressionKindSubtraction,
    k_expressionKindMultiplication,
    k_expressionKindDivision,
    k_expressionKindEquals,
    k_expressionKindNotEquals,
    k_expressionKindLogicalAnd,
    k_expressionKindLogicalOr,
    k_expressionKindNegate,
    k_expressionKindNumNegate
};

enum VariableDeclKind {
    k_variableDeclKindDecl,
    k_variableDeclKindDeclList
};

enum StatementKind {
    k_statementKindRead,
    k_statementKindPrint,
    k_statementKindAssignment,
    k_statementKindIfElse,
    k_statementKindWhile,
    k_statementKIndStatementList
};

#endif