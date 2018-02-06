#ifndef TYPES_H
#define TYPES_H

enum AllowedTypes {
    t_typeInteger,
    t_typeFloat,
    t_typeBool,
    t_typeString
};

enum ExpressionFamily {
    ef_Expression,
    ef_VarDeclarations,
    ef_Statements,
    ef_Literal,
    ef_Root
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

    k_expressionKindVarDecl,
    k_expressionKindVarDeclarationList,

    k_expressionKindReadStatement,
    k_expressionKindWriteStatement,
    k_expressionKindAssignmentStatement,
    k_expressionKindIfElseStatement,
    k_expressionKindWhileStatement,

    k_expressionKindStatementList,

    k_expressionProgramBody
};

#endif