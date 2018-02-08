%{
#include <stdio.h>
extern int yylineno;

#include "types.h"
#include "tree.h"
#include "pretty.h"
#include "symbol.h"
#include "type.h"


#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

PROGRAM *root;
%}
%locations
%error-verbose
%token-table
%union {
	int intval;
	float floatval;
	int boolval;
	char* stringval;
	char* identifierval;
	char* datatype;

	struct EXP *exp;
	struct VAR_DECL *var_decl;
	struct STATEMENT *statement;
	struct PROGRAM *program;
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

%type <program> program mini
%type <var_decl> var_dec_list var_dec
%type <statement> stmt_list stmt
%type <exp> expr litteral binary_expr unary_expr identifier

%left tOR
%left tAND
%left tEQUALS tNOTEQUALS
%left tPLUS tMINUS
%left tTIMES tDIV
%left tNEGATE tNOT

%%
program: mini { root = $1; }
mini:
	  var_dec_list stmt_list { $$ = makePROGRAM_programBody($1, $2, @1.first_line); }
	| stmt_list { $$ = makePROGRAM_programBody(0, $1, @1.first_line); }
;

var_dec_list:
	  var_dec_list var_dec { $$ = makeVARDECL_varDeclarationList($2, $1, @1.first_line); }
	| var_dec { $$ = $1; }
;

var_dec:
	  tVARDECL identifier tTINTEGER tASSIGN expr tSEMICOLON { $$ = makeVARDECL_varDeclaration($2, t_typeInteger, $5, @1.first_line); }
	| tVARDECL identifier tTFLOAT tASSIGN expr tSEMICOLON { $$ = makeVARDECL_varDeclaration($2, t_typeFloat, $5, @1.first_line); }
	| tVARDECL identifier tTBOOLEAN tASSIGN expr tSEMICOLON { $$ = makeVARDECL_varDeclaration($2, t_typeBool, $5, @1.first_line); }
	| tVARDECL identifier tTSTRING tASSIGN expr tSEMICOLON { $$ = makeVARDECL_varDeclaration($2, t_typeString, $5, @1.first_line); }
;

stmt_list:
	  stmt_list stmt { $$ = makeSTATMENT_statementList($2, $1, @1.first_line); }
	| stmt { $$ = $1; }
;

stmt:
	  tREAD identifier tSEMICOLON { $$ = makeSTATEMENT_readStatement($2, @1.first_line); }
	| tPRINT expr tSEMICOLON { $$ = makeSTATEMENT_printStatement($2, @1.first_line); }
	| identifier tASSIGN expr tSEMICOLON { $$ = makeSTATEMENT_assignmentStatement($1 , $3, @1.first_line); }
	| identifier tASSIGN litteral tSEMICOLON { $$ = makeSTATEMENT_assignmentStatement($1 , $3, @1.first_line); }
	| tIF expr tBEGIN stmt_list tEND { $$ = makeSTATEMENT_ifElseStatement($2, $4, 0, @1.first_line); }
	| tIF expr tBEGIN stmt_list tEND tELSE tBEGIN stmt_list tEND { $$ = makeSTATEMENT_ifElseStatement($2, $4, $8, @1.first_line); }
	| tWHILE expr tBEGIN stmt_list tEND { $$ = makeSTATEMENT_whileStatement($2, $4, @1.first_line); }
;

expr:
	  binary_expr { $$ = $1; }
	| unary_expr { $$ = $1; }
	| identifier { $$ = $1; }
	| litteral { $$ = $1; }
	| '(' expr ')' { $$ = $2; }
;

identifier:
	| tIDENTIFIER { $$ = makeEXP_identifier($1, @1.first_line); }
;

litteral:
	  tINTEGER  { $$ = makeEXP_intLiteral($1, @1.first_line); } 
	| tFLOAT { $$ = makeEXP_floatLiteral($1, @1.first_line); }
	| tBOOLEAN { $$ = makeEXP_boolLiteral($1, @1.first_line); }
	| tSTRING { $$ = makeEXP_stringLiteral($1, @1.first_line); }
;

binary_expr:
	  expr tPLUS expr { $$ = makeEXP_binary(k_expressionKindAddition, $1, $3, @1.first_line); }
	| expr tMINUS expr { $$ = makeEXP_binary(k_expressionKindSubtraction, $1, $3, @1.first_line); }
	| expr tTIMES expr { $$ = makeEXP_binary(k_expressionKindMultiplication, $1, $3, @1.first_line); }
	| expr tDIV expr { $$ = makeEXP_binary(k_expressionKindDivision, $1, $3, @1.first_line); }

	| expr tEQUALS expr { $$ = makeEXP_binary(k_expressionKindEquals, $1, $3, @1.first_line); }
	| expr tNOTEQUALS expr { $$ = makeEXP_binary(k_expressionKindNotEquals, $1, $3, @1.first_line); }
	
	| expr tAND expr { $$ = makeEXP_binary(k_expressionKindLogicalAnd, $1, $3, @1.first_line); }
	| expr tOR expr { $$ = makeEXP_binary(k_expressionKindLogicalOr, $1, $3, @1.first_line); }
;

unary_expr:
	  tNEGATE expr { $$ = makeEXP_negate($2, @1.first_line); }
	| tMINUS tINTEGER { $$ = makeEXP_intLiteral($2 * -1, @1.first_line); } 
	| tMINUS tFLOAT { $$ = makeEXP_intLiteral($2 * -1.0, @1.first_line); }
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
			//printf("OK");
			prettyPROGRAM(root);
			return 0;
		}
	} else if (strcmp("symbol", command) == 0) {
		if(yyparse() == 0) {
			symPROGRAM(root);
			noErrors();
			printSymbolTable(mainSymbolTable);
		}
		return 0;
	} else if (strcmp("typecheck", command) == 0) {
		if(yyparse() == 0) {
			symPROGRAM(root);
			noErrors();
			typeImplementationPROGRAM(root);
			noErrors();
		}
	} else if(strcmp("codegen", command) == 0) {
		if(yyparse() == 0) {
			symPROGRAM(root);
			noErrors();
			typeImplementationPROGRAM(root);
			noErrors();
			initializeCode(stdout);
			codePROGRAM(root);
		}
	}
	return 1;
}
