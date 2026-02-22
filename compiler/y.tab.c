/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
extern FILE *yyin;
extern char buffer[];
char err[1000];
int e = 0;
int offset = 0;
int saveoffset;

void yyerror(char* msg);
int yylex(void);

// Loop depth tracking for break/continue validation
int loop_depth = 0;
int switch_depth = 0;  // Separate tracker for switch statements

// Current function context
char current_function[100] = "";
char current_return_type[100] = "";
int in_function = 0;
int has_return_statement = 0;

// Symbol table structures
typedef struct Symbol{
    char name[100];
    char type[100];
    int size;
    int offset;
    int dimensions[10];
    int dim_count;
    struct Symbol* next;
} Symbol;

struct Type{
    char str[1000];
    int size;
};

struct Decl{
    char key[1000];
    char type[1000];
    char lt[100];
    char op[100];
    int size;
    int is_literal;
    int re;
    struct Decl* next;
};

struct Expr{
    char str[1000];
    char type[100];
    int lv;
    int str_len;
    struct Expr* next;
};

typedef struct Param{
    char name[100];
    char type[100];
    struct Param* next;
} Param;

typedef struct Function{
    char name[100];
    char return_type[100];
    Param* params;
    int param_count;
    struct Function* next;
} Function;

Function* func_list = NULL;

Symbol* createSymbol(char* name){
    Symbol* node = (Symbol*)malloc(sizeof(Symbol));
    memset(node, 0, sizeof(Symbol));
    strcpy(node->name, name);
    node->dim_count = 0;
    return node;
}

struct Decl* createDecl(char* key){
    struct Decl *node = (struct Decl*)malloc(sizeof(struct Decl));
    node->re = 0;
    node->is_literal = 0;
    strcpy(node->key, key);
    strcpy(node->type, "");
    strcpy(node->lt, "");
    strcpy(node->op, "");
    node->next = NULL;
    return node;
}

struct Expr* createExpr(){
    struct Expr* e = (struct Expr*)malloc(sizeof(struct Expr));
    memset(e, 0, sizeof(struct Expr));
    e->next = NULL;
    e->str_len = 0;
    e->lv = 0;
    return e;
}

struct Type* createType(){
    return (struct Type*)malloc(sizeof(struct Type));
}

Param* createParam(char* name, char* type){
    Param* p = (Param*)malloc(sizeof(Param));
    strcpy(p->name, name);
    strcpy(p->type, type);
    p->next = NULL;
    return p;
}

Function* createFunction(char* name, char* ret_type){
    Function* f = (Function*)malloc(sizeof(Function));
    strcpy(f->name, name);
    strcpy(f->return_type, ret_type);
    f->params = NULL;
    f->param_count = 0;
    f->next = NULL;
    return f;
}

void addFunction(Function* f){
    f->next = func_list;
    func_list = f;
}

