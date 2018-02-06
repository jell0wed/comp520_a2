%{
#include <stdio.h>
#include "mini.tab.h"
int g_token = 0;
%}
ALLOWED_CHARS [ a-zA-Z0-9~@#\$%\^&\*\-\+/`<>=_|\'(\\")\.\?,;:!^\{\}\[\]\(\)\a\b\f\n\r\t\v)]
ALLOWED_IDENTIFIER [a-zA-Z_][a-zA-Z_0-9]*
ALLOWED_DATATYPES (float|boolean|int|string)
%option noyywrap
%%
[ \t\n\r]+				    ;
\/\/([^\n]+)                ;
:                           ;
;                           { if (g_token) { printf("tSEMICOLON\n"); }  return tSEMICOLON; }

\{                          { if (g_token) { printf("tBEGIN\n"); }  return tBEGIN; }
\}                          { if (g_token) { printf("tEND\n"); } return tEND; }

\+                          { if (g_token) { printf("tPLUS\n"); } return tPLUS; }
\-                          { if (g_token) { printf("tMINUS\n"); }return tMINUS; }
\*                          { if (g_token) { printf("tTIMES\n"); }return tTIMES; }
\/                          { if (g_token) { printf("tDIV\n"); }return tDIV; }
==                          { if (g_token) { printf("tEQUALS\n"); }return tEQUALS; }
!=                          { if (g_token) { printf("tNOTEQUALS\n"); }return tNOTEQUALS; }
&&                          { if (g_token) { printf("tAND\n"); }return tAND; }
\|\|                        { if (g_token) { printf("tOR\n"); } tOR; } 
!                           { if (g_token) { printf("tNOT\n"); }return tNOT; }

\(                          ;
\)                          ;

read                        { if (g_token) { printf("tREAD\n"); } return tREAD; }
print                       { if (g_token) { printf("tPRINT\n"); } return tPRINT; }
=                           { if (g_token) { printf("tASSIGN\n"); } return tASSIGN; }
if                          { if (g_token) { printf("tIF\n"); } return tIF; }
else                        { if (g_token) { printf("tELSE\n"); } return tELSE; }
while                       { if (g_token) { printf("tWHILE\n"); } return tWHILE; }

var                         { if (g_token) { printf("tVARDECL\n"); } return tVARDECL; }
int                         { if (g_token) { printf("tTINTEGER\n"); } return tTINTEGER; }
float                       { if (g_token) { printf("tTFLOAT\n"); } return tTFLOAT; }
bool                        { if (g_token) { printf("tTBOOLEAN\n"); } return tTBOOLEAN; }
string                      { if (g_token) { printf("tTSTRING\n"); } return tTSTRING; }

(TRUE|FALSE)				{ yylval.boolval = strcmp("TRUE", yytext) == 0 ? 1 : 0;
                              if (g_token) { printf("tBOOLEAN(%d)\n", yylval.boolval); }
                              return tBOOLEAN; }
(0|[1-9][0-9]*)				{ yylval.intval = atoi(yytext);
                              if (g_token) { printf("tINTEGER(%d)\n", yylval.intval); }
                              return tINTEGER; }
(0|[1-9]+).([0-9]+)			{ yylval.floatval = atof(yytext);
                              if (g_token) { printf("tFLOAT(%.6f)\n", yylval.floatval); }
                              return tFLOAT; }

{ALLOWED_IDENTIFIER}        { yylval.identifierval = strdup(yytext);
                              if (g_token) { printf("tIDENTIFIER(%s)\n", yylval.identifierval); }
                              return tIDENTIFIER; }
\"{ALLOWED_CHARS}*\"	 	{   int n = strlen(yytext);
                                char* cpy = malloc((n - 2 + 1) * sizeof(char));
                                strncpy(cpy, yytext + sizeof(char), n - 2); 
                                yylval.stringval = cpy;
                                if (g_token) { printf("tSTRING(%s)\n", yylval.stringval); }
                                return tSTRING;
                            } 

.							{ fprintf(stderr, "Error: unexpected char '%s'", yytext); exit(1); }
%%
