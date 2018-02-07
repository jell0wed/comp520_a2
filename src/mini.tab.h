/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tINTEGER = 258,
     tBOOLEAN = 259,
     tFLOAT = 260,
     tSTRING = 261,
     tIDENTIFIER = 262,
     tTINTEGER = 263,
     tTFLOAT = 264,
     tTBOOLEAN = 265,
     tTSTRING = 266,
     tVARDECL = 267,
     tPLUS = 268,
     tMINUS = 269,
     tTIMES = 270,
     tDIV = 271,
     tEQUALS = 272,
     tNOTEQUALS = 273,
     tAND = 274,
     tOR = 275,
     tNEGATE = 276,
     tNOT = 277,
     tREAD = 278,
     tPRINT = 279,
     tASSIGN = 280,
     tIF = 281,
     tELSE = 282,
     tWHILE = 283,
     tLPAREN = 284,
     tRPAREN = 285,
     tBEGIN = 286,
     tEND = 287,
     tSEMICOLON = 288
   };
#endif
/* Tokens.  */
#define tINTEGER 258
#define tBOOLEAN 259
#define tFLOAT 260
#define tSTRING 261
#define tIDENTIFIER 262
#define tTINTEGER 263
#define tTFLOAT 264
#define tTBOOLEAN 265
#define tTSTRING 266
#define tVARDECL 267
#define tPLUS 268
#define tMINUS 269
#define tTIMES 270
#define tDIV 271
#define tEQUALS 272
#define tNOTEQUALS 273
#define tAND 274
#define tOR 275
#define tNEGATE 276
#define tNOT 277
#define tREAD 278
#define tPRINT 279
#define tASSIGN 280
#define tIF 281
#define tELSE 282
#define tWHILE 283
#define tLPAREN 284
#define tRPAREN 285
#define tBEGIN 286
#define tEND 287
#define tSEMICOLON 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 17 "mini.y"
{
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
/* Line 1529 of yacc.c.  */
#line 129 "mini.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
