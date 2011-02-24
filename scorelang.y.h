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
     COMMENT = 258,
     BLANKLINE = 259,
     START_SECTION_DIRECTION = 260,
     END_SECTION_DIRECTION = 261,
     START_HILEVEL_DIRECTION = 262,
     END_HILEVEL_DIRECTION = 263,
     START_DIRECTION = 264,
     END_DIRECTION = 265,
     START_INSERT = 266,
     END_INSERT = 267,
     SPACE = 268,
     SECTION = 269,
     SET1 = 270,
     SET2 = 271,
     SET3 = 272,
     SET4 = 273,
     LINE = 274,
     NAME = 275,
     VALUE = 276,
     JOINER = 277
   };
#endif
/* Tokens.  */
#define COMMENT 258
#define BLANKLINE 259
#define START_SECTION_DIRECTION 260
#define END_SECTION_DIRECTION 261
#define START_HILEVEL_DIRECTION 262
#define END_HILEVEL_DIRECTION 263
#define START_DIRECTION 264
#define END_DIRECTION 265
#define START_INSERT 266
#define END_INSERT 267
#define SPACE 268
#define SECTION 269
#define SET1 270
#define SET2 271
#define SET3 272
#define SET4 273
#define LINE 274
#define NAME 275
#define VALUE 276
#define JOINER 277




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 14 "scorelang.y"
{
	int ival;
	bracetype bval;
	char* sval;
	scline* lineval;
	scchunk* chunkval;
	scchunklist* chunklistval;
	schilevlist* hilevlistval;
	schilevdirective* hilevdirectiveval;
}
/* Line 1489 of yacc.c.  */
#line 104 "scorelang.y.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE scyylval;

