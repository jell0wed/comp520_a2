%{
#include <stdio.h>
void yyerror(const char *s) { 
	fprintf(stderr, "Error: %s\n", s);
	exit(1);
}

#include "types.h"
#include "tree.h"

EXP *root;
%}

%token-table
%union {
	int intval;
	float floatval;
	int boolval;
	char* stringval;
	char* identifierval;
	char* datatype;
	struct EXP *exp;
}
%token <intval> tINTEGER 					// raw integer value
%token <boolval> tBOOLEAN					// raw boolean value
%token <floatval> tFLOAT					// raw flow value
%token <stringval> tSTRING					// raw string value (unquoted)
%token <identifierval> tIDENTIFIER			// identifiers for variables
%token tTINTEGER tTFLOAT tTBOOLEAN tTSTRING // type declarations 
%token tVARDECL								// variable declaration
%token tPLUS tMINUS tTIMES tDIV tEQUALS tNOTEQUALS tAND tOR	// binary expressions
%token tNEGATE tNOT							// unary expression
%token tREAD tPRINT tASSIGN tIF tELSE tWHILE // statements
%token tLPAREN tRPAREN						// parenthesis
%token tBEGIN tEND							// statement block
%token tSEMICOLON							// end of line
%type <exp> expr litteral binary_expr unary_expr var_dec var_dec_list stmt stmt_list mini identifier

%left tOR
%left tAND
%left tEQUALS tNOTEQUALS
%left tPLUS tMINUS
%left tTIMES tDIV
%left tNEGATE tNOT

%%
program: mini { root = $1; }
mini:
	  var_dec_list stmt_list { $$ = makeEXP_programBody($1, $2); }
	| stmt_list { $$ = makeEXP_programBody(0, $1); }
;

var_dec_list:
	  var_dec_list var_dec { $$ = makeEXP_varDeclarationList($2, $1); }
	| var_dec { $$ = $1; }
;

var_dec:
	  tVARDECL identifier tTINTEGER tASSIGN expr tSEMICOLON { $$ = makeEXP_varDeclaration($2, t_typeInteger, $5); }
	| tVARDECL identifier tTFLOAT tASSIGN expr tSEMICOLON { $$ = makeEXP_varDeclaration($2, t_typeFloat, $5); }
	| tVARDECL identifier tTBOOLEAN tASSIGN expr tSEMICOLON { $$ = makeEXP_varDeclaration($2, t_typeBool, $5); }
	| tVARDECL identifier tTSTRING tASSIGN expr tSEMICOLON { $$ = makeEXP_varDeclaration($2, t_typeString, $5); }
;

stmt_list:
	  stmt_list stmt { $$ = makeEXP_statementList($2, $1); }
	| stmt { $$ = $1; }
;

stmt:
	  tREAD identifier tSEMICOLON { $$ = makeEXP_readStatement($2); }
	| tPRINT expr tSEMICOLON { $$ = makeEXP_writeStatement($2); }
	| identifier tASSIGN expr tSEMICOLON { $$ = makeEXP_assignmentStatement($1 , $3); }
	| identifier tASSIGN litteral tSEMICOLON { $$ = makeEXP_assignmentStatement($1 , $3); }
	| tIF expr tBEGIN stmt_list tEND { $$ = makeEXP_ifElseStatement($2, $4, 0); }
	| tIF expr tBEGIN stmt_list tEND tELSE tBEGIN stmt_list tEND { $$ = makeEXP_ifElseStatement($2, $4, $8); }
	| tWHILE expr tBEGIN stmt_list tEND { $$ = makeEXP_whileStatement($2, $4); }
;

expr:
	  binary_expr { $$ = $1; }
	| unary_expr { $$ = $1; }
	| identifier { $$ = $1; }
	| litteral { $$ = $1; }
	| '(' expr ')' { $$ = $2; }
;

identifier:
	| tIDENTIFIER { $$ = makeEXP_identifier($1); }
;

litteral:
	  tINTEGER  { $$ = makeEXP_intLiteral($1); } 
	| tFLOAT { $$ = makeEXP_floatLiteral($1); }
	| tBOOLEAN { $$ = makeEXP_boolLiteral($1); }
	| tSTRING { $$ = makeEXP_stringLiteral($1); }
;

binary_expr:
	  expr tPLUS expr { $$ = makeEXP_binary(k_expressionKindAddition, $1, $3); }
	| expr tMINUS expr { $$ = makeEXP_binary(k_expressionKindSubtraction, $1, $3); }
	| expr tTIMES expr { $$ = makeEXP_binary(k_expressionKindMultiplication, $1, $3); }
	| expr tDIV expr { $$ = makeEXP_binary(k_expressionKindDivision, $1, $3); }

	| expr tEQUALS expr { $$ = makeEXP_binary(k_expressionKindEquals, $1, $3); }
	| expr tNOTEQUALS expr { $$ = makeEXP_binary(k_expressionKindNotEquals, $1, $3); }
	
	| expr tAND expr { $$ = makeEXP_binary(k_expressionKindLogicalAnd, $1, $3); }
	| expr tOR expr { $$ = makeEXP_binary(k_expressionKindLogicalOr, $1, $3); }
;

unary_expr:
	  tNEGATE expr { $$ = makeEXP_negate($2); }
	| tMINUS tINTEGER { $$ = makeEXP_intLiteral($2 * -1); } 
	| tMINUS tFLOAT { $$ = makeEXP_intLiteral($2 * -1.0); }
;
%%

int g_token;
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf(stderr, "Wrong usage. Correct usage: ./mini {scan|tokens|parse} < input.min");
		return 1;
	}

	char* command = argv[1];
	if (strcmp("scan", command) == 0) {
		while(yylex() != 0) {

		}
		printf("OK");
		return 0;
	} else if (strcmp("tokens", command) == 0) {
		g_token = 1;
		int token_type;
		while((token_type = yylex()) != 0) {
			//printf("%s\n", yytname[YYTRANSLATE(token_type)]);
		}
		return 0;
	} else if (strcmp("parse", command) == 0) {
		if(yyparse() == 0) {
			printf("OK");
			return 0;
		}
	}
	return 1;
}