Function* findFunction(char* name){
    Function* temp = func_list;
    while(temp){
        if(strcmp(temp->name, name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Environment and hash table for symbol table
typedef struct Env {
    struct Env* prev;
    int prev_offset;
    struct Table* table;
} Env;

Env* envs[1000];
int env_count = 0;

typedef struct TableEntry {
    char* key;
    Symbol* value;
    struct TableEntry* next;
} TableEntry;

typedef struct Table {
    TableEntry** buckets;
    int size;
} Table;

#define TABLE_SIZE 501

unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % TABLE_SIZE;
}

Table* create_table() {
    Table* table = malloc(sizeof(Table));
    table->buckets = calloc(TABLE_SIZE, sizeof(TableEntry*));
    table->size = TABLE_SIZE;
    return table;
}

void put(Table* table, const char* key, Symbol* sym) {
    unsigned int index = hash(key);
    TableEntry* new_entry = malloc(sizeof(TableEntry));
    new_entry->key = strdup(key);
    new_entry->value = sym;
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
}

Symbol* get(Table* table, const char* key) {
    unsigned int index = hash(key);
    TableEntry* entry = table->buckets[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

Env* create_env(Env* prev, int offset_val) {
    Env* env = malloc(sizeof(Env));
    env->prev = prev;
    env->table = create_table();
    env->prev_offset = offset_val;
    envs[env_count++] = env;
    return env;
}

void env_put(Env* env, const char* key, Symbol* sym) {
    put(env->table, key, sym);
}

Symbol* env_get(Env* env, const char* key) {
    for (Env* e = env; e != NULL; e = e->prev) {
        Symbol* found = get(e->table, key);
        if (found != NULL) {
            return found;
        }
    }
    return NULL;
}

Env* top = NULL;

void print_table(Table* table) {
    for (int i = 0; i < table->size; i++) {
        TableEntry* entry = table->buckets[i];
        while (entry) {
            printf("%d    %s                  %s\n", entry->value->offset, entry->value->name, entry->value->type);
            entry = entry->next;
        }
    }
}

void print_all_envs() {
    printf("\n=== Symbol Table (Storage Layout) ===\n");
    for (int i = 0; i < env_count; i++) {
        printf("\nScope %d:\n", i);
        printf("Offset    Name                  Type\n");
        printf("-----------------------------------------------\n");
        print_table(envs[i]->table);
    }
    printf("======================================\n");
}

int checkfloat(char* t){
    while(*t){
        if (*t == '.') return 1;
        t++;
    }
    return 0;
}

int isNumericConstant(char* str) {
    if (!str || *str == '\0') return 0;
    char* ptr = str;
    if (*ptr == '-' || *ptr == '+') ptr++;
    int has_digit = 0;
    int has_dot = 0;
    while (*ptr) {
        if (isdigit(*ptr)) has_digit = 1;
        else if (*ptr == '.' && !has_dot) has_dot = 1;
        else return 0;
        ptr++;
    }
    return has_digit;
}

int isLiteral(char* str) {
    if (!str || *str == '\0') return 0;
    return (isdigit(str[0]) || (str[0] == '-' && isdigit(str[1])) || 
            (str[0] == '.' && isdigit(str[1])));
}

char* getBaseType(char* type_str) {
    static char base[100];
    
    char* bracket = strchr(type_str, '[');
    if (bracket) {
        int len = bracket - type_str;
        strncpy(base, type_str, len);
        base[len] = '\0';
        return base;
    }
    
    return type_str;
}

int getTypeRank(char* type){
    if (strcmp(type, "char") == 0) return 1;
    if (strcmp(type, "short") == 0) return 2;
    if (strcmp(type, "int") == 0) return 3;
    if (strcmp(type, "long") == 0) return 4;
    if (strcmp(type, "float") == 0) return 5;
    if (strcmp(type, "double") == 0) return 6;
    return 0;
}

char* promoteType(char* t1, char* t2){
    if (strcmp(t1, t2) == 0) return t1;
    
    int rank1 = getTypeRank(t1);
    int rank2 = getTypeRank(t2);
    
    return (rank1 > rank2) ? t1 : t2;
}

int isFloatingType(char* type){
    return (strcmp(type, "float") == 0 || strcmp(type, "double") == 0);
}

int isIntegerType(char* type){
    return (strcmp(type, "char") == 0 || 
            strcmp(type, "short") == 0 || 
            strcmp(type, "int") == 0 || 
            strcmp(type, "long") == 0);
}

void checkType(struct Expr* op1, struct Expr* op2, char* type){
    char* base_op1 = getBaseType(op1->type);
    char* base_op2 = getBaseType(op2->type);
    
    if (strcmp(base_op1, base_op2) == 0){
        strcpy(type, base_op1);
        return;
    }
    
    char* promoted = promoteType(base_op1, base_op2);
    strcpy(type, promoted);
}

void checkTypeAssign(struct Expr* op1, struct Expr* op2){
    if (strcmp(op1->type, op2->type) == 0){
        return;
    }
    
    char base_op1_type[100], base_op2_type[100];
    strcpy(base_op1_type, op1->type);
    strcpy(base_op2_type, op2->type);
    
    char* actual_op1 = base_op1_type;
    char* actual_op2 = base_op2_type;
    
    if (strncmp(base_op1_type, "const ", 6) == 0) {
        actual_op1 = base_op1_type + 6;
    }
    if (strncmp(base_op2_type, "const ", 6) == 0) {
        actual_op2 = base_op2_type + 6;
    }
    
    char* op1_base = getBaseType(actual_op1);
    char* op2_base = getBaseType(actual_op2);
    
    if (strcmp(op1_base, op2_base) != 0) {
        printf("Warning: Type conversion from %s to %s\n", op2_base, op1_base);
    }
}

// Check if parameter names are duplicated
int checkDuplicateParams(struct Decl* params) {
    struct Decl* p1 = params;
    while(p1) {
        struct Decl* p2 = p1->next;
        while(p2) {
            if(strcmp(p1->key, p2->key) == 0) {
                return 1; // Duplicate found
            }
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    return 0;
}

// Check if types are compatible for return
int isTypeCompatible(char* expected, char* actual) {
    // Remove const qualifier
    char exp_clean[100], act_clean[100];
    strcpy(exp_clean, expected);
    strcpy(act_clean, actual);
    
    if(strncmp(exp_clean, "const ", 6) == 0) {
        strcpy(exp_clean, exp_clean + 6);
    }
    if(strncmp(act_clean, "const ", 6) == 0) {
        strcpy(act_clean, act_clean + 6);
    }
    
    // Get base types (without array dimensions)
      char* exp_base = getBaseType(exp_clean);
    char* act_base = getBaseType(act_clean);
    
    // float/double compatible with each other
    if((strcmp(exp_base, "float") == 0 || strcmp(exp_base, "double") == 0) &&
       (strcmp(act_base, "float") == 0 || strcmp(act_base, "double") == 0)) {
        return 1;
    }
    
    // Integer widening: smaller type can go into larger type
    // e.g., int literal into long variable = OK
    int exp_rank = getTypeRank(exp_base);
    int act_rank = getTypeRank(act_base);
    
    if(exp_rank > 0 && act_rank > 0) {
        // Both are integer types
        if(act_rank <= exp_rank) {
            // assigning smaller/equal to larger = OK (widening)
            return 1;
        } else {
            // assigning larger to smaller = warning but allow
            return 1;  // change to 0 if you want strict narrowing errors
        }
    }
    
    return strcmp(exp_base, act_base) == 0;
}


#line 491 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 421 "parser.y"

    char str[1000];
    struct Expr* expr;
    struct Type* type;
    struct Decl* decl;

#line 671 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDEN = 3,                       /* IDEN  */
  YYSYMBOL_NUM = 4,                        /* NUM  */
  YYSYMBOL_INT = 5,                        /* INT  */
  YYSYMBOL_FLOAT = 6,                      /* FLOAT  */
  YYSYMBOL_CHAR = 7,                       /* CHAR  */
  YYSYMBOL_DOUBLE = 8,                     /* DOUBLE  */
  YYSYMBOL_SHORT = 9,                      /* SHORT  */
  YYSYMBOL_LONG = 10,                      /* LONG  */
  YYSYMBOL_VOID = 11,                      /* VOID  */
  YYSYMBOL_BOOL = 12,                      /* BOOL  */
  YYSYMBOL_CONST = 13,                     /* CONST  */
  YYSYMBOL_CHARR = 14,                     /* CHARR  */
  YYSYMBOL_STRING = 15,                    /* STRING  */
  YYSYMBOL_PASN = 16,                      /* PASN  */
  YYSYMBOL_MASN = 17,                      /* MASN  */
  YYSYMBOL_DASN = 18,                      /* DASN  */
  YYSYMBOL_SASN = 19,                      /* SASN  */
  YYSYMBOL_INC = 20,                       /* INC  */
  YYSYMBOL_DEC = 21,                       /* DEC  */
  YYSYMBOL_BAND = 22,                      /* BAND  */
  YYSYMBOL_BOR = 23,                       /* BOR  */
  YYSYMBOL_BXOR = 24,                      /* BXOR  */
  YYSYMBOL_BNOT = 25,                      /* BNOT  */
  YYSYMBOL_LSHIFT = 26,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 27,                    /* RSHIFT  */
  YYSYMBOL_BANDASN = 28,                   /* BANDASN  */
  YYSYMBOL_BORASN = 29,                    /* BORASN  */
  YYSYMBOL_BXORASN = 30,                   /* BXORASN  */
  YYSYMBOL_LSHIFTASN = 31,                 /* LSHIFTASN  */
  YYSYMBOL_RSHIFTASN = 32,                 /* RSHIFTASN  */
  YYSYMBOL_DEFAULT = 33,                   /* DEFAULT  */
  YYSYMBOL_BREAK = 34,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 35,                  /* CONTINUE  */
  YYSYMBOL_AND = 36,                       /* AND  */
  YYSYMBOL_OR = 37,                        /* OR  */
  YYSYMBOL_EQ = 38,                        /* EQ  */
  YYSYMBOL_NE = 39,                        /* NE  */
  YYSYMBOL_LT = 40,                        /* LT  */
  YYSYMBOL_GT = 41,                        /* GT  */
  YYSYMBOL_LE = 42,                        /* LE  */
  YYSYMBOL_GE = 43,                        /* GE  */
  YYSYMBOL_TR = 44,                        /* TR  */
  YYSYMBOL_FL = 45,                        /* FL  */
  YYSYMBOL_IF = 46,                        /* IF  */
  YYSYMBOL_ELSE = 47,                      /* ELSE  */
  YYSYMBOL_FUNCTION = 48,                  /* FUNCTION  */
  YYSYMBOL_RETURN = 49,                    /* RETURN  */
  YYSYMBOL_CALL = 50,                      /* CALL  */
  YYSYMBOL_PRINT = 51,                     /* PRINT  */
  YYSYMBOL_INPUT = 52,                     /* INPUT  */
  YYSYMBOL_SIZEOF = 53,                    /* SIZEOF  */
  YYSYMBOL_WHILE = 54,                     /* WHILE  */
  YYSYMBOL_DO = 55,                        /* DO  */
  YYSYMBOL_FOR = 56,                       /* FOR  */
  YYSYMBOL_SWITCH = 57,                    /* SWITCH  */
  YYSYMBOL_CASE = 58,                      /* CASE  */
  YYSYMBOL_MEOF = 59,                      /* MEOF  */
  YYSYMBOL_60_ = 60,                       /* '?'  */
  YYSYMBOL_61_ = 61,                       /* ':'  */
  YYSYMBOL_62_ = 62,                       /* '!'  */
  YYSYMBOL_63_ = 63,                       /* '+'  */
  YYSYMBOL_64_ = 64,                       /* '-'  */
  YYSYMBOL_65_ = 65,                       /* '*'  */
  YYSYMBOL_66_ = 66,                       /* '/'  */
  YYSYMBOL_67_ = 67,                       /* '%'  */
  YYSYMBOL_68_ = 68,                       /* '('  */
  YYSYMBOL_69_ = 69,                       /* ')'  */
  YYSYMBOL_UMINUS = 70,                    /* UMINUS  */
  YYSYMBOL_71_ = 71,                       /* '$'  */
  YYSYMBOL_72_ = 72,                       /* '{'  */
  YYSYMBOL_73_ = 73,                       /* '}'  */
  YYSYMBOL_74_ = 74,                       /* ','  */
  YYSYMBOL_75_ = 75,                       /* '='  */
  YYSYMBOL_76_ = 76,                       /* '['  */
  YYSYMBOL_77_ = 77,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 78,                  /* $accept  */
  YYSYMBOL_S = 79,                         /* S  */
  YYSYMBOL_80_1 = 80,                      /* $@1  */
  YYSYMBOL_PROGRAM = 81,                   /* PROGRAM  */
  YYSYMBOL_FUNDECL = 82,                   /* FUNDECL  */
  YYSYMBOL_83_2 = 83,                      /* $@2  */
  YYSYMBOL_PARAMLIST = 84,                 /* PARAMLIST  */
  YYSYMBOL_STMNTS = 85,                    /* STMNTS  */
  YYSYMBOL_A = 86,                         /* A  */
  YYSYMBOL_87_3 = 87,                      /* $@3  */
  YYSYMBOL_88_4 = 88,                      /* $@4  */
  YYSYMBOL_89_5 = 89,                      /* $@5  */
  YYSYMBOL_90_6 = 90,                      /* $@6  */
  YYSYMBOL_91_7 = 91,                      /* $@7  */
  YYSYMBOL_CASE_LIST = 92,                 /* CASE_LIST  */
  YYSYMBOL_CASE_ITEM = 93,                 /* CASE_ITEM  */
  YYSYMBOL_CASE_EXPR = 94,                 /* CASE_EXPR  */
  YYSYMBOL_DECLSTATEMENT = 95,             /* DECLSTATEMENT  */
  YYSYMBOL_INIT_LIST = 96,                 /* INIT_LIST  */
  YYSYMBOL_DECLLIST = 97,                  /* DECLLIST  */
  YYSYMBOL_INDEX = 98,                     /* INDEX  */
  YYSYMBOL_TYPE = 99,                      /* TYPE  */
  YYSYMBOL_BASE_TYPE = 100,                /* BASE_TYPE  */
  YYSYMBOL_ASSGN = 101,                    /* ASSGN  */
  YYSYMBOL_BOOLEXPR = 102,                 /* BOOLEXPR  */
  YYSYMBOL_ASNEXPR = 103,                  /* ASNEXPR  */
  YYSYMBOL_EXPR = 104,                     /* EXPR  */
  YYSYMBOL_FUNCALL = 105,                  /* FUNCALL  */
  YYSYMBOL_ARGLIST = 106,                  /* ARGLIST  */
  YYSYMBOL_TERM = 107,                     /* TERM  */
  YYSYMBOL_INCRDECR = 108,                 /* INCRDECR  */
  YYSYMBOL_UN = 109                        /* UN  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1091

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  258

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    62,     2,     2,    71,    67,     2,     2,
      68,    69,    65,    63,    74,    64,     2,    66,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    61,     2,
       2,    75,     2,    60,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    76,     2,    77,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    72,     2,    73,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    70
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   471,   471,   471,   480,   481,   488,   489,   490,   491,
     493,   493,   601,   608,   614,   616,   617,   619,   619,   630,
     636,   642,   666,   682,   685,   686,   693,   694,   695,   702,
     706,   706,   707,   711,   711,   712,   712,   713,   713,   718,
     719,   720,   722,   723,   725,   726,   728,   739,   747,   885,
     893,   899,   906,   919,   934,   946,   963,   982,   996,  1021,
    1049,  1067,  1086,  1089,  1097,  1098,  1099,  1100,  1101,  1102,
    1103,  1104,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,
    1114,  1115,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,
    1125,  1126,  1127,  1128,  1130,  1146,  1195,  1210,  1225,  1240,
    1264,  1293,  1306,  1325,  1344,  1363,  1380,  1397,  1414,  1415,
    1417,  1479,  1516,  1522,  1538,  1545,  1565,  1592,  1619,  1646,
    1696,  1745,  1765,  1771,  1779,  1787,  1795,  1804,  1805,  1807,
    1808,  1809
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDEN", "NUM", "INT",
  "FLOAT", "CHAR", "DOUBLE", "SHORT", "LONG", "VOID", "BOOL", "CONST",
  "CHARR", "STRING", "PASN", "MASN", "DASN", "SASN", "INC", "DEC", "BAND",
  "BOR", "BXOR", "BNOT", "LSHIFT", "RSHIFT", "BANDASN", "BORASN",
  "BXORASN", "LSHIFTASN", "RSHIFTASN", "DEFAULT", "BREAK", "CONTINUE",
  "AND", "OR", "EQ", "NE", "LT", "GT", "LE", "GE", "TR", "FL", "IF",
  "ELSE", "FUNCTION", "RETURN", "CALL", "PRINT", "INPUT", "SIZEOF",
  "WHILE", "DO", "FOR", "SWITCH", "CASE", "MEOF", "'?'", "':'", "'!'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'('", "')'", "UMINUS", "'$'", "'{'",
  "'}'", "','", "'='", "'['", "']'", "$accept", "S", "$@1", "PROGRAM",
  "FUNDECL", "$@2", "PARAMLIST", "STMNTS", "A", "$@3", "$@4", "$@5", "$@6",
  "$@7", "CASE_LIST", "CASE_ITEM", "CASE_EXPR", "DECLSTATEMENT",
  "INIT_LIST", "DECLLIST", "INDEX", "TYPE", "BASE_TYPE", "ASSGN",
  "BOOLEXPR", "ASNEXPR", "EXPR", "FUNCALL", "ARGLIST", "TERM", "INCRDECR",
  "UN", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-126)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-86)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     277,   -47,  -126,    41,   825,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,   356,  -126,   516,   -27,   -18,
     390,   232,   322,    44,    17,    25,   459,  -126,    52,    57,
    -126,  -126,   136,    50,     5,   689,   757,  -126,    58,  -126,
       4,   204,  -126,  -126,   174,  -126,  -126,  -126,  -126,  -126,
    -126,   528,   251,   -17,  1024,   123,  -126,    89,    64,   516,
     135,   251,    -7,   893,   516,   516,    66,   732,  -126,   893,
    -126,  -126,  -126,   140,    13,   105,  -126,   108,  -126,  -126,
    -126,  -126,   516,   516,   516,   516,   516,  -126,  -126,  -126,
    -126,  -126,   516,   516,   516,   516,   516,  -126,  -126,   516,
     -13,     3,  -126,   178,   181,   137,   251,  -126,     2,   969,
     528,   528,   893,   516,   516,   516,   516,   516,   516,   122,
    -126,   112,   800,   127,    55,   893,   143,   120,   950,   976,
     516,  -126,   483,  -126,    58,   516,   199,    14,   147,  -126,
    -126,  -126,   584,    -9,   596,  -126,  -126,     7,     7,    80,
      80,    80,   616,  -126,  -126,   516,  -126,  -126,  -126,  -126,
    -126,  -126,    67,   621,  -126,   175,   163,   616,   616,   616,
     616,   616,   616,   232,  -126,   820,   149,   141,  -126,    81,
    -126,   156,   528,   157,  -126,  -126,  -126,  -126,   888,   172,
      58,   179,  -126,   107,  -126,   203,   893,   183,   252,   516,
    -126,  -126,   893,   528,   -11,  -126,    58,   189,  -126,   516,
     196,   893,  -126,  -126,   180,  -126,  -126,   132,   516,   -12,
    -126,  -126,   200,   963,   516,  -126,   202,   232,   206,   224,
     233,    36,   226,   -12,   229,   516,   642,   893,  -126,  -126,
    -126,   893,  -126,  -126,   239,  -126,  -126,    58,  -126,  -126,
     552,   893,   345,   893,  -126,  -126,  -126,   414
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     4,     0,   131,     5,     1,    64,    65,    67,
      70,    68,    69,    71,    66,     0,   114,   131,     0,     0,
     131,     0,   131,     0,     0,     0,   131,    33,     0,     0,
     130,   129,   131,    37,     0,   131,   131,    41,     0,    62,
       0,     0,   108,   109,     0,    63,   107,    19,    20,    92,
      93,   131,   131,     0,     0,     0,    22,     0,     0,   131,
       0,   131,     0,   131,   131,   131,     0,     0,    39,   131,
       3,     6,     7,    54,     0,     0,    24,     0,    73,    74,
      75,    76,   131,   131,   131,   131,   131,    77,    78,    79,
      80,    81,   131,   131,   131,   131,   131,    40,    72,   131,
     121,   118,   122,   127,   128,     0,   131,    84,     0,     0,
     131,   131,   131,   131,   131,   131,   131,   131,   131,     0,
      21,   131,     0,     0,     0,   131,     0,     0,     0,     0,
     131,   101,   131,    16,     0,   131,     0,    57,     0,    48,
      25,    28,   102,   103,   104,   105,   106,    96,    97,    98,
      99,   100,    94,   127,   128,   131,   117,   125,   126,   123,
     124,   116,     0,   131,    83,    82,     0,    88,    89,    86,
      87,    90,    91,    14,   111,   113,     0,     0,    95,    30,
      32,     0,   131,     0,   115,    38,    15,    52,    55,     0,
       0,     0,    49,     0,    85,    26,   131,     0,     0,   131,
     110,    23,   131,   131,     0,    17,     0,    60,    53,   131,
     119,   131,    29,    10,    13,   112,    31,     0,   131,     0,
      56,    61,     0,    51,   131,    27,     0,    14,     0,     0,
       0,     0,     0,    43,    58,   131,     0,   131,    12,    34,
      35,   131,    46,    47,     0,    18,    42,     0,    50,   120,
     131,   131,   131,   131,    59,    11,    36,   131
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -126,  -126,  -126,    74,  -126,  -126,    78,   -28,    -4,  -126,
    -126,  -126,  -126,  -126,    73,  -126,  -126,  -126,    72,  -125,
     101,   -10,   294,  -126,   -24,   -61,   -16,  -126,   111,  -126,
     216,  -126
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,    34,    35,   226,   197,   132,   133,   219,
     202,    63,   251,    69,   232,   233,   244,    37,   222,    74,
     137,    38,    39,    99,    53,    40,    41,    42,   176,    43,
     105,    44
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,    46,    62,   127,    54,    75,    57,   153,   154,   187,
      54,    55,     5,    82,   138,    84,    67,    85,    86,   110,
     111,   230,    66,   157,   158,   110,   111,   107,   108,   110,
     111,    36,    36,    85,    86,    54,   109,   124,   110,   111,
     242,     6,    66,   122,    47,   109,   231,    58,   128,   129,
     243,    66,   112,    48,    92,    93,    94,    95,    96,   126,
     218,    73,   125,   155,    70,   208,   142,   143,   144,   145,
     146,   163,    94,    95,    96,    76,   147,   148,   149,   150,
     151,   220,   162,   152,   139,    59,   164,   165,   190,   191,
     109,   110,   111,    60,    54,    54,    66,   167,   168,   169,
     170,   171,   172,   110,   111,   175,    85,    86,   166,    71,
      72,    82,    83,    84,   184,    85,    86,   -85,   -85,   188,
      64,   180,   254,    68,   179,    65,   119,    16,   186,    82,
      83,    84,   121,    85,    86,   130,   194,    17,   123,   193,
     161,     7,     8,     9,    10,    11,    12,    13,    14,    15,
     -85,    16,    92,    93,    94,    95,    96,   229,   204,   195,
     120,    17,    23,   198,   140,    25,    54,   141,   110,   111,
      92,    93,    94,    95,    96,    30,    31,   100,   101,   217,
      32,   174,   159,   175,   210,   160,    23,    54,   102,    25,
     173,   182,   212,   223,   103,   104,   178,   181,   216,    30,
      31,   228,   128,   189,    32,    77,   192,   225,   236,   250,
     196,   110,   201,   252,   134,   135,   136,   198,   200,   223,
      78,    79,    80,    81,   203,   257,    82,    83,    84,   205,
      85,    86,    87,    88,    89,    90,    91,     7,     8,     9,
      10,    11,    12,    13,    14,    15,   186,   256,   186,   207,
     211,   209,   213,   186,   227,   214,     7,     8,     9,    10,
      11,    12,    13,    14,    15,   136,    16,    92,    93,    94,
      95,    96,   224,   234,   237,    97,    17,   239,     1,    98,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,   240,   241,    49,    50,    -2,    -2,   245,
     253,    23,    -2,   247,    25,   238,   246,   248,   221,    45,
     215,    -2,    -2,    51,    30,    31,   156,     0,     0,   106,
       0,     0,     0,    -2,     0,    -2,    -2,    -2,    -2,     0,
      -2,    -2,    -2,    -2,    -2,     0,     2,    16,     0,     0,
      -2,    -2,     0,     0,     0,    -2,     0,    17,     0,    -2,
       7,     8,     9,    10,    11,    12,    13,    14,    15,     0,
      16,     7,     8,     9,    10,    11,    12,    13,    14,     0,
      17,     0,    23,     0,     0,    25,     0,     0,   -45,    18,
      19,     0,     0,     0,     0,    30,    31,     0,     0,     0,
      32,    20,     0,    56,    22,    23,    24,     0,    25,    26,
      27,    28,    29,   -45,     0,    16,     0,     0,    30,    31,
       0,     0,     0,    32,     0,    17,     0,    33,   -45,     7,
       8,     9,    10,    11,    12,    13,    14,    15,     0,    16,
       0,     0,     0,     0,    49,    50,     0,     0,     0,    17,
      23,     0,     0,    25,     0,     0,     0,   -44,    18,    19,
       0,     0,    51,    30,    31,     0,     0,     0,    52,     0,
      20,     0,     0,    22,    23,    24,     0,    25,    26,    27,
      28,    29,   -44,     0,    16,     0,     0,    30,    31,     0,
       0,     0,    32,     0,    17,     0,    33,   -44,     7,     8,
       9,    10,    11,    12,    13,    14,    15,     0,    16,     0,
       0,     0,     0,    49,    50,     0,     0,     0,    17,    23,
       0,     0,    25,     0,     0,     0,     0,    18,    19,     0,
       0,    51,    30,    31,     0,     0,     0,    61,     0,    20,
       0,    16,    22,    23,    24,     0,    25,    26,    27,    28,
      29,    17,     0,    16,     0,     0,    30,    31,     0,     0,
       0,    32,     0,    17,     0,    33,   185,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    23,    16,     0,    25,
       0,     0,    49,    50,     0,     0,     0,    17,    23,    30,
      31,    25,     0,     0,    32,     0,    18,    19,     0,     0,
      51,    30,    31,     0,     0,     0,   106,     0,    20,     0,
       0,    22,    23,    24,     0,    25,    26,    27,    28,    29,
      85,    86,     0,     0,     0,    30,    31,     0,    82,     0,
      32,     0,    85,    86,    33,   255,     7,     8,     9,    10,
      11,    12,    13,    14,    15,     0,    16,     0,    82,    83,
      84,     0,    85,    86,     0,     0,    17,    92,    93,    94,
      95,    96,     0,     0,     0,    18,    19,   -85,   -85,    92,
      93,    94,    95,    96,    82,    83,    84,    20,    85,    86,
      22,    23,    24,     0,    25,    26,    27,    28,    29,    92,
      93,    94,    95,    96,    30,    31,     0,     0,     0,    32,
     -85,     0,     0,    33,     7,     8,     9,    10,    11,    12,
      13,    14,    15,     0,    16,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    17,     0,     0,     0,     0,   249,
       0,     0,     0,    18,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    20,     0,    21,    22,    23,
      24,     0,    25,    26,    27,    28,    29,     0,    -8,     0,
       0,     0,    30,    31,    82,    83,    84,    32,    85,    86,
       0,    33,     7,     8,     9,    10,    11,    12,    13,    14,
      15,     0,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       0,    18,    19,     0,     0,    92,    93,    94,    95,    96,
       0,   131,     0,    20,     0,    21,    22,    23,    24,     0,
      25,    26,    27,    28,    29,     0,    -9,     0,     0,     0,
      30,    31,    82,    83,    84,    32,    85,    86,     0,    33,
       7,     8,     9,    10,    11,    12,    13,    14,    15,     0,
      16,     0,    82,    83,    84,     0,    85,    86,     0,     0,
      17,     0,     0,     0,     0,     0,     0,     0,     0,    18,
      19,     0,     0,    92,    93,    94,    95,    96,     0,   177,
       0,    20,     0,    21,    22,    23,    24,     0,    25,    26,
      27,    28,    29,    92,    93,    94,    95,    96,    30,    31,
       0,     0,     0,    32,   199,     0,     0,    33,     7,     8,
       9,    10,    11,    12,    13,    14,    15,     0,    16,     0,
      82,    83,    84,     0,    85,    86,     0,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,    18,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    20,
       0,     0,    22,    23,    24,     0,    25,    26,    27,    28,
      29,    92,    93,    94,    95,    96,    30,    31,     0,     0,
       0,    32,   206,     0,     0,    33,    78,    79,    80,    81,
       0,     0,    82,    83,    84,     0,    85,    86,    87,    88,
      89,    90,    91,     0,     0,    82,    83,    84,     0,    85,
      86,    82,    83,    84,     0,    85,    86,     0,    82,    83,
      84,     0,    85,    86,     0,     0,     0,   113,   114,   115,
     116,   117,   118,    92,    93,    94,    95,    96,     0,     0,
       0,     0,     0,     0,     0,    98,    92,    93,    94,    95,
      96,     0,    92,    93,    94,    95,    96,   235,   131,    92,
      93,    94,    95,    96,     0,   183,    82,    83,    84,     0,
      85,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,   115,   116,   117,   118,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96
};

static const yytype_int16 yycheck[] =
{
       4,    17,    26,    64,    20,     1,    22,    20,    21,   134,
      26,    21,    59,    22,     1,    24,    32,    26,    27,    36,
      37,    33,    32,    20,    21,    36,    37,    51,    52,    36,
      37,    35,    36,    26,    27,    51,    52,    61,    36,    37,
       4,     0,    52,    59,    71,    61,    58,     3,    64,    65,
      14,    61,    69,    71,    63,    64,    65,    66,    67,    63,
      71,     3,    69,    76,    59,   190,    82,    83,    84,    85,
      86,    69,    65,    66,    67,    71,    92,    93,    94,    95,
      96,   206,   106,    99,    71,    68,   110,   111,    74,    75,
     106,    36,    37,    68,   110,   111,   106,   113,   114,   115,
     116,   117,   118,    36,    37,   121,    26,    27,   112,    35,
      36,    22,    23,    24,   130,    26,    27,    36,    37,   135,
      68,   125,   247,    73,    69,    68,     3,    15,   132,    22,
      23,    24,    68,    26,    27,    69,    69,    25,     3,   155,
       3,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      69,    15,    63,    64,    65,    66,    67,   218,   182,   163,
      71,    25,    50,   173,    59,    53,   182,    59,    36,    37,
      63,    64,    65,    66,    67,    63,    64,     3,     4,   203,
      68,    69,     4,   199,    77,     4,    50,   203,    14,    53,
      68,    71,   196,   209,    20,    21,    69,    54,   202,    63,
      64,    69,   218,     4,    68,     1,    59,   211,   224,   237,
      47,    36,    71,   241,    74,    75,    76,   227,    69,   235,
      16,    17,    18,    19,    68,   253,    22,    23,    24,    72,
      26,    27,    28,    29,    30,    31,    32,     5,     6,     7,
       8,     9,    10,    11,    12,    13,   250,   251,   252,    77,
      47,    72,    69,   257,    74,     3,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    76,    15,    63,    64,    65,
      66,    67,    76,    73,    72,    71,    25,    71,     1,    75,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    69,    61,    44,    45,    20,    21,    73,
      61,    50,    25,    74,    53,   227,   233,   235,   207,    15,
     199,    34,    35,    62,    63,    64,   100,    -1,    -1,    68,
      -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    -1,    59,    15,    -1,    -1,
      63,    64,    -1,    -1,    -1,    68,    -1,    25,    -1,    72,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    -1,
      15,     5,     6,     7,     8,     9,    10,    11,    12,    -1,
      25,    -1,    50,    -1,    -1,    53,    -1,    -1,    33,    34,
      35,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,    46,    -1,    71,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    -1,    15,    -1,    -1,    63,    64,
      -1,    -1,    -1,    68,    -1,    25,    -1,    72,    73,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    -1,    15,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    25,
      50,    -1,    -1,    53,    -1,    -1,    -1,    33,    34,    35,
      -1,    -1,    62,    63,    64,    -1,    -1,    -1,    68,    -1,
      46,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    15,    -1,    -1,    63,    64,    -1,
      -1,    -1,    68,    -1,    25,    -1,    72,    73,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    -1,    15,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    25,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    34,    35,    -1,
      -1,    62,    63,    64,    -1,    -1,    -1,    68,    -1,    46,
      -1,    15,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    25,    -1,    15,    -1,    -1,    63,    64,    -1,    -1,
      -1,    68,    -1,    25,    -1,    72,    73,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    50,    15,    -1,    53,
      -1,    -1,    44,    45,    -1,    -1,    -1,    25,    50,    63,
      64,    53,    -1,    -1,    68,    -1,    34,    35,    -1,    -1,
      62,    63,    64,    -1,    -1,    -1,    68,    -1,    46,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      26,    27,    -1,    -1,    -1,    63,    64,    -1,    22,    -1,
      68,    -1,    26,    27,    72,    73,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    15,    -1,    22,    23,
      24,    -1,    26,    27,    -1,    -1,    25,    63,    64,    65,
      66,    67,    -1,    -1,    -1,    34,    35,    36,    37,    63,
      64,    65,    66,    67,    22,    23,    24,    46,    26,    27,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    63,
      64,    65,    66,    67,    63,    64,    -1,    -1,    -1,    68,
      69,    -1,    -1,    72,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    -1,    15,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    48,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    -1,    59,    -1,
      -1,    -1,    63,    64,    22,    23,    24,    68,    26,    27,
      -1,    72,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    -1,    63,    64,    65,    66,    67,
      -1,    69,    -1,    46,    -1,    48,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    -1,    59,    -1,    -1,    -1,
      63,    64,    22,    23,    24,    68,    26,    27,    -1,    72,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    -1,
      15,    -1,    22,    23,    24,    -1,    26,    27,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      35,    -1,    -1,    63,    64,    65,    66,    67,    -1,    69,
      -1,    46,    -1,    48,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    63,    64,    65,    66,    67,    63,    64,
      -1,    -1,    -1,    68,    74,    -1,    -1,    72,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    -1,    15,    -1,
      22,    23,    24,    -1,    26,    27,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    63,    64,    65,    66,    67,    63,    64,    -1,    -1,
      -1,    68,    74,    -1,    -1,    72,    16,    17,    18,    19,
      -1,    -1,    22,    23,    24,    -1,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    22,    23,    24,    -1,    26,
      27,    22,    23,    24,    -1,    26,    27,    -1,    22,    23,
      24,    -1,    26,    27,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    43,    63,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    63,    64,    65,    66,
      67,    -1,    63,    64,    65,    66,    67,    74,    69,    63,
      64,    65,    66,    67,    -1,    69,    22,    23,    24,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    67
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    59,    79,    80,    59,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    15,    25,    34,    35,
      46,    48,    49,    50,    51,    53,    54,    55,    56,    57,
      63,    64,    68,    72,    81,    82,    86,    95,    99,   100,
     103,   104,   105,   107,   109,   100,   104,    71,    71,    44,
      45,    62,    68,   102,   104,    99,    71,   104,     3,    68,
      68,    68,   102,    89,    68,    68,    99,   104,    73,    91,
      59,    81,    81,     3,    97,     1,    71,     1,    16,    17,
      18,    19,    22,    23,    24,    26,    27,    28,    29,    30,
      31,    32,    63,    64,    65,    66,    67,    71,    75,   101,
       3,     4,    14,    20,    21,   108,    68,   102,   102,   104,
      36,    37,    69,    38,    39,    40,    41,    42,    43,     3,
      71,    68,   104,     3,   102,    69,    86,   103,   104,   104,
      69,    69,    85,    86,    74,    75,    76,    98,     1,    71,
      59,    59,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,    20,    21,    76,   108,    20,    21,     4,
       4,     3,   102,    69,   102,   102,    86,   104,   104,   104,
     104,   104,   104,    68,    69,   104,   106,    69,    69,    69,
      86,    54,    71,    69,   104,    73,    86,    97,   104,     4,
      74,    75,    59,   104,    69,    86,    47,    84,    99,    74,
      69,    71,    88,    68,   102,    72,    74,    77,    97,    72,
      77,    47,    86,    69,     3,   106,    86,   102,    71,    87,
      97,    98,    96,   104,    76,    86,    83,    74,    69,   103,
      33,    58,    92,    93,    73,    74,   104,    72,    84,    71,
      69,    61,     4,    14,    94,    73,    92,    74,    96,    77,
      85,    90,    85,    61,    97,    73,    86,    85
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    78,    80,    79,    79,    79,    81,    81,    81,    81,
      83,    82,    84,    84,    84,    85,    85,    87,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      88,    86,    86,    89,    86,    90,    86,    91,    86,    86,
      86,    86,    92,    92,    93,    93,    94,    94,    95,    95,
      96,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      98,    98,    99,    99,   100,   100,   100,   100,   100,   100,
     100,   100,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   103,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     105,   105,   106,   106,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   108,   108,   109,
     109,   109
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     2,     2,     2,     1,     1,
       0,    10,     4,     2,     0,     2,     1,     0,     8,     2,
       2,     3,     2,     5,     2,     3,     5,     7,     3,     6,
       0,     6,     4,     0,     8,     0,    10,     0,     4,     2,
       2,     1,     2,     1,     4,     3,     1,     1,     3,     4,
       3,     1,     3,     4,     1,     3,     5,     2,     6,     8,
       3,     4,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     3,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       5,     4,     3,     1,     1,     4,     3,     3,     2,     5,
       8,     2,     2,     3,     3,     3,     3,     1,     1,     1,
       1,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 471 "parser.y"
   {top = create_env(top, 0);}
#line 2102 "y.tab.c"
    break;

  case 3: /* S: $@1 PROGRAM MEOF  */
#line 471 "parser.y"
                                            {
        if (e) {
            printf("%s\nRejected -> %s\n", buffer, err);
        } else {
            printf("%s\nAccepted\n", buffer);
            print_all_envs();
        }
        YYACCEPT;
    }
#line 2116 "y.tab.c"
    break;

  case 4: /* S: MEOF  */
#line 480 "parser.y"
           { YYACCEPT; }
#line 2122 "y.tab.c"
    break;

  case 5: /* S: error MEOF  */
#line 481 "parser.y"
                 {
        e = 1;
        strcpy(err, "Invalid Statements");
        printf("%s\nRejected -> %s\n", buffer, err);
        YYACCEPT;
    }
#line 2133 "y.tab.c"
    break;

  case 10: /* $@2: %empty  */
#line 493 "parser.y"
                                              {
        if(!e){
            // Check for duplicate parameter names
            if(checkDuplicateParams((yyvsp[-1].decl))) {
                e = 1;
                sprintf(err + strlen(err), "Duplicate parameter names in function %s\n", (yyvsp[-3].str));
            }
            
            // FIX: Strip @ prefix from return type
            char* ret_type = (yyvsp[-4].type)->str;
            if(ret_type[0] == '@') {
                ret_type++;  // Skip the @ character
            }
            
            Function* f = createFunction((yyvsp[-3].str), ret_type);  // NOW USES CLEAN TYPE
            
            struct Decl* p = (yyvsp[-1].decl);
            int pcount = 0;
            Param* param_tail = NULL;
            
            while(p){
                // FIX: Also strip @ from parameter types
                char* param_type = p->type;
                if(param_type[0] == '@') {
                    param_type++;
                }
                
                Param* param = createParam(p->key, param_type);  // USE CLEAN TYPE
                pcount++;
                
                if(f->params == NULL){
                    f->params = param;
                    param_tail = param;
                } else {
                    param_tail->next = param;
                    param_tail = param;
                }
                p = p->next;
            }
            f->param_count = pcount;
            
            if(findFunction((yyvsp[-3].str)) != NULL){
                e = 1;
                sprintf(err + strlen(err), "Redeclaration of function %s\n", (yyvsp[-3].str));
            } else {
                addFunction(f);
            }
            
            printf("Function: %s (returns %s, %d parameters)\n", (yyvsp[-3].str), ret_type, pcount);
            
            // Set current function context (ALSO FIX HERE)
            strcpy(current_function, (yyvsp[-3].str));
            strcpy(current_return_type, ret_type);  // USE CLEAN TYPE
            in_function = 1;
            has_return_statement = 0;
            
            // Create new environment for function scope
            top = create_env(top, offset);
            offset = 0;
            
            // Add parameters to symbol table
            p = (yyvsp[-1].decl);
            while(p){
                Symbol* s = createSymbol(p->key);
                
                // FIX: Strip @ from parameter type before storing
                char* clean_param_type = p->type;
                if(clean_param_type[0] == '@') {
                    clean_param_type++;
                }
                strcpy(s->type, clean_param_type);
                s->offset = offset;
                
                if(strcmp(clean_param_type, "char") == 0) 
                    offset += 1;
                else if(strcmp(clean_param_type, "short") == 0) 
                    offset += 2;
                else if(strcmp(clean_param_type, "int") == 0 || strcmp(clean_param_type, "float") == 0) 
                    offset += 4;
                else if(strcmp(clean_param_type, "long") == 0 || strcmp(clean_param_type, "double") == 0) 
                    offset += 8;
                    
                put(top->table, p->key, s);
                p = p->next;
            }
        }
    }
#line 2225 "y.tab.c"
    break;

  case 11: /* FUNDECL: FUNCTION TYPE IDEN '(' PARAMLIST ')' $@2 '{' STMNTS '}'  */
#line 579 "parser.y"
                     {
        if(!e){
            // Check if non-void function has return statement
           
            if(strcmp(current_return_type, "void") != 0 && !has_return_statement) {
    e = 1;
    sprintf(err + strlen(err), "Error: Non-void function '%s' must return a value of type %s\n", 
            current_function, current_return_type);
}

            // Restore previous environment
            top = top->prev;
            if(!top) offset = 0;
            else offset = top->prev_offset;
            
            // Reset function context
            in_function = 0;
            current_function[0] = '\0';
            current_return_type[0] = '\0';
        }
    }
#line 2251 "y.tab.c"
    break;

  case 12: /* PARAMLIST: TYPE IDEN ',' PARAMLIST  */
#line 601 "parser.y"
                                   {
        if(!e){
            (yyval.decl) = createDecl((yyvsp[-2].str));
            strcpy((yyval.decl)->type, (yyvsp[-3].type)->str);
            (yyval.decl)->next = (yyvsp[0].decl);
        }
    }
#line 2263 "y.tab.c"
    break;

  case 13: /* PARAMLIST: TYPE IDEN  */
#line 608 "parser.y"
                {
        if(!e){
            (yyval.decl) = createDecl((yyvsp[0].str));
            strcpy((yyval.decl)->type, (yyvsp[-1].type)->str);
        }
    }
#line 2274 "y.tab.c"
    break;

  case 14: /* PARAMLIST: %empty  */
#line 614 "parser.y"
      {(yyval.decl) = NULL;}
#line 2280 "y.tab.c"
    break;

  case 17: /* $@3: %empty  */
#line 619 "parser.y"
                           {
        saveoffset = offset;
        switch_depth++;
        top = create_env(top, offset);  // CREATE NEW SCOPE FOR SWITCH
        offset = 0;
    }
#line 2291 "y.tab.c"
    break;

  case 18: /* A: SWITCH '(' EXPR ')' '{' $@3 CASE_LIST '}'  */
#line 624 "parser.y"
                    {
        top = top->prev;  // RESTORE PREVIOUS SCOPE
        if (!top) offset = 0;
        else offset = saveoffset;
        switch_depth--;
    }
#line 2302 "y.tab.c"
    break;

  case 19: /* A: BREAK '$'  */
#line 630 "parser.y"
                {
        if(!e && loop_depth == 0 && switch_depth == 0) {
            e = 1;
            sprintf(err + strlen(err), "break statement not within loop or switch\n");
        }
    }
#line 2313 "y.tab.c"
    break;

  case 20: /* A: CONTINUE '$'  */
#line 636 "parser.y"
                   {
        if(!e && loop_depth == 0) {  // Only check loop_depth, NOT switch_depth
            e = 1;
            sprintf(err + strlen(err), "continue statement not within loop\n");
        }
    }
#line 2324 "y.tab.c"
    break;

  case 21: /* A: RETURN EXPR '$'  */
#line 642 "parser.y"
                      {
        if(!e) {
                                        has_return_statement = 1;
            if(!in_function) {
                e = 1;
                sprintf(err + strlen(err), "return statement outside function\n");
            } else {
            
                // Check if void function is returning a value
                if(strcmp(current_return_type, "void") == 0 || strcmp(current_return_type, "@void") == 0) {
                    e = 1;
                    sprintf(err + strlen(err), "void function '%s' should not return a value\n", current_function);
                } else {
                    // Check return type compatibility
                    if(!isTypeCompatible(current_return_type, (yyvsp[-1].expr)->type)) {
                        e = 1;
                        sprintf(err + strlen(err), "Return type mismatch in function '%s': expected %s, got %s\n", 
                                current_function, current_return_type, (yyvsp[-1].expr)->type);
                    }
                }
                printf("Return statement with value of type: %s\n", (yyvsp[-1].expr)->type);
            }
        }
    }
#line 2353 "y.tab.c"
    break;

  case 22: /* A: RETURN '$'  */
#line 666 "parser.y"
                 {
    if(!e) {
        has_return_statement = 1;
        
        if(!in_function) {
            e = 1;
            sprintf(err + strlen(err), "return statement outside function\n");
        } else {
            // Check if non-void function is returning without value
            if(strcmp(current_return_type, "void") != 0) {
    e = 1;
    sprintf(err + strlen(err), "Non-void function '%s' must return a value\n", current_function);
}
        }
    }
}
#line 2374 "y.tab.c"
    break;

  case 23: /* A: PRINT '(' EXPR ')' '$'  */
#line 682 "parser.y"
                             {
        printf("Print: type=%s\n", (yyvsp[-2].expr)->type);
    }
#line 2382 "y.tab.c"
    break;

  case 25: /* A: ASNEXPR error MEOF  */
#line 686 "parser.y"
                         {
        strcat(err, "Syntax error (missing $ or invalid statement)\n");
        yyerrok;
        e = 1;
        printf("%s\nRejected -> %s\n", buffer, err);
        YYACCEPT;
    }
#line 2394 "y.tab.c"
    break;

  case 28: /* A: EXPR error MEOF  */
#line 695 "parser.y"
                      {
            strcat(err, "Syntax error (missing $ or unknown keyword — did you mean @int/@float etc?)\n");
        yyerrok;
        e = 1;
        printf("%s\nRejected -> %s\n", buffer, err);
        YYACCEPT;
    }
#line 2406 "y.tab.c"
    break;

  case 29: /* A: IF BOOLEXPR ')' A ELSE A  */
#line 702 "parser.y"
                               {
        strcat(err, "missing (\n");
        e = 1;
    }
#line 2415 "y.tab.c"
    break;

  case 30: /* $@4: %empty  */
#line 706 "parser.y"
                             {loop_depth++;}
#line 2421 "y.tab.c"
    break;

  case 31: /* A: WHILE '(' BOOLEXPR ')' $@4 A  */
#line 706 "parser.y"
                                               {loop_depth--;}
#line 2427 "y.tab.c"
    break;

  case 32: /* A: WHILE BOOLEXPR ')' A  */
#line 707 "parser.y"
                           {
        strcat(err, "missing (\n");
        e = 1;
    }
#line 2436 "y.tab.c"
    break;

  case 33: /* $@5: %empty  */
#line 711 "parser.y"
         {loop_depth++;}
#line 2442 "y.tab.c"
    break;

  case 34: /* A: DO $@5 A WHILE '(' BOOLEXPR ')' '$'  */
#line 711 "parser.y"
                                                      {loop_depth--;}
#line 2448 "y.tab.c"
    break;

  case 35: /* $@6: %empty  */
#line 712 "parser.y"
                                                   {loop_depth++;}
#line 2454 "y.tab.c"
    break;

  case 36: /* A: FOR '(' ASNEXPR '$' BOOLEXPR '$' ASNEXPR ')' $@6 A  */
#line 712 "parser.y"
                                                                     {loop_depth--;}
#line 2460 "y.tab.c"
    break;

  case 37: /* $@7: %empty  */
#line 713 "parser.y"
          {top = create_env(top, offset); offset = 0;}
#line 2466 "y.tab.c"
    break;

  case 38: /* A: '{' $@7 STMNTS '}'  */
#line 713 "parser.y"
                                                                  {
        top = top->prev;
        if (!top) offset = 0;
        else offset = top->prev_offset;
    }
#line 2476 "y.tab.c"
    break;

  case 46: /* CASE_EXPR: NUM  */
#line 728 "parser.y"
               {
        if(!e) {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, (yyvsp[0].str));
            if(checkfloat((yyvsp[0].str))) {
                strcpy((yyval.expr)->type, "float");
            } else {
                strcpy((yyval.expr)->type, "int");
            }
        }
    }
#line 2492 "y.tab.c"
    break;

  case 47: /* CASE_EXPR: CHARR  */
#line 739 "parser.y"
            {
        if(!e) {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, (yyvsp[0].str));
            strcpy((yyval.expr)->type, "char");
        }
    }
#line 2504 "y.tab.c"
    break;

  case 48: /* DECLSTATEMENT: TYPE DECLLIST '$'  */
#line 747 "parser.y"
                                 {
        struct Decl* temp = (yyvsp[-1].decl);
        while(temp){
            if (temp->re){
                e = 1;
                Symbol* s = get(top->table, temp->key);
                if (strcmp(s->type, (yyvsp[-2].type)->str) == 0){
                    sprintf(err + strlen(err), "Redeclaration of %s\n", s->name);
                } else {
                    sprintf(err + strlen(err), "Conflicting types for %s\n", s->name);
                }
            }
            
            if (strcmp(temp->type, "") == 0){
                Symbol* s = get(top->table, temp->key);
                s->offset = offset;
                offset += (yyvsp[-2].type)->size;
                
                char* type_to_store = (yyvsp[-2].type)->str;
                // Remove @ prefix if present
                if (type_to_store[0] == '@') {
                    type_to_store++;
                }
                // Remove const prefix if present
                if (strncmp(type_to_store, "const ", 6) == 0) {
                    strcpy(s->type, type_to_store); // Keep const for const variables
                } else {
                    strcpy(s->type, type_to_store);
                }
                s->dim_count = 0;
                
                printf("Declared: %s (type=%s, offset=0x%04X)\n", temp->key, s->type, s->offset);
            } else {
                Symbol* s = get(top->table, temp->key);
                s->offset = offset;
                offset += (temp->size * (yyvsp[-2].type)->size);
                
                char* base_type_str = (yyvsp[-2].type)->str;
                // Remove @ prefix if present
                if (base_type_str[0] == '@') {
                    base_type_str++;
                }
                
                sprintf(s->type, "%s[%d]", base_type_str, temp->size);
                
                s->dim_count = 0;
                char dim_str[1000];
                strcpy(dim_str, temp->type);
                
                char* token = strtok(dim_str, " ");
                if (token && strcmp(token, "array") == 0) {
                    token = strtok(NULL, " ");
                }
                
                int dims[10];
                while(token != NULL && s->dim_count < 10) {
                    int dim = atoi(token);
                    if (dim > 0) {
                        dims[s->dim_count] = dim;
                        s->dimensions[s->dim_count] = dim;
                        s->dim_count++;
                    }
                    token = strtok(NULL, " ");
                }
                
                if(s->dim_count == 0) {
                    sprintf(s->type, "%s[%d]", base_type_str, temp->size);
                } else if(s->dim_count == 1) {
                    sprintf(s->type, "%s[%d]", base_type_str, dims[0]);
                } else {
                    strcpy(s->type, base_type_str);
                    for(int i = 0; i < s->dim_count; i++){
                        char dim_part[20];
                        sprintf(dim_part, "[%d]", dims[i]);
                        strcat(s->type, dim_part);
                    }
                }
                
                printf("Declared array: %s (type=%s, offset=0x%04X)\n", temp->key, s->type, s->offset);
            }
            
            // Handle initialization
            if (strcmp(temp->lt, "u") != 0 && strcmp(temp->lt, "array_init") != 0){
                // Get clean type without @ prefix and const
                char base_type[100];
                strcpy(base_type, (yyvsp[-2].type)->str);
                char* actual_type = base_type;
                
                // Remove @ prefix if present
                if(actual_type[0] == '@'){
                    actual_type++;
                }
                
                // Remove const prefix if present
                if(strncmp(actual_type, "const ", 6) == 0){
                    actual_type += 6;
                }
                
                // Also clean temp->lt
                char init_type[100];
                strcpy(init_type, temp->lt);
                char* clean_init_type = init_type;
                if(clean_init_type[0] == '@'){
                    clean_init_type++;
                }
                
                if (strcmp(actual_type, clean_init_type) == 0){
                    printf("  Initialized %s = %s\n", temp->key, temp->op);
                } else {
                    if(!temp->is_literal){
                        printf("  Type conversion during initialization: %s to %s\n", clean_init_type, actual_type);
                    } else {
                        printf("  Initialized %s = %s\n", temp->key, temp->op);
                    }
                }
            }
            
            // Check array initialization size
            if(strcmp(temp->lt, "array_init") == 0) {
                // Count initializers
                int init_count = 1;
                for(int i = 0; temp->op[i]; i++) {
                    if(temp->op[i] == ',') init_count++;
                }
                
                if(init_count > temp->size) {
                    e = 1;
                    sprintf(err + strlen(err), "Too many initializers for array %s (expected %d, got %d)\n", 
                            temp->key, temp->size, init_count);
                } else if(init_count < temp->size) {
                    printf("  Warning: Array %s partially initialized (%d/%d elements)\n", 
                           temp->key, init_count, temp->size);
                }
            }
            
            temp = temp->next;
        }
    }
#line 2647 "y.tab.c"
    break;

  case 49: /* DECLSTATEMENT: TYPE DECLLIST error MEOF  */
#line 885 "parser.y"
                               {
       strcat(err, "Syntax error in declaration (  missing $)\n");
        yyerrok;
        e = 1;
        printf("%s\nRejected -> %s\n", buffer, err);
        YYACCEPT;
    }
#line 2659 "y.tab.c"
    break;

  case 50: /* INIT_LIST: EXPR ',' INIT_LIST  */
#line 893 "parser.y"
                              {
        if(!e){
            (yyval.expr) = (yyvsp[-2].expr);
            (yyval.expr)->next = (yyvsp[0].expr);
        }
    }
#line 2670 "y.tab.c"
    break;

  case 51: /* INIT_LIST: EXPR  */
#line 899 "parser.y"
           {
        if(!e){
            (yyval.expr) = (yyvsp[0].expr);
            (yyval.expr)->next = NULL;
        }
    }
#line 2681 "y.tab.c"
    break;

  case 52: /* DECLLIST: IDEN ',' DECLLIST  */
#line 906 "parser.y"
                            {
        if (get(top->table, (yyvsp[-2].str)) == NULL){
            Symbol* s = createSymbol((yyvsp[-2].str));
            put(top->table, (yyvsp[-2].str), s);
            (yyval.decl) = createDecl((yyvsp[-2].str));
            (yyval.decl)->next = (yyvsp[0].decl);
        } else {
            (yyval.decl) = createDecl((yyvsp[-2].str));
            strcpy((yyval.decl)->type, "");
            (yyval.decl)->re = 1;
        }
        strcpy((yyval.decl)->lt, "u");
    }
#line 2699 "y.tab.c"
    break;

  case 53: /* DECLLIST: IDEN INDEX ',' DECLLIST  */
#line 919 "parser.y"
                              {
        if (get(top->table, (yyvsp[-3].str)) == NULL){
            Symbol* s = createSymbol((yyvsp[-3].str));
            put(top->table, (yyvsp[-3].str), s);
            (yyval.decl) = createDecl((yyvsp[-3].str));
            (yyval.decl)->next = (yyvsp[0].decl);
            strcpy((yyval.decl)->type, (yyvsp[-2].type)->str);
            (yyval.decl)->size = (yyvsp[-2].type)->size;
        } else {
            (yyval.decl) = createDecl((yyvsp[-3].str));
            strcpy((yyval.decl)->type, "");
            (yyval.decl)->re = 1;
        }
        strcpy((yyval.decl)->lt, "u");
    }
#line 2719 "y.tab.c"
    break;

  case 54: /* DECLLIST: IDEN  */
#line 934 "parser.y"
           {
        if (get(top->table, (yyvsp[0].str)) == NULL){
            Symbol* s = createSymbol((yyvsp[0].str));
            put(top->table, (yyvsp[0].str), s);
            (yyval.decl) = createDecl((yyvsp[0].str));
        } else {
            (yyval.decl) = createDecl((yyvsp[0].str));
            strcpy((yyval.decl)->type, "");
            (yyval.decl)->re = 1;
        }
        strcpy((yyval.decl)->lt, "u");
    }
#line 2736 "y.tab.c"
    break;

  case 55: /* DECLLIST: IDEN '=' EXPR  */
#line 946 "parser.y"
                    {
        if (get(top->table, (yyvsp[-2].str)) == NULL){
            Symbol* s = createSymbol((yyvsp[-2].str));
            put(top->table, (yyvsp[-2].str), s);
            (yyval.decl) = createDecl((yyvsp[-2].str));
            strcpy((yyval.decl)->lt, (yyvsp[0].expr)->type);
            strcpy((yyval.decl)->op, (yyvsp[0].expr)->str);
            (yyval.decl)->is_literal = isLiteral((yyvsp[0].expr)->str);
        } else {
            (yyval.decl) = createDecl((yyvsp[-2].str));
            strcpy((yyval.decl)->type, "");
            strcpy((yyval.decl)->lt, (yyvsp[0].expr)->type);
            strcpy((yyval.decl)->op, (yyvsp[0].expr)->str);
            (yyval.decl)->is_literal = isLiteral((yyvsp[0].expr)->str);
            (yyval.decl)->re = 1;
        }
    }
#line 2758 "y.tab.c"
    break;

  case 56: /* DECLLIST: IDEN '=' EXPR ',' DECLLIST  */
#line 963 "parser.y"
                                 {
        if (get(top->table, (yyvsp[-4].str)) == NULL){
            Symbol* s = createSymbol((yyvsp[-4].str));
            put(top->table, (yyvsp[-4].str), s);
            (yyval.decl) = createDecl((yyvsp[-4].str));
            strcpy((yyval.decl)->lt, (yyvsp[-2].expr)->type);
            strcpy((yyval.decl)->op, (yyvsp[-2].expr)->str);
            (yyval.decl)->is_literal = isLiteral((yyvsp[-2].expr)->str);
            (yyval.decl)->next = (yyvsp[0].decl);
        } else {
            (yyval.decl) = createDecl((yyvsp[-4].str));
            strcpy((yyval.decl)->type, "");
            strcpy((yyval.decl)->lt, (yyvsp[-2].expr)->type);
            strcpy((yyval.decl)->op, (yyvsp[-2].expr)->str);
            (yyval.decl)->is_literal = isLiteral((yyvsp[-2].expr)->str);
            (yyval.decl)->re = 1;
            (yyval.decl)->next = (yyvsp[0].decl);
        }
    }
#line 2782 "y.tab.c"
    break;

  case 57: /* DECLLIST: IDEN INDEX  */
#line 982 "parser.y"
                 {
        if (get(top->table, (yyvsp[-1].str)) == NULL){
            Symbol* s = createSymbol((yyvsp[-1].str));
            put(top->table, (yyvsp[-1].str), s);
            (yyval.decl) = createDecl((yyvsp[-1].str));
            strcpy((yyval.decl)->type, (yyvsp[0].type)->str);
            (yyval.decl)->size = (yyvsp[0].type)->size;
        } else {
            (yyval.decl) = createDecl((yyvsp[-1].str));
            strcpy((yyval.decl)->type, "");
            (yyval.decl)->re = 1;
        }
        strcpy((yyval.decl)->lt, "u");
    }
#line 2801 "y.tab.c"
    break;

  case 58: /* DECLLIST: IDEN INDEX '=' '{' INIT_LIST '}'  */
#line 996 "parser.y"
                                       {
        if (get(top->table, (yyvsp[-5].str)) == NULL){
            Symbol* s = createSymbol((yyvsp[-5].str));
            put(top->table, (yyvsp[-5].str), s);
            (yyval.decl) = createDecl((yyvsp[-5].str));
            strcpy((yyval.decl)->type, (yyvsp[-4].type)->str);
            (yyval.decl)->size = (yyvsp[-4].type)->size;
            strcpy((yyval.decl)->lt, "array_init");
            
            char init_str[1000] = "";
            struct Expr* e = (yyvsp[-1].expr);
            int idx = 0;
            while(e){
                if(idx > 0) strcat(init_str, ",");
                strcat(init_str, e->str);
                e = e->next;
                idx++;
            }
            strcpy((yyval.decl)->op, init_str);
        } else {
            (yyval.decl) = createDecl((yyvsp[-5].str));
            strcpy((yyval.decl)->type, "");
            (yyval.decl)->re = 1;
        }
    }
#line 2831 "y.tab.c"
    break;

  case 59: /* DECLLIST: IDEN INDEX '=' '{' INIT_LIST '}' ',' DECLLIST  */
#line 1021 "parser.y"
                                                    {
        if (get(top->table, (yyvsp[-7].str)) == NULL){
            Symbol* s = createSymbol((yyvsp[-7].str));
            put(top->table, (yyvsp[-7].str), s);
            (yyval.decl) = createDecl((yyvsp[-7].str));
            strcpy((yyval.decl)->type, (yyvsp[-6].type)->str);
            (yyval.decl)->size = (yyvsp[-6].type)->size;
            (yyval.decl)->next = (yyvsp[0].decl);
            strcpy((yyval.decl)->lt, "array_init");
            
            char init_str[1000] = "";
            struct Expr* e = (yyvsp[-3].expr);
            int idx = 0;
            while(e){
                if(idx > 0) strcat(init_str, ",");
                strcat(init_str, e->str);
                e = e->next;
                idx++;
            }
            strcpy((yyval.decl)->op, init_str);
        } else {
            (yyval.decl) = createDecl((yyvsp[-7].str));
            strcpy((yyval.decl)->type, "");
            (yyval.decl)->re = 1;
            (yyval.decl)->next = (yyvsp[0].decl);
        }
    }
#line 2863 "y.tab.c"
    break;

  case 60: /* INDEX: '[' NUM ']'  */
#line 1049 "parser.y"
                   {
        (yyval.type) = createType();
        int arr_size = atoi((yyvsp[-1].str));
        
        // Check for negative or zero array size
        if(arr_size <= 0) {
            e = 1;
            sprintf(err + strlen(err), "Array size must be positive (got %d)\n", arr_size);
        }
        
        (yyval.type)->size = arr_size;
        sprintf((yyval.type)->str, "array %d ", arr_size);
        
        if (checkfloat((yyvsp[-1].str))){
            e = 1;
            sprintf(err + strlen(err), "Array index cannot be float\n");
        }
    }
#line 2886 "y.tab.c"
    break;

  case 61: /* INDEX: '[' NUM ']' INDEX  */
#line 1067 "parser.y"
                        {
        (yyval.type) = createType();
        int arr_size = atoi((yyvsp[-2].str));
        
        // Check for negative or zero array size
        if(arr_size <= 0) {
            e = 1;
            sprintf(err + strlen(err), "Array size must be positive (got %d)\n", arr_size);
        }
        
        (yyval.type)->size = (yyvsp[0].type)->size * arr_size;
        sprintf((yyval.type)->str, "array %d %s", arr_size, (yyvsp[0].type)->str);
        
        if (checkfloat((yyvsp[-2].str))){
            e = 1;
            sprintf(err + strlen(err), "Array index cannot be float\n");
        }
    }
#line 2909 "y.tab.c"
    break;

  case 62: /* TYPE: BASE_TYPE  */
#line 1086 "parser.y"
                {
        (yyval.type) = (yyvsp[0].type);
    }
#line 2917 "y.tab.c"
    break;

  case 63: /* TYPE: CONST BASE_TYPE  */
#line 1089 "parser.y"
                     {
    (yyval.type) = (yyvsp[0].type);
    char base[100];
    strcpy(base, (yyvsp[0].type)->str);
    if (base[0] == '@') memmove(base, base+1, strlen(base));
    sprintf((yyval.type)->str, "const %s", base);
}
#line 2929 "y.tab.c"
    break;

  case 64: /* BASE_TYPE: INT  */
#line 1097 "parser.y"
               {(yyval.type) = createType(); strcpy((yyval.type)->str, (yyvsp[0].str)); (yyval.type)->size = 4;}
#line 2935 "y.tab.c"
    break;

  case 65: /* BASE_TYPE: FLOAT  */
#line 1098 "parser.y"
            {(yyval.type) = createType(); strcpy((yyval.type)->str, (yyvsp[0].str)); (yyval.type)->size = 4;}
#line 2941 "y.tab.c"
    break;

  case 66: /* BASE_TYPE: BOOL  */
#line 1099 "parser.y"
           {(yyval.type) = createType(); strcpy((yyval.type)->str, (yyvsp[0].str)); (yyval.type)->size = 1;}
#line 2947 "y.tab.c"
    break;

  case 67: /* BASE_TYPE: CHAR  */
#line 1100 "parser.y"
           {(yyval.type) = createType(); strcpy((yyval.type)->str, (yyvsp[0].str)); (yyval.type)->size = 1;}
#line 2953 "y.tab.c"
    break;

  case 68: /* BASE_TYPE: SHORT  */
#line 1101 "parser.y"
            {(yyval.type) = createType(); strcpy((yyval.type)->str, (yyvsp[0].str)); (yyval.type)->size = 2;}
#line 2959 "y.tab.c"
    break;

  case 69: /* BASE_TYPE: LONG  */
#line 1102 "parser.y"
           {(yyval.type) = createType(); strcpy((yyval.type)->str, (yyvsp[0].str)); (yyval.type)->size = 8;}
#line 2965 "y.tab.c"
    break;

  case 70: /* BASE_TYPE: DOUBLE  */
#line 1103 "parser.y"
             {(yyval.type) = createType(); strcpy((yyval.type)->str, (yyvsp[0].str)); (yyval.type)->size = 8;}
#line 2971 "y.tab.c"
    break;

  case 71: /* BASE_TYPE: VOID  */
#line 1104 "parser.y"
           {(yyval.type) = createType(); strcpy((yyval.type)->str, (yyvsp[0].str)); (yyval.type)->size = 0;}
#line 2977 "y.tab.c"
    break;

  case 72: /* ASSGN: '='  */
#line 1106 "parser.y"
           { strcpy((yyval.str), "="); }
#line 2983 "y.tab.c"
    break;

  case 73: /* ASSGN: PASN  */
#line 1107 "parser.y"
           { strcpy((yyval.str), (yyvsp[0].str)); }
#line 2989 "y.tab.c"
    break;

  case 74: /* ASSGN: MASN  */
#line 1108 "parser.y"
           { strcpy((yyval.str), (yyvsp[0].str)); }
#line 2995 "y.tab.c"
    break;

  case 75: /* ASSGN: DASN  */
#line 1109 "parser.y"
           { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3001 "y.tab.c"
    break;

  case 76: /* ASSGN: SASN  */
#line 1110 "parser.y"
           { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3007 "y.tab.c"
    break;

  case 77: /* ASSGN: BANDASN  */
#line 1111 "parser.y"
              { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3013 "y.tab.c"
    break;

  case 78: /* ASSGN: BORASN  */
#line 1112 "parser.y"
             { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3019 "y.tab.c"
    break;

  case 79: /* ASSGN: BXORASN  */
#line 1113 "parser.y"
              { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3025 "y.tab.c"
    break;

  case 80: /* ASSGN: LSHIFTASN  */
#line 1114 "parser.y"
                { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3031 "y.tab.c"
    break;

  case 81: /* ASSGN: RSHIFTASN  */
#line 1115 "parser.y"
                { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3037 "y.tab.c"
    break;

  case 94: /* ASNEXPR: EXPR ASSGN EXPR  */
#line 1130 "parser.y"
                         {
        if (!e && (yyvsp[-2].expr)->lv){
            Symbol* sym = env_get(top, (yyvsp[-2].expr)->str);
            if(sym && strstr(sym->type, "const") != NULL){
                e = 1;
                sprintf(err + strlen(err), "Cannot assign to const variable %s\n", (yyvsp[-2].expr)->str);
            }
            checkTypeAssign((yyvsp[-2].expr), (yyvsp[0].expr));
            printf("Assignment: %s = %s (types: %s = %s)\n", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str, (yyvsp[-2].expr)->type, (yyvsp[0].expr)->type);
        }
        if (!(yyvsp[-2].expr)->lv){
            e = 1;
            strcat(err, "L value not assignable\n");
        }
    }
#line 3057 "y.tab.c"
    break;

  case 95: /* EXPR: SIZEOF '(' IDEN ')'  */
#line 1146 "parser.y"
                          {
        if(!e){
            (yyval.expr) = createExpr();
            Env* temp = top;
            int found = 0;
            int var_size = 4;
            
            while(temp){
                Symbol* sym = get(temp->table, (yyvsp[-1].str));
                if(sym){
                    found = 1;
                    char* base_type = getBaseType(sym->type);
                    
                    if(strcmp(base_type, "char") == 0) 
                        var_size = 1;
                    else if(strcmp(base_type, "short") == 0) 
                        var_size = 2;
                    else if(strcmp(base_type, "int") == 0 || strcmp(base_type, "float") == 0) 
                        var_size = 4;
                    else if(strcmp(base_type, "long") == 0 || strcmp(base_type, "double") == 0) 
                        var_size = 8;
                    
                    if(sym->dim_count > 0){
                        int total_elements = 1;
                        for(int i = 0; i < sym->dim_count; i++){
                            total_elements *= sym->dimensions[i];
                        }
                        var_size *= total_elements;
                    }
                    break;
                }
                temp = temp->prev;
            }
            
            if(!found){
                e = 1;
                sprintf(err + strlen(err), "%s is not declared in scope\n", (yyvsp[-1].str));
            }
            
            sprintf((yyval.expr)->str, "%d", var_size);
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "0");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3111 "y.tab.c"
    break;

  case 96: /* EXPR: EXPR '+' EXPR  */
#line 1195 "parser.y"
                    {
        if (!e){
            (yyval.expr) = createExpr();
            char result_type[100];
            checkType((yyvsp[-2].expr), (yyvsp[0].expr), result_type);
            strcpy((yyval.expr)->type, result_type);
            sprintf((yyval.expr)->str, "(%s + %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3131 "y.tab.c"
    break;

  case 97: /* EXPR: EXPR '-' EXPR  */
#line 1210 "parser.y"
                    {
        if (!e){
            (yyval.expr) = createExpr();
            char result_type[100];
            checkType((yyvsp[-2].expr), (yyvsp[0].expr), result_type);
            strcpy((yyval.expr)->type, result_type);
            sprintf((yyval.expr)->str, "(%s - %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3151 "y.tab.c"
    break;

  case 98: /* EXPR: EXPR '*' EXPR  */
#line 1225 "parser.y"
                    {
        if (!e){
            (yyval.expr) = createExpr();
            char result_type[100];
            checkType((yyvsp[-2].expr), (yyvsp[0].expr), result_type);
            strcpy((yyval.expr)->type, result_type);
            sprintf((yyval.expr)->str, "(%s * %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3171 "y.tab.c"
    break;

  case 99: /* EXPR: EXPR '/' EXPR  */
#line 1240 "parser.y"
                    {
        if (!e){
            // Check for division by zero for numeric constants
            if(isLiteral((yyvsp[0].expr)->str)) {
                double divisor = atof((yyvsp[0].expr)->str);
                if(divisor == 0.0) {
                    e = 1;
                    sprintf(err + strlen(err), "Division by zero\n");
                }
            }
            
            (yyval.expr) = createExpr();
            char result_type[100];
            checkType((yyvsp[-2].expr), (yyvsp[0].expr), result_type);
            strcpy((yyval.expr)->type, result_type);
            sprintf((yyval.expr)->str, "(%s / %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3200 "y.tab.c"
    break;

  case 100: /* EXPR: EXPR '%' EXPR  */
#line 1264 "parser.y"
                    {
        if (!e){
            if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){
                e = 1;
                sprintf(err + strlen(err), "Invalid operands to binary %% (float/double)\n");
            }
            
            // Check for modulo by zero for numeric constants
            if(isLiteral((yyvsp[0].expr)->str)) {
                int divisor = atoi((yyvsp[0].expr)->str);
                if(divisor == 0) {
                    e = 1;
                    sprintf(err + strlen(err), "Modulo by zero\n");
                }
            }
            
            (yyval.expr) = createExpr();
            char result_type[100];
            checkType((yyvsp[-2].expr), (yyvsp[0].expr), result_type);
            strcpy((yyval.expr)->type, result_type);
            sprintf((yyval.expr)->str, "(%s %% %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3234 "y.tab.c"
    break;

  case 101: /* EXPR: '(' EXPR ')'  */
#line 1293 "parser.y"
                   {
        if (!e){
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, (yyvsp[-1].expr)->str);
            strcpy((yyval.expr)->type, (yyvsp[-1].expr)->type);
            (yyval.expr)->lv = (yyvsp[-1].expr)->lv;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3252 "y.tab.c"
    break;

  case 102: /* EXPR: EXPR BAND EXPR  */
#line 1306 "parser.y"
                     {
        if (!e){ 
            if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to bitwise & (float/double)\n");
            }
            (yyval.expr) = createExpr();
            char result_type[100];
            checkType((yyvsp[-2].expr), (yyvsp[0].expr), result_type);
            strcpy((yyval.expr)->type, result_type);
            sprintf((yyval.expr)->str, "(%s & %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3276 "y.tab.c"
    break;

  case 103: /* EXPR: EXPR BOR EXPR  */
#line 1325 "parser.y"
                    {
        if (!e){ 
            if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to bitwise | (float/double)\n");
            }
            (yyval.expr) = createExpr();
            char result_type[100];
            checkType((yyvsp[-2].expr), (yyvsp[0].expr), result_type);
            strcpy((yyval.expr)->type, result_type);
            sprintf((yyval.expr)->str, "(%s | %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3300 "y.tab.c"
    break;

  case 104: /* EXPR: EXPR BXOR EXPR  */
#line 1344 "parser.y"
                     {
        if (!e){ 
            if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to bitwise ^ (float/double)\n");
            }
            (yyval.expr) = createExpr();
            char result_type[100];
            checkType((yyvsp[-2].expr), (yyvsp[0].expr), result_type);
            strcpy((yyval.expr)->type, result_type);
            sprintf((yyval.expr)->str, "(%s ^ %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3324 "y.tab.c"
    break;

  case 105: /* EXPR: EXPR LSHIFT EXPR  */
#line 1363 "parser.y"
                       {
        if (!e){ 
            if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to << (float/double)\n");
            }
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->type, (yyvsp[-2].expr)->type);
            sprintf((yyval.expr)->str, "(%s << %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3346 "y.tab.c"
    break;

  case 106: /* EXPR: EXPR RSHIFT EXPR  */
#line 1380 "parser.y"
                       {
        if (!e){ 
            if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to >> (float/double)\n");
            }
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->type, (yyvsp[-2].expr)->type);
            sprintf((yyval.expr)->str, "(%s >> %s)", (yyvsp[-2].expr)->str, (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3368 "y.tab.c"
    break;

  case 107: /* EXPR: BNOT EXPR  */
#line 1397 "parser.y"
                {
        if (!e){
            if (isFloatingType((yyvsp[0].expr)->type)){
                e = 1;
                sprintf(err + strlen(err), "Invalid operand to bitwise ~ (float/double)\n");
            }
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->type, (yyvsp[0].expr)->type);
            sprintf((yyval.expr)->str, "(~%s)", (yyvsp[0].expr)->str);
            (yyval.expr)->lv = 0;
        } else {
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->str, "error");
            strcpy((yyval.expr)->type, "int");
            (yyval.expr)->lv = 0;
        }
    }
#line 3390 "y.tab.c"
    break;

  case 108: /* EXPR: FUNCALL  */
#line 1414 "parser.y"
              {(yyval.expr) = (yyvsp[0].expr);}
#line 3396 "y.tab.c"
    break;

  case 109: /* EXPR: TERM  */
#line 1415 "parser.y"
           {(yyval.expr) = (yyvsp[0].expr);}
#line 3402 "y.tab.c"
    break;

  case 110: /* FUNCALL: CALL IDEN '(' ARGLIST ')'  */
#line 1417 "parser.y"
                                   {
        if(!e){
            Function* f = findFunction((yyvsp[-3].str));
            if(f == NULL){
                e = 1;
                sprintf(err + strlen(err), "Function %s not declared\n", (yyvsp[-3].str));
                (yyval.expr) = createExpr();
                strcpy((yyval.expr)->type, "int");  // Default type to prevent segfault
                strcpy((yyval.expr)->str, (yyvsp[-3].str));
                (yyval.expr)->lv = 0;
            } else {
                int arg_count = 0;
                struct Expr* arg = (yyvsp[-1].expr);
                Param* param = f->params;
                
                // Count arguments and check types
                while(arg && param){
                    arg_count++;
                    
                    // Type check arguments
                    if(!isTypeCompatible(param->type, arg->type)) {
                        printf("Warning: Argument %d type mismatch in call to %s: expected %s, got %s\n",
                               arg_count, (yyvsp[-3].str), param->type, arg->type);
                    }
                    
                    arg = arg->next;
                    param = param->next;
                }
                
                // Count remaining arguments
                while(arg) {
                    arg_count++;
                    arg = arg->next;
                }
                
                if(arg_count != f->param_count){
                    e = 1;
                    sprintf(err + strlen(err), "Function %s expects %d arguments, got %d\n",
                            (yyvsp[-3].str), f->param_count, arg_count);
                }
                
                (yyval.expr) = createExpr();
                sprintf((yyval.expr)->str, "%s(...)", (yyvsp[-3].str));
                strcpy((yyval.expr)->type, f->return_type);
                (yyval.expr)->lv = 0;

                if(strcmp(f->return_type, "void") == 0) {
    e = 1;
    sprintf(err + strlen(err), "Cannot use return value of void function '%s'\n", (yyvsp[-3].str));
}

                
                printf("Function call: %s (returns %s)\n", (yyvsp[-3].str), f->return_type);
            }
        } else {
            // If already in error state, still create expr to prevent NULL pointer
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->type, "int");
            strcpy((yyval.expr)->str, "error");
            (yyval.expr)->lv = 0;
        }
    }
#line 3469 "y.tab.c"
    break;

  case 111: /* FUNCALL: CALL IDEN '(' ')'  */
#line 1479 "parser.y"
                        {
        if(!e){
            Function* f = findFunction((yyvsp[-2].str));
            if(f == NULL){
                e = 1;
                sprintf(err + strlen(err), "Function %s not declared\n", (yyvsp[-2].str));
                (yyval.expr) = createExpr();
                strcpy((yyval.expr)->type, "int");  // Default type to prevent segfault
                strcpy((yyval.expr)->str, (yyvsp[-2].str));
                (yyval.expr)->lv = 0;
            } else {
                if(f->param_count != 0){
                    e = 1;
                    sprintf(err + strlen(err), "Function %s expects %d arguments, got 0\n",
                            (yyvsp[-2].str), f->param_count);
                }
                
                (yyval.expr) = createExpr();
                sprintf((yyval.expr)->str, "%s()", (yyvsp[-2].str));
                strcpy((yyval.expr)->type, f->return_type);
                (yyval.expr)->lv = 0;
                
                 if(strcmp(f->return_type, "void") == 0) {
    e = 1;
    sprintf(err + strlen(err), "Cannot use return value of void function '%s'\n", (yyvsp[-2].str));
}
                printf("Function call: %s (no args, returns %s)\n", (yyvsp[-2].str), f->return_type);
            }
        } else {
            // If already in error state, still create expr to prevent NULL pointer
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->type, "int");
            strcpy((yyval.expr)->str, "error");
            (yyval.expr)->lv = 0;
        }
    }
#line 3510 "y.tab.c"
    break;

  case 112: /* ARGLIST: EXPR ',' ARGLIST  */
#line 1516 "parser.y"
                          {
        if(!e){
            (yyval.expr) = (yyvsp[-2].expr);
            (yyval.expr)->next = (yyvsp[0].expr);
        }
    }
#line 3521 "y.tab.c"
    break;

  case 113: /* ARGLIST: EXPR  */
#line 1522 "parser.y"
           {
        if(!e){
            (yyval.expr) = (yyvsp[0].expr);
            (yyval.expr)->next = NULL;
        }
    }
#line 3532 "y.tab.c"
    break;

  case 114: /* TERM: STRING  */
#line 1538 "parser.y"
             {
        (yyval.expr) = createExpr();
        strcpy((yyval.expr)->str, (yyvsp[0].str));
        strcpy((yyval.expr)->type, "char*");
        (yyval.expr)->str_len = strlen((yyvsp[0].str)) - 2 + 1;
        (yyval.expr)->lv = 0;
    }
#line 3544 "y.tab.c"
    break;

  case 115: /* TERM: '(' TYPE ')' EXPR  */
#line 1545 "parser.y"
                        {
    if(!e){
        if((yyvsp[0].expr) == NULL) {
            e = 1;
            sprintf(err + strlen(err), "Invalid cast expression\n");
            (yyval.expr) = createExpr();
            strcpy((yyval.expr)->type, (yyvsp[-2].type)->str);
        } else {
            (yyval.expr) = createExpr();
            sprintf((yyval.expr)->str, "(%s)%s", (yyvsp[-2].type)->str, (yyvsp[0].expr)->str);
            strcpy((yyval.expr)->type, (yyvsp[-2].type)->str);
            (yyval.expr)->lv = 0;
        }
    } else {
        (yyval.expr) = createExpr();
        strcpy((yyval.expr)->type, "int");
        strcpy((yyval.expr)->str, "error");
        (yyval.expr)->lv = 0;
    }
}
#line 3569 "y.tab.c"
    break;

  case 116: /* TERM: UN INCRDECR IDEN  */
#line 1565 "parser.y"
                       {
        (yyval.expr) = createExpr();
        Env* temp = top;
        int found = 0;
        while(temp){
            if (get(temp->table, (yyvsp[0].str))){
                found = 1;
                Symbol* t = get(temp->table, (yyvsp[0].str));
                
                // Check if const
                if(strstr(t->type, "const") != NULL) {
                    e = 1;
                    sprintf(err + strlen(err), "Cannot modify const variable %s\n", (yyvsp[0].str));
                }
                
                strcpy((yyval.expr)->type, t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){
            sprintf(err + strlen(err), "%s is not declared in scope\n", (yyvsp[0].str));
            e = 1;
        }
        sprintf((yyval.expr)->str, "%s%s%s", (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].str));
        (yyval.expr)->lv = 0;
    }
#line 3601 "y.tab.c"
    break;

  case 117: /* TERM: UN IDEN INCRDECR  */
#line 1592 "parser.y"
                       {
        (yyval.expr) = createExpr();
        Env* temp = top;
        int found = 0;
        while(temp){
            if (get(temp->table, (yyvsp[-1].str))){
                found = 1;
                Symbol* t = get(temp->table, (yyvsp[-1].str));
                
                // Check if const
                if(strstr(t->type, "const") != NULL) {
                    e = 1;
                    sprintf(err + strlen(err), "Cannot modify const variable %s\n", (yyvsp[-1].str));
                }
                
                strcpy((yyval.expr)->type, t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){
            sprintf(err + strlen(err), "%s is not declared in scope\n", (yyvsp[-1].str));
            e = 1;
        }
        sprintf((yyval.expr)->str, "%s%s%s", (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].str));
        (yyval.expr)->lv = 0;
    }
#line 3633 "y.tab.c"
    break;

  case 118: /* TERM: UN NUM  */
#line 1619 "parser.y"
             {
    (yyval.expr) = createExpr();
    sprintf((yyval.expr)->str, "%s%s", (yyvsp[-1].str), (yyvsp[0].str));
    
    char* num = (yyvsp[0].str);
    
    if(checkfloat(num)) {
        // Has decimal point
        // Check if it needs double precision (more than 6-7 significant digits)
        // Simple rule: tag as "double" by default (like real C)
        strcpy((yyval.expr)->type, "double");
    } else {
        // Integer — check range
        long long val = atoll(num);
        if((yyvsp[-1].str)[0] == '-') val = -val;
        
        if(val >= -128 && val <= 127)
            strcpy((yyval.expr)->type, "int");   // could be char but default int
        else if(val >= -32768 && val <= 32767)
            strcpy((yyval.expr)->type, "int");
        else if(val >= -2147483648LL && val <= 2147483647LL)
            strcpy((yyval.expr)->type, "int");
        else
            strcpy((yyval.expr)->type, "long");  // too big for int
    }
    (yyval.expr)->lv = 0;
}
#line 3665 "y.tab.c"
    break;

  case 119: /* TERM: UN IDEN '[' EXPR ']'  */
#line 1646 "parser.y"
                           {
    (yyval.expr) = createExpr();
    Env* temp = top;
    int found = 0;
    Symbol* sym_found = NULL;
    
    while(temp){
        if (get(temp->table, (yyvsp[-3].str))){
            found = 1;
            sym_found = get(temp->table, (yyvsp[-3].str));
            char* base_type = getBaseType(sym_found->type);
            strcpy((yyval.expr)->type, base_type);
            
            // BOUNDS CHECKING: Check if index is a constant
            if(isLiteral((yyvsp[-1].expr)->str)) {
                int index = atoi((yyvsp[-1].expr)->str);
                
                // Extract array size from type (e.g., "int[3]" -> 3)
                char* type_str = sym_found->type;
                char* bracket_start = strchr(type_str, '[');
                if(bracket_start) {
                    int array_size = atoi(bracket_start + 1);
                    
                    if(index < 0) {
                        e = 1;
                        sprintf(err + strlen(err), 
                                "Array index %d is negative for array %s\n", 
                                index, (yyvsp[-3].str));
                    } else if(index >= array_size) {
                        e = 1;
                        sprintf(err + strlen(err), 
                                "Array index %d out of bounds for array %s[%d] (valid range: 0-%d)\n", 
                                index, (yyvsp[-3].str), array_size, array_size - 1);
                    }
                }
            }
            
            break;
        }
        temp = temp->prev;
    }
    
    if (!found){
        sprintf(err + strlen(err), "%s is not declared in scope\n", (yyvsp[-3].str));
        e = 1;
    }
    
    sprintf((yyval.expr)->str, "%s%s[%s]", (yyvsp[-4].str), (yyvsp[-3].str), (yyvsp[-1].expr)->str);
    (yyval.expr)->lv = 1;
}
#line 3720 "y.tab.c"
    break;

  case 120: /* TERM: UN IDEN '[' EXPR ']' '[' EXPR ']'  */
#line 1696 "parser.y"
                                    {
    (yyval.expr) = createExpr();
    Env* temp = top;
    int found = 0;
    Symbol* sym_found = NULL;
    
    while(temp){
        if (get(temp->table, (yyvsp[-6].str))){
            found = 1;
            sym_found = get(temp->table, (yyvsp[-6].str));
            char* base_type = getBaseType(sym_found->type);
            strcpy((yyval.expr)->type, base_type);
            
            // Bounds checking for multi-dimensional array
            if(sym_found->dim_count >= 2) {
                if(isLiteral((yyvsp[-4].expr)->str)) {
                    int idx1 = atoi((yyvsp[-4].expr)->str);
                    if(idx1 < 0 || idx1 >= sym_found->dimensions[0]) {
                        e = 1;
                        sprintf(err + strlen(err), 
                                "First index %d out of bounds for array %s (valid: 0-%d)\n", 
                                idx1, (yyvsp[-6].str), sym_found->dimensions[0] - 1);
                    }
                }
                
                if(isLiteral((yyvsp[-1].expr)->str)) {
                    int idx2 = atoi((yyvsp[-1].expr)->str);
                    if(idx2 < 0 || idx2 >= sym_found->dimensions[1]) {
                        e = 1;
                        sprintf(err + strlen(err), 
                                "Second index %d out of bounds for array %s (valid: 0-%d)\n", 
                                idx2, (yyvsp[-6].str), sym_found->dimensions[1] - 1);
                    }
                }
            }
            
            break;
        }
        temp = temp->prev;
    }
    
    if (!found){
        sprintf(err + strlen(err), "%s is not declared in scope\n", (yyvsp[-6].str));
        e = 1;
    }
    
    sprintf((yyval.expr)->str, "%s%s[%s][%s]", (yyvsp[-7].str), (yyvsp[-6].str), (yyvsp[-4].expr)->str, (yyvsp[-1].expr)->str);
    (yyval.expr)->lv = 1;
}
#line 3774 "y.tab.c"
    break;

  case 121: /* TERM: UN IDEN  */
#line 1745 "parser.y"
              {
        (yyval.expr) = createExpr();
        Env* temp = top;
        int found = 0;
        while(temp){
            if (get(temp->table, (yyvsp[0].str))){
                found = 1;
                Symbol* t = get(temp->table, (yyvsp[0].str));
                strcpy((yyval.expr)->type, t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){
            sprintf(err + strlen(err), "%s is not declared in scope\n", (yyvsp[0].str));
            e = 1;
        }
        sprintf((yyval.expr)->str, "%s%s", (yyvsp[-1].str), (yyvsp[0].str));
        (yyval.expr)->lv = 1;
    }
#line 3799 "y.tab.c"
    break;

  case 122: /* TERM: UN CHARR  */
#line 1765 "parser.y"
               {
        (yyval.expr) = createExpr();
        sprintf((yyval.expr)->str, "%s%s", (yyvsp[-1].str), (yyvsp[0].str));
        strcpy((yyval.expr)->type, "char");
        (yyval.expr)->lv = 0;
    }
#line 3810 "y.tab.c"
    break;

  case 123: /* TERM: UN INC NUM  */
#line 1771 "parser.y"
                 {
        e = 1;
        sprintf(err + strlen(err), "Cannot increment a constant value\n");
        (yyval.expr) = createExpr();
        strcpy((yyval.expr)->str, "error");
        strcpy((yyval.expr)->type, "int");
        (yyval.expr)->lv = 0;
    }
#line 3823 "y.tab.c"
    break;

  case 124: /* TERM: UN DEC NUM  */
#line 1779 "parser.y"
                 {
        e = 1;
        sprintf(err + strlen(err), "Cannot decrement a constant value\n");
        (yyval.expr) = createExpr();
        strcpy((yyval.expr)->str, "error");
        strcpy((yyval.expr)->type, "int");
        (yyval.expr)->lv = 0;
    }
#line 3836 "y.tab.c"
    break;

  case 125: /* TERM: UN NUM INC  */
#line 1787 "parser.y"
                 {
        e = 1;
        sprintf(err + strlen(err), "Cannot increment a constant value\n");
        (yyval.expr) = createExpr();
        strcpy((yyval.expr)->str, "error");
        strcpy((yyval.expr)->type, "int");
        (yyval.expr)->lv = 0;
    }
#line 3849 "y.tab.c"
    break;

  case 126: /* TERM: UN NUM DEC  */
#line 1795 "parser.y"
                 {
        e = 1;
        sprintf(err + strlen(err), "Cannot decrement a constant value\n");
        (yyval.expr) = createExpr();
        strcpy((yyval.expr)->str, "error");
        strcpy((yyval.expr)->type, "int");
        (yyval.expr)->lv = 0;
    }
#line 3862 "y.tab.c"
    break;

  case 127: /* INCRDECR: INC  */
#line 1804 "parser.y"
              { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3868 "y.tab.c"
    break;

  case 128: /* INCRDECR: DEC  */
#line 1805 "parser.y"
          { strcpy((yyval.str), (yyvsp[0].str)); }
#line 3874 "y.tab.c"
    break;

  case 129: /* UN: '-'  */
#line 1807 "parser.y"
        { strcpy((yyval.str), "-"); }
#line 3880 "y.tab.c"
    break;

  case 130: /* UN: '+'  */
#line 1808 "parser.y"
          { strcpy((yyval.str), "+"); }
#line 3886 "y.tab.c"
    break;

  case 131: /* UN: %empty  */
#line 1809 "parser.y"
      { strcpy((yyval.str), ""); }
#line 3892 "y.tab.c"
    break;


#line 3896 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1811 "parser.y"


void yyerror(char* msg) {
    return;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Enter your code \n");
        yyin = stdin;
    } else {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            printf("Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
    }
    
    yyparse();
    
    if (argc >= 2 && yyin != stdin) {
        fclose(yyin);
    }
    return 0;
}
