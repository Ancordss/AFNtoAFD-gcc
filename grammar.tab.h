/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    T_EOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_AUTOMATA_AFN = 258,          /* T_AUTOMATA_AFN  */
    T_ALFABETO = 259,              /* T_ALFABETO  */
    T_ESTADO = 260,                /* T_ESTADO  */
    T_INICIAL = 261,               /* T_INICIAL  */
    T_FINAL = 262,                 /* T_FINAL  */
    T_TRANSICIONES = 263,          /* T_TRANSICIONES  */
    T_END_TAG = 264,               /* T_END_TAG  */
    T_INT = 265,                   /* T_INT  */
    T_END_ALFABETO = 266,          /* T_END_ALFABETO  */
    T_END_AUTOMATA_AFN = 267,      /* T_END_AUTOMATA_AFN  */
    T_END_ESTADO = 268,            /* T_END_ESTADO  */
    T_END_INICIAL = 269,           /* T_END_INICIAL  */
    T_END_FINAL = 270,             /* T_END_FINAL  */
    T_END_TRANSICIONES = 271,      /* T_END_TRANSICIONES  */
    T_LETRA_A = 272,               /* T_LETRA_A  */
    T_LETRA_B = 273,               /* T_LETRA_B  */
    T_SIMBOLO = 274,               /* T_SIMBOLO  */
    T_COMMENT_OPEN = 275,          /* T_COMMENT_OPEN  */
    T_COMMENT_CLOSE = 276,         /* T_COMMENT_CLOSE  */
    T_STRING = 277,                /* T_STRING  */
    T_STRING_SINGLE_QUOTE = 278,   /* T_STRING_SINGLE_QUOTE  */
    T_POSITIVE_INTEGER = 279,      /* T_POSITIVE_INTEGER  */
    T_SLASH_END_TAG = 280,         /* T_SLASH_END_TAG  */
    T_STRING_DQ_SPACE = 281,       /* T_STRING_DQ_SPACE  */
    T_DOT = 282,                   /* T_DOT  */
    T_COMMA = 284,                 /* T_COMMA  */
    T_ASSIGN = 286,                /* T_ASSIGN  */
    T_COLON = 288,                 /* ":"  */
    T_LBRACK = 289,                /* T_LBRACK  */
    T_RBRACK = 291,                /* T_RBRACK  */
    T_SLASH = 293,                 /* "/"  */
    T_EXCLAMATION = 294,           /* "!"  */
    T_DASH = 295,                  /* "-"  */
    T_LBRACES = 296,               /* "{"  */
    T_RBRACES = 297,               /* "}"  */
    T_AT = 298,                    /* T_AT  */
    T_QUESTION_MARK = 300,         /* "?"  */
    T_UNDERSCORE = 301,            /* "_"  */
    T_HASH = 302,                  /* "#"  */
    T_SQUOTES = 303                /* "'"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 76 "grammar.y"

   int intval;
   float floatval;
   char charval;
   char *strval;

#line 113 "grammar.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
