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
    IDEN = 258,                    /* IDEN  */
    NUM = 259,                     /* NUM  */
    INT = 260,                     /* INT  */
    FLOAT = 261,                   /* FLOAT  */
    CHAR = 262,                    /* CHAR  */
    DOUBLE = 263,                  /* DOUBLE  */
    SHORT = 264,                   /* SHORT  */
    LONG = 265,                    /* LONG  */
    VOID = 266,                    /* VOID  */
    BOOL = 267,                    /* BOOL  */
    CONST = 268,                   /* CONST  */
    CHARR = 269,                   /* CHARR  */
    STRING = 270,                  /* STRING  */
    PASN = 271,                    /* PASN  */
    MASN = 272,                    /* MASN  */
    DASN = 273,                    /* DASN  */
    SASN = 274,                    /* SASN  */
    INC = 275,                     /* INC  */
    DEC = 276,                     /* DEC  */
    BAND = 277,                    /* BAND  */
    BOR = 278,                     /* BOR  */
    BXOR = 279,                    /* BXOR  */
    BNOT = 280,                    /* BNOT  */
    LSHIFT = 281,                  /* LSHIFT  */
    RSHIFT = 282,                  /* RSHIFT  */
    BANDASN = 283,                 /* BANDASN  */
    BORASN = 284,                  /* BORASN  */
    BXORASN = 285,                 /* BXORASN  */
    LSHIFTASN = 286,               /* LSHIFTASN  */
    RSHIFTASN = 287,               /* RSHIFTASN  */
    DEFAULT = 288,                 /* DEFAULT  */
    BREAK = 289,                   /* BREAK  */
    CONTINUE = 290,                /* CONTINUE  */
    AND = 291,                     /* AND  */
    OR = 292,                      /* OR  */
    EQ = 293,                      /* EQ  */
    NE = 294,                      /* NE  */
    LT = 295,                      /* LT  */
    GT = 296,                      /* GT  */
    LE = 297,                      /* LE  */
    GE = 298,                      /* GE  */
    TR = 299,                      /* TR  */
    FL = 300,                      /* FL  */
    IF = 301,                      /* IF  */
    ELSE = 302,                    /* ELSE  */
    FUNCTION = 303,                /* FUNCTION  */
    RETURN = 304,                  /* RETURN  */
    CALL = 305,                    /* CALL  */
    PRINT = 306,                   /* PRINT  */
    INPUT = 307,                   /* INPUT  */
    SIZEOF = 308,                  /* SIZEOF  */
    WHILE = 309,                   /* WHILE  */
    DO = 310,                      /* DO  */
    FOR = 311,                     /* FOR  */
    SWITCH = 312,                  /* SWITCH  */
    CASE = 313,                    /* CASE  */
    MEOF = 314,                    /* MEOF  */
    UMINUS = 315                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IDEN 258
#define NUM 259
#define INT 260
#define FLOAT 261
#define CHAR 262
#define DOUBLE 263
#define SHORT 264
#define LONG 265
#define VOID 266
#define BOOL 267
#define CONST 268
#define CHARR 269
#define STRING 270
#define PASN 271
#define MASN 272
#define DASN 273
#define SASN 274
#define INC 275
#define DEC 276
#define BAND 277
#define BOR 278
#define BXOR 279
#define BNOT 280
#define LSHIFT 281
#define RSHIFT 282
#define BANDASN 283
#define BORASN 284
#define BXORASN 285
#define LSHIFTASN 286
#define RSHIFTASN 287
#define DEFAULT 288
#define BREAK 289
#define CONTINUE 290
#define AND 291
#define OR 292
#define EQ 293
#define NE 294
#define LT 295
#define GT 296
#define LE 297
#define GE 298
#define TR 299
#define FL 300
#define IF 301
#define ELSE 302
#define FUNCTION 303
#define RETURN 304
#define CALL 305
#define PRINT 306
#define INPUT 307
#define SIZEOF 308
#define WHILE 309
#define DO 310
#define FOR 311
#define SWITCH 312
#define CASE 313
#define MEOF 314
#define UMINUS 315

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 443 "parser.y"

    char str[1000];
    struct Expr* expr;
    struct Type* type;
    struct Decl* decl;

#line 194 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
