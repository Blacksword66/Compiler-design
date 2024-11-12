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
     NUMBER = 258,
     IDENTIFIER = 259,
     INT = 260,
     IF = 261,
     ELSE = 262,
     WHILE = 263,
     ASSIGN = 264,
     EQ = 265,
     NE = 266,
     LT = 267,
     GT = 268,
     LE = 269,
     GE = 270,
     PLUS = 271,
     MINUS = 272,
     MULTIPLY = 273,
     DIVIDE = 274,
     LPAREN = 275,
     RPAREN = 276,
     LBRACE = 277,
     RBRACE = 278,
     SEMICOLON = 279,
     UMINUS = 280,
     IFX = 281
   };
#endif
/* Tokens.  */
#define NUMBER 258
#define IDENTIFIER 259
#define INT 260
#define IF 261
#define ELSE 262
#define WHILE 263
#define ASSIGN 264
#define EQ 265
#define NE 266
#define LT 267
#define GT 268
#define LE 269
#define GE 270
#define PLUS 271
#define MINUS 272
#define MULTIPLY 273
#define DIVIDE 274
#define LPAREN 275
#define RPAREN 276
#define LBRACE 277
#define RBRACE 278
#define SEMICOLON 279
#define UMINUS 280
#define IFX 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 37 "three_address.y"
{
    int number;
    char* string;
    Attrs attrs;
}
/* Line 1529 of yacc.c.  */
#line 107 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

