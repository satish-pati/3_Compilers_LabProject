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
    BREAK = 259,                   /* BREAK  */
    CONTINUE = 260,                /* CONTINUE  */
    FOR = 261,                     /* FOR  */
    DO = 262,                      /* DO  */
    IDEN = 263,                    /* IDEN  */
    NUM = 264,                     /* NUM  */
    PASN = 265,                    /* PASN  */
    MASN = 266,                    /* MASN  */
    DASN = 267,                    /* DASN  */
    SASN = 268,                    /* SASN  */
    INC = 269,                     /* INC  */
    DEC = 270,                     /* DEC  */
    LT = 271,                      /* LT  */
    GT = 272,                      /* GT  */
    LE = 273,                      /* LE  */
    GE = 274,                      /* GE  */
    NE = 275,                      /* NE  */
    OR = 276,                      /* OR  */
    AND = 277,                     /* AND  */
    EQ = 278,                      /* EQ  */
    IF = 279,                      /* IF  */
    ELSE = 280,                    /* ELSE  */
    TR = 281,                      /* TR  */
    FL = 282,                      /* FL  */
    WHILE = 283,                   /* WHILE  */
    INT = 284,                     /* INT  */
    FLOAT = 285,                   /* FLOAT  */
    CHAR = 286,                    /* CHAR  */
    CHARR = 287,                   /* CHARR  */
    MEOF = 288,                    /* MEOF  */
    CONST = 289,                   /* CONST  */
    SWITCH = 290,                  /* SWITCH  */
    CASE = 291,                    /* CASE  */
    DEFAULT = 292,                 /* DEFAULT  */
    GOTO = 293,                    /* GOTO  */
    STRING = 294,                  /* STRING  */
    BOOL = 295,                    /* BOOL  */
    PRINT = 296,                   /* PRINT  */
    INPUT = 297,                   /* INPUT  */
    SIZEOF = 298,                  /* SIZEOF  */
    BANDASN = 299,                 /* BANDASN  */
    BORASN = 300,                  /* BORASN  */
    BXORASN = 301,                 /* BXORASN  */
    LSHIFTASN = 302,               /* LSHIFTASN  */
    RSHIFTASN = 303,               /* RSHIFTASN  */
    SHORT = 304,                   /* SHORT  */
    LONG = 305,                    /* LONG  */
    DOUBLE = 306,                  /* DOUBLE  */
    VOID = 307,                    /* VOID  */
    BAND = 308,                    /* BAND  */
    BOR = 309,                     /* BOR  */
    BXOR = 310,                    /* BXOR  */
    BNOT = 311,                    /* BNOT  */
    LSHIFT = 312,                  /* LSHIFT  */
    RSHIFT = 313,                  /* RSHIFT  */
    FUNCTION = 314,                /* FUNCTION  */
    RETURN = 315,                  /* RETURN  */
    CALL = 316                     /* CALL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define UMINUS 258
#define BREAK 259
#define CONTINUE 260
#define FOR 261
#define DO 262
#define IDEN 263
#define NUM 264
#define PASN 265
#define MASN 266
#define DASN 267
#define SASN 268
#define INC 269
#define DEC 270
#define LT 271
#define GT 272
#define LE 273
#define GE 274
#define NE 275
#define OR 276
#define AND 277
#define EQ 278
#define IF 279
#define ELSE 280
#define TR 281
#define FL 282
#define WHILE 283
#define INT 284
#define FLOAT 285
#define CHAR 286
#define CHARR 287
#define MEOF 288
#define CONST 289
#define SWITCH 290
#define CASE 291
#define DEFAULT 292
#define GOTO 293
#define STRING 294
#define BOOL 295
#define PRINT 296
#define INPUT 297
#define SIZEOF 298
#define BANDASN 299
#define BORASN 300
#define BXORASN 301
#define LSHIFTASN 302
#define RSHIFTASN 303
#define SHORT 304
#define LONG 305
#define DOUBLE 306
#define VOID 307
#define BAND 308
#define BOR 309
#define BXOR 310
#define BNOT 311
#define LSHIFT 312
#define RSHIFT 313
#define FUNCTION 314
#define RETURN 315
#define CALL 316

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 986 "parser.y"

        char str[1000];
        struct BoolNode* b;
        struct Expr *expr;
        int addr;
        struct Type* type;
        struct Decl* decl;
        struct Subscript* sub;

#line 199 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
