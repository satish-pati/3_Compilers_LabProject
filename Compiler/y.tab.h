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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    UMINUS = 258,                  /* UMINUS  */
    ABS = 259,                     /* ABS  */
    MIN = 260,                     /* MIN  */
    MAX = 261,                     /* MAX  */
    MODASN = 262,                  /* MODASN  */
    BREAK = 263,                   /* BREAK  */
    CONTINUE = 264,                /* CONTINUE  */
    FOR = 265,                     /* FOR  */
    DO = 266,                      /* DO  */
    IDEN = 267,                    /* IDEN  */
    NUM = 268,                     /* NUM  */
    PASN = 269,                    /* PASN  */
    MASN = 270,                    /* MASN  */
    DASN = 271,                    /* DASN  */
    SASN = 272,                    /* SASN  */
    INC = 273,                     /* INC  */
    DEC = 274,                     /* DEC  */
    LT = 275,                      /* LT  */
    GT = 276,                      /* GT  */
    LE = 277,                      /* LE  */
    GE = 278,                      /* GE  */
    NE = 279,                      /* NE  */
    OR = 280,                      /* OR  */
    AND = 281,                     /* AND  */
    EQ = 282,                      /* EQ  */
    IF = 283,                      /* IF  */
    ELSE = 284,                    /* ELSE  */
    TR = 285,                      /* TR  */
    FL = 286,                      /* FL  */
    WHILE = 287,                   /* WHILE  */
    INT = 288,                     /* INT  */
    FLOAT = 289,                   /* FLOAT  */
    CHAR = 290,                    /* CHAR  */
    CHARR = 291,                   /* CHARR  */
    MEOF = 292,                    /* MEOF  */
    CONST = 293,                   /* CONST  */
    SWITCH = 294,                  /* SWITCH  */
    CASE = 295,                    /* CASE  */
    DEFAULT = 296,                 /* DEFAULT  */
    GOTO = 297,                    /* GOTO  */
    STRING = 298,                  /* STRING  */
    BOOL = 299,                    /* BOOL  */
    PRINT = 300,                   /* PRINT  */
    INPUT = 301,                   /* INPUT  */
    SIZEOF = 302,                  /* SIZEOF  */
    BANDASN = 303,                 /* BANDASN  */
    BORASN = 304,                  /* BORASN  */
    BXORASN = 305,                 /* BXORASN  */
    LSHIFTASN = 306,               /* LSHIFTASN  */
    RSHIFTASN = 307,               /* RSHIFTASN  */
    SHORT = 308,                   /* SHORT  */
    LONG = 309,                    /* LONG  */
    DOUBLE = 310,                  /* DOUBLE  */
    VOID = 311,                    /* VOID  */
    BAND = 312,                    /* BAND  */
    BOR = 313,                     /* BOR  */
    BXOR = 314,                    /* BXOR  */
    BNOT = 315,                    /* BNOT  */
    LSHIFT = 316,                  /* LSHIFT  */
    RSHIFT = 317,                  /* RSHIFT  */
    FUNCTION = 318,                /* FUNCTION  */
    RETURN = 319,                  /* RETURN  */
    CALL = 320                     /* CALL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define UMINUS 258
#define ABS 259
#define MIN 260
#define MAX 261
#define MODASN 262
#define BREAK 263
#define CONTINUE 264
#define FOR 265
#define DO 266
#define IDEN 267
#define NUM 268
#define PASN 269
#define MASN 270
#define DASN 271
#define SASN 272
#define INC 273
#define DEC 274
#define LT 275
#define GT 276
#define LE 277
#define GE 278
#define NE 279
#define OR 280
#define AND 281
#define EQ 282
#define IF 283
#define ELSE 284
#define TR 285
#define FL 286
#define WHILE 287
#define INT 288
#define FLOAT 289
#define CHAR 290
#define CHARR 291
#define MEOF 292
#define CONST 293
#define SWITCH 294
#define CASE 295
#define DEFAULT 296
#define GOTO 297
#define STRING 298
#define BOOL 299
#define PRINT 300
#define INPUT 301
#define SIZEOF 302
#define BANDASN 303
#define BORASN 304
#define BXORASN 305
#define LSHIFTASN 306
#define RSHIFTASN 307
#define SHORT 308
#define LONG 309
#define DOUBLE 310
#define VOID 311
#define BAND 312
#define BOR 313
#define BXOR 314
#define BNOT 315
#define LSHIFT 316
#define RSHIFT 317
#define FUNCTION 318
#define RETURN 319
#define CALL 320

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 2298 "parser_unoptimized.y"

        char str[1000];
        struct BoolNode* b;
        struct Expr *expr;
        int addr;
        struct Type* type;
        struct Decl* decl;
        struct Subscript* sub;

#line 207 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
