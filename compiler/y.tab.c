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
#line 2 "parser.y"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<float.h>
#include<math.h>
extern FILE *yyin;
extern char buffer[];
char err[1000];
int e=0;
int label=0;
char* genvar();
char imcode[10000][10000];
int code=0;
int offset=0;
int saveoffset;
int string_count = 0;

int loop_depth = 0;
int switch_depth = 0;

char current_function[100] = "";
char current_return_type[100] = "";
int in_function = 0;
int has_return_statement = 0;

typedef struct Param{
    char name[100];
    char type[100];
    struct Param* next;
} Param;
typedef struct BasicBlock {
    int start_line;
    int end_line;
    int block_id;
    struct BasicBlock* next;
} BasicBlock;

BasicBlock* blocks = NULL;
int block_count = 0;


char current_switch_var[100];
typedef struct Function{
    char name[100];
    char return_type[100];
    Param* params;
    int param_count;
    int start_label;
    struct Function* next;
} Function;




Function* func_list = NULL;

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

typedef struct Symbol{
        char name[100];
        char type[100];
        int size;
        int offset;
        int dimensions[10];
        int dim_count;
        struct Symbol*next;
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
        int str_len;
        struct Decl* next;
};

struct Node{
        struct Node* next;
        int addr;
};
struct Expr{
        char str[1000];
        char type[100];
        int lv;
        int str_len;
        struct Expr* next;
};
struct BoolNode{
        struct Node* T;
        struct Node* F;
        struct Node* N;
        struct Node* B;
        struct Node* C;
};
struct Subscript{
        char indices[1000];
        int count;
};

struct Node* createNode(int addr){
        struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        node->next = NULL;
        node->addr = addr;
        return node;
}

struct Decl* createDecl(char* key){
        struct Decl *node = (struct Decl*)malloc(sizeof(struct Decl));
        node->re=0;
        node->is_literal=0;  
        strcpy(node->key,key);
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

struct BoolNode* createBoolNode(){
        struct BoolNode* node = (struct BoolNode*)malloc(sizeof(struct BoolNode));
    node->T = NULL;
    node->F = NULL;
    node->N = NULL;
    node->B = NULL;
    node->C = NULL;
    return node;
}
struct Subscript* createSubscript(){
        struct Subscript* node = (struct Subscript*)malloc(sizeof(struct Subscript));
        strcpy(node->indices, "");
        node->count = 0;
        return node;
}
struct Node* merge(struct Node* a,struct Node* b){
        if (a==NULL && b==NULL) return NULL;
        if (a==NULL) return b;
        if (b==NULL) return a;
        struct Node* t = a;
        while(t->next!=NULL){
                t = t->next;
        }
        t->next = b;
        return a;
}

int checkfloat(char* t){
        while(*t){
                if (*t=='.') return 1;
                t++;
        }
        return 0;
}

void backpatch(struct Node* a,int addr){
        while(a!=NULL){
                int len = strlen(imcode[a->addr]);
                if (imcode[a->addr][len-1] != '\n') {
                    sprintf(imcode[a->addr] + len, "%d\n", addr);
                }
                a = a->next;
        }
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

int isFloatingType(char* type){
    return (strcmp(type, "float") == 0 || strcmp(type, "double") == 0);
}

int isIntegerType(char* type){
    return (strcmp(type, "char") == 0 || 
            strcmp(type, "short") == 0 || 
            strcmp(type, "int") == 0 || 
            strcmp(type, "long") == 0);
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


// Check if types are compatible for return
int isTypeCompatible(char* expected, char* actual) {
    // Remove const qualifier for comparison
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
    
    // Exact match
    if (strcmp(exp_base, act_base) == 0) return 1;
    
    // Get type ranks
    int exp_rank = getTypeRank(exp_base);
    int act_rank = getTypeRank(act_base);
    
    // Both are numeric types - allow conversion
    if (exp_rank > 0 && act_rank > 0) {
        // Issue warnings for problematic conversions
        if (act_rank > exp_rank) {
            // Narrowing conversion
            sprintf(err+strlen(err), 
                "Warning: Implicit narrowing conversion from %s to %s\n",
                act_base, exp_base);
        }
        
        // Float to integer conversion loses precision
        if (isFloatingType(act_base) && isIntegerType(exp_base)) {
            sprintf(err+strlen(err), 
                "Warning: Conversion from %s to %s may lose precision\n",
                act_base, exp_base);
        }
        
        // Integer to float is usually safe but mention it
        if (isIntegerType(act_base) && isFloatingType(exp_base)) {
            // This is generally safe, no warning needed for promotion
        }
        
        return 1;  // Allow all numeric conversions
    }
    
    // Not compatible
    return 0;
}




void validateNumericLiteral(char* literal, char* target_type, char* var_name) {
    if (!isNumericConstant(literal)) return;

    double value = atof(literal);

    if (strcmp(target_type, "char") == 0) {
        if (value < -128 || value > 127)
            sprintf(err+strlen(err),
                "Warning: Initializer %.0f for '%s' exceeds char range [-128, 127]\n",
                value, var_name);
    }
    else if (strcmp(target_type, "short") == 0) {
        if (value < -32768 || value > 32767)
            sprintf(err+strlen(err),
                "Warning: Initializer %.0f for '%s' exceeds short range [-32768, 32767]\n",
                value, var_name);
    }
    else if (strcmp(target_type, "int") == 0) {
        if (value < -2147483648.0 || value > 2147483647.0)
            sprintf(err+strlen(err),
                "Warning: Initializer %.0f for '%s' exceeds int range [-2147483648, 2147483647]\n",
                value, var_name);
    }
    else if (strcmp(target_type, "long") == 0) {
        if (value > 9223372036854775807.0 || value < -9223372036854775808.0)
            sprintf(err+strlen(err),
                "Warning: Initializer %.0f for '%s' exceeds long range\n",
                value, var_name);
    }
    else if (strcmp(target_type, "float") == 0) {
        if (value > FLT_MAX || value < -FLT_MAX) {
            sprintf(err+strlen(err),
                "Warning: Initializer %g for '%s' exceeds float range [-%g, %g] (will be inf)\n",
                value, var_name, FLT_MAX, FLT_MAX);
        } else if (value != 0.0 && fabs(value) < FLT_MIN) {
            sprintf(err+strlen(err),
                "Warning: Initializer %g for '%s' is too small for float (will underflow to 0)\n",
                value, var_name);
        }
    }
    else if (strcmp(target_type, "double") == 0) {
        if (isinf(value) || (value > DBL_MAX || value < -DBL_MAX)) {
            sprintf(err+strlen(err),
                "Warning: Initializer for '%s' exceeds double range (will be inf)\n",
                var_name);
        } else if (value != 0.0 && fabs(value) < DBL_MIN) {
            sprintf(err+strlen(err),
                "Warning: Initializer %g for '%s' is too small for double (will underflow to 0)\n",
                value, var_name);
        }
    }
}

/*duplicate parameter name check*/
int checkDuplicateParams(struct Decl* params) {
    struct Decl* p1 = params;
    while(p1) {
        struct Decl* p2 = p1->next;
        while(p2) {
            if(strcmp(p1->key, p2->key) == 0) return 1;
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    return 0;
}

int tryConstantFold(struct Expr* op1, struct Expr* op2, char op, struct Expr* result) {
    if (!isNumericConstant(op1->str) || !isNumericConstant(op2->str)) return 0;
    double val1, val2, res;
    val1 = atof(op1->str);
    val2 = atof(op2->str);
    switch(op) {
        case '+': res = val1 + val2; break;
        case '-': res = val1 - val2; break;
        case '*': res = val1 * val2; break;
        case '/': if (val2 == 0) return 0; res = val1 / val2; break;
        case '%':
            if (val2 == 0) return 0;
            if (isFloatingType(op1->type) || isFloatingType(op2->type)) return 0;
            res = (int)val1 % (int)val2; break;
        case '&': res = (int)val1 & (int)val2; break;
        case '|': res = (int)val1 | (int)val2; break;
        case '^': res = (int)val1 ^ (int)val2; break;
        case 'l': res = (int)val1 << (int)val2; break;
        case 'r': res = (int)val1 >> (int)val2; break;
        default: return 0;
    }
    if(op=='&'||op=='|'||op=='^'||op=='>>'||op=='<<'){
        sprintf(result->str, "%d", (int)res);
        strcpy(result->type, "int");
    } else if (isFloatingType(op1->type) || isFloatingType(op2->type)) {
        sprintf(result->str, "%g", res);
        strcpy(result->type, "float");
    } else {
        /* int op int → always int (C semantics: truncate toward zero) */
        sprintf(result->str, "%d", (int)res);
        strcpy(result->type, "int");
    }
    result->lv = 0;
    return 1;
}

Symbol* createSymbol(char* name){
        Symbol* node = (Symbol*)malloc(sizeof(Symbol));
        memset(node, 0, sizeof(Symbol));
        strcpy(node->name,name);
        return node;
}

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
        if (strcmp(entry->key, key) == 0) return entry->value;
        entry = entry->next;
    }
    return NULL;
}

Env* create_env(Env* prev,int offset) {
    Env* env = malloc(sizeof(Env));
    env->prev = prev;
    env->table = create_table();
    env->prev_offset = offset;
    envs[env_count++] = env;
    return env;
}

void env_put(Env* env, const char* key, Symbol* sym) {
    put(env->table, key, sym);
}

Symbol* env_get(Env* env, const char* key) {
    for (Env* e = env; e != NULL; e = e->prev) {
        Symbol* found = get(e->table, key);
        if (found != NULL) return found;
    }
    return NULL;
}

Env* top = NULL;
/*
void print_table(Table* table) {
    for (int i = 0; i < table->size; i++) {
        TableEntry* entry = table->buckets[i];
        while (entry) {
                        printf("%-10d %-25s %-20s\n", 
                entry->value->offset, 
                entry->value->name, 
                entry->value->type);

            entry = entry->next;
        }
    }
}

void print_all_envs() {
    printf("\n=== Symbol Table (Storage Layout) ===\n");
    for (int i = 0; i < env_count; i++) {
        printf("\nScope %d:\n", i);
        printf("%-10s %-25s %-20s\n", "Offset", "Name", "Type");
printf("-----------------------------------------------\n");
        print_table(envs[i]->table);
    }
    printf("======================================\n");
}

*/



/* Return byte-size of a base type */
static int type_byte_size(const char* base) {
    if (strcmp(base,"char"  )==0) return 1;
    if (strcmp(base,"short" )==0) return 2;
    if (strcmp(base,"int"   )==0) return 4;
    if (strcmp(base,"float" )==0) return 4;
    if (strcmp(base,"long"  )==0) return 8;
    if (strcmp(base,"double")==0) return 8;
    return 4; /* default */
}

/* Extract base type from "int[5]" → "int", "char[2][3]" → "char", "int" → "int" */
static void extract_base(const char* full_type, char* out) {
    strcpy(out, full_type);
    char* bracket = strchr(out, '[');
    if (bracket) *bracket = '\0';
    /* strip leading @ if present */
    if (out[0] == '@') memmove(out, out+1, strlen(out));
}


/* Collect all symbols from a table into a sorted array (by offset) */
#define MAX_SYMS 512
static int collect_symbols(Table* table, Symbol** out) {
    int n = 0;
    for (int i = 0; i < table->size && n < MAX_SYMS; i++) {
        TableEntry* e = table->buckets[i];
        while (e && n < MAX_SYMS) { out[n++] = e->value; e = e->next; }
    }
    /* insertion sort by offset */
    for (int i = 1; i < n; i++) {
        Symbol* key = out[i]; int j = i-1;
        while (j >= 0 && out[j]->offset > key->offset) { out[j+1]=out[j]; j--; }
        out[j+1] = key;
    }
    return n;
}




void print_table(Table* table) {
    Symbol* syms[MAX_SYMS];
    int n = collect_symbols(table, syms);
    for (int i = 0; i < n; i++) {
        Symbol* s = syms[i];
        char base[100]; extract_base(s->type, base);
        int elem_size = type_byte_size(base);
        int total_elems = 1;
        for (int d = 0; d < s->dim_count; d++) total_elems *= s->dimensions[d];
        int total_bytes = elem_size * (total_elems > 0 ? total_elems : 1);

        /* Build dimension string e.g. "[3][4]" or "" */
        char dim_str[100] = "";
        if (s->dim_count > 0) {
            for (int d = 0; d < s->dim_count; d++) {
                char part[20]; sprintf(part, "[%d]", s->dimensions[d]);
                strcat(dim_str, part);
            }
        } else if (strchr(s->type, '[') != NULL) {
            /* fallback: pull dims from type string */
            char* p = strchr(s->type, '[');
            strcpy(dim_str, p ? p : "");
        }

        /* Category */
        char category[20];
        if (s->dim_count > 1)       strcpy(category, "multi-dim array");
        else if (s->dim_count == 1) strcpy(category, "array");
        else if (strchr(s->type,'[') != NULL) strcpy(category, "array");
        else                        strcpy(category, "scalar");

        /* merge base + dim_str into one type field */
        char full_type[100];
        snprintf(full_type, sizeof(full_type), "%s%s", base, dim_str);

        printf("  %-8d  %-20s  %-20s  %-8d  %-8d  %-16s\n",
            s->offset,
            s->name,
            full_type,
            elem_size,
            total_bytes,
            category);
    }
}

void print_all_envs() {
    /* Count non-empty scopes */
    int non_empty = 0;
    for (int i = 0; i < env_count; i++) {
        for (int b = 0; b < envs[i]->table->size; b++) {
            if (envs[i]->table->buckets[b]) { non_empty++; break; }
        }
    }

    printf("\n");
    printf("==================================================================================\n");
    printf("                        SYMBOL TABLE  (Storage Layout)                      \n");
    printf("==================================================================================\n");

    int scope_printed = 0;
    for (int i = 0; i < env_count; i++) {
        /* Check if scope has any symbols */
        int has_syms = 0;
        for (int b = 0; b < envs[i]->table->size; b++)
            if (envs[i]->table->buckets[b]) { has_syms = 1; break; }

        char scope_label[50];
            printf("\n");

        if (i == 0) strcpy(scope_label, "Global");
        else        sprintf(scope_label, "Local (scope %d)", i);

        printf("  Scope %d  [%s]\n", i, scope_label);
        printf("  %-8s  %-20s  %-20s  %-8s  %-8s  %-16s\n",
               "Offset", "Name", "Type", "ESize", "Total", "Category");
        printf("  %-8s  %-20s  %-20s  %-8s  %-8s  %-16s\n",
               "--------", "--------------------", "--------------------",
               "--------", "--------", "----------------");

        if (!has_syms) {
            printf("  (no symbols)\n");
        } else {
            print_table(envs[i]->table);
        }
        scope_printed++;
    }

    /* Grand total */
    int total_vars = 0;
    int total_bytes = 0;
    for (int i = 0; i < env_count; i++) {
        for (int b = 0; b < envs[i]->table->size; b++) {
            TableEntry* e = envs[i]->table->buckets[b];
            while (e) {
                Symbol* s = e->value;
                char base[100]; extract_base(s->type, base);
                int elem_size = type_byte_size(base);
                int total_elems = 1;
                for (int d = 0; d < s->dim_count; d++) total_elems *= s->dimensions[d];
                total_bytes += elem_size * (total_elems > 0 ? total_elems : 1);
                total_vars++;
                e = e->next;
            }
        }
    }
                printf("\n");

    printf("==================================================================================\n");
    printf("  Total symbols: %d  |  Total storage: %d bytes\n", total_vars, total_bytes);
        printf("==================================================================================\n");


}


char* calculateArrayOffset(Symbol* sym, struct Subscript* sub, char* base_name) {
    if (sym->dim_count == 0) return base_name;

    char indices_copy[1000];
    strcpy(indices_copy, sub->indices);
    char* idx_list[10];
    int idx_count = 0;

    char* ptr = indices_copy;
    while (*ptr) {
        if (*ptr == '[') {
            ptr++;
            idx_list[idx_count++] = ptr;
            while (*ptr && *ptr != ']') ptr++;
            if (*ptr == ']') *ptr = '\0';
            ptr++;
        } else {
            ptr++;
        }
    }

    if (idx_count > sym->dim_count) {
        e = 1;
        sprintf(err+strlen(err), "Too many subscripts for array %s\n", base_name);
        return genvar();
    }

    int elem_size = 4;
    if (strstr(sym->type, "char") != NULL) 
        elem_size = 1;
    else if (strstr(sym->type, "short") != NULL) 
        elem_size = 2;
    else if (strstr(sym->type, "int") != NULL || strstr(sym->type, "float") != NULL) 
        elem_size = 4;
    else if (strstr(sym->type, "long") != NULL || strstr(sym->type, "double") != NULL) 
        elem_size = 8;

    int all_constant = 1;
    for (int i = 0; i < idx_count; i++) {
        if (!isNumericConstant(idx_list[i])) { all_constant = 0; break; }
    }

    if (all_constant && sym->dim_count == 1) {
        int idx = atoi(idx_list[0]);
        /*  bounds check for constant index  */
        if (idx < 0) {
            e = 1;
            sprintf(err+strlen(err), "Array index %d is negative for array %s\n", idx, base_name);
        } else if (idx >= sym->dimensions[0] && sym->dimensions[0] > 0) {
            e = 1;
            sprintf(err+strlen(err), "Array index %d out of bounds for array %s[%d] (valid range: 0-%d)\n",
                    idx, base_name, sym->dimensions[0], sym->dimensions[0]-1);
        }
        int byte_offset = idx * elem_size;
        char* result = genvar();
        sprintf(result, "%d", byte_offset);
        return result;
    }

    if (all_constant && sym->dim_count > 1) {
        /*  multi-dim bounds check */
        for (int i = 0; i < idx_count && i < sym->dim_count; i++) {
            int idx = atoi(idx_list[i]);
            if (idx < 0) {
                e = 1;
                sprintf(err+strlen(err), "Index %d at dimension %d is negative for array %s\n", idx, i, base_name);
            } else if (idx >= sym->dimensions[i] && sym->dimensions[i] > 0) {
                e = 1;
                sprintf(err+strlen(err), "Index %d out of bounds at dimension %d for array %s (valid: 0-%d)\n",
                        idx, i, base_name, sym->dimensions[i]-1);
            }
        }
        int total_offset = 0;
        int multiplier = 1;
        for (int i = sym->dim_count - 1; i >= 0; i--) {
            if (i < idx_count) {
                int idx = atoi(idx_list[i]);
                total_offset += idx * multiplier;
            }
            if (i > 0) multiplier *= sym->dimensions[i];
        }
        total_offset *= elem_size;
        char* result = genvar();
        sprintf(result, "%d", total_offset);
        return result;
    }

    char* result = genvar();
    if (sym->dim_count == 1) {
        sprintf(imcode[code], "%d %s = %s * %d\n", code, result, idx_list[0], elem_size);
        code++;
        return result;
    }

    char* running_offset = genvar();
    sprintf(imcode[code], "%d %s = %s\n", code, running_offset, idx_list[0]);
    code++;

    for (int i = 1; i < idx_count; i++) {
        int multiplier = 1;
        for (int j = i; j < sym->dim_count; j++) multiplier *= sym->dimensions[j];
        char* temp1 = genvar();
        sprintf(imcode[code], "%d %s = %s * %d\n", code, temp1, running_offset, multiplier);
        code++;
        char* temp2 = genvar();
        sprintf(imcode[code], "%d %s = %s + %s\n", code, temp2, temp1, idx_list[i]);
        code++;
        strcpy(running_offset, temp2);
    }

    sprintf(imcode[code], "%d %s = %s * %d\n", code, result, running_offset, elem_size);
    code++;
    return result;
}

int isLiteral(char* str) {
    if (!str || *str == '\0') return 0;
    
    // Handle character literals: 'a', 'b', '\n', etc.
    if (str[0] == '\'' && strlen(str) >= 3 && str[strlen(str)-1] == '\'') {
        return 1;
    }
    
    // Handle casted constants: (type) value
    if (str[0] == '(') {
        char* closing = strchr(str, ')');
        if (closing) {
            char* value_part = closing + 1;
            while (*value_part == ' ') value_part++;
            return isLiteral(value_part);  // Recursive check on the value part
        }
    }
    
    // Handle numeric literals: 123, -45, 3.14, .5
    return (isdigit(str[0]) || 
            (str[0] == '-' && isdigit(str[1])) || 
            (str[0] == '.' && isdigit(str[1])));
}


int checkLiteralRange(char* literal, char* target_type) {
    if (!isNumericConstant(literal)) return 1;

    double value = atof(literal);
    int has_warning = 0;

    if (strcmp(target_type, "char") == 0) {
        if (value < -128 || value > 127) {
            sprintf(err+strlen(err),
                "Warning: Value %.0f out of range for char (valid range: -128 to 127)\n", value);
            has_warning = 1;
        }
    }
    else if (strcmp(target_type, "short") == 0) {
        if (value < -32768 || value > 32767) {
            sprintf(err+strlen(err),
                "Warning: Value %.0f out of range for short (valid range: -32768 to 32767)\n", value);
            has_warning = 1;
        }
    }
    else if (strcmp(target_type, "int") == 0) {
        if (value < -2147483648.0 || value > 2147483647.0) {
            sprintf(err+strlen(err),
                "Warning: Value %.0f out of range for int (valid range: -2147483648 to 2147483647)\n", value);
            has_warning = 1;
        }
    }
    else if (strcmp(target_type, "long") == 0) {
        if (fabs(value) > 9.223372e18) {
            sprintf(err+strlen(err),
                "Warning: Value %.0f may be out of range for long\n", value);
            has_warning = 1;
        }
    }
    else if (strcmp(target_type, "float") == 0) {
        if (value > FLT_MAX || value < -FLT_MAX) {
            sprintf(err+strlen(err),
                "Warning: Value %g exceeds float range [-%g, %g] (will be inf)\n",
                value, FLT_MAX, FLT_MAX);
            has_warning = 1;
        } else if (value != 0.0 && fabs(value) < FLT_MIN) {
            sprintf(err+strlen(err),
                "Warning: Value %g is too small for float (will underflow to 0)\n",
                value);
            has_warning = 1;
        }
    }
    else if (strcmp(target_type, "double") == 0) {
        if (isinf(value) || value > DBL_MAX || value < -DBL_MAX) {
            sprintf(err+strlen(err),
                "Warning: Value %g exceeds double range (will be inf)\n",
                value);
            has_warning = 1;
        } else if (value != 0.0 && fabs(value) < DBL_MIN) {
            sprintf(err+strlen(err),
                "Warning: Value %g is too small for double (will underflow to 0)\n",
                value);
            has_warning = 1;
        }
    }

    return !has_warning;
}

/* Check for narrowing conversions between types */
int isNarrowingConversion(char* target_type, char* source_type) {
    int target_rank = getTypeRank(target_type);
    int source_rank = getTypeRank(source_type);
    
    if (target_rank > 0 && source_rank > 0) {
        return source_rank > target_rank;
    }
    return 0;
}

void checkType(struct Expr* op1, struct Expr* op2, char* opr1, char* opr2, char* type){
    char* base_op1 = getBaseType(op1->type);
    char* base_op2 = getBaseType(op2->type);
    if (strcmp(base_op1, base_op2) == 0){
        strcpy(type, base_op1);
        strcpy(opr1, "");
        strcpy(opr2, "");
        return;
    }
    char* promoted = promoteType(base_op1, base_op2);
    strcpy(type, promoted);
    if (strcmp(base_op1, promoted) != 0){
        char* t = genvar();
        strcpy(opr1, t);
        sprintf(imcode[code], "%d %s = (%s) %s\n", code, opr1, promoted, op1->str);
        code++;
    } else {
        strcpy(opr1, "");
    }
    if (strcmp(base_op2, promoted) != 0){
        char* t = genvar();
        strcpy(opr2, t);
        sprintf(imcode[code], "%d %s = (%s) %s\n", code, opr2, promoted, op2->str);
        code++;
    } else {
        strcpy(opr2, "");
    }
}

void checkTypeAssign(struct Expr* op1, struct Expr* op2, char* opr){
    // If types match exactly, no conversion needed
    if (strcmp(op1->type, op2->type) == 0){ 
        strcpy(opr, ""); 
        return; 
    }
    
    // Clean up const qualifiers
    char base_op1_type[100], base_op2_type[100];
    strcpy(base_op1_type, op1->type);
    strcpy(base_op2_type, op2->type);
    char* actual_op1 = base_op1_type;
    char* actual_op2 = base_op2_type;
    if (strncmp(base_op1_type, "const ", 6) == 0) actual_op1 = base_op1_type + 6;
    if (strncmp(base_op2_type, "const ", 6) == 0) actual_op2 = base_op2_type + 6;
    
    // Get base types (without array dimensions)
    char* op1_base = getBaseType(actual_op1);
    char* op2_base = getBaseType(actual_op2);
    
    // If base types match, no conversion needed
    if (strcmp(op1_base, op2_base) == 0) { 
        strcpy(opr, ""); 
        return; 
    }
    
    // Check for narrowing conversion and issue warnings
    int rank1 = getTypeRank(op1_base);
    int rank2 = getTypeRank(op2_base);
    
    if (rank1 > 0 && rank2 > 0) {
        // Both are numeric types
        if (rank2 > rank1) {
            // Narrowing conversion
            sprintf(err+strlen(err), 
                "Warning: Implicit narrowing conversion from %s to %s may lose data\n",
                op2_base, op1_base);
        }
        
        // Special warning for float/double to integer conversion
        if (isFloatingType(op2_base) && isIntegerType(op1_base)) {
            sprintf(err+strlen(err), 
                "Warning: Conversion from %s to %s will discard fractional part\n",
                op2_base, op1_base);
        }
        
        // Check literal range if it's a constant value
        if (isLiteral(op2->str)) {
            checkLiteralRange(op2->str, op1_base);
        }
    }
    
    // Generate the conversion
    if (isLiteral(op2->str)) {
        // For literals, do compile-time cast
        sprintf(opr, "(%s)%s", op1_base, op2->str);
    } else {
        // For variables, generate cast instruction
        char* temp = genvar();
        sprintf(imcode[code], "%d %s = (%s) %s\n", code, temp, op1_base, op2->str);
        code++;
        strcpy(opr, temp);
    }
}


void identityAssignmentElimination() {
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char lhs[100], rhs[100];
        if (sscanf(line, "%d %s = %[^\n]", &line_num, lhs, rhs) == 3) {
            char* end = rhs + strlen(rhs) - 1;
            while (end > rhs && (*end == ' ' || *end == '\n')) { *end = '\0'; end--; }
            if (strcmp(lhs, rhs) == 0)
                sprintf(imcode[i], "%d // IDENTITY: %s = %s (eliminated)\n", line_num, lhs, rhs);
        }
    }
}

void deadStoreElimination() {
    for (int i = 0; i < code - 1; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char lhs[100], rhs[100];
        if (sscanf(line, "%d %s = %[^\n]", &line_num, lhs, rhs) == 3) {
            if (strstr(rhs, "Call") != NULL || strstr(rhs, "PopParam") != NULL) continue;
            for (int j = i + 1; j < code; j++) {
                if (strstr(imcode[j], "// DEAD") != NULL) continue;
                /* Stop at control flow boundaries */
                if (strstr(imcode[j], "if ") != NULL || strstr(imcode[j], "goto ") != NULL ||
                    strstr(imcode[j], "BeginFunc") != NULL ||
                    strstr(imcode[j], "EndFunc") != NULL) break;
                char check_line[10000]; strcpy(check_line, imcode[j]);
                char check_lhs[100];
                char* equals = strchr(check_line, '=');
                if (equals) {
                    /* Variable used on RHS of an assignment — not a dead store */
                    char* rhs_part = equals + 1;
                    if (strstr(rhs_part, lhs) != NULL) break;
                } else {
                    /* No '=' — this is a use-only instruction:
                       printint/printfloat/printchar/printstring,
                       PushParam, Return, inputint etc.
                       If lhs appears anywhere in the line it is being used. */
                    /* Build a word-boundary check to avoid false matches
                       e.g. lhs="x" matching inside "xy" */
                    char* p = check_line;
                    int lhs_len = strlen(lhs);
                    int used = 0;
                    while ((p = strstr(p, lhs)) != NULL) {
                        char before = (p == check_line) ? ' ' : *(p - 1);
                        char after  = *(p + lhs_len);
                        if (!isalnum((unsigned char)before) && before != '_' &&
                            !isalnum((unsigned char)after)  && after  != '_') {
                            used = 1; break;
                        }
                        p++;
                    }
                    if (used) break;
                }
                /* Variable overwritten before any use — dead store */
                if (sscanf(check_line, "%*d %s =", check_lhs) == 1) {
                    if (strcmp(check_lhs, lhs) == 0) {
                        sprintf(imcode[i], "%d // DEAD STORE: %s = %s\n", line_num, lhs, rhs);
                        break;
                    }
                }
            }
        }
    }
}



void redundantLoadElimination() {
    for (int i = 0; i < code - 1; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char lhs[100], rhs[100];
        if (sscanf(line, "%d %s = %[^\n]", &line_num, lhs, rhs) == 3) {
            char* end = rhs + strlen(rhs) - 1;
            while (end > rhs && (*end == ' ' || *end == '\n')) { *end = '\0'; end--; }

            /* Skip complex expressions and non-variable RHS */
            if (strchr(rhs, '+') != NULL || strchr(rhs, '-') != NULL ||
                strchr(rhs, '*') != NULL || strchr(rhs, '/') != NULL ||
                strchr(rhs, '%') != NULL || strchr(rhs, '&') != NULL ||
                strchr(rhs, '|') != NULL || strchr(rhs, '^') != NULL ||
                strstr(rhs, "<<") != NULL || strstr(rhs, ">>") != NULL ||
                strstr(rhs, "Call") != NULL) continue;
            if (isNumericConstant(rhs) || isLiteral(rhs)) continue;

            int lhs_len = strlen(lhs);
            int rhs_len = strlen(rhs);

            /* Scan forward for a redundant load of the same rhs */
            for (int j = i + 1; j < code; j++) {
                if (strstr(imcode[j], "// DEAD") != NULL) continue;

                /* Stop at control-flow / function boundaries */
                if (strstr(imcode[j], "if ")      != NULL ||
                    strstr(imcode[j], "goto ")     != NULL ||
                    strstr(imcode[j], "Return")    != NULL ||
                    strstr(imcode[j], "BeginFunc") != NULL ||
                    strstr(imcode[j], "EndFunc")   != NULL) break;

                char jline[10000]; strcpy(jline, imcode[j]);
                int j_line_num; char j_lhs[100], j_rhs[100];

                /* Word-boundary check: is lhs used on this line?
                   (prevents "a" matching inside "arr") */
                {
                    int used = 0;
                    char* p = jline;
                    while ((p = strstr(p, lhs)) != NULL) {
                        char before = (p == jline) ? ' ' : *(p - 1);
                        char after  = *(p + lhs_len);
                        if (!isalnum((unsigned char)before) && before != '_' &&
                            !isalnum((unsigned char)after)  && after  != '_') {
                            used = 1; break;
                        }
                        p++;
                    }
                    if (used) break;
                }

                /* If rhs (source) is overwritten here, stop — later loads differ */
                if (sscanf(jline, "%*d %s =", j_lhs) == 1) {
                    /* word-boundary check on rhs too (e.g. rhs="arr[8]", lhs="arr") */
                    if (strcmp(j_lhs, rhs) == 0) break;
                    /* also stop if LHS of this line IS rhs (handles array base names) */
                    if (strncmp(j_lhs, rhs, rhs_len) == 0 &&
                        (j_lhs[rhs_len] == '[' || j_lhs[rhs_len] == '\0')) break;
                }

                /* Found another load of the same rhs into a different variable */
                if (sscanf(jline, "%d %s = %[^\n]", &j_line_num, j_lhs, j_rhs) == 3) {
                    char* je = j_rhs + strlen(j_rhs) - 1;
                    while (je > j_rhs && (*je == ' ' || *je == '\n')) { *je = '\0'; je--; }
                    if (strcmp(j_rhs, rhs) == 0 && strcmp(j_lhs, lhs) != 0) {
                        sprintf(imcode[j], "%d %s = %s\n", j_line_num, j_lhs, lhs);
                    }
                }
            }
        }
    }
}

void peepholeOptimization() {
    for (int i = 0; i < code - 1; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        char line1[10000], line2[10000];
        strcpy(line1, imcode[i]); strcpy(line2, imcode[i+1]);
        int line_num1, line_num2; char lhs1[100], rhs1[100], lhs2[100], rhs2[100];
        if (sscanf(line1, "%d %s = %[^\n]", &line_num1, lhs1, rhs1) == 3 &&
            sscanf(line2, "%d %s = %[^\n]", &line_num2, lhs2, rhs2) == 3) {
            char* end = rhs1 + strlen(rhs1) - 1;
            while (end > rhs1 && (*end == ' ' || *end == '\n')) { *end = '\0'; end--; }
            end = rhs2 + strlen(rhs2) - 1;
            while (end > rhs2 && (*end == ' ' || *end == '\n')) { *end = '\0'; end--; }
            if (strchr(rhs1,'+') == NULL && strchr(rhs1,'-') == NULL &&
                strchr(rhs1,'*') == NULL && strchr(rhs1,'/') == NULL &&
                strchr(rhs2,'+') == NULL && strchr(rhs2,'-') == NULL &&
                strchr(rhs2,'*') == NULL && strchr(rhs2,'/') == NULL) {
                if (strcmp(lhs1, rhs2) == 0 && strcmp(rhs1, lhs2) == 0)
                    sprintf(imcode[i+1], "%d // PEEPHOLE: %s = %s (redundant swap)\n", line_num2, lhs2, rhs2);
            }
        }
    }
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        if (strstr(imcode[i], "// PEEPHOLE") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char lhs[100], rhs[100];
        if (sscanf(line, "%d %s = %[^\n]", &line_num, lhs, rhs) == 3) {
            char* end = rhs + strlen(rhs) - 1;
            while (end > rhs && (*end == ' ' || *end == '\n')) { *end = '\0'; end--; }
            if (strchr(rhs,'+') != NULL || strchr(rhs,'-') != NULL ||
                strchr(rhs,'*') != NULL || strchr(rhs,'/') != NULL ||
                strchr(rhs,'(') != NULL) continue;
            for (int j = i + 1; j < code; j++) {
                if (strstr(imcode[j], "// DEAD") != NULL) continue;
                if (strstr(imcode[j], "// PEEPHOLE") != NULL) continue;
                char check_line[10000]; strcpy(check_line, imcode[j]);
                if (strstr(check_line, "BeginFunc") != NULL || strstr(check_line, "EndFunc") != NULL) break;
                if (strstr(check_line, "if ") != NULL || strstr(check_line, "goto ") != NULL ||
                    strstr(check_line, "Return") != NULL) break;
                char check_lhs[100], check_rhs[100];
                if (sscanf(check_line, "%*d %s = %[^\n]", check_lhs, check_rhs) == 2) {
                    char* check_end = check_rhs + strlen(check_rhs) - 1;
                    while (check_end > check_rhs && (*check_end == ' ' || *check_end == '\n')) { *check_end = '\0'; check_end--; }
                    if (strcmp(check_lhs, lhs) == 0) {
                        if (strcmp(check_rhs, rhs) == 0) {
                            int check_line_num; sscanf(imcode[j], "%d", &check_line_num);
                            sprintf(imcode[j], "%d // PEEPHOLE: %s = %s (redundant reassignment)\n", check_line_num, check_lhs, check_rhs);
                        }
                        break;
                    }
                    if (strcmp(check_lhs, rhs) == 0) break;
                }
            }
        }
    }
}

void strengthReduction() {
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char result[100], op1[100], op[10], op2[100];
        if (sscanf(line, "%d %s = %s %s %s", &line_num, result, op1, op, op2) == 5) {
            if (!isNumericConstant(op2)) {
                if (!isNumericConstant(op1)) continue;
                char temp[100]; strcpy(temp, op1); strcpy(op1, op2); strcpy(op2, temp);
            }
            int constant = atoi(op2);
            if (strcmp(op, "*") == 0 && constant > 0) {
                if ((constant & (constant - 1)) == 0) {
                    int shift = 0; int temp = constant;
                    while (temp > 1) { temp >>= 1; shift++; }
                    if (constant == 2) sprintf(imcode[i], "%d %s = %s + %s\n", line_num, result, op1, op1);
                    else sprintf(imcode[i], "%d %s = %s << %d\n", line_num, result, op1, shift);
                    continue;
                }
            }
            if (strcmp(op, "/") == 0 && constant > 0) {
                if ((constant & (constant - 1)) == 0) {
                    int shift = 0; int temp = constant;
                    while (temp > 1) { temp >>= 1; shift++; }
                    sprintf(imcode[i], "%d %s = %s >> %d\n", line_num, result, op1, shift);
                    continue;
                }
            }
        }
    }
}

void algebraicSimplification() {
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD CODE:") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char result[100], op1[100], op[10], op2[100];
        if (sscanf(line, "%d %s = %s %s %s", &line_num, result, op1, op, op2) == 5) {

            /*  Multiplication  */
            if (strcmp(op,"*")==0 && strcmp(op2,"1")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }
            if (strcmp(op,"*")==0 && strcmp(op1,"1")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op2); continue; }
            if (strcmp(op,"*")==0 && strcmp(op2,"0")==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }
            if (strcmp(op,"*")==0 && strcmp(op1,"0")==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }

            /*  Addition / Subtraction  */
            if ((strcmp(op,"+")==0||strcmp(op,"-")==0) && strcmp(op2,"0")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }
            if (strcmp(op,"+")==0 && strcmp(op1,"0")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op2); continue; }
            if (strcmp(op,"-")==0 && strcmp(op1,op2)==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }

            /*  Division  */
            if (strcmp(op,"/")==0 && strcmp(op2,"1")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }
            if (strcmp(op,"/")==0 && strcmp(op1,op2)==0){ sprintf(imcode[i],"%d %s = 1\n",line_num,result); continue; }
            if (strcmp(op,"/")==0 && strcmp(op1,"0")==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }

            /*  Modulo  */
            if (strcmp(op,"%")==0 && strcmp(op2,"1")==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }  /* x % 1 == 0 */
            if (strcmp(op,"%")==0 && strcmp(op1,"0")==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }  /* 0 % x == 0 */
            if (strcmp(op,"%")==0 && strcmp(op1,op2)==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }  /* x % x == 0 */

            /*  Bitwise AND (bitand)  */
            if (strcmp(op,"&")==0 && (strcmp(op2,"0")==0||strcmp(op1,"0")==0)){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }
            if (strcmp(op,"&")==0 && strcmp(op1,op2)==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }  /* x & x == x */

            /*  Bitwise OR (bitor)  */
            if (strcmp(op,"|")==0 && strcmp(op2,"0")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }
            if (strcmp(op,"|")==0 && strcmp(op1,"0")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op2); continue; }
            if (strcmp(op,"|")==0 && strcmp(op1,op2)==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }  /* x | x == x */

            /*  Bitwise XOR (bitxor)  */
            if (strcmp(op,"^")==0 && strcmp(op2,"0")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }
            if (strcmp(op,"^")==0 && strcmp(op1,"0")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op2); continue; }
            if (strcmp(op,"^")==0 && strcmp(op1,op2)==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }  /* x ^ x == 0 */

            /*  Left Shift (lshift → <<)  */
            if (strcmp(op,"<<")==0 && strcmp(op2,"0")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }  /* x << 0 == x */
            if (strcmp(op,"<<")==0 && strcmp(op1,"0")==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }        /* 0 << x == 0 */

            /*  Right Shift (rshift → >>)  */
            if (strcmp(op,">>")==0 && strcmp(op2,"0")==0){ sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }  /* x >> 0 == x */
            if (strcmp(op,">>")==0 && strcmp(op1,"0")==0){ sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }        /* 0 >> x == 0 */
        }
    }
}

/*
void constantFoldConditionals() {
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        
        char line[10000];
        strcpy(line, imcode[i]);
        
        int line_num;
        char cond[1000];
        if (sscanf(line, "%d if %[^\n]", &line_num, cond) == 2) {
            char op1[100], op[10], op2[100], rest[100];
            if (sscanf(cond, "%s %s %s %[^\n]", op1, op, op2, rest) == 4) {
                if (isNumericConstant(op1) && isNumericConstant(op2)) {
                    double v1 = atof(op1), v2 = atof(op2);
                    int result = 0;
                    if (strcmp(op, "<") == 0) result = (v1 < v2);
                    else if (strcmp(op, ">") == 0) result = (v1 > v2);
                    else if (strcmp(op, "<=") == 0) result = (v1 <= v2);
                    else if (strcmp(op, ">=") == 0) result = (v1 >= v2);
                    else if (strcmp(op, "==") == 0) result = (v1 == v2);
                    else if (strcmp(op, "!=") == 0) result = (v1 != v2);
                    
                    if (result) {
                        char* goto_ptr = strstr(rest, "goto");
                        if (goto_ptr) {
                            sprintf(imcode[i], "%d %s\n", line_num, goto_ptr);
                        }
                    } else {
                        sprintf(imcode[i], "%d // DEAD BRANCH: if %s %s %s always false\n", 
                                line_num, op1, op, op2);
                    }
                }
            }
        }
    }
}
*/





void constantFoldConditionals() {
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        
        char line[10000];
        strcpy(line, imcode[i]);
        
        int line_num;
        char cond[1000];
        if (sscanf(line, "%d if %[^\n]", &line_num, cond) == 2) {
            char op1[100], op[10], op2[100], rest[100];
            if (sscanf(cond, "%s %s %s %[^\n]", op1, op, op2, rest) == 4) {
                // Try to resolve operands to their constant values
                char resolved_op1[100], resolved_op2[100];
                strcpy(resolved_op1, op1);
                strcpy(resolved_op2, op2);
                int op1_is_truly_constant = 1;
                int op2_is_truly_constant = 1;
                
                // Look up op1 if it's a variable
                if (!isNumericConstant(op1)) {
                    int assignment_count = 0;
                    for (int j = 0; j < code; j++) {
                        if (strstr(imcode[j], "// DEAD") != NULL) continue;
                        char check_line[10000]; strcpy(check_line, imcode[j]);
                        char lhs[100];
                        if (sscanf(check_line, "%*d %s = %*[^\n]", lhs) == 1) {
                            if (strcmp(lhs, op1) == 0) {
                                assignment_count++;
                                if (assignment_count > 1) {
                                    op1_is_truly_constant = 0;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if (op1_is_truly_constant) {
                        for (int j = i - 1; j >= 0; j--) {
                            if (strstr(imcode[j], "// DEAD") != NULL) continue;
                            char check_line[10000]; strcpy(check_line, imcode[j]);
                            char lhs[100], rhs[100];
                            if (sscanf(check_line, "%*d %s = %[^\n]", lhs, rhs) == 2) {
                                if (strcmp(lhs, op1) == 0) {
                                    char* trimmed = rhs;
                                    while (*trimmed == ' ') trimmed++;
                                    char* end = trimmed + strlen(trimmed) - 1;
                                    while (end > trimmed && (*end == ' ' || *end == '\n')) end--;
                                    *(end + 1) = '\0';
                                    
                                    if (isNumericConstant(trimmed) && 
                                        !strchr(trimmed, '+') && !strchr(trimmed, '-') &&
                                        !strchr(trimmed, '*') && !strchr(trimmed, '/')) {
                                        strcpy(resolved_op1, trimmed);
                                    } else {
                                        op1_is_truly_constant = 0;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
                
                // Look up op2 if it's a variable
                if (!isNumericConstant(op2)) {
                    int assignment_count = 0;
                    for (int j = 0; j < code; j++) {
                        if (strstr(imcode[j], "// DEAD") != NULL) continue;
                        char check_line[10000]; strcpy(check_line, imcode[j]);
                        char lhs[100];
                        if (sscanf(check_line, "%*d %s = %*[^\n]", lhs) == 1) {
                            if (strcmp(lhs, op2) == 0) {
                                assignment_count++;
                                if (assignment_count > 1) {
                                    op2_is_truly_constant = 0;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if (op2_is_truly_constant) {
                        for (int j = i - 1; j >= 0; j--) {
                            if (strstr(imcode[j], "// DEAD") != NULL) continue;
                            char check_line[10000]; strcpy(check_line, imcode[j]);
                            char lhs[100], rhs[100];
                            if (sscanf(check_line, "%*d %s = %[^\n]", lhs, rhs) == 2) {
                                if (strcmp(lhs, op2) == 0) {
                                    char* trimmed = rhs;
                                    while (*trimmed == ' ') trimmed++;
                                    char* end = trimmed + strlen(trimmed) - 1;
                                    while (end > trimmed && (*end == ' ' || *end == '\n')) end--;
                                    *(end + 1) = '\0';
                                    
                                    if (isNumericConstant(trimmed) && 
                                        !strchr(trimmed, '+') && !strchr(trimmed, '-') &&
                                        !strchr(trimmed, '*') && !strchr(trimmed, '/')) {
                                        strcpy(resolved_op2, trimmed);
                                    } else {
                                        op2_is_truly_constant = 0;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
                
                // Only fold if both are truly constant (assigned only once with constant values)
                if (op1_is_truly_constant && op2_is_truly_constant &&
                    isNumericConstant(resolved_op1) && isNumericConstant(resolved_op2)) {
                    double v1 = atof(resolved_op1), v2 = atof(resolved_op2);
                    int result = 0;
                    if (strcmp(op, "<") == 0) result = (v1 < v2);
                    else if (strcmp(op, ">") == 0) result = (v1 > v2);
                    else if (strcmp(op, "<=") == 0) result = (v1 <= v2);
                    else if (strcmp(op, ">=") == 0) result = (v1 >= v2);
                    else if (strcmp(op, "==") == 0) result = (v1 == v2);
                    else if (strcmp(op, "!=") == 0) result = (v1 != v2);
                    
                    if (result) {
                        char* goto_ptr = strstr(rest, "goto");
                        if (goto_ptr) {
                            sprintf(imcode[i], "%d %s // FOLDED: if %s %s %s always true\n", 
                                    line_num, goto_ptr, resolved_op1, op, resolved_op2);
                        }
                    } else {
                        sprintf(imcode[i], "%d // DEAD BRANCH: if %s %s %s always false\n", 
                                line_num, resolved_op1, op, resolved_op2);
                    }
                }
            }
        }
    }
}



void copyPropagation() {
    int is_jump_target[10000] = {0};
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "goto") != NULL) {
            char* goto_ptr = strstr(imcode[i], "goto");
            char* ptr = goto_ptr + 4;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            if (isdigit(*ptr)) { int target = atoi(ptr); if (target >= 0 && target < code) is_jump_target[target] = 1; }
        }
    }
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD CODE:") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char lhs[100], rhs[100];
        if (sscanf(line, "%d %s = %[^\n]", &line_num, lhs, rhs) == 3) {
            char* end = rhs + strlen(rhs) - 1;
            while (end > rhs && (*end == ' ' || *end == '\n' || *end == '\r')) { *end = '\0'; end--; }
            if (strchr(lhs, '[') != NULL) continue;
            if (strchr(rhs,'+')!=NULL||strchr(rhs,'-')!=NULL||strchr(rhs,'*')!=NULL||strchr(rhs,'/')!=NULL||
                strchr(rhs,'%')!=NULL||strchr(rhs,'&')!=NULL||strchr(rhs,'|')!=NULL||strchr(rhs,'^')!=NULL||
                strchr(rhs,'<')!=NULL||strchr(rhs,'>')!=NULL||strchr(rhs,'~')!=NULL||strchr(rhs,'(')!=NULL||
                strstr(rhs,"Call")!=NULL||strstr(rhs,"PopParam")!=NULL||strstr(rhs,"PushParam")!=NULL) continue;
            if (is_jump_target[i]) continue;
            int func_start = i, func_end = code;
            for (int k = i; k >= 0; k--) { if (strstr(imcode[k], "BeginFunc") != NULL) { func_start = k; break; } }
            for (int k = i; k < code; k++) { if (strstr(imcode[k], "EndFunc") != NULL) { func_end = k; break; } }
            int in_loop = 0;
            for (int j = func_start; j < func_end; j++) {
                if (strstr(imcode[j], "goto") != NULL) {
                    char* goto_ptr = strstr(imcode[j], "goto");
                    char* ptr = goto_ptr + 4;
                    while (*ptr == ' ' || *ptr == '\t') ptr++;
                    if (isdigit(*ptr)) { int target = atoi(ptr); if (target <= i && j >= i && i >= target && i <= j) { in_loop = 1; break; } }
                }
            }
            int first_modification = func_end;
            for (int j = i + 1; j < func_end; j++) {
                if (strstr(imcode[j], "// DEAD") != NULL) continue;
                if (strstr(imcode[j], "EndFunc") != NULL) break;
                char check_lhs[100];
                if (sscanf(imcode[j], "%*d %s =", check_lhs) == 1) { if (strcmp(check_lhs, lhs) == 0) { first_modification = j; break; } }
            }
            int rhs_first_modification = func_end;
            if (!isNumericConstant(rhs)) {
                for (int j = i + 1; j < func_end; j++) {
                    if (strstr(imcode[j], "// DEAD") != NULL) continue;
                    if (strstr(imcode[j], "EndFunc") != NULL) break;
                    char check_lhs[100];
                    if (sscanf(imcode[j], "%*d %s =", check_lhs) == 1) { if (strcmp(check_lhs, rhs) == 0) { rhs_first_modification = j; break; } }
                }
            }
            if (in_loop) continue;
            
            // Check if there's a goto between current line and first_modification
            int has_goto_before_modification = 0;
            int goto_target = -1;
            for (int j = i + 1; j < first_modification && j < code; j++) {
                if (strstr(imcode[j], "// DEAD") != NULL) continue;
                if (strstr(imcode[j], "goto") != NULL) {
                    char* goto_ptr = strstr(imcode[j], "goto");
                    char* ptr = goto_ptr + 4;
                    while (*ptr == ' ' || *ptr == '\t') ptr++;
                    if (isdigit(*ptr)) {
                        goto_target = atoi(ptr);
                        // If goto jumps past first_modification, we need to check beyond
                        if (goto_target > first_modification || goto_target > i + 1) {
                            has_goto_before_modification = 1;
                            break;
                        }
                    }
                }
            }
            
            // If there's a goto, extend safe_limit to check beyond the goto target
            int safe_limit = (first_modification < rhs_first_modification) ? first_modification : rhs_first_modification;
            if (has_goto_before_modification && goto_target > safe_limit) {
                // Only extend safe_limit if there are no further live assignments
                // to lhs between goto_target and func_end. If there are, the goto
                // skips over one assignment but another follows — we cannot safely
                // propagate the current value all the way to a use past that point.
                int has_later_assignment = 0;
                for (int j = goto_target; j < func_end; j++) {
                    if (strstr(imcode[j], "// DEAD") != NULL) continue;
                    if (strstr(imcode[j], "EndFunc") != NULL) break;
                    char check_lhs2[100];
                    if (sscanf(imcode[j], "%*d %s =", check_lhs2) == 1) {
                        if (strcmp(check_lhs2, lhs) == 0) { has_later_assignment = 1; break; }
                    }
                }
                if (!has_later_assignment) {
                    // Safe to extend: goto skips intermediate assignment but no
                    // further writes exist, so the current value reaches the use.
                    safe_limit = (goto_target + 10 < func_end) ? goto_target + 10 : func_end;
                }
                // else: leave safe_limit at first_modification — don't propagate past it
            }
            
            int is_used = 0, last_use_line = -1;
            for (int j = i + 1; j < safe_limit && j < code; j++) {
                if (strstr(imcode[j], "// DEAD CODE:") != NULL) continue;
                char check_line[10000]; strcpy(check_line, imcode[j]);
                if (strstr(check_line, lhs) != NULL) {
                    char* pos = strstr(check_line, lhs);
                    while (pos) {
                        int is_whole = 1;
                        if (pos > check_line && (isalnum(*(pos-1)) || *(pos-1) == '_' || *(pos-1) == '\'')) is_whole = 0;
char after = *(pos + strlen(lhs));
if (isalnum(after) || after == '_' || after == '\'') is_whole = 0;

                        if (is_whole) { is_used = 1; last_use_line = j; }
                        pos = strstr(pos + 1, lhs);
                    }
                }
            }
            int is_constant = isLiteral(rhs); 
            if (is_used) {
               
                int use_inside_loop = 0;
                for (int j = func_start; j < func_end; j++) {
                    if (strstr(imcode[j], "// DEAD") != NULL) continue;
                    if (strstr(imcode[j], "goto") != NULL) {
                        char* goto_ptr = strstr(imcode[j],"goto");
                        char* ptr = goto_ptr + 4;
                        while (*ptr == ' ' || *ptr == '\t') ptr++;
                        if (isdigit(*ptr)) {
                            int target = atoi(ptr);
                            /* backward branch whose target is at or before
                               last_use_line: use sites are inside a loop */
                            if (target <= last_use_line && j > target) {
                                use_inside_loop = 1;
                                break;
                            }
                        }
                    }
                }
                /* lhs is modified somewhere in the function after its definition
                   - its pre-loop value is stale on iteration 2+ - unsafe */
                if (use_inside_loop && first_modification < func_end) continue;

                int has_use_at_jump_target = 0;
                for (int j = i + 1; j <= last_use_line && j < code; j++) {
                    if (strstr(imcode[j], "// DEAD CODE:") != NULL) continue;
                    if (is_jump_target[j]) {
                        char check_line[10000]; strcpy(check_line, imcode[j]);
                        if (strstr(check_line, lhs) != NULL) {
                            char* pos = strstr(check_line, lhs);
                            while (pos) {
                                int is_whole = 1;
                                if (pos > check_line && (isalnum(*(pos-1)) || *(pos-1) == '_' || *(pos-1) == '\'')) is_whole = 0;
char after = *(pos + strlen(lhs));
if (isalnum(after) || after == '_' || after == '\'') is_whole = 0;

                                if (is_whole) { has_use_at_jump_target = 1; break; }
                                pos = strstr(pos + 1, lhs);
                            }
                        }
                    }
                    if (has_use_at_jump_target) break;
                }
                if (has_use_at_jump_target) continue;
                int used_in_subscript = 0;
                for (int j = i + 1; j <= last_use_line && j < code; j++) {
                    if (strstr(imcode[j], "// DEAD CODE:") != NULL) continue;
                    char check_line[10000]; strcpy(check_line, imcode[j]);
                    char* bracket_start = strchr(check_line, '[');
                    while (bracket_start) {
                        char* bracket_end = strchr(bracket_start, ']');
                        if (bracket_end) {
                            *bracket_end = '\0';
                            if (strstr(bracket_start, lhs) != NULL) { used_in_subscript = 1; *bracket_end = ']'; break; }
                            *bracket_end = ']';
                            bracket_start = strchr(bracket_end + 1, '[');
                        } else break;
                    }
                    if (used_in_subscript) break;
                }
                if (used_in_subscript && lhs[0] == 't' && isdigit(lhs[1]) && !is_constant) continue;
                int can_propagate = 0;
                if (lhs[0] == 't' && isdigit(lhs[1])) can_propagate = 1;
                else if (is_constant && !used_in_subscript) can_propagate = 1;
                if (!can_propagate) continue;
                /* Safety check for user variables (non-temps):
                   If any goto in the function can SKIP OVER line i
                   (goto source < i, goto target > i) then line i is inside
                   a conditional branch and may not execute — unsafe to propagate. */
                if (!(lhs[0] == 't' && isdigit(lhs[1]))) {
                    int inside_branch = 0;
                    for (int k = 0; k < func_end; k++) {
                        if (strstr(imcode[k], "// DEAD") != NULL) continue;
                        if (strstr(imcode[k], "goto") != NULL) {
                            char* goto_ptr = strstr(imcode[k], "goto");
                            char* ptr = goto_ptr + 4;
                            while (*ptr == ' ' || *ptr == '\t') ptr++;
                            if (isdigit(*ptr)) {
                                int gtarget = atoi(ptr);
                                /* A goto from before i that jumps past i means
                                   line i can be bypassed → it's in a branch */
                                if (k < i && gtarget > i) {
                                    inside_branch = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if (inside_branch) continue;
                }
                for (int j = i + 1; j <= last_use_line && j < code && j < safe_limit; j++) {
                    if (strstr(imcode[j], "// DEAD CODE:") != NULL) continue;
                    char new_line[10000]; strcpy(new_line, imcode[j]);
                    char* equals_sign = strchr(new_line, '=');
                    if (equals_sign == NULL) {
                        char* line_num_end = strchr(new_line, ' ');
                        char* search_start = line_num_end ? line_num_end + 1 : new_line;
                        char* pos = strstr(search_start, lhs);
                        while (pos != NULL) {
                            int is_whole_word = 1;
                             if (pos > search_start && (isalnum(*(pos-1)) || *(pos-1) == '_' || *(pos-1) == '\'')) is_whole_word = 0;
char after = *(pos + strlen(lhs));
if (isalnum(after) || after == '_' || after == '\'') is_whole_word = 0;
                            if (is_whole_word) {
                                char temp[10000]; *pos = '\0';
                                sprintf(temp, "%s%s%s", new_line, rhs, pos + strlen(lhs));
                                strcpy(new_line, temp); strcpy(imcode[j], new_line);
                                line_num_end = strchr(new_line, ' ');
                                search_start = line_num_end ? line_num_end + 1 : new_line;
                                pos = strstr(search_start, lhs);
                            } else pos = strstr(pos + 1, lhs);
                        }
                    } else {
                        char* line_num_end = strchr(new_line, ' ');
                        char* lhs_start = line_num_end ? line_num_end + 1 : new_line;
                        char* bracket_in_lhs = strchr(lhs_start, '[');
                        if (bracket_in_lhs && bracket_in_lhs < equals_sign) {
                            char* bracket_end = strchr(bracket_in_lhs, ']');
                            if (bracket_end && bracket_end < equals_sign) {
                                char* pos = strstr(bracket_in_lhs, lhs);
                                while (pos && pos < bracket_end) {
                                    int is_whole_word = 1;
                                    if (pos > bracket_in_lhs && (isalnum(*(pos-1)) || *(pos-1) == '_' || *(pos-1) == '\'')) is_whole_word = 0;
char after = *(pos + strlen(lhs));
if (isalnum(after) || after == '_' || after == '\'') is_whole_word = 0;
                                    if (is_whole_word) {
                                        char temp[10000]; *pos = '\0';
                                        sprintf(temp, "%s%s%s", new_line, rhs, pos + strlen(lhs));
                                        strcpy(new_line, temp); strcpy(imcode[j], new_line);
                                        equals_sign = strchr(new_line, '=');
                                        bracket_in_lhs = strchr(new_line, '[');
                                        bracket_end = strchr(bracket_in_lhs, ']');
                                        pos = strstr(bracket_in_lhs, lhs);
                                    } else { pos = strstr(pos + 1, lhs); if (pos >= bracket_end) break; }
                                }
                            }
                        }
                        char* rhs_start = equals_sign + 1;
                        char* pos = strstr(rhs_start, lhs);
                        while (pos != NULL) {
                            int is_whole_word = 1;
                            if (pos > rhs_start && (isalnum(*(pos-1)) || *(pos-1) == '_' || *(pos-1) == '\'')) is_whole_word = 0;
char after = *(pos + strlen(lhs));
if (isalnum(after) || after == '_' || after == '\'') is_whole_word = 0;

                            if (is_whole_word) {
                                char temp[10000]; *pos = '\0';
                                sprintf(temp, "%s%s%s", new_line, rhs, pos + strlen(lhs));
                                strcpy(new_line, temp); strcpy(imcode[j], new_line);
                                equals_sign = strchr(new_line, '=');
                                rhs_start = equals_sign + 1;
                                pos = strstr(rhs_start, lhs);
                            } else pos = strstr(pos + 1, lhs);
                        }
                    }
               /* }
                sprintf(imcode[i], "%d // DEAD COPY: %s = %s\n", line_num, lhs, rhs);
            }
            if (!is_used && is_constant) sprintf(imcode[i], "%d // DEAD CONST: %s = %s\n", line_num, lhs, rhs);
            */                }
                // Don't mark as DEAD COPY - keep the source definition
            } else if (!is_used && is_constant) {
                /* Before marking as DEAD CONST, do a full scan of the entire
                   function to make sure lhs is truly never READ anywhere after
                   this point. safe_limit may have cut the search short (stopped
                   at first_modification), missing a printint/return further down. */
                int globally_used = 0;
                for (int j = i + 1; j < func_end; j++) {
                    if (strstr(imcode[j], "// DEAD CODE:") != NULL) continue;
                    char check_line[10000]; strcpy(check_line, imcode[j]);
                    /* Only count lhs as "used" when it appears on the READ side.
                       For an assignment "N lhs = ...", lhs on the LHS is a write,
                       not a read — skip those. We check reads by looking at:
                         - print/if/goto lines (no '=' before lhs)
                         - the RHS of assignment lines (after the '=')           */
                    char* eq = strchr(check_line, '=');
                    char* search_start;
                    if (eq == NULL) {
                        /* No '=' → whole line is a read context (printint, goto, if) */
                        search_start = check_line;
                    } else {
                        /* Has '=' → only scan the RHS (after '=') for reads */
                        search_start = eq + 1;
                    }
                    char* pos = strstr(search_start, lhs);
                    while (pos) {
                        int is_whole = 1;
                        if (pos > check_line &&
                            (isalnum(*(pos-1)) || *(pos-1) == '_' || *(pos-1) == '\''))
                            is_whole = 0;
                        char after2 = *(pos + strlen(lhs));
                        if (isalnum(after2) || after2 == '_' || after2 == '\'')
                            is_whole = 0;
                        if (is_whole) { globally_used = 1; break; }
                        pos = strstr(pos + 1, lhs);
                    }
                    if (globally_used) break;
                }
                if (!globally_used)
                    sprintf(imcode[i], "%d // DEAD CONST: %s = %s\n", line_num, lhs, rhs);
                /* else: keep the assignment live so the variable is initialised */
            }
        }      
    }
}



void booleanSimplification() {
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;

        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char result[100], op1[100], op[10], op2[100];

        if (sscanf(line, "%d %s = %s %s %s", &line_num, result, op1, op, op2) == 5) {

            /* ── && (logical AND) ─────────────────────────────────────── */

            /* true && x  →  x */
            if (strcmp(op,"&&")==0 && strcmp(op1,"1")==0){
                sprintf(imcode[i],"%d %s = %s\n",line_num,result,op2); continue; }
            /* x && true  →  x */
            if (strcmp(op,"&&")==0 && strcmp(op2,"1")==0){
                sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }
            /* false && x  →  0 */
            if (strcmp(op,"&&")==0 && strcmp(op1,"0")==0){
                sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }
            /* x && false  →  0 */
            if (strcmp(op,"&&")==0 && strcmp(op2,"0")==0){
                sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }
            /* x && x  →  x */
            if (strcmp(op,"&&")==0 && strcmp(op1,op2)==0){
                sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }

            /* ── || (logical OR) ─────────────────────────────────────── */

            /* false || x  →  x */
            if (strcmp(op,"||")==0 && strcmp(op1,"0")==0){
                sprintf(imcode[i],"%d %s = %s\n",line_num,result,op2); continue; }
            /* x || false  →  x */
            if (strcmp(op,"||")==0 && strcmp(op2,"0")==0){
                sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }
            /* true || x  →  1 */
            if (strcmp(op,"||")==0 && strcmp(op1,"1")==0){
                sprintf(imcode[i],"%d %s = 1\n",line_num,result); continue; }
            /* x || true  →  1 */
            if (strcmp(op,"||")==0 && strcmp(op2,"1")==0){
                sprintf(imcode[i],"%d %s = 1\n",line_num,result); continue; }
            /* x || x  →  x */
            if (strcmp(op,"||")==0 && strcmp(op1,op2)==0){
                sprintf(imcode[i],"%d %s = %s\n",line_num,result,op1); continue; }

            /* ── == (equality) ───────────────────────────────────────── */

            /* x == x  →  1 */
            if (strcmp(op,"==")==0 && strcmp(op1,op2)==0){
                sprintf(imcode[i],"%d %s = 1\n",line_num,result); continue; }
            /* two different numeric constants: fold at compile time */
            if (strcmp(op,"==")==0 && isNumericConstant(op1) && isNumericConstant(op2)){
                int r = (atof(op1) == atof(op2));
                sprintf(imcode[i],"%d %s = %d\n",line_num,result,r); continue; }

            /* ── != (not-equal) ──────────────────────────────────────── */

            /* x != x  →  0 */
            if (strcmp(op,"!=")==0 && strcmp(op1,op2)==0){
                sprintf(imcode[i],"%d %s = 0\n",line_num,result); continue; }
            /* two different numeric constants: fold at compile time */
            if (strcmp(op,"!=")==0 && isNumericConstant(op1) && isNumericConstant(op2)){
                int r = (atof(op1) != atof(op2));
                sprintf(imcode[i],"%d %s = %d\n",line_num,result,r); continue; }
        }
    }
}




void conservativeJumpChaining() {
    // Pass 1: Count how many live gotos point to each line number.
    int ref_count[10000] = {0};
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        char* p = imcode[i];
        char* gp;
        while ((gp = strstr(p, "goto")) != NULL) {
            char* tp = gp + 4;
            while (*tp == ' ' || *tp == '\t') tp++;
            if (isdigit(*tp)) {
                int t = atoi(tp);
                if (t >= 0 && t < code) ref_count[t]++;
            }
            p = gp + 1;
        }
    }

    // Pass 2: For each unconditional goto, follow the chain when safe.
    // We follow through line T only when:
    //   (a) T is a live, pure unconditional goto (no "if" before "goto"), AND
    //   (b) ref_count[T] == 1, meaning only the current jumper reaches T,
    //       so we can redirect past it without affecting other paths.
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;

        char line[10000];
        strcpy(line, imcode[i]);

        // Must be an unconditional goto: "N goto M"
        char* if_ptr   = strstr(line, "if");
        char* goto_ptr = strstr(line, "goto");
        if (!goto_ptr) continue;
        if (if_ptr && if_ptr < goto_ptr) continue;  // conditional — leave alone

        int line_num, target;
        if (sscanf(line, "%d goto %d", &line_num, &target) != 2) continue;

        int final_target = target;
        int steps = 0;

        while (steps++ < 1000) {
            if (final_target < 0 || final_target >= code) break;
            if (strstr(imcode[final_target], "// DEAD") != NULL) break;

            // Only safe to skip through if exactly one jumper reaches it
            if (ref_count[final_target] != 1) break;

            char mid[10000];
            strcpy(mid, imcode[final_target]);

            char* mid_if   = strstr(mid, "if");
            char* mid_goto = strstr(mid, "goto");
            if (!mid_goto) break;
            if (mid_if && mid_if < mid_goto) break;  // conditional

            int mid_num, next_target;
            if (sscanf(mid, "%d goto %d", &mid_num, &next_target) != 2) break;

            // Redirect: the intermediate line loses our reference, next gains it
            ref_count[final_target]--;
            ref_count[next_target]++;
            final_target = next_target;
        }

        if (final_target != target) {
            sprintf(imcode[i], "%d goto %d\n", line_num, final_target);
        }
    }
}


void constantFolding() {
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char result[100], op1_str[100], op[10], op2_str[100];
        if (sscanf(line, "%d %s = %s %s %s", &line_num, result, op1_str, op, op2_str) == 5) {
            char resolved_op1[100], resolved_op2[100];
            strcpy(resolved_op1, op1_str); strcpy(resolved_op2, op2_str);
            int in_loop = 0;
            int func_start = i, func_end = code;
            for (int k = i; k >= 0; k--) { if (strstr(imcode[k], "BeginFunc") != NULL) { func_start = k; break; } }
            for (int k = i; k < code; k++) { if (strstr(imcode[k], "EndFunc") != NULL) { func_end = k; break; } }
            for (int k = func_start; k < func_end; k++) {
                if (strstr(imcode[k], "goto") != NULL) {
                    char* goto_ptr = strstr(imcode[k], "goto");
                    char* ptr = goto_ptr + 4;
                    while (*ptr == ' ' || *ptr == '\t') ptr++;
                    if (isdigit(*ptr)) { int target = atoi(ptr); if (target <= i && k >= i && i >= target && i <= k) { in_loop = 1; break; } }
                }
            }
            if (!isNumericConstant(op1_str)) {
                for (int j = i - 1; j >= 0; j--) {
                    if (strstr(imcode[j], "// DEAD") != NULL) continue;
                    if (strstr(imcode[j], "BeginFunc") != NULL) break;
                    char check_line[10000]; strcpy(check_line, imcode[j]);
                    char check_lhs[100], check_rhs[100];
                    if (sscanf(check_line, "%*d %s = %[^\n]", check_lhs, check_rhs) == 2) {
                        char* end = check_rhs + strlen(check_rhs) - 1;
                        while (end > check_rhs && (*end == ' ' || *end == '\n')) { *end = '\0'; end--; }
                        if (strcmp(check_lhs, op1_str) == 0) {
                            int is_modified = 0;
                            for (int k = j + 1; k < i; k++) {
                                if (strstr(imcode[k], "// DEAD") != NULL) continue;
                                char mod_check[100];
                                if (sscanf(imcode[k], "%*d %s =", mod_check) == 1) { if (strcmp(mod_check, op1_str) == 0) { is_modified = 1; break; } }
                            }
                            if (!is_modified && !in_loop && isNumericConstant(check_rhs) &&
                                strchr(check_rhs,'+')==NULL && strchr(check_rhs,'-')==NULL &&
                                strchr(check_rhs,'*')==NULL && strchr(check_rhs,'/')==NULL)
                                strcpy(resolved_op1, check_rhs);
                            break;
                        }
                    }
                }
            }
            if (!isNumericConstant(op2_str)) {
                for (int j = i - 1; j >= 0; j--) {
                    if (strstr(imcode[j], "// DEAD") != NULL) continue;
                    if (strstr(imcode[j], "BeginFunc") != NULL) break;
                    char check_line[10000]; strcpy(check_line, imcode[j]);
                    char check_lhs[100], check_rhs[100];
                    if (sscanf(check_line, "%*d %s = %[^\n]", check_lhs, check_rhs) == 2) {
                        char* end = check_rhs + strlen(check_rhs) - 1;
                        while (end > check_rhs && (*end == ' ' || *end == '\n')) { *end = '\0'; end--; }
                        if (strcmp(check_lhs, op2_str) == 0) {
                            int is_modified = 0;
                            for (int k = j + 1; k < i; k++) {
                                if (strstr(imcode[k], "// DEAD") != NULL) continue;
                                char mod_check[100];
                                if (sscanf(imcode[k], "%*d %s =", mod_check) == 1) { if (strcmp(mod_check, op2_str) == 0) { is_modified = 1; break; } }
                            }
                            if (!is_modified && !in_loop && isNumericConstant(check_rhs) &&
                                strchr(check_rhs,'+')==NULL && strchr(check_rhs,'-')==NULL &&
                                strchr(check_rhs,'*')==NULL && strchr(check_rhs,'/')==NULL)
                                strcpy(resolved_op2, check_rhs);
                            break;
                        }
                    }
                }
            }
            if (isNumericConstant(resolved_op1) && isNumericConstant(resolved_op2)) {
                double val1 = atof(resolved_op1), val2 = atof(resolved_op2), res = 0;
                int is_valid = 1;
                int is_float = (strchr(resolved_op1,'.')!=NULL || strchr(resolved_op2,'.')!=NULL);
                if (strcmp(op,"+")==0) res = val1+val2;
                else if (strcmp(op,"-")==0) res = val1-val2;
                else if (strcmp(op,"*")==0) res = val1*val2;
                else if (strcmp(op,"/")==0) { if (val2==0) is_valid=0; else { res=val1/val2;  } }
                else if (strcmp(op,"%")==0) { if (val2==0||is_float) is_valid=0; else res=(int)val1%(int)val2; }
                else if (strcmp(op,"&")==0) res=(int)val1&(int)val2;
                else if (strcmp(op,"|")==0) res=(int)val1|(int)val2;
                else if (strcmp(op,"^")==0) res=(int)val1^(int)val2;
                else if (strcmp(op,"<<")==0) res=(int)val1<<(int)val2;
                else if (strcmp(op,">>")==0) res=(int)val1>>(int)val2;
                else is_valid=0;
                if (is_valid) {
                    if (is_float) sprintf(imcode[i], "%d %s = %g\n", line_num, result, res);
                    else sprintf(imcode[i], "%d %s = %d\n", line_num, result, (int)res);
                }
            }
        }
    }
}

void deadVariableElimination() {
    int used[10000] = {0};
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        if (strstr(line,"print")!=NULL||strstr(line,"Return")!=NULL||strstr(line,"PushParam")!=NULL||
            strstr(line,"Call")!=NULL||strstr(line,"if")!=NULL) {
            char* ptr = line;
            while (*ptr) {
                if (isalpha(*ptr) || *ptr == '_') {
                    char var[100]; int idx = 0;
                    while (isalnum(*ptr) || *ptr == '_') var[idx++] = *ptr++;
                    var[idx] = '\0';
                    if (strcmp(var,"if")==0||strcmp(var,"goto")==0||strcmp(var,"print")==0||
                        strcmp(var,"printint")==0||strcmp(var,"printfloat")==0||strcmp(var,"printchar")==0||
                        strcmp(var,"printstring")==0||strcmp(var,"Return")==0||strcmp(var,"Call")==0||
                        strcmp(var,"PushParam")==0||strcmp(var,"PopParam")==0||strcmp(var,"BeginFunc")==0||
                        strcmp(var,"EndFunc")==0) continue;
                    for (int j = 0; j < code; j++) {
                        if (strstr(imcode[j], "// DEAD") != NULL) continue;
                        char check_line[10000]; strcpy(check_line, imcode[j]);
                        char lhs[100];
                        if (sscanf(check_line, "%*d %s =", lhs) == 1) { if (strcmp(lhs, var) == 0) used[j] = 1; }
                    }
                } else ptr++;
            }
        }
        char lhs_full[100];
        if (sscanf(line, "%*d %[^=]", lhs_full) == 1) {
            char* bracket_start = strchr(lhs_full, '[');
            if (bracket_start) {
                char* bracket_end = strchr(bracket_start, ']');
                if (bracket_end) {
                    char* ptr = bracket_start + 1;
                    while (ptr < bracket_end) {
                        if (isalpha(*ptr) || *ptr == '_') {
                            char var[100]; int idx = 0;
                            while ((isalnum(*ptr) || *ptr == '_') && ptr < bracket_end) var[idx++] = *ptr++;
                            var[idx] = '\0';
                            for (int j = 0; j < code; j++) {
                                if (strstr(imcode[j], "// DEAD") != NULL) continue;
                                char check_line[10000]; strcpy(check_line, imcode[j]);
                                char check_lhs[100];
                                if (sscanf(check_line, "%*d %s =", check_lhs) == 1) { if (strcmp(check_lhs, var) == 0) used[j] = 1; }
                            }
                        } else ptr++;
                    }
                }
            }
        }
        char lhs[100];
        if (sscanf(line, "%*d %s =", lhs) == 1) {
            char* equals = strchr(line, '=');
            if (equals) {
                char* rhs = equals + 1;
                char* ptr = rhs;
                while (*ptr) {
                    if (isalpha(*ptr) || *ptr == '_') {
                        char var[100]; int idx = 0;
                        while (isalnum(*ptr) || *ptr == '_') var[idx++] = *ptr++;
                        var[idx] = '\0';
                        if (strcmp(var,"Call")==0||strcmp(var,"PopParam")==0||strcmp(var,"int")==0||
                            strcmp(var,"float")==0||strcmp(var,"char")==0||strcmp(var,"double")==0||
                            strcmp(var,"long")==0||strcmp(var,"short")==0) continue;
                        for (int j = 0; j < code; j++) {
                            if (strstr(imcode[j], "// DEAD") != NULL) continue;
                            char check_line[10000]; strcpy(check_line, imcode[j]);
                            char check_lhs[100];
                            if (sscanf(check_line, "%*d %s =", check_lhs) == 1) { if (strcmp(check_lhs, var) == 0) used[j] = 1; }
                        }
                    } else ptr++;
                }
            }
        }
    }
    for (int i = 0; i < code; i++) {
        if (used[i] == 0 && strstr(imcode[i], "// DEAD") == NULL) {
            char line[10000]; strcpy(line, imcode[i]);
            char lhs[100];
            if (sscanf(line, "%*d %s =", lhs) == 1) {
                if (strstr(line,"BeginFunc")!=NULL||strstr(line,"EndFunc")!=NULL||
                    strstr(line,"PopParam")!=NULL||strstr(line,"Call")!=NULL) continue;
                if (lhs[0] == 't' && isdigit(lhs[1])) {
                    char* content = strchr(line, ' ');
                    if (content) { content++; sprintf(imcode[i], "%d // DEAD VAR: %s", i, content); }
                }
            }
        }
    }
}



void eliminateDeadCode() {
    // BUILD JUMP TARGET TABLE FIRST (moved to top) 
    int is_jump_target[10000] = {0};
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD CODE:") != NULL) continue;
        if (strstr(imcode[i], "goto") != NULL) {
            char* goto_ptr = strstr(imcode[i], "goto");
            char* ptr = goto_ptr + 4;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            if (isdigit(*ptr)) { int target = atoi(ptr); if (target >= 0 && target < code) is_jump_target[target] = 1; }
        }
    }
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD CODE:") != NULL) continue;
        int is_unconditional = 0;
        if (strstr(imcode[i], "Return") != NULL) is_unconditional = 1;
        else if (strstr(imcode[i], "goto") != NULL) {
            char* if_ptr = strstr(imcode[i], "if");
            char* goto_ptr = strstr(imcode[i], "goto");
            if (goto_ptr && (!if_ptr || if_ptr > goto_ptr)) is_unconditional = 1;
        }
        if (is_unconditional) {
            int j = i + 1;
            if (j < code && strstr(imcode[j],"BeginFunc")==NULL && strstr(imcode[j],"EndFunc")==NULL &&
                strstr(imcode[j],"// DEAD CODE:")==NULL) {
                int is_live_target = is_jump_target[j];  // USE PREBUILT TABLE 
                if (!is_live_target) {
                    char original[10000]; strcpy(original, imcode[j]);
                    char* space_ptr = strchr(original, ' ');
                    if (space_ptr != NULL) { space_ptr++; sprintf(imcode[j], "%d // DEAD CODE: %s", j, space_ptr); }
                }
            }
        }
    }
    for (int i = 0; i < code; i++) {
        char* line = imcode[i];
        if (strstr(line, "// DEAD CODE:") != NULL) continue;
        int is_unconditional_jump = 0;
        if (strstr(line, "goto") != NULL) {
            char* if_ptr = strstr(line, "if");
            char* goto_ptr = strstr(line, "goto");
            if (goto_ptr && (!if_ptr || if_ptr > goto_ptr)) is_unconditional_jump = 1;
        } else if (strstr(line, "Return") != NULL) is_unconditional_jump = 1;
        if (is_unconditional_jump) {
            int j = i + 1;
            while (j < code) {
                if (strstr(imcode[j],"BeginFunc")!=NULL||strstr(imcode[j],"EndFunc")!=NULL) break;
                if (is_jump_target[j]) break;
                if (strstr(imcode[j],"// DEAD CODE:")!=NULL) { j++; continue; }
                char original[10000]; strcpy(original, imcode[j]);
                char* space_ptr = strchr(original, ' ');
                if (space_ptr != NULL) { space_ptr++; sprintf(imcode[j], "%d // DEAD CODE: %s", j, space_ptr); }
                j++;
            }
        }
    }
    int has_live_pred[10000] = {0};
    for (int i = 0; i < code; i++) { if (strstr(imcode[i],"BeginFunc")!=NULL && i+1<code) has_live_pred[i+1]=1; }
    has_live_pred[0] = 1;
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i],"// DEAD CODE:")!=NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        if (strstr(line,"goto")!=NULL) {
            char* goto_ptr = strstr(line,"goto");
            char* ptr = goto_ptr+4;
            while (*ptr==' '||*ptr=='\t') ptr++;
            if (isdigit(*ptr)) { int target=atoi(ptr); if (target>=0&&target<code) has_live_pred[target]=1; }
        }
        int is_uncond = 0;
        if (strstr(line,"Return")!=NULL) is_uncond=1;
        else if (strstr(line,"goto")!=NULL) {
            char* if_ptr=strstr(line,"if"); char* goto_ptr=strstr(line,"goto");
            if (goto_ptr&&(!if_ptr||if_ptr>goto_ptr)) is_uncond=1;
        }
        if (!is_uncond && i+1<code) has_live_pred[i+1]=1;
    }
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i],"// DEAD CODE:")!=NULL) continue;
        if (strstr(imcode[i],"BeginFunc")!=NULL||strstr(imcode[i],"EndFunc")!=NULL) continue;
        if (!has_live_pred[i]) {
            char original[10000]; strcpy(original, imcode[i]);
            char* space_ptr = strchr(original, ' ');
            if (space_ptr!=NULL) { space_ptr++; sprintf(imcode[i], "%d // DEAD CODE: %s", i, space_ptr); }
        }
    }
}


void commonSubexpressionElimination() {
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD CODE:") != NULL) continue;
        char line[10000]; strcpy(line, imcode[i]);
        int line_num; char result[100], op1[100], op[10], op2[100];
        if (sscanf(line, "%d %s = %s %s %s", &line_num, result, op1, op, op2) == 5) {
            int func_start = i, func_end = code;
            for (int k = i; k >= 0; k--) { if (strstr(imcode[k],"BeginFunc")!=NULL) { func_start=k; break; } }
            for (int k = i; k < code; k++) { if (strstr(imcode[k],"EndFunc")!=NULL) { func_end=k; break; } }
            int loop_start = -1, loop_end = -1, min_loop_size = func_end;
            for (int k = i; k < func_end; k++) {
                if (strstr(imcode[k],"goto")!=NULL) {
                    char* goto_ptr=strstr(imcode[k],"goto");
                    char* if_ptr=strstr(imcode[k],"if");
                    if (if_ptr && if_ptr < goto_ptr) continue;
                    char* ptr = goto_ptr+4;
                    while (*ptr==' '||*ptr=='\t') ptr++;
                    if (isdigit(*ptr)) {
                        int target=atoi(ptr);
                        if (target<=i && k>=i) { int sz=k-target; if (sz<min_loop_size) { loop_start=target; loop_end=k; min_loop_size=sz; } }
                    }
                }
            }
            int search_start = i+1, search_end = (loop_start!=-1) ? loop_end : func_end;
            char resolved_op1[100], resolved_op2[100];
            strcpy(resolved_op1, op1); strcpy(resolved_op2, op2);
            for (int k = i-1; k >= func_start; k--) {
                if (strstr(imcode[k],"// DEAD")!=NULL) continue;
                if (strstr(imcode[k],"BeginFunc")!=NULL) break;
                char check_lhs[100], check_rhs[100];
                if (sscanf(imcode[k], "%*d %s = %[^\n]", check_lhs, check_rhs) == 2) {
                    char* end = check_rhs+strlen(check_rhs)-1;
                    while (end>check_rhs && (*end==' '||*end=='\n')) { *end='\0'; end--; }
                    if (strchr(check_rhs,'+')==NULL&&strchr(check_rhs,'-')==NULL&&strchr(check_rhs,'*')==NULL&&strchr(check_rhs,'/')==NULL&&strchr(check_rhs,'(')==NULL) {
                        if (strcmp(check_lhs,op1)==0) strcpy(resolved_op1, check_rhs);
                        if (strcmp(check_lhs,op2)==0) strcpy(resolved_op2, check_rhs);
                    }
                    if (strcmp(resolved_op1,op1)!=0 && strcmp(resolved_op2,op2)!=0) break;
                }
            }
            for (int j = search_start; j < search_end; j++) {
                if (strstr(imcode[j],"// DEAD CODE:")!=NULL) continue;
                char check_line[10000]; strcpy(check_line, imcode[j]);
                if (strstr(check_line,"if ")!=NULL||strstr(check_line,"goto ")!=NULL||strstr(check_line,"Return")!=NULL||
                    strstr(check_line,"BeginFunc")!=NULL||strstr(check_line,"EndFunc")!=NULL||
                    strstr(check_line,"PopParam")!=NULL||strstr(check_line,"PushParam")!=NULL||strstr(check_line,"Call")!=NULL) continue;
                char redefined_var[100];
                if (sscanf(check_line, "%*d %s =", redefined_var) == 1) {
                    if (strcmp(redefined_var,resolved_op1)==0||strcmp(redefined_var,resolved_op2)==0) break;
                }
                int check_line_num; char check_result[100], check_op1[100], check_op[10], check_op2[100];
                if (sscanf(check_line, "%d %s = %s %s %s", &check_line_num, check_result, check_op1, check_op, check_op2) == 5) {
                    char resolved_check_op1[100], resolved_check_op2[100];
                    strcpy(resolved_check_op1, check_op1); strcpy(resolved_check_op2, check_op2);
                    for (int k = j-1; k >= func_start; k--) {
                        if (strstr(imcode[k],"// DEAD")!=NULL) continue;
                        if (strstr(imcode[k],"BeginFunc")!=NULL) break;
                        char k_lhs[100], k_rhs[100];
                        if (sscanf(imcode[k], "%*d %s = %[^\n]", k_lhs, k_rhs) == 2) {
                            char* end = k_rhs+strlen(k_rhs)-1;
                            while (end>k_rhs && (*end==' '||*end=='\n')) { *end='\0'; end--; }
                            if (strchr(k_rhs,'+')==NULL&&strchr(k_rhs,'-')==NULL&&strchr(k_rhs,'*')==NULL&&strchr(k_rhs,'/')==NULL&&strchr(k_rhs,'(')==NULL) {
                                if (strcmp(k_lhs,check_op1)==0) strcpy(resolved_check_op1, k_rhs);
                                if (strcmp(k_lhs,check_op2)==0) strcpy(resolved_check_op2, k_rhs);
                            }
                            if (strcmp(resolved_check_op1,check_op1)!=0 && strcmp(resolved_check_op2,check_op2)!=0) break;
                        }
                    }
                    if (strcmp(op,check_op)==0 && strcmp(resolved_op1,resolved_check_op1)==0 && strcmp(resolved_op2,resolved_check_op2)==0)
                        sprintf(imcode[j], "%d %s = %s\n", check_line_num, check_result, result);
                }
            }
        }
    }
}


#line 2367 "y.tab.c"

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
#line 2298 "parser.y"

        char str[1000];
        struct BoolNode* b;
        struct Expr *expr;
        int addr;
        struct Type* type;
        struct Decl* decl;
        struct Subscript* sub;

#line 2552 "y.tab.c"

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
  YYSYMBOL_3_ = 3,                         /* '?'  */
  YYSYMBOL_4_ = 4,                         /* ':'  */
  YYSYMBOL_5_ = 5,                         /* '!'  */
  YYSYMBOL_6_ = 6,                         /* '+'  */
  YYSYMBOL_7_ = 7,                         /* '-'  */
  YYSYMBOL_8_ = 8,                         /* '/'  */
  YYSYMBOL_9_ = 9,                         /* '*'  */
  YYSYMBOL_10_ = 10,                       /* '%'  */
  YYSYMBOL_11_ = 11,                       /* '('  */
  YYSYMBOL_12_ = 12,                       /* ')'  */
  YYSYMBOL_UMINUS = 13,                    /* UMINUS  */
  YYSYMBOL_14_ = 14,                       /* '$'  */
  YYSYMBOL_BREAK = 15,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 16,                  /* CONTINUE  */
  YYSYMBOL_FOR = 17,                       /* FOR  */
  YYSYMBOL_DO = 18,                        /* DO  */
  YYSYMBOL_IDEN = 19,                      /* IDEN  */
  YYSYMBOL_NUM = 20,                       /* NUM  */
  YYSYMBOL_PASN = 21,                      /* PASN  */
  YYSYMBOL_MASN = 22,                      /* MASN  */
  YYSYMBOL_DASN = 23,                      /* DASN  */
  YYSYMBOL_SASN = 24,                      /* SASN  */
  YYSYMBOL_INC = 25,                       /* INC  */
  YYSYMBOL_DEC = 26,                       /* DEC  */
  YYSYMBOL_LT = 27,                        /* LT  */
  YYSYMBOL_GT = 28,                        /* GT  */
  YYSYMBOL_LE = 29,                        /* LE  */
  YYSYMBOL_GE = 30,                        /* GE  */
  YYSYMBOL_NE = 31,                        /* NE  */
  YYSYMBOL_OR = 32,                        /* OR  */
  YYSYMBOL_AND = 33,                       /* AND  */
  YYSYMBOL_EQ = 34,                        /* EQ  */
  YYSYMBOL_IF = 35,                        /* IF  */
  YYSYMBOL_ELSE = 36,                      /* ELSE  */
  YYSYMBOL_TR = 37,                        /* TR  */
  YYSYMBOL_FL = 38,                        /* FL  */
  YYSYMBOL_WHILE = 39,                     /* WHILE  */
  YYSYMBOL_INT = 40,                       /* INT  */
  YYSYMBOL_FLOAT = 41,                     /* FLOAT  */
  YYSYMBOL_CHAR = 42,                      /* CHAR  */
  YYSYMBOL_CHARR = 43,                     /* CHARR  */
  YYSYMBOL_MEOF = 44,                      /* MEOF  */
  YYSYMBOL_CONST = 45,                     /* CONST  */
  YYSYMBOL_SWITCH = 46,                    /* SWITCH  */
  YYSYMBOL_CASE = 47,                      /* CASE  */
  YYSYMBOL_DEFAULT = 48,                   /* DEFAULT  */
  YYSYMBOL_GOTO = 49,                      /* GOTO  */
  YYSYMBOL_STRING = 50,                    /* STRING  */
  YYSYMBOL_BOOL = 51,                      /* BOOL  */
  YYSYMBOL_PRINT = 52,                     /* PRINT  */
  YYSYMBOL_INPUT = 53,                     /* INPUT  */
  YYSYMBOL_SIZEOF = 54,                    /* SIZEOF  */
  YYSYMBOL_BANDASN = 55,                   /* BANDASN  */
  YYSYMBOL_BORASN = 56,                    /* BORASN  */
  YYSYMBOL_BXORASN = 57,                   /* BXORASN  */
  YYSYMBOL_LSHIFTASN = 58,                 /* LSHIFTASN  */
  YYSYMBOL_RSHIFTASN = 59,                 /* RSHIFTASN  */
  YYSYMBOL_SHORT = 60,                     /* SHORT  */
  YYSYMBOL_LONG = 61,                      /* LONG  */
  YYSYMBOL_DOUBLE = 62,                    /* DOUBLE  */
  YYSYMBOL_VOID = 63,                      /* VOID  */
  YYSYMBOL_BAND = 64,                      /* BAND  */
  YYSYMBOL_BOR = 65,                       /* BOR  */
  YYSYMBOL_BXOR = 66,                      /* BXOR  */
  YYSYMBOL_BNOT = 67,                      /* BNOT  */
  YYSYMBOL_LSHIFT = 68,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 69,                    /* RSHIFT  */
  YYSYMBOL_FUNCTION = 70,                  /* FUNCTION  */
  YYSYMBOL_RETURN = 71,                    /* RETURN  */
  YYSYMBOL_CALL = 72,                      /* CALL  */
  YYSYMBOL_73_ = 73,                       /* '{'  */
  YYSYMBOL_74_ = 74,                       /* '}'  */
  YYSYMBOL_75_ = 75,                       /* ','  */
  YYSYMBOL_76_ = 76,                       /* '='  */
  YYSYMBOL_77_ = 77,                       /* '['  */
  YYSYMBOL_78_ = 78,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 79,                  /* $accept  */
  YYSYMBOL_S = 80,                         /* S  */
  YYSYMBOL_81_1 = 81,                      /* $@1  */
  YYSYMBOL_PROGRAM = 82,                   /* PROGRAM  */
  YYSYMBOL_FUNDECL = 83,                   /* FUNDECL  */
  YYSYMBOL_84_2 = 84,                      /* $@2  */
  YYSYMBOL_PARAMLIST = 85,                 /* PARAMLIST  */
  YYSYMBOL_A = 86,                         /* A  */
  YYSYMBOL_87_3 = 87,                      /* $@3  */
  YYSYMBOL_88_4 = 88,                      /* $@4  */
  YYSYMBOL_89_5 = 89,                      /* $@5  */
  YYSYMBOL_90_6 = 90,                      /* $@6  */
  YYSYMBOL_91_7 = 91,                      /* $@7  */
  YYSYMBOL_92_8 = 92,                      /* $@8  */
  YYSYMBOL_CASE_LIST = 93,                 /* CASE_LIST  */
  YYSYMBOL_CASE_ITEM = 94,                 /* CASE_ITEM  */
  YYSYMBOL_95_9 = 95,                      /* @9  */
  YYSYMBOL_CASE_EXPR = 96,                 /* CASE_EXPR  */
  YYSYMBOL_DECLSTATEMENT = 97,             /* DECLSTATEMENT  */
  YYSYMBOL_INIT_LIST = 98,                 /* INIT_LIST  */
  YYSYMBOL_DECLLIST = 99,                  /* DECLLIST  */
  YYSYMBOL_INDEX = 100,                    /* INDEX  */
  YYSYMBOL_TYPE = 101,                     /* TYPE  */
  YYSYMBOL_STMNTS = 102,                   /* STMNTS  */
  YYSYMBOL_ASSGN = 103,                    /* ASSGN  */
  YYSYMBOL_BOOLEXPR = 104,                 /* BOOLEXPR  */
  YYSYMBOL_M = 105,                        /* M  */
  YYSYMBOL_NN = 106,                       /* NN  */
  YYSYMBOL_ASNEXPR = 107,                  /* ASNEXPR  */
  YYSYMBOL_EXPR = 108,                     /* EXPR  */
  YYSYMBOL_FUNCALL = 109,                  /* FUNCALL  */
  YYSYMBOL_ARGLIST = 110,                  /* ARGLIST  */
  YYSYMBOL_OP = 111,                       /* OP  */
  YYSYMBOL_SUBSCRIPTS = 112,               /* SUBSCRIPTS  */
  YYSYMBOL_TERM = 113,                     /* TERM  */
  YYSYMBOL_OPR = 114,                      /* OPR  */
  YYSYMBOL_B = 115,                        /* B  */
  YYSYMBOL_C = 116,                        /* C  */
  YYSYMBOL_UN = 117                        /* UN  */
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
#define YYLAST   1145

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  162
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  314

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   316


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
       2,     2,     2,     5,     2,     2,    14,    10,     2,     2,
      11,    12,     9,     6,    75,     7,     2,     8,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     4,     2,
       2,    76,     2,     3,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    77,     2,    78,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,     2,    74,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,    13,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  2354,  2354,  2354,  2396,  2397,  2402,  2408,  2409,  2421,
    2421,  2512,  2522,  2531,  2534,  2534,  2567,  2580,  2593,  2625,
    2643,  2672,  2692,  2707,  2728,  2729,  2732,  2738,  2746,  2749,
    2751,  2751,  2762,  2764,  2764,  2777,  2781,  2776,  2795,  2795,
    2804,  2805,  2806,  2811,  2819,  2823,  2823,  2837,  2845,  2853,
    2866,  3176,  3182,  3185,  3187,  3199,  3214,  3224,  3241,  3260,
    3272,  3288,  3305,  3320,  3338,  3350,  3363,  3364,  3365,  3366,
    3367,  3368,  3369,  3370,  3371,  3380,  3386,  3393,  3394,  3395,
    3396,  3397,  3398,  3399,  3400,  3401,  3402,  3405,  3410,  3415,
    3418,  3421,  3426,  3431,  3436,  3441,  3446,  3451,  3457,  3465,
    3466,  3473,  3474,  3475,  3476,  3477,  3478,  3591,  3619,  3635,
    3651,  3667,  3687,  3711,  3712,  3713,  3729,  3745,  3761,  3773,
    3785,  3801,  3802,  3805,  3873,  3907,  3910,  3914,  3914,  3914,
    3914,  3914,  3914,  3914,  3914,  3914,  3914,  3916,  3923,  3932,
    3939,  3961,  4005,  4048,  4060,  4098,  4120,  4132,  4133,  4134,
    4135,  4138,  4138,  4140,  4141,  4142,  4143,  4144,  4145,  4146,
    4147,  4147,  4147
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
  "\"end of file\"", "error", "\"invalid token\"", "'?'", "':'", "'!'",
  "'+'", "'-'", "'/'", "'*'", "'%'", "'('", "')'", "UMINUS", "'$'",
  "BREAK", "CONTINUE", "FOR", "DO", "IDEN", "NUM", "PASN", "MASN", "DASN",
  "SASN", "INC", "DEC", "LT", "GT", "LE", "GE", "NE", "OR", "AND", "EQ",
  "IF", "ELSE", "TR", "FL", "WHILE", "INT", "FLOAT", "CHAR", "CHARR",
  "MEOF", "CONST", "SWITCH", "CASE", "DEFAULT", "GOTO", "STRING", "BOOL",
  "PRINT", "INPUT", "SIZEOF", "BANDASN", "BORASN", "BXORASN", "LSHIFTASN",
  "RSHIFTASN", "SHORT", "LONG", "DOUBLE", "VOID", "BAND", "BOR", "BXOR",
  "BNOT", "LSHIFT", "RSHIFT", "FUNCTION", "RETURN", "CALL", "'{'", "'}'",
  "','", "'='", "'['", "']'", "$accept", "S", "$@1", "PROGRAM", "FUNDECL",
  "$@2", "PARAMLIST", "A", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8",
  "CASE_LIST", "CASE_ITEM", "@9", "CASE_EXPR", "DECLSTATEMENT",
  "INIT_LIST", "DECLLIST", "INDEX", "TYPE", "STMNTS", "ASSGN", "BOOLEXPR",
  "M", "NN", "ASNEXPR", "EXPR", "FUNCALL", "ARGLIST", "OP", "SUBSCRIPTS",
  "TERM", "OPR", "B", "C", "UN", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-174)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-137)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     345,   -35,  -174,    11,   573,  -174,  -174,  -174,  -174,   759,
      17,    23,    36,  -174,   155,  -174,  -174,  -174,  -174,   114,
      45,  -174,  -174,    62,    75,    82,  -174,  -174,  -174,  -174,
    -174,  -174,  -174,  -174,  -174,   919,   114,    44,    76,    25,
    -174,   505,  -174,  -174,    88,    77,     0,   279,  -174,  -174,
     256,   100,   111,   310,  -174,  -174,   721,  -174,   532,   700,
    -174,  -174,    16,   997,   600,  -174,   919,   919,   919,   108,
    -174,   117,  -174,  1003,   133,  -174,   641,    81,  -174,  -174,
     -14,    71,   641,   123,  -174,   128,    70,   778,   784,   829,
     835,  -174,  -174,  -174,  -174,  -174,  -174,  -174,  -174,  -174,
    -174,   841,   855,   861,   867,   886,  -174,   919,   165,   126,
     194,   160,   161,    26,   164,   176,   919,  -174,   171,   413,
     641,   700,  -174,    42,   968,  -174,  -174,  -174,   919,   919,
     919,   919,   919,   919,   700,   106,  1032,  1039,  1046,   182,
     177,  -174,   892,   121,  -174,    88,   919,   188,    -5,   167,
    -174,  -174,  -174,  -174,    32,    32,    72,    72,    72,   223,
    1076,   304,  -174,  -174,  1012,  -174,  -174,  -174,  -174,  -174,
     919,   148,   305,  -174,  -174,  -174,  -174,  -174,  -174,  -174,
     305,   912,  -174,  -174,   195,   174,   209,   641,   532,   532,
    1012,  1012,  1012,  1012,  1012,  1012,   212,  -174,   163,   225,
     238,  -174,   114,   239,   240,   243,  -174,  -174,   419,   184,
      88,   714,  -174,    14,   919,  -174,  -174,  -174,  -174,  -174,
    -174,   251,   532,  -174,  -174,   641,   228,   232,  -174,   209,
     641,  -174,  -174,  -174,   257,   252,  -174,   919,   258,    88,
     197,  -174,   919,   432,  -174,   192,  -174,   136,   267,   247,
    -174,  -174,  -174,   110,  -174,   215,  -174,  -174,  -174,  -174,
     220,   440,    88,  -174,  -174,   532,  -174,  -174,   641,    -8,
     292,   224,   249,   248,   114,   222,   919,  -174,   721,   295,
    -174,   641,  -174,  -174,  -174,   322,  -174,  -174,   110,   641,
    -174,    88,  -174,  -174,   315,   641,  -174,  -174,   641,  -174,
     259,  -174,   320,  -174,  -174,  -174,   -30,  -174,  -174,   641,
    -174,   -22,   641,  -174
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     4,     0,   162,     5,     1,   161,   160,   162,
       0,     0,     0,    99,   162,    99,    66,    67,    69,     0,
       0,   139,    68,     0,     0,     0,   101,   102,   103,   104,
     105,    70,    71,    72,    73,   162,     0,   162,     0,    38,
      99,   162,    99,    42,     0,    99,     0,     0,   121,   122,
       0,     0,     0,     0,    16,    17,   162,    33,   162,   162,
      97,    98,     0,     0,   162,    74,   162,   162,   162,     0,
     120,     0,    19,     0,     0,    40,   162,     0,     6,    76,
      56,     0,   162,     0,    24,     0,   162,   162,   162,   162,
     162,    41,    78,    79,    80,    81,    82,    83,    84,    85,
      86,   162,   162,   162,   162,   162,    77,   162,     0,   159,
     159,   151,   152,   159,     0,     0,   162,   113,     0,     0,
     162,   162,    89,     0,     0,    99,    99,    99,   162,   162,
     162,   162,   162,   162,   162,     0,     0,     0,     0,     0,
       0,    18,   162,    99,     3,     0,   162,     0,    59,     0,
      50,    75,    25,    28,   108,   109,   111,   110,   112,   115,
     116,   117,   118,   119,   106,   114,   157,   158,   151,   152,
     162,   144,   156,   145,   149,   150,   143,   147,   148,   146,
     156,   162,   140,    99,     0,     0,    99,   162,   162,   162,
      91,    92,    95,    96,    94,    93,     0,    99,     0,     0,
       0,   107,    13,   124,   126,     0,    39,    54,    57,     0,
       0,   162,    51,     0,   162,   154,   155,   153,   142,   141,
     124,     0,   162,    99,    90,   162,     0,    87,    88,    99,
     162,    14,    22,    23,     0,     0,    21,   162,   123,     0,
      64,    55,   162,    62,   137,     0,   123,     0,     0,    26,
     100,    30,    32,     0,     9,    12,   125,    20,    58,    65,
       0,    53,     0,   138,    99,   162,   100,    99,   162,     0,
       0,     0,    99,     0,    13,    60,   162,    63,   162,     0,
      99,   162,    31,    48,    49,     0,    99,    15,     0,   162,
      11,     0,    52,    35,     0,   162,    29,    99,   162,    43,
      99,    61,     0,    34,    27,    45,    99,    10,    99,   162,
      36,    99,   162,    37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -174,  -174,  -174,   298,  -174,  -174,    66,   -77,  -174,  -174,
    -174,  -174,  -174,  -174,    53,  -174,  -174,  -174,  -174,    73,
    -113,   102,    -6,   -72,  -174,   -25,   -13,    84,   -50,     1,
    -174,  -173,  -174,  -174,  -174,   -43,   173,   -94,  -174
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,    40,    41,   273,   234,    42,   253,   268,
     120,   302,   312,    76,   271,   272,   309,   285,    43,   260,
      81,   148,    44,    45,   107,    62,    82,   267,    46,    47,
      48,   205,   108,   171,    49,   217,   218,   173,    50
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,    83,    64,    52,   143,   151,   118,   114,   221,     5,
      53,     6,   283,    65,    84,    63,   176,   -47,   -47,   179,
      86,    87,    88,    89,    90,   -46,   -46,    77,   125,    79,
      71,    54,   207,   122,   123,   284,    70,    55,    73,   135,
      88,    89,    90,   184,   -47,   166,   167,    56,   126,   127,
       7,     8,   -46,    52,   186,     9,    66,   119,    72,    63,
     124,   145,   146,   147,   256,    63,   172,   136,   137,   138,
     210,   211,   149,    67,   126,   127,     7,     8,   101,   102,
     103,     9,   104,   105,  -127,   150,    68,   154,   155,   156,
     157,   158,   244,    69,    21,    74,   185,   241,    25,    75,
     104,   105,   159,   160,   161,   162,   163,    80,   164,   196,
     226,    35,   187,   188,   189,    52,    51,   182,   197,   115,
      21,    -7,   124,   116,    25,   144,   258,   139,    52,   190,
     191,   192,   193,   194,   195,   124,   140,    35,   126,   127,
     104,   105,    51,   204,   142,   166,   167,   208,   249,   277,
     264,   168,   169,   252,    16,    17,    18,   269,   270,    19,
      58,     7,     8,   227,   228,    22,    59,   152,   126,   127,
     222,   213,   153,   225,    31,    32,    33,    34,   301,   165,
     177,   178,   204,   180,   230,   183,   224,   181,   202,    63,
      63,   282,    60,    61,   201,   206,   235,   247,    86,    87,
      88,    89,    90,   170,   296,    21,   126,   127,   209,    25,
     248,   212,   243,   166,   167,   245,   251,   300,   304,   174,
     175,   -90,    35,    63,   229,   214,   306,    51,   293,    86,
      87,    88,    89,    90,   223,   313,   231,   311,   204,   232,
     279,   -90,   -90,   261,   126,   127,    86,    87,    88,    89,
      90,   278,   233,   236,   281,   238,   101,   102,   103,   288,
     104,   105,   240,   246,   250,   127,    63,   295,   235,   254,
     263,   255,   257,   298,   147,   109,   110,   261,   265,   119,
      85,   111,   112,   266,   305,    86,    87,    88,    89,    90,
     274,   104,   105,    91,   275,   310,   286,   291,   287,   113,
      92,    93,    94,    95,   101,   102,   103,   294,   104,   105,
      86,    87,    88,    89,    90,   237,    86,    87,    88,    89,
      90,   289,   117,   -44,   215,   216,   297,   126,   127,   303,
     168,   169,   308,   307,    96,    97,    98,    99,   100,    78,
     290,   299,   259,   101,   102,   103,     1,   104,   105,   292,
     280,    -2,    -2,   219,     0,   106,    -2,     0,     0,     0,
      -2,    -2,    -2,    -2,    -2,    -2,     0,     0,   101,     0,
      -2,    -2,   104,   105,   101,   102,   103,     0,   104,   105,
      -2,     0,     0,     0,    -2,    -2,    -2,    -2,    -2,     2,
      -2,    -2,     0,     0,     0,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,     0,
       0,     0,    -2,     0,     0,    -2,    -2,    -2,    -2,    86,
      87,    88,    89,    90,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    92,    93,    94,    95,    86,    87,
      88,    89,    90,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,   102,   103,
       0,   104,   105,   101,   102,   103,     0,   104,   105,   106,
       0,     0,     0,     0,   239,     0,   101,   102,   103,     0,
     104,   105,     0,     0,   101,   102,   103,   262,   104,   105,
       0,     7,     8,     0,     0,   276,     9,     0,     0,     0,
      10,    11,    12,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     7,     8,
      14,     0,     0,   121,    15,    16,    17,    18,     0,    -8,
      19,    20,     0,     0,     0,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    60,
      61,     0,    35,     0,     0,    36,    37,    38,    39,     7,
       8,     0,    21,     0,     9,     0,    25,     0,    10,    11,
      12,    13,     0,     0,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    51,    58,     7,     8,    14,     0,
       0,   134,    15,    16,    17,    18,     0,     0,    19,    20,
       0,     0,     0,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    60,    61,     0,
      35,     0,     0,    36,    37,    38,    39,     7,     8,     0,
      21,     0,     9,     0,    25,     0,    10,    11,    12,    13,
       0,     0,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    51,     0,     0,     0,    14,     0,     0,     0,
      15,    16,    17,    18,     0,     0,    19,    20,     0,     0,
       0,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    58,     7,     8,    35,     0,
       0,   121,    37,    38,    39,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     9,     0,     7,     8,     0,
       0,     0,     9,     0,     0,     0,     0,    60,    61,     0,
      16,    17,    18,     0,     0,    19,     0,     0,     0,     0,
      21,    22,     0,     0,    25,     0,     0,     0,     0,     0,
      31,    32,    33,    34,    21,     7,     8,    35,    25,     0,
       9,    21,    51,     0,     0,    25,    26,    27,    28,    29,
      30,    35,     0,     0,     7,     8,    51,   242,    35,     9,
       7,     8,  -128,    51,     0,     9,     0,     0,  -130,    16,
      17,    18,     0,     0,    19,     0,     0,     0,     0,    21,
      22,     0,     0,    25,     0,     0,     0,     0,     0,    31,
      32,    33,    34,     0,     0,     0,    35,     0,    21,     0,
       0,    51,    25,     0,    21,     7,     8,     0,    25,     0,
       9,     7,     8,  -129,     0,    35,     9,     7,     8,  -131,
      51,    35,     9,     0,     0,  -132,    51,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     9,     7,     8,  -133,
       0,     0,     9,     7,     8,  -134,     0,     0,     9,    21,
       0,  -135,     0,    25,     0,    21,     0,     0,     0,    25,
       0,    21,     7,     8,     0,    25,    35,     9,     7,     8,
    -136,    51,    35,     9,   203,    21,     0,    51,    35,    25,
       0,    21,     0,    51,     0,    25,     0,    21,     7,     8,
       0,    25,    35,     9,   220,     7,     8,    51,    35,     0,
       9,     0,     0,    51,    35,     0,    21,     0,     0,    51,
      25,     0,    21,     0,     0,     0,    25,     0,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    51,    35,
       0,     0,    21,     0,    51,     0,    25,     0,     0,    21,
       0,     0,     0,    25,    86,    87,    88,    89,    90,    35,
     117,     0,     0,     0,    51,     0,    35,     0,     0,     0,
       0,    51,     0,     0,     0,   128,   129,   130,   131,   132,
       0,     0,   133,    86,    87,    88,    89,    90,     0,    86,
      87,    88,    89,    90,     0,     0,     0,   141,    86,    87,
      88,    89,    90,     0,   128,   129,   130,   131,   132,     0,
       0,   133,   101,   102,   103,     0,   104,   105,    86,    87,
      88,    89,    90,     0,   198,    86,    87,    88,    89,    90,
       0,   199,    86,    87,    88,    89,    90,     0,   200,     0,
       0,   101,   102,   103,     0,   104,   105,   101,   102,   103,
       0,   104,   105,     0,     0,     0,   101,   102,   103,     0,
     104,   105,    86,    87,    88,    89,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,   102,   103,     0,
     104,   105,     0,   101,   102,   103,     0,   104,   105,     0,
     101,   102,   103,     0,   104,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,   103,     0,   104,   105
};

static const yytype_int16 yycheck[] =
{
      13,     1,    15,     9,    76,    82,    56,    50,   181,    44,
       9,     0,    20,    19,    14,    14,   110,    47,    48,   113,
       6,     7,     8,     9,    10,    47,    48,    40,    12,    42,
      36,    14,   145,    58,    59,    43,    35,    14,    37,    64,
       8,     9,    10,   120,    74,    19,    20,    11,    32,    33,
       6,     7,    74,    59,    12,    11,    11,    56,    14,    58,
      59,    75,    76,    77,   237,    64,   109,    66,    67,    68,
      75,    76,     1,    11,    32,    33,     6,     7,    64,    65,
      66,    11,    68,    69,    14,    14,    11,    86,    87,    88,
      89,    90,    78,    11,    50,    19,   121,   210,    54,    74,
      68,    69,   101,   102,   103,   104,   105,    19,   107,   134,
     187,    67,   125,   126,   127,   121,    72,   116,    12,    19,
      50,    44,   121,    12,    54,    44,   239,    19,   134,   128,
     129,   130,   131,   132,   133,   134,    19,    67,    32,    33,
      68,    69,    72,   142,    11,    19,    20,   146,   225,   262,
      14,    25,    26,   230,    40,    41,    42,    47,    48,    45,
       5,     6,     7,   188,   189,    51,    11,    44,    32,    33,
     183,   170,    44,   186,    60,    61,    62,    63,   291,    14,
      20,    20,   181,    19,   197,    14,    12,    11,    11,   188,
     189,   268,    37,    38,    12,    74,   202,   222,     6,     7,
       8,     9,    10,    77,   281,    50,    32,    33,    20,    54,
     223,    44,   211,    19,    20,   214,   229,   289,   295,    25,
      26,    12,    67,   222,    12,    77,   298,    72,   278,     6,
       7,     8,     9,    10,    39,   312,    73,   309,   237,    14,
     265,    32,    33,   242,    32,    33,     6,     7,     8,     9,
      10,   264,    14,    14,   267,    12,    64,    65,    66,   272,
      68,    69,    78,    12,    36,    33,   265,   280,   274,    12,
      78,    19,    14,   286,    77,    19,    20,   276,    11,   278,
       1,    25,    26,    36,   297,     6,     7,     8,     9,    10,
      75,    68,    69,    14,    74,   308,     4,    75,    74,    43,
      21,    22,    23,    24,    64,    65,    66,    12,    68,    69,
       6,     7,     8,     9,    10,    75,     6,     7,     8,     9,
      10,    73,    12,    74,    19,    20,     4,    32,    33,    14,
      25,    26,    12,    74,    55,    56,    57,    58,    59,    41,
     274,   288,   240,    64,    65,    66,     1,    68,    69,   276,
     266,     6,     7,   180,    -1,    76,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    20,    -1,    -1,    64,    -1,
      25,    26,    68,    69,    64,    65,    66,    -1,    68,    69,
      35,    -1,    -1,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      -1,    -1,    67,    -1,    -1,    70,    71,    72,    73,     6,
       7,     8,     9,    10,    -1,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    -1,    21,    22,    23,    24,     6,     7,
       8,     9,    10,    -1,    -1,    -1,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,    59,    -1,    -1,    -1,    -1,    64,    65,    66,
      -1,    68,    69,    64,    65,    66,    -1,    68,    69,    76,
      -1,    -1,    -1,    -1,    75,    -1,    64,    65,    66,    -1,
      68,    69,    -1,    -1,    64,    65,    66,    75,    68,    69,
      -1,     6,     7,    -1,    -1,    75,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,     6,     7,
      35,    -1,    -1,    11,    39,    40,    41,    42,    -1,    44,
      45,    46,    -1,    -1,    -1,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    37,
      38,    -1,    67,    -1,    -1,    70,    71,    72,    73,     6,
       7,    -1,    50,    -1,    11,    -1,    54,    -1,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    72,     5,     6,     7,    35,    -1,
      -1,    11,    39,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    37,    38,    -1,
      67,    -1,    -1,    70,    71,    72,    73,     6,     7,    -1,
      50,    -1,    11,    -1,    54,    -1,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,     5,     6,     7,    67,    -1,
      -1,    11,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
       6,     7,    -1,    -1,    -1,    11,    -1,     6,     7,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      60,    61,    62,    63,    50,     6,     7,    67,    54,    -1,
      11,    50,    72,    -1,    -1,    54,    55,    56,    57,    58,
      59,    67,    -1,    -1,     6,     7,    72,    73,    67,    11,
       6,     7,    14,    72,    -1,    11,    -1,    -1,    14,    40,
      41,    42,    -1,    -1,    45,    -1,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    67,    -1,    50,    -1,
      -1,    72,    54,    -1,    50,     6,     7,    -1,    54,    -1,
      11,     6,     7,    14,    -1,    67,    11,     6,     7,    14,
      72,    67,    11,    -1,    -1,    14,    72,    -1,    -1,    -1,
      -1,     6,     7,    -1,    -1,    -1,    11,     6,     7,    14,
      -1,    -1,    11,     6,     7,    14,    -1,    -1,    11,    50,
      -1,    14,    -1,    54,    -1,    50,    -1,    -1,    -1,    54,
      -1,    50,     6,     7,    -1,    54,    67,    11,     6,     7,
      14,    72,    67,    11,    12,    50,    -1,    72,    67,    54,
      -1,    50,    -1,    72,    -1,    54,    -1,    50,     6,     7,
      -1,    54,    67,    11,    12,     6,     7,    72,    67,    -1,
      11,    -1,    -1,    72,    67,    -1,    50,    -1,    -1,    72,
      54,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    72,    67,
      -1,    -1,    50,    -1,    72,    -1,    54,    -1,    -1,    50,
      -1,    -1,    -1,    54,     6,     7,     8,     9,    10,    67,
      12,    -1,    -1,    -1,    72,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    27,    28,    29,    30,    31,
      -1,    -1,    34,     6,     7,     8,     9,    10,    -1,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,     6,     7,
       8,     9,    10,    -1,    27,    28,    29,    30,    31,    -1,
      -1,    34,    64,    65,    66,    -1,    68,    69,     6,     7,
       8,     9,    10,    -1,    12,     6,     7,     8,     9,    10,
      -1,    12,     6,     7,     8,     9,    10,    -1,    12,    -1,
      -1,    64,    65,    66,    -1,    68,    69,    64,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    64,    65,    66,    -1,
      68,    69,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    -1,
      68,    69,    -1,    64,    65,    66,    -1,    68,    69,    -1,
      64,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    66,    -1,    68,    69
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    44,    80,    81,    44,     0,     6,     7,    11,
      15,    16,    17,    18,    35,    39,    40,    41,    42,    45,
      46,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    67,    70,    71,    72,    73,
      82,    83,    86,    97,   101,   102,   107,   108,   109,   113,
     117,    72,   101,   108,    14,    14,    11,   105,     5,    11,
      37,    38,   104,   108,   105,   101,    11,    11,    11,    11,
     108,   101,    14,   108,    19,    74,    92,   105,    82,   105,
      19,    99,   105,     1,    14,     1,     6,     7,     8,     9,
      10,    14,    21,    22,    23,    24,    55,    56,    57,    58,
      59,    64,    65,    66,    68,    69,    76,   103,   111,    19,
      20,    25,    26,    43,   114,    19,    12,    12,   107,   108,
      89,    11,   104,   104,   108,    12,    32,    33,    27,    28,
      29,    30,    31,    34,    11,   104,   108,   108,   108,    19,
      19,    14,    11,   102,    44,    75,    76,    77,   100,     1,
      14,    86,    44,    44,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,    14,    19,    20,    25,    26,
      77,   112,   114,   116,    25,    26,   116,    20,    20,   116,
      19,    11,   108,    14,    86,   104,    12,   105,   105,   105,
     108,   108,   108,   108,   108,   108,   104,    12,    12,    12,
      12,    12,    11,    12,   108,   110,    74,    99,   108,    20,
      75,    76,    44,   108,    77,    19,    20,   114,   115,   115,
      12,   110,   105,    39,    12,   105,    86,   104,   104,    12,
     105,    73,    14,    14,    85,   101,    14,    75,    12,    75,
      78,    99,    73,   108,    78,   108,    12,   104,   105,    86,
      36,   105,    86,    87,    12,    19,   110,    14,    99,   100,
      98,   108,    75,    78,    14,    11,    36,   106,    88,    47,
      48,    93,    94,    84,    75,    74,    75,    99,   105,   104,
     106,   105,    86,    20,    43,    96,     4,    74,   105,    73,
      85,    75,    98,   107,    12,   105,    86,     4,   105,    93,
     102,    99,    90,    14,    86,   105,   102,    74,    12,    95,
     105,   102,    91,    86
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    79,    81,    80,    80,    80,    82,    82,    82,    84,
      83,    85,    85,    85,    87,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      88,    86,    86,    89,    86,    90,    91,    86,    92,    86,
      86,    86,    86,    93,    93,    95,    94,    94,    96,    96,
      97,    97,    98,    98,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,   100,   100,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   102,   102,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   105,
     106,   107,   107,   107,   107,   107,   107,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   109,   109,   110,   110,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   112,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   114,   114,   115,   115,   115,   115,   116,   116,   116,
     117,   117,   117
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     4,     1,     2,     2,     1,     1,     0,
      10,     4,     2,     0,     0,     8,     2,     2,     3,     2,
       6,     5,     5,     5,     2,     3,     6,    10,     3,     9,
       0,     8,     6,     0,    10,     0,     0,    14,     0,     4,
       2,     2,     1,     3,     1,     0,     6,     4,     1,     1,
       3,     4,     3,     1,     3,     4,     1,     3,     5,     2,
       6,     8,     4,     6,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     2,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     0,
       0,     1,     1,     1,     1,     1,     3,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     5,     4,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     1,
       4,     4,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     0,     1,     1,     0,
       1,     1,     0
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
#line 2354 "parser.y"
        {top = create_env(top,0);}
#line 4028 "y.tab.c"
    break;

  case 3: /* S: $@1 PROGRAM M MEOF  */
#line 2354 "parser.y"
                                                 {
        if (e){
                        printf("%s\nRejected \n%s \nCould not generate Three Address Code / Storage Layout\n",buffer,err);
                        e=0;err[0]="\0";buffer[0]='\0';}
                else {
                        backpatch((yyvsp[-2].b)->N,(yyvsp[-1].addr));
                        printf("%s\nAccepted -Unoptimized > Three Address :\n",buffer);
                     /*for (int pass = 0; pass < 12; pass++) {
    constantFolding();
     constantFoldConditionals();      
    copyPropagation();             
    algebraicSimplification();
        booleanSimplification();   
    strengthReduction();            
    commonSubexpressionElimination();
    peepholeOptimization();
    identityAssignmentElimination();
    deadStoreElimination();         
    redundantLoadElimination(); 
        eliminateDeadCode();
    conservativeJumpChaining();
        eliminateDeadCode();
}
deadVariableElimination();
int prev_dead_count = -1;
for (int dce_pass = 0; dce_pass < 10; dce_pass++) {
    eliminateDeadCode();
    int dead_count = 0;
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD CODE:") != NULL) dead_count++;
    }
    if (dead_count == prev_dead_count) break;
    prev_dead_count = dead_count;
}*/
                        for (int i=0;i<code;i++){
                                printf("%s",imcode[i]);
                        }
                          /*if (strlen(err) > 0) {
        printf("\n=== Warnings ===\n%s", err);
    }
                        print_all_envs(top);*/
                }YYACCEPT;}
#line 4075 "y.tab.c"
    break;

  case 4: /* S: MEOF  */
#line 2396 "parser.y"
              {YYACCEPT;}
#line 4081 "y.tab.c"
    break;

  case 5: /* S: error MEOF  */
#line 2397 "parser.y"
                    {e=1;strcpy(err,"Invalid Statements");
                //printf("%s \nRejected -> %s \nCould not generate Three Address Code / Storage Layout\n",buffer,err);
                YYACCEPT;}
#line 4089 "y.tab.c"
    break;

  case 6: /* PROGRAM: FUNDECL PROGRAM  */
#line 2402 "parser.y"
                         {
            if(!e){
                (yyval.b) = createBoolNode();
                (yyval.b)->N = merge((yyvsp[-1].b)->N, (yyvsp[0].b)->N);
            }
        }
#line 4100 "y.tab.c"
    break;

  case 7: /* PROGRAM: STMNTS  */
#line 2408 "parser.y"
                 {(yyval.b) = (yyvsp[0].b);}
#line 4106 "y.tab.c"
    break;

  case 8: /* PROGRAM: FUNDECL  */
#line 2409 "parser.y"
                  {
            if(!e){
                (yyval.b) = createBoolNode();
                (yyval.b)->N = (yyvsp[0].b)->N;
            }
        }
#line 4117 "y.tab.c"
    break;

  case 9: /* $@2: %empty  */
#line 2421 "parser.y"
                                              {
    if(!e){
        /*  duplicate parameter check */
        if(checkDuplicateParams((yyvsp[-1].decl))){
            e = 1;
            sprintf(err+strlen(err), "Duplicate parameter names in function %s\n", (yyvsp[-3].str));
        }

       // Function* f = createFunction($3, $2->str);
       char clean_ret_type[100];
strcpy(clean_ret_type, (yyvsp[-4].type)->str);
if(clean_ret_type[0] == '@') memmove(clean_ret_type, clean_ret_type+1, strlen(clean_ret_type));
Function* f = createFunction((yyvsp[-3].str), clean_ret_type);  // stores "void", "int", "float" etc.
        f->start_label = code;

        struct Decl* p = (yyvsp[-1].decl);
        int pcount = 0;
        Param* param_tail = NULL;
        while(p){
            Param* param = createParam(p->key, p->type);
            pcount++;
            if(f->params == NULL){ f->params = param; param_tail = param; }
            else { param_tail->next = param; param_tail = param; }
            p = p->next;
        }
        f->param_count = pcount;

        if(findFunction((yyvsp[-3].str)) != NULL){
            e=1;
            sprintf(err+strlen(err),"Redeclaration of function %s\n",(yyvsp[-3].str));
        } else {
            addFunction(f);
        }

        sprintf(imcode[code], "%d BeginFunc %s %d\n", code, (yyvsp[-3].str), pcount);
        code++;

        /*  set current function context for return checking */
        char* ret_type_clean = (yyvsp[-4].type)->str;
        if(ret_type_clean[0]=='@') ret_type_clean++;
        strcpy(current_function, (yyvsp[-3].str));
        strcpy(current_return_type, ret_type_clean);
        in_function = 1;
        has_return_statement = 0;

        top = create_env(top, offset);
        offset = 0;

        p = (yyvsp[-1].decl);
        while(p){
            Symbol* s = createSymbol(p->key);
            char clean_param_type[100];
            strcpy(clean_param_type, p->type);
            if (clean_param_type[0] == '@') memmove(clean_param_type, clean_param_type+1, strlen(clean_param_type));
            strcpy(s->type, clean_param_type);
            s->offset = offset;
            if(strcmp(clean_param_type,"char")==0) offset+=1;
            else if(strcmp(clean_param_type,"short")==0) offset+=2;
            else if(strcmp(clean_param_type,"int")==0||strcmp(clean_param_type,"float")==0) offset+=4;
            else if(strcmp(clean_param_type,"long")==0||strcmp(clean_param_type,"double")==0) offset+=8;
            put(top->table, p->key, s);
            sprintf(imcode[code], "%d PopParam %s\n", code, p->key);
            code++;
            p = p->next;
        }
    }
}
#line 4189 "y.tab.c"
    break;

  case 10: /* FUNDECL: FUNCTION TYPE IDEN '(' PARAMLIST ')' $@2 '{' STMNTS '}'  */
#line 2487 "parser.y"
                 {
    if(!e){
    if(strcmp(current_return_type, "void") != 0 && !has_return_statement) {
    e = 1;
    sprintf(err + strlen(err), "Error: Non-void function '%s' must return a value of type %s\n", 
            current_function, current_return_type);
}

        sprintf(imcode[code], "%d EndFunc %s\n", code, (yyvsp[-7].str));
        code++;

        (yyval.b) = createBoolNode();
        (yyval.b)->N = (yyvsp[-1].b)->N;

        top = top->prev;
        if(!top) offset = 0;
        else offset = top->prev_offset;

        /*  reset function context */
        in_function = 0;
        current_function[0] = '\0';
        current_return_type[0] = '\0';
    }
}
#line 4218 "y.tab.c"
    break;

  case 11: /* PARAMLIST: TYPE IDEN ',' PARAMLIST  */
#line 2512 "parser.y"
                                   {
            if(!e){
                (yyval.decl) = createDecl((yyvsp[-2].str));
                char clean_type[100];
                strcpy(clean_type, (yyvsp[-3].type)->str);
                if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));
                strcpy((yyval.decl)->type, clean_type);
                (yyval.decl)->next = (yyvsp[0].decl);
            }
        }
#line 4233 "y.tab.c"
    break;

  case 12: /* PARAMLIST: TYPE IDEN  */
#line 2522 "parser.y"
                    {
            if(!e){
                (yyval.decl) = createDecl((yyvsp[0].str));
                char clean_type[100];
                strcpy(clean_type, (yyvsp[-1].type)->str);
                if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));
                strcpy((yyval.decl)->type, clean_type);
            }
        }
#line 4247 "y.tab.c"
    break;

  case 13: /* PARAMLIST: %empty  */
#line 2531 "parser.y"
          {(yyval.decl) = NULL;}
#line 4253 "y.tab.c"
    break;

  case 14: /* $@3: %empty  */
#line 2534 "parser.y"
                            { 
       if(!e) {
           strcpy(current_switch_var, (yyvsp[-2].expr)->str);
           //saveoffset = offset;
                   top = create_env(top, offset);  
                  offset = 0;
                         char* base = getBaseType((yyvsp[-2].expr)->type);
if(strstr((yyvsp[-2].expr)->type, "[") != NULL) {
    // it's an array — always reject
    e = 1;
    sprintf(err+strlen(err),
        "Error: switch expression cannot be an array type '%s'\n", (yyvsp[-2].expr)->type);
} else if(!isIntegerType(base)) {
    e = 1;
    sprintf(err+strlen(err),
        "Error: switch expression must be integer type, got '%s'\n", base);
}

           /*"" track switch depth "" */
           switch_depth++;
       }
   }
#line 4280 "y.tab.c"
    break;

  case 15: /* A: SWITCH '(' EXPR ')' '{' $@3 CASE_LIST '}'  */
#line 2555 "parser.y"
                   {
       if(!e) {
           (yyval.b) = createBoolNode();
           backpatch((yyvsp[-1].b)->N, code);
           backpatch((yyvsp[-1].b)->B, code);
           //offset = saveoffset;
                   top = top->prev;            //  restore scope
        offset = top->prev_offset;  //  restore offset
           /*   restore switch depth  */
           switch_depth--;
       }
   }
#line 4297 "y.tab.c"
    break;

  case 16: /* A: BREAK '$'  */
#line 2567 "parser.y"
            {
    if (!e) {
        /*  break must be inside loop or switch */
        if(loop_depth == 0 && switch_depth == 0){
            e = 1;
            sprintf(err+strlen(err), "break statement not within loop or switch\n");
        }
        (yyval.b) = createBoolNode();
        (yyval.b)->B = createNode(code);
        sprintf(imcode[code], "%d goto ", code);
        code++;
    }
}
#line 4315 "y.tab.c"
    break;

  case 17: /* A: CONTINUE '$'  */
#line 2580 "parser.y"
               {
    if (!e) {
        /*  continue must be inside a loop */
        if(loop_depth == 0){
            e = 1;
            sprintf(err+strlen(err), "continue statement not within a loop\n");
        }
        (yyval.b) = createBoolNode();
        (yyval.b)->C = createNode(code);
        sprintf(imcode[code], "%d goto ", code);
        code++;
    }
}
#line 4333 "y.tab.c"
    break;

  case 18: /* A: RETURN EXPR '$'  */
#line 2593 "parser.y"
                  {
    if(!e){
        has_return_statement = 1;
        /*  return checks */
        if(!in_function){
            e = 1;
            sprintf(err+strlen(err), "return statement outside function\n");
        } else if(strcmp(current_return_type,"void")==0){
            e = 1;
            sprintf(err+strlen(err), "void function '%s' should not return a value\n", current_function);
        } else if(!isTypeCompatible(current_return_type, (yyvsp[-1].expr)->type)){
            e = 1;
            sprintf(err+strlen(err), "Return type mismatch in function '%s': expected %s, got %s\n",
                    current_function, current_return_type, (yyvsp[-1].expr)->type);
        }
         char ret_val[200];
        strcpy(ret_val, (yyvsp[-1].expr)->str);
        char* ret_base = getBaseType(current_return_type);
        char* expr_base = getBaseType((yyvsp[-1].expr)->type);
        if (!e && strlen(ret_base) > 0 && strcmp(ret_base, expr_base) != 0
               && getTypeRank(ret_base) > 0 && getTypeRank(expr_base) > 0) {
            /* generate:  t = (target_type) expr  then  Return t  */
            char* tmp = genvar();
            sprintf(imcode[code], "%d %s = (%s) %s\n", code, tmp, ret_base, (yyvsp[-1].expr)->str);
            code++;
            strcpy(ret_val, tmp);
        }
                        sprintf(imcode[code], "%d Return %s\n", code, ret_val);
        code++;
        (yyval.b) = createBoolNode();
    }
}
#line 4370 "y.tab.c"
    break;

  case 19: /* A: RETURN '$'  */
#line 2625 "parser.y"
             {
    if(!e){
        has_return_statement = 1;
        /*  return checks */
        if(!in_function){
            e = 1;
            sprintf(err+strlen(err), "return statement outside function\n");
        } else if(strcmp(current_return_type, "void") != 0) {
    e = 1;
    sprintf(err + strlen(err), "Non-void function '%s' must return a value\n", current_function);
}

        sprintf(imcode[code], "%d Return\n", code);
        code++;
        (yyval.b) = createBoolNode();
    }
}
#line 4392 "y.tab.c"
    break;

  case 20: /* A: CALL IDEN '(' ARGLIST ')' '$'  */
#line 2643 "parser.y"
                                {
    if(!e){
        Function* f = findFunction((yyvsp[-4].str));
        if(f == NULL){
            e=1;
            sprintf(err+strlen(err), "Function %s not declared\n", (yyvsp[-4].str));
        } else {
            struct Expr* arg = (yyvsp[-2].expr);
            int given = 0;
            while(arg){ given++; arg = arg->next; }
            if(given != f->param_count){
                e=1;
                sprintf(err+strlen(err),
                    "Function %s expects %d arguments, got %d\n",
                    (yyvsp[-4].str), f->param_count, given);
            } else {
                arg = (yyvsp[-2].expr);
                while(arg){
                    sprintf(imcode[code], "%d PushParam %s\n", code, arg->str);
                    code++;
                    arg = arg->next;
                }
                sprintf(imcode[code], "%d Call %s\n", code, (yyvsp[-4].str));
                code++;
            }
        }
        (yyval.b) = createBoolNode();
    }
}
#line 4426 "y.tab.c"
    break;

  case 21: /* A: CALL IDEN '(' ')' '$'  */
#line 2672 "parser.y"
                        {
    if(!e){
        Function* f = findFunction((yyvsp[-3].str));
        if(f == NULL){
            e=1;
            sprintf(err+strlen(err), "Function %s not declared\n", (yyvsp[-3].str));
        } else {
            if(f->param_count != 0){
                e=1;
                sprintf(err+strlen(err),
                    "Function %s expects %d arguments, got 0\n",
                    (yyvsp[-3].str), f->param_count);
            } else {
                sprintf(imcode[code], "%d Call %s\n", code, (yyvsp[-3].str));
                code++;
            }
        }
        (yyval.b) = createBoolNode();
    }
}
#line 4451 "y.tab.c"
    break;

  case 22: /* A: PRINT '(' EXPR ')' '$'  */
#line 2692 "parser.y"
                        {
    if (!e) {
        char* base_type = getBaseType((yyvsp[-2].expr)->type);
        if(strstr((yyvsp[-2].expr)->type, "char[") != NULL)
            sprintf(imcode[code], "%d printstring %s\n", code, (yyvsp[-2].expr)->str);
        else if(strcmp(base_type, "char") == 0)
            sprintf(imcode[code], "%d printchar %s\n", code, (yyvsp[-2].expr)->str);
        else if(strcmp(base_type, "float") == 0 || strcmp(base_type, "double") == 0)
            sprintf(imcode[code], "%d printfloat %s\n", code, (yyvsp[-2].expr)->str);
        else
            sprintf(imcode[code], "%d printint %s\n", code, (yyvsp[-2].expr)->str);
        code++;
        (yyval.b) = createBoolNode();
    }
}
#line 4471 "y.tab.c"
    break;

  case 23: /* A: INPUT '(' EXPR ')' '$'  */
#line 2707 "parser.y"
                        {
    if (!e) {
        if(!(yyvsp[-2].expr)->lv) {
            e = 1;
            sprintf(err + strlen(err), "Input requires a variable, not an expression\n");
        } 
        else {
        char* base_type = getBaseType((yyvsp[-2].expr)->type);
        if(strstr((yyvsp[-2].expr)->type, "char[") != NULL)
            sprintf(imcode[code], "%d inputstring %s\n", code, (yyvsp[-2].expr)->str);
        else if(strcmp(base_type, "char") == 0)
            sprintf(imcode[code], "%d inputchar %s\n", code, (yyvsp[-2].expr)->str);
        else if(strcmp(base_type, "float") == 0 || strcmp(base_type, "double") == 0)
            sprintf(imcode[code], "%d inputfloat %s\n", code, (yyvsp[-2].expr)->str);
        else
            sprintf(imcode[code], "%d inputint %s\n", code, (yyvsp[-2].expr)->str);
        code++;
        (yyval.b) = createBoolNode();
    }
}
}
#line 4497 "y.tab.c"
    break;

  case 24: /* A: ASNEXPR '$'  */
#line 2728 "parser.y"
              {if (!e){(yyval.b) = (yyvsp[-1].b);}}
#line 4503 "y.tab.c"
    break;

  case 25: /* A: ASNEXPR error MEOF  */
#line 2729 "parser.y"
                            {strcat(err,"$ missing\n");yyerrok;e=1;
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT;}
#line 4511 "y.tab.c"
    break;

  case 26: /* A: IF '(' BOOLEXPR ')' M A  */
#line 2732 "parser.y"
                                   {if (!e){backpatch((yyvsp[-3].b)->T,(yyvsp[-1].addr));
                                                                                (yyval.b) = createBoolNode();
                                                                                (yyval.b)->N = merge((yyvsp[-3].b)->F,(yyvsp[0].b)->N);
                                                                                (yyval.b)->B = (yyvsp[0].b)->B;
                                                                                (yyval.b)->C = (yyvsp[0].b)->C;
                                                                                }}
#line 4522 "y.tab.c"
    break;

  case 27: /* A: IF '(' BOOLEXPR ')' M A ELSE NN M A  */
#line 2738 "parser.y"
                                              {if (!e){
                backpatch((yyvsp[-7].b)->T,(yyvsp[-5].addr));
                backpatch((yyvsp[-7].b)->F,(yyvsp[-1].addr));
                (yyval.b) = createBoolNode();
                (yyval.b)->N = merge(merge((yyvsp[-4].b)->N,(yyvsp[-2].b)->N),(yyvsp[0].b)->N);
                (yyval.b)->B = merge((yyvsp[-4].b)->B, (yyvsp[0].b)->B);
                (yyval.b)->C = merge((yyvsp[-4].b)->C, (yyvsp[0].b)->C);
        }}
#line 4535 "y.tab.c"
    break;

  case 28: /* A: EXPR error MEOF  */
#line 2746 "parser.y"
                         {{strcat(err,"$ missing");yyerrok;e=1;}
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT;}
#line 4543 "y.tab.c"
    break;

  case 29: /* A: IF BOOLEXPR ')' M A ELSE NN M A  */
#line 2749 "parser.y"
                                         {{strcat(err,"missing (\n");e=1;}}
#line 4549 "y.tab.c"
    break;

  case 30: /* $@4: %empty  */
#line 2751 "parser.y"
                                     {loop_depth++;}
#line 4555 "y.tab.c"
    break;

  case 31: /* A: WHILE M '(' BOOLEXPR ')' M $@4 A  */
#line 2751 "parser.y"
                                                       {if (!e){
    loop_depth--;
    backpatch((yyvsp[0].b)->N,(yyvsp[-6].addr));
    backpatch((yyvsp[0].b)->C, (yyvsp[-6].addr));
    backpatch((yyvsp[-4].b)->T,(yyvsp[-2].addr));
    sprintf(imcode[code],"%d goto %d\n",code,(yyvsp[-6].addr));
    code++;
    backpatch((yyvsp[0].b)->B, code);
    (yyval.b) = createBoolNode();
    (yyval.b)->N = (yyvsp[-4].b)->F;
}}
#line 4571 "y.tab.c"
    break;

  case 32: /* A: WHILE M BOOLEXPR ')' M A  */
#line 2762 "parser.y"
                                   {{strcat(err,"missing (\n");e=1;}}
#line 4577 "y.tab.c"
    break;

  case 33: /* $@5: %empty  */
#line 2764 "parser.y"
               {loop_depth++;}
#line 4583 "y.tab.c"
    break;

  case 34: /* A: DO M $@5 A WHILE M '(' BOOLEXPR ')' '$'  */
#line 2764 "parser.y"
                                                              {
    if (!e) {
        loop_depth--;
        backpatch((yyvsp[-6].b)->N, (yyvsp[-4].addr)); 
        backpatch((yyvsp[-6].b)->C, (yyvsp[-4].addr)); 
        backpatch((yyvsp[-2].b)->T, (yyvsp[-8].addr));
        backpatch((yyvsp[-6].b)->B, code);
        backpatch((yyvsp[-2].b)->F, code);
        (yyval.b) = createBoolNode();
        (yyval.b)->N = NULL;
    }
}
#line 4600 "y.tab.c"
    break;

  case 35: /* $@6: %empty  */
#line 2777 "parser.y"
    { 
        sprintf(imcode[code], "%d goto %d\n", code, (yyvsp[-4].addr)); 
        code++; 
    }
#line 4609 "y.tab.c"
    break;

  case 36: /* $@7: %empty  */
#line 2781 "parser.y"
        {loop_depth++;}
#line 4615 "y.tab.c"
    break;

  case 37: /* A: FOR '(' ASNEXPR '$' M BOOLEXPR '$' M ASNEXPR $@6 ')' M $@7 A  */
#line 2782 "parser.y"
    {
        if (!e) {
            loop_depth--;
            backpatch((yyvsp[-8].b)->T, (yyvsp[-2].addr));
            backpatch((yyvsp[0].b)->N, (yyvsp[-6].addr));
            backpatch((yyvsp[0].b)->C, (yyvsp[-6].addr));
            sprintf(imcode[code], "%d goto %d\n", code, (yyvsp[-6].addr));
            code++;
            backpatch((yyvsp[0].b)->B, code);
            (yyval.b) = createBoolNode();
            (yyval.b)->N = (yyvsp[-8].b)->F;
        }
    }
#line 4633 "y.tab.c"
    break;

  case 38: /* $@8: %empty  */
#line 2795 "parser.y"
     {top = create_env(top,offset);offset=0;}
#line 4639 "y.tab.c"
    break;

  case 39: /* A: '{' $@8 STMNTS '}'  */
#line 2795 "parser.y"
                                                         {if (!e) {
                                                (yyval.b) = createBoolNode();
                                                (yyval.b)->N = (yyvsp[-1].b)->N;
                                                (yyval.b)->B = (yyvsp[-1].b)->B;
                                                (yyval.b)->C = (yyvsp[-1].b)->C;
                                                top = top->prev;
                                                if (!top) offset =0;
                                                else offset = top->prev_offset;
                                                }}
#line 4653 "y.tab.c"
    break;

  case 40: /* A: '{' '}'  */
#line 2804 "parser.y"
                  {if (!e){(yyval.b)=createBoolNode();}}
#line 4659 "y.tab.c"
    break;

  case 41: /* A: EXPR '$'  */
#line 2805 "parser.y"
                  {if (!e) {(yyval.b)=createBoolNode();}}
#line 4665 "y.tab.c"
    break;

  case 42: /* A: DECLSTATEMENT  */
#line 2806 "parser.y"
                        {
            if (!e){(yyval.b)=createBoolNode();}
            }
#line 4673 "y.tab.c"
    break;

  case 43: /* CASE_LIST: CASE_ITEM M CASE_LIST  */
#line 2811 "parser.y"
                                 {
       if(!e) {
           backpatch((yyvsp[-2].b)->N, (yyvsp[-1].addr));
           (yyval.b) = createBoolNode();
           (yyval.b)->N = (yyvsp[0].b)->N;
           (yyval.b)->B = merge((yyvsp[-2].b)->B, (yyvsp[0].b)->B);
       }
   }
#line 4686 "y.tab.c"
    break;

  case 44: /* CASE_LIST: CASE_ITEM  */
#line 2819 "parser.y"
               {
       if(!e) { (yyval.b) = (yyvsp[0].b); }
   }
#line 4694 "y.tab.c"
    break;

  case 45: /* @9: %empty  */
#line 2823 "parser.y"
                                   {
       if(!e) {
           sprintf(imcode[code], "%d if %s != %s goto ", code, 
                   current_switch_var, (yyvsp[-2].expr)->str);
           (yyval.addr) = code;
           code++;
       }
   }
#line 4707 "y.tab.c"
    break;

  case 46: /* CASE_ITEM: CASE CASE_EXPR ':' M @9 STMNTS  */
#line 2830 "parser.y"
            {
       if(!e) {
           (yyval.b) = createBoolNode();
           (yyval.b)->N = createNode((yyvsp[-1].addr));
           (yyval.b)->B = (yyvsp[0].b)->B;
       }
   }
#line 4719 "y.tab.c"
    break;

  case 47: /* CASE_ITEM: DEFAULT ':' M STMNTS  */
#line 2837 "parser.y"
                          {
       if(!e) {
           (yyval.b) = createBoolNode();
           (yyval.b)->N = NULL;
           (yyval.b)->B = (yyvsp[0].b)->B;
       }
   }
#line 4731 "y.tab.c"
    break;

  case 48: /* CASE_EXPR: NUM  */
#line 2845 "parser.y"
                  {
       if(!e) {
           (yyval.expr) = createExpr();
           strcpy((yyval.expr)->str, (yyvsp[0].str));
           if(checkfloat((yyvsp[0].str))) strcpy((yyval.expr)->type, "float");
           else strcpy((yyval.expr)->type, "int");
       }
   }
#line 4744 "y.tab.c"
    break;

  case 49: /* CASE_EXPR: CHARR  */
#line 2853 "parser.y"
           {
       if(!e) {
           (yyval.expr) = createExpr();
           strcpy((yyval.expr)->str, (yyvsp[0].str));
           strcpy((yyval.expr)->type, "char");
       }
   }
#line 4756 "y.tab.c"
    break;

  case 50: /* DECLSTATEMENT: TYPE DECLLIST '$'  */
#line 2866 "parser.y"
                                 {
        struct Decl* temp = (yyvsp[-1].decl);
        while(temp){
            char clean_type[100];
            strcpy(clean_type, (yyvsp[-2].type)->str);
            if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));

            if (temp->re){
                e = 1;
                Symbol* s = get(top->table, temp->key);
                char* decl_type = (yyvsp[-2].type)->str;
                if (decl_type[0] == '@') decl_type++;  // strip @
                
                if (strcmp(s->type, decl_type) == 0){
                    sprintf(err + strlen(err), "Redeclaration of %s\n", s->name);
                } else {
                    sprintf(err + strlen(err), "Conflicting types for %s\n", s->name);
                }
            }
            
            if (strcmp(temp->type,"")==0){
                Symbol* s = get(top->table,temp->key);
                s->offset = offset;
                offset+=(yyvsp[-2].type)->size;
                strcpy(s->type, clean_type);
                s->dim_count = 0;
            }
            else{
                Symbol* s = get(top->table,temp->key);
                s->offset = offset;
                offset+=(temp->size*(yyvsp[-2].type)->size);
                sprintf(s->type,"%s[%d]", clean_type, temp->size);
                s->dim_count = 0;
                char dim_str[1000]; strcpy(dim_str, temp->type);
                char* token = strtok(dim_str, " ");
                if (token && strcmp(token, "array") == 0) token = strtok(NULL, " ");
                int dims[10];
                while(token != NULL && s->dim_count < 10) {
                    int dim = atoi(token);
                    if (dim > 0) { dims[s->dim_count]=dim; s->dimensions[s->dim_count]=dim; s->dim_count++; }
                    token = strtok(NULL, " ");
                }
                if(s->dim_count == 0) sprintf(s->type,"%s[%d]", clean_type, temp->size);
                else if(s->dim_count == 1) sprintf(s->type,"%s[%d]", clean_type, dims[0]);
                else {
                    strcpy(s->type, clean_type);
                    for(int i = 0; i < s->dim_count; i++){ char dim_part[20]; sprintf(dim_part,"[%d]",dims[i]); strcat(s->type,dim_part); }
                }
            }

             if (strcmp(temp->lt,"u")!=0 && strcmp(temp->lt,"array_init")!=0
                    && strcmp(temp->lt,"void")==0) {
                e = 1;
                sprintf(err+strlen(err),
                    "Cannot assign void expression to variable '%s'\n", temp->key);
                break;  /* stop loop — empty op string would segfault */
            }

            if (e) break;  /* abort loop if any earlier check set e=1 */

            if (strcmp(temp->lt,"u")==0){ /* no init */ }
            else if (strcmp(temp->lt,"array_init")==0){
                Symbol* s = get(top->table, temp->key);
                char init_copy[1000]; strcpy(init_copy, temp->op);
                char* token = strtok(init_copy, ",");
                int idx = 0;
                int elem_size = 4;
                char* base_type = getBaseType(s->type);
                if (strcmp(base_type,"char")==0) elem_size=1;
                else if (strcmp(base_type,"short")==0) elem_size=2;
                else if (strcmp(base_type,"int")==0||strcmp(base_type,"float")==0) elem_size=4;
                else if (strcmp(base_type,"long")==0||strcmp(base_type,"double")==0) elem_size=8;

                /*  validate init list size */
                int init_count = 0;
                char* count_ptr = temp->op;
                while(*count_ptr){ if(*count_ptr==',') init_count++; count_ptr++; }
                if(strlen(temp->op)>0) init_count++;  /* count = commas + 1 */
                if(init_count > temp->size){
                    e=1;
                    sprintf(err+strlen(err),"Too many initializers for array %s (expected %d, got %d)\n",
                            temp->key, temp->size, init_count);
                }

                /* NEW: Check each array element for range overflow */
                char check_copy[1000]; strcpy(check_copy, temp->op);
                char* check_token = strtok(check_copy, ",");
                int check_idx = 0;
             char fixed_op[1000]; fixed_op[0] = '\0';
                while(check_token != NULL){
                    // Trim whitespace
                    while(*check_token == ' ') check_token++;
                    char* end = check_token + strlen(check_token) - 1;
                    while(end > check_token && *end == ' ') { *end = '\0'; end--; }
                    
                    // Check range for this element
                    char current_val[50];
                    strcpy(current_val, check_token); /* default: keep original */
                    if(isNumericConstant(check_token)){
                        double elem_val = atof(check_token);
                        if(strcmp(base_type,"char")==0 && (elem_val < -128 || elem_val > 127)){
                            int wrapped = (int)elem_val & 0xFF;
                            if(wrapped > 127) wrapped -= 256;
                            sprintf(err+strlen(err),
                                "Warning: Array element %.0f at index %d out of range for char [-128, 127] in '%s' (will wrap to %d)\n",
                                elem_val, check_idx, temp->key, wrapped);
                            sprintf(current_val, "%d", wrapped);
                        }
                        else if(strcmp(base_type,"short")==0 && (elem_val < -32768 || elem_val > 32767)){
                            int wrapped = (int)elem_val & 0xFFFF;
                            if(wrapped > 32767) wrapped -= 65536;
                            sprintf(err+strlen(err),
                                "Warning: Array element %.0f at index %d out of range for short [-32768, 32767] in '%s' (will wrap to %d)\n",
                                elem_val, check_idx, temp->key, wrapped);
                            sprintf(current_val, "%d", wrapped);
                        }
                        else if(strcmp(base_type,"int")==0 && (elem_val < -2147483648.0 || elem_val > 2147483647.0)){
                            long long wrapped = (long long)elem_val & 0xFFFFFFFF;
                            if(wrapped > 2147483647LL) wrapped -= 4294967296LL;
                            sprintf(err+strlen(err),
                                "Warning: Array element %.0f at index %d out of range for int in '%s' (will wrap to %lld)\n",
                                elem_val, check_idx, temp->key, wrapped);
                            sprintf(current_val, "%lld", wrapped);
                        }
                        else if(strcmp(base_type,"float")==0){
                            if(elem_val > FLT_MAX || elem_val < -FLT_MAX){
                                sprintf(err+strlen(err),
                                    "Warning: Array element %g at index %d overflows float in '%s' (will be inf)\n",
                                    elem_val, check_idx, temp->key);
                                sprintf(current_val, "inf");
                            }
                            else if(elem_val != 0.0 && fabs(elem_val) < FLT_MIN){
                                sprintf(err+strlen(err),
                                    "Warning: Array element %g at index %d underflows float in '%s' (will be 0)\n",
                                    elem_val, check_idx, temp->key);
                                sprintf(current_val, "0.0");
                            }
                        }
                        else if(strcmp(base_type,"double")==0){
                            if(isinf(elem_val) || elem_val > DBL_MAX || elem_val < -DBL_MAX){
                                sprintf(err+strlen(err),
                                    "Warning: Array element %g at index %d overflows double in '%s' (will be inf)\n",
                                    elem_val, check_idx, temp->key);
                                sprintf(current_val, "inf");
                            }
                            else if(elem_val != 0.0 && fabs(elem_val) < DBL_MIN){
                                sprintf(err+strlen(err),
                                    "Warning: Array element %g at index %d underflows double in '%s' (will be 0)\n",
                                    elem_val, check_idx, temp->key);
                                sprintf(current_val, "0.0");
                            }
                        }
                    }
                    if(check_idx > 0) strcat(fixed_op, ",");
                    strcat(fixed_op, current_val);
                    check_token = strtok(NULL, ",");
                    check_idx++;
                }
                strcpy(temp->op, fixed_op);               
                /* Generate TAC for array initialization */
                strcpy(init_copy, temp->op);
                token = strtok(init_copy, ",");
                idx = 0;
                while(token != NULL){
                    int byte_offset = idx * elem_size;
                    sprintf(imcode[code], "%d %s[%d] = %s\n", code, temp->key, byte_offset, token);
                    code++;
                    idx++;
                    token = strtok(NULL, ",");
                }
            }
            else if (strcmp(clean_type,temp->lt)==0){
    validateNumericLiteral(temp->op, clean_type, temp->key);
    if(isNumericConstant(temp->op)){
        double val = atof(temp->op);
        if(strcmp(clean_type,"char")==0)
            sprintf(imcode[code],"%d %s = %d\n",code,temp->key,(signed char)(long long)val);
        else if(strcmp(clean_type,"short")==0)
            sprintf(imcode[code],"%d %s = %d\n",code,temp->key,(short)(long long)val);
        else if(strcmp(clean_type,"int")==0)
            sprintf(imcode[code],"%d %s = %d\n",code,temp->key,(int)(long long)val);
        else if(strcmp(clean_type,"long")==0)
            sprintf(imcode[code],"%d %s = %ld\n",code,temp->key,(long)(long long)val);
        else if(strcmp(clean_type,"float")==0)
            sprintf(imcode[code],"%d %s = %g\n",code,temp->key,(float)val);
        else if(strcmp(clean_type,"double")==0)
            sprintf(imcode[code],"%d %s = %g\n",code,temp->key,val);
        else
            sprintf(imcode[code],"%d %s = %s\n",code,temp->key,temp->op);
    } else {
        sprintf(imcode[code],"%d %s = %s\n",code,temp->key,temp->op);
    }
    code++;
}

            else if (strcmp(temp->lt,"char*")==0 && strcmp(clean_type,"char")==0 &&
                     strstr(get(top->table,temp->key)->type, "char[") != NULL){
                Symbol* s = get(top->table,temp->key);
                char str_copy[1000]; strcpy(str_copy, temp->op);
                int len = strlen(str_copy);
                if(str_copy[0]=='"' && str_copy[len-1]=='"'){
                    str_copy[len-1]='\0';
                    char* str_content = str_copy + 1;
                    int idx = 0;
                    while(*str_content != '\0'){
                        sprintf(imcode[code], "%d %s[%d] = '%c'\n", code, temp->key, idx, *str_content);
                        code++; str_content++; idx++;
                    }
                    sprintf(imcode[code], "%d %s[%d] = '\\0'\n", code, temp->key, idx);
                    code++;
                }
            }
            else {
                /* Types don't match - need conversion */
                char base_type[100]; strcpy(base_type, clean_type);
                char* actual_type = base_type;
                if(strncmp(base_type,"const ",6)==0) actual_type = base_type+6;

                /* Also strip const from RHS type */
                char rhs_type[100]; strcpy(rhs_type, temp->lt);
                char* rhs_actual = rhs_type;
                if(strncmp(rhs_type,"const ",6)==0) rhs_actual = rhs_type+6;

                if(strcmp(actual_type, rhs_actual)==0){
                    /* Types match exactly - no cast needed */
                    sprintf(imcode[code],"%d %s = %s\n",code,temp->key,temp->op);
                    code++;
                }
                else {
                    /* NEW: Enhanced type checking and range validation */
                    if(temp->is_literal && isNumericConstant(temp->op)){
                        validateNumericLiteral(temp->op, actual_type, temp->key);
                        double val = atof(temp->op);
                        
                        /* Check ranges and generate appropriate code */
                        if(strcmp(actual_type,"char")==0){
                            if(val < -128 || val > 127){
                                sprintf(err+strlen(err),
                                    "Warning: Value %.0f out of range for char [-128, 127] in '%s' (will wrap to %d)\n",
                                    val, temp->key, (char)(int)val);
                            }
                            sprintf(imcode[code],"%d %s = %d\n",code,temp->key,(char)(int)val);
                        }
                        else if(strcmp(actual_type,"short")==0){
                            if(val < -32768 || val > 32767){
                                sprintf(err+strlen(err),
                                    "Warning: Value %.0f out of range for short [-32768, 32767] in '%s' (will wrap to %d)\n",
                                    val, temp->key, (short)(int)val);
                            }
                            sprintf(imcode[code],"%d %s = %d\n",code,temp->key,(short)(int)val);
                        }
                       
                        else if(strcmp(actual_type,"int")==0){
    if(val < -2147483648.0 || val > 2147483647.0){
        sprintf(err+strlen(err),
            "Warning: Value %.0f out of range for int [-2147483648, 2147483647] in '%s' (will wrap to %d)\n",
            val, temp->key, (int)(long long)val);
    }
    sprintf(imcode[code],"%d %s = %d\n",code,temp->key,(int)(long long)val);
}

                        else if(strcmp(actual_type,"long")==0){
    if(val > 9223372036854775807.0 || val < -9223372036854775808.0){
        sprintf(err+strlen(err),
            "Warning: Value %.0f out of range for long in '%s' (will wrap)\n",
            val, temp->key);
    }
    sprintf(imcode[code],"%d %s = %ld\n",code,temp->key,(long)(long long)val);
}
                        else if(strcmp(actual_type,"float")==0){
    float fval = (float)val;
    if(isinf(fval) && !isinf(val))
        sprintf(err+strlen(err),
            "Warning: Value %g for '%s' overflows float (stored as inf)\n",
            val, temp->key);
    sprintf(imcode[code],"%d %s = %g\n",code,temp->key,fval);
}
else if(strcmp(actual_type,"double")==0){
    /* double is our storage type already - just emit */
    sprintf(imcode[code],"%d %s = %g\n",code,temp->key,val);
}
                        else{
                            sprintf(imcode[code],"%d %s = (%s) %s\n",code,temp->key,actual_type,temp->op);
                        }
                        code++;
                    } else {
                        /* Non-literal: check for type compatibility warnings */
                        int target_rank = getTypeRank(actual_type);
                        int source_rank = getTypeRank(rhs_actual);
                        
                        if(target_rank > 0 && source_rank > 0 && source_rank > target_rank){
                            sprintf(err+strlen(err),
                                "Warning: Narrowing conversion from %s to %s in '%s'\n",
                                rhs_actual, actual_type, temp->key);
                        }
                        
                        if(isFloatingType(rhs_actual) && isIntegerType(actual_type)){
                            sprintf(err+strlen(err),
                                "Warning: Conversion from %s to %s will discard fractional part in '%s'\n",
                                rhs_actual, actual_type, temp->key);
                        }
                        
                        sprintf(imcode[code],"%d %s = (%s) %s\n",code,temp->key,actual_type,temp->op);
                        code++;
                    }
                }
            }
            temp = temp->next;
        }
}
#line 5071 "y.tab.c"
    break;

  case 51: /* DECLSTATEMENT: TYPE DECLLIST error MEOF  */
#line 3176 "parser.y"
                           {{strcat(err,"$ missing\n");yyerrok;e=1;}
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT; }
#line 5079 "y.tab.c"
    break;

  case 52: /* INIT_LIST: EXPR ',' INIT_LIST  */
#line 3182 "parser.y"
                              {
        if(!e){ (yyval.expr) = (yyvsp[-2].expr); (yyval.expr)->next = (yyvsp[0].expr); }
    }
#line 5087 "y.tab.c"
    break;

  case 53: /* INIT_LIST: EXPR  */
#line 3185 "parser.y"
           { if(!e){ (yyval.expr) = (yyvsp[0].expr); (yyval.expr)->next = NULL; } }
#line 5093 "y.tab.c"
    break;

  case 54: /* DECLLIST: IDEN ',' DECLLIST  */
#line 3187 "parser.y"
                            {if (get(top->table,(yyvsp[-2].str))==NULL){
                                                                Symbol* s = createSymbol((yyvsp[-2].str));
                                                                put(top->table,(yyvsp[-2].str),s);
                                                                (yyval.decl) = createDecl((yyvsp[-2].str));
                                                                (yyval.decl)->next = (yyvsp[0].decl);
                                                        }
                                                        else{
                                                                (yyval.decl) = createDecl((yyvsp[-2].str));
                                                                strcpy((yyval.decl)->type,"");
                                                                (yyval.decl)->re =1;
                                                                }strcpy((yyval.decl)->lt,"u");
                                                }
#line 5110 "y.tab.c"
    break;

  case 55: /* DECLLIST: IDEN INDEX ',' DECLLIST  */
#line 3199 "parser.y"
                                  {
                                if (get(top->table,(yyvsp[-3].str))==NULL){
                                        Symbol* s = createSymbol((yyvsp[-3].str));
                                        put(top->table,(yyvsp[-3].str),s);
                                        (yyval.decl) = createDecl((yyvsp[-3].str));
                                        (yyval.decl)->next = (yyvsp[0].decl);
                                        strcpy((yyval.decl)->type,(yyvsp[-2].type)->str);
                                        (yyval.decl)->size = (yyvsp[-2].type)->size;
                                }
                                else{
                                        (yyval.decl) = createDecl((yyvsp[-3].str));
                                        strcpy((yyval.decl)->type,"");
                                        (yyval.decl)->re =1;
                                }strcpy((yyval.decl)->lt,"u");
        }
#line 5130 "y.tab.c"
    break;

  case 56: /* DECLLIST: IDEN  */
#line 3214 "parser.y"
               {if (get(top->table,(yyvsp[0].str))==NULL){
                                                Symbol* s = createSymbol((yyvsp[0].str));
                                                put(top->table,(yyvsp[0].str),s);
                                                (yyval.decl) = createDecl((yyvsp[0].str));
                        }
                        else{
                                                (yyval.decl) = createDecl((yyvsp[0].str));
                                                strcpy((yyval.decl)->type,"");
                                                (yyval.decl)->re = 1;
                                                }strcpy((yyval.decl)->lt,"u");}
#line 5145 "y.tab.c"
    break;

  case 57: /* DECLLIST: IDEN '=' EXPR  */
#line 3224 "parser.y"
                        {
                                        if (get(top->table,(yyvsp[-2].str))==NULL){
                                                Symbol* s = createSymbol((yyvsp[-2].str));
                                                put(top->table,(yyvsp[-2].str),s);
                                                (yyval.decl) = createDecl((yyvsp[-2].str));
                                                strcpy((yyval.decl)->lt,(yyvsp[0].expr)->type);
                                                strcpy((yyval.decl)->op,(yyvsp[0].expr)->str);
                                                (yyval.decl)->is_literal = isLiteral((yyvsp[0].expr)->str);
                                                }
                                        else{
                                                (yyval.decl) = createDecl((yyvsp[-2].str));
                                                strcpy((yyval.decl)->type,"");
                                                strcpy((yyval.decl)->lt,(yyvsp[0].expr)->type);
                                                strcpy((yyval.decl)->op,(yyvsp[0].expr)->str);
                                                (yyval.decl)->is_literal = isLiteral((yyvsp[0].expr)->str);
                                                (yyval.decl)->re=1;
                                        }}
#line 5167 "y.tab.c"
    break;

  case 58: /* DECLLIST: IDEN '=' EXPR ',' DECLLIST  */
#line 3241 "parser.y"
                              {
                                        if (get(top->table,(yyvsp[-4].str))==NULL){
                                                Symbol* s = createSymbol((yyvsp[-4].str));
                                                put(top->table,(yyvsp[-4].str),s);
                                                (yyval.decl) = createDecl((yyvsp[-4].str));
                                                strcpy((yyval.decl)->lt,(yyvsp[-2].expr)->type);
                                                strcpy((yyval.decl)->op,(yyvsp[-2].expr)->str);
                                                (yyval.decl)->is_literal = isLiteral((yyvsp[-2].expr)->str);
                                                (yyval.decl)->next = (yyvsp[0].decl);
                                                }
                                        else{
                                                (yyval.decl) = createDecl((yyvsp[-4].str));
                                                strcpy((yyval.decl)->type,"");
                                                strcpy((yyval.decl)->lt,(yyvsp[-2].expr)->type);
                                                strcpy((yyval.decl)->op,(yyvsp[-2].expr)->str);
                                                (yyval.decl)->is_literal = isLiteral((yyvsp[-2].expr)->str);
                                                (yyval.decl)->re=1;
                                                (yyval.decl)->next = (yyvsp[0].decl);
                                        }}
#line 5191 "y.tab.c"
    break;

  case 59: /* DECLLIST: IDEN INDEX  */
#line 3260 "parser.y"
                     {if (get(top->table,(yyvsp[-1].str))==NULL){
                                                Symbol* s = createSymbol((yyvsp[-1].str));
                                                put(top->table,(yyvsp[-1].str),s);
                                                (yyval.decl) = createDecl((yyvsp[-1].str));
                                                strcpy((yyval.decl)->type,(yyvsp[0].type)->str);
                                                (yyval.decl)->size = (yyvsp[0].type)->size;
                                                }
                                        else{
                                                (yyval.decl) = createDecl((yyvsp[-1].str));
                                                strcpy((yyval.decl)->type,"");
                                                (yyval.decl)->re=1;
                                        }strcpy((yyval.decl)->lt,"u");}
#line 5208 "y.tab.c"
    break;

  case 60: /* DECLLIST: IDEN INDEX '=' '{' INIT_LIST '}'  */
#line 3272 "parser.y"
                                           {
        if (get(top->table,(yyvsp[-5].str))==NULL){
            Symbol* s = createSymbol((yyvsp[-5].str));
            put(top->table,(yyvsp[-5].str),s);
            (yyval.decl) = createDecl((yyvsp[-5].str));
            strcpy((yyval.decl)->type,(yyvsp[-4].type)->str);
            (yyval.decl)->size = (yyvsp[-4].type)->size;
            strcpy((yyval.decl)->lt,"array_init");
            char init_str[1000] = "";
            struct Expr* e = (yyvsp[-1].expr); int idx = 0;
            while(e){ if(idx>0) strcat(init_str,","); strcat(init_str,e->str); e=e->next; idx++; }
            strcpy((yyval.decl)->op, init_str);
        } else {
            (yyval.decl) = createDecl((yyvsp[-5].str)); strcpy((yyval.decl)->type,""); (yyval.decl)->re=1;
        }
    }
#line 5229 "y.tab.c"
    break;

  case 61: /* DECLLIST: IDEN INDEX '=' '{' INIT_LIST '}' ',' DECLLIST  */
#line 3288 "parser.y"
                                                    {
        if (get(top->table,(yyvsp[-7].str))==NULL){
            Symbol* s = createSymbol((yyvsp[-7].str));
            put(top->table,(yyvsp[-7].str),s);
            (yyval.decl) = createDecl((yyvsp[-7].str));
            strcpy((yyval.decl)->type,(yyvsp[-6].type)->str);
            (yyval.decl)->size = (yyvsp[-6].type)->size;
            (yyval.decl)->next = (yyvsp[0].decl);
            strcpy((yyval.decl)->lt,"array_init");
            char init_str[1000] = "";
            struct Expr* e = (yyvsp[-3].expr); int idx = 0;
            while(e){ if(idx>0) strcat(init_str,","); strcat(init_str,e->str); e=e->next; idx++; }
            strcpy((yyval.decl)->op, init_str);
        } else {
            (yyval.decl) = createDecl((yyvsp[-7].str)); strcpy((yyval.decl)->type,""); (yyval.decl)->re=1; (yyval.decl)->next = (yyvsp[0].decl);
        }
    }
#line 5251 "y.tab.c"
    break;

  case 62: /* DECLLIST: IDEN INDEX '=' EXPR  */
#line 3305 "parser.y"
                              {
                if (get(top->table,(yyvsp[-3].str))==NULL){
                    Symbol* s = createSymbol((yyvsp[-3].str));
                    put(top->table,(yyvsp[-3].str),s);
                    (yyval.decl) = createDecl((yyvsp[-3].str));
                    strcpy((yyval.decl)->type,(yyvsp[-2].type)->str);
                    (yyval.decl)->size = (yyvsp[-2].type)->size;
                    strcpy((yyval.decl)->lt, (yyvsp[0].expr)->type);
                    strcpy((yyval.decl)->op, (yyvsp[0].expr)->str);
                    if(strcmp((yyvsp[0].expr)->type, "char*") == 0) (yyval.decl)->str_len = (yyvsp[0].expr)->str_len;
                    (yyval.decl)->is_literal = 1;
                } else {
                    (yyval.decl) = createDecl((yyvsp[-3].str)); strcpy((yyval.decl)->type,""); (yyval.decl)->re=1;
                }
        }
#line 5271 "y.tab.c"
    break;

  case 63: /* DECLLIST: IDEN INDEX '=' EXPR ',' DECLLIST  */
#line 3320 "parser.y"
                                           {
                if (get(top->table,(yyvsp[-5].str))==NULL){
                    Symbol* s = createSymbol((yyvsp[-5].str));
                    put(top->table,(yyvsp[-5].str),s);
                    (yyval.decl) = createDecl((yyvsp[-5].str));
                    strcpy((yyval.decl)->type,(yyvsp[-4].type)->str);
                    (yyval.decl)->size = (yyvsp[-4].type)->size;
                    (yyval.decl)->next = (yyvsp[0].decl);
                    strcpy((yyval.decl)->lt, (yyvsp[-2].expr)->type);
                    strcpy((yyval.decl)->op, (yyvsp[-2].expr)->str);
                    if(strcmp((yyvsp[-2].expr)->type, "char*") == 0) (yyval.decl)->str_len = (yyvsp[-2].expr)->str_len;
                    (yyval.decl)->is_literal = 1;
                } else {
                    (yyval.decl) = createDecl((yyvsp[-5].str)); strcpy((yyval.decl)->type,""); (yyval.decl)->re=1; (yyval.decl)->next = (yyvsp[0].decl);
                }
        }
#line 5292 "y.tab.c"
    break;

  case 64: /* INDEX: '[' NUM ']'  */
#line 3338 "parser.y"
                   {(yyval.type) = createType();
                    int arr_size = atoi((yyvsp[-1].str));
                    /* positive size check*/
                    if(arr_size <= 0){
                        e=1;
                        sprintf(err+strlen(err),"Array size must be positive (got %d)\n", arr_size);
                    }
                    (yyval.type)->size=arr_size;
                    sprintf((yyval.type)->str,"array %d ", arr_size);
                    if (checkfloat((yyvsp[-1].str))){
                            e=1;sprintf(err+strlen(err),"Array index cannot be float\n");
                    }}
#line 5309 "y.tab.c"
    break;

  case 65: /* INDEX: '[' NUM ']' INDEX  */
#line 3350 "parser.y"
                            {(yyval.type) = createType();
                             int arr_size = atoi((yyvsp[-2].str));
                             /* positive size check  */
                             if(arr_size <= 0){
                                 e=1;
                                 sprintf(err+strlen(err),"Array size must be positive (got %d)\n", arr_size);
                             }
                             (yyval.type)->size=(yyvsp[0].type)->size*arr_size;
                             sprintf((yyval.type)->str,"array %d %s", arr_size, (yyvsp[0].type)->str);
                             if (checkfloat((yyvsp[-2].str))){
                                     e=1;sprintf(err+strlen(err),"Array index cannot be float\n");
                             }}
#line 5326 "y.tab.c"
    break;

  case 66: /* TYPE: INT  */
#line 3363 "parser.y"
          {(yyval.type) = createType(); strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=4;}
#line 5332 "y.tab.c"
    break;

  case 67: /* TYPE: FLOAT  */
#line 3364 "parser.y"
                 {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=4;}
#line 5338 "y.tab.c"
    break;

  case 68: /* TYPE: BOOL  */
#line 3365 "parser.y"
           {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=1;}
#line 5344 "y.tab.c"
    break;

  case 69: /* TYPE: CHAR  */
#line 3366 "parser.y"
               {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=1;}
#line 5350 "y.tab.c"
    break;

  case 70: /* TYPE: SHORT  */
#line 3367 "parser.y"
            {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=2;}
#line 5356 "y.tab.c"
    break;

  case 71: /* TYPE: LONG  */
#line 3368 "parser.y"
           {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=8;}
#line 5362 "y.tab.c"
    break;

  case 72: /* TYPE: DOUBLE  */
#line 3369 "parser.y"
             {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=8;}
#line 5368 "y.tab.c"
    break;

  case 73: /* TYPE: VOID  */
#line 3370 "parser.y"
           {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=0;}
#line 5374 "y.tab.c"
    break;

  case 74: /* TYPE: CONST TYPE  */
#line 3371 "parser.y"
                 {
    (yyval.type) = (yyvsp[0].type);
    char base[100];
    strcpy(base, (yyvsp[0].type)->str);
    if (base[0] == '@') memmove(base, base+1, strlen(base));
    sprintf((yyval.type)->str, "const %s", base);
}
#line 5386 "y.tab.c"
    break;

  case 75: /* STMNTS: STMNTS M A  */
#line 3380 "parser.y"
                   {if (!e){backpatch((yyvsp[-2].b)->N,(yyvsp[-1].addr));
                                        (yyval.b) = createBoolNode();
                                        (yyval.b)->N = (yyvsp[0].b)->N;
                                        (yyval.b)->B = merge((yyvsp[-2].b)->B, (yyvsp[0].b)->B);
                                        (yyval.b)->C = merge((yyvsp[-2].b)->C, (yyvsp[0].b)->C);
}}
#line 5397 "y.tab.c"
    break;

  case 76: /* STMNTS: A M  */
#line 3386 "parser.y"
             {if (!e){(yyval.b) = createBoolNode();
                (yyval.b)->N = (yyvsp[-1].b)->N;
                (yyval.b)->B = (yyvsp[-1].b)->B;
                (yyval.b)->C = (yyvsp[-1].b)->C;
}}
#line 5407 "y.tab.c"
    break;

  case 77: /* ASSGN: '='  */
#line 3393 "parser.y"
           {strcpy((yyval.str),"=");}
#line 5413 "y.tab.c"
    break;

  case 78: /* ASSGN: PASN  */
#line 3394 "parser.y"
                {strcpy((yyval.str),(yyvsp[0].str));}
#line 5419 "y.tab.c"
    break;

  case 79: /* ASSGN: MASN  */
#line 3395 "parser.y"
                {strcpy((yyval.str),(yyvsp[0].str));}
#line 5425 "y.tab.c"
    break;

  case 80: /* ASSGN: DASN  */
#line 3396 "parser.y"
            {strcpy((yyval.str),(yyvsp[0].str));}
#line 5431 "y.tab.c"
    break;

  case 81: /* ASSGN: SASN  */
#line 3397 "parser.y"
            {strcpy((yyval.str),(yyvsp[0].str));}
#line 5437 "y.tab.c"
    break;

  case 82: /* ASSGN: BANDASN  */
#line 3398 "parser.y"
               {strcpy((yyval.str),"&=");}
#line 5443 "y.tab.c"
    break;

  case 83: /* ASSGN: BORASN  */
#line 3399 "parser.y"
               {strcpy((yyval.str),"|=");}
#line 5449 "y.tab.c"
    break;

  case 84: /* ASSGN: BXORASN  */
#line 3400 "parser.y"
               {strcpy((yyval.str),"^=");}
#line 5455 "y.tab.c"
    break;

  case 85: /* ASSGN: LSHIFTASN  */
#line 3401 "parser.y"
                 {strcpy((yyval.str),"<<=");}
#line 5461 "y.tab.c"
    break;

  case 86: /* ASSGN: RSHIFTASN  */
#line 3402 "parser.y"
                 {strcpy((yyval.str),">>=");}
#line 5467 "y.tab.c"
    break;

  case 87: /* BOOLEXPR: BOOLEXPR OR M BOOLEXPR  */
#line 3405 "parser.y"
                                {  if (!e){backpatch((yyvsp[-3].b)->F,(yyvsp[-1].addr));
                                                                (yyval.b) = createBoolNode();
                                                                (yyval.b)->T = merge((yyvsp[-3].b)->T,(yyvsp[0].b)->T);
                                                                (yyval.b)->F = (yyvsp[0].b)->F;
                                                         }}
#line 5477 "y.tab.c"
    break;

  case 88: /* BOOLEXPR: BOOLEXPR AND M BOOLEXPR  */
#line 3410 "parser.y"
                              { if (!e){backpatch((yyvsp[-3].b)->T,(yyvsp[-1].addr));
                                                                (yyval.b) = createBoolNode();
                                                                (yyval.b)->T = (yyvsp[0].b)->T;
                                                                (yyval.b)->F = merge((yyvsp[-3].b)->F,(yyvsp[0].b)->F);
                                                                }}
#line 5487 "y.tab.c"
    break;

  case 89: /* BOOLEXPR: '!' BOOLEXPR  */
#line 3415 "parser.y"
                       {
                if (!e){ (yyval.b) = createBoolNode(); (yyval.b)->T = (yyvsp[0].b)->F; (yyval.b)->F = (yyvsp[0].b)->T; }
        }
#line 5495 "y.tab.c"
    break;

  case 90: /* BOOLEXPR: '(' BOOLEXPR ')'  */
#line 3418 "parser.y"
                           {
                if (!e){ (yyval.b) = createBoolNode(); (yyval.b)->T = (yyvsp[-1].b)->T; (yyval.b)->F = (yyvsp[-1].b)->F; }
        }
#line 5503 "y.tab.c"
    break;

  case 91: /* BOOLEXPR: EXPR LT EXPR  */
#line 3421 "parser.y"
                        {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 5513 "y.tab.c"
    break;

  case 92: /* BOOLEXPR: EXPR GT EXPR  */
#line 3426 "parser.y"
                    {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 5523 "y.tab.c"
    break;

  case 93: /* BOOLEXPR: EXPR EQ EXPR  */
#line 3431 "parser.y"
                        {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 5533 "y.tab.c"
    break;

  case 94: /* BOOLEXPR: EXPR NE EXPR  */
#line 3436 "parser.y"
                    {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 5543 "y.tab.c"
    break;

  case 95: /* BOOLEXPR: EXPR LE EXPR  */
#line 3441 "parser.y"
                        {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 5553 "y.tab.c"
    break;

  case 96: /* BOOLEXPR: EXPR GE EXPR  */
#line 3446 "parser.y"
                    {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 5563 "y.tab.c"
    break;

  case 97: /* BOOLEXPR: TR  */
#line 3451 "parser.y"
             {if (!e){
                (yyval.b) = createBoolNode();
                (yyval.b)->T = createNode(code);
                sprintf(imcode[code],"%d goto ",code);
                code++;
        }}
#line 5574 "y.tab.c"
    break;

  case 98: /* BOOLEXPR: FL  */
#line 3457 "parser.y"
             {if (!e){
                (yyval.b) = createBoolNode();
                (yyval.b)->F = createNode(code);
                sprintf(imcode[code],"%d goto ",code);
                code++;
        }}
#line 5585 "y.tab.c"
    break;

  case 99: /* M: %empty  */
#line 3465 "parser.y"
   {(yyval.addr)=code;}
#line 5591 "y.tab.c"
    break;

  case 100: /* NN: %empty  */
#line 3466 "parser.y"
    {(yyval.b)=createBoolNode();
        (yyval.b)->N = createNode(code);
        sprintf(imcode[code],"%d goto ",code);
        code++;
        }
#line 5601 "y.tab.c"
    break;

  case 101: /* ASNEXPR: BANDASN  */
#line 3473 "parser.y"
                 {strcpy((yyval.b), "&=");}
#line 5607 "y.tab.c"
    break;

  case 102: /* ASNEXPR: BORASN  */
#line 3474 "parser.y"
              {strcpy((yyval.b), "|=");}
#line 5613 "y.tab.c"
    break;

  case 103: /* ASNEXPR: BXORASN  */
#line 3475 "parser.y"
               {strcpy((yyval.b), "^=");}
#line 5619 "y.tab.c"
    break;

  case 104: /* ASNEXPR: LSHIFTASN  */
#line 3476 "parser.y"
                 {strcpy((yyval.b), "<<=");}
#line 5625 "y.tab.c"
    break;

  case 105: /* ASNEXPR: RSHIFTASN  */
#line 3477 "parser.y"
                 {strcpy((yyval.b), ">>=");}
#line 5631 "y.tab.c"
    break;

  case 106: /* ASNEXPR: EXPR ASSGN EXPR  */
#line 3478 "parser.y"
                 {
    if (!e && (yyvsp[-2].expr)->lv){
         if(strcmp((yyvsp[0].expr)->type, "void")==0){
            e=1;
            sprintf(err+strlen(err), "Cannot assign void expression to '%s'\n", (yyvsp[-2].expr)->str);
        }
        Symbol* sym = env_get(top, (yyvsp[-2].expr)->str);
        if(sym && strstr(sym->type, "const") != NULL){
            e=1;
            sprintf(err+strlen(err), "Cannot assign to const variable %s\n", (yyvsp[-2].expr)->str);
        }

        if (strcmp((yyvsp[0].expr)->type, "char*") == 0 && 
            strstr((yyvsp[-2].expr)->type, "char[") != NULL &&
            strlen((yyvsp[-1].str))==1 && (yyvsp[-1].str)[0]=='=') {
            char* bracket = strchr((yyvsp[-2].expr)->type, '[');
            if (bracket) {
                int array_size = atoi(bracket + 1);
                int string_len = (yyvsp[0].expr)->str_len;
                if (string_len > array_size) {
                    e = 1;
                    sprintf(err+strlen(err), "String literal too large for array (needs %d bytes, array is %d bytes)\n", string_len, array_size);
                } else {
                    char str_copy[1000]; strcpy(str_copy, (yyvsp[0].expr)->str);
                    int len = strlen(str_copy);
                    if(str_copy[0]=='"' && str_copy[len-1]=='"'){
                        str_copy[len-1]='\0';
                        char* str_content = str_copy + 1;
                        int idx = 0;
                        while(*str_content != '\0'){
                            sprintf(imcode[code], "%d %s[%d] = '%c'\n", code, (yyvsp[-2].expr)->str, idx, *str_content);
                            code++; str_content++; idx++;
                        }
                        sprintf(imcode[code], "%d %s[%d] = '\\0'\n", code, (yyvsp[-2].expr)->str, idx);
                        code++;
                    }
                }
            }
            (yyval.b) = createBoolNode();
        }
        else {
            if (strlen((yyvsp[-1].str))==1){
                sprintf(imcode[code],"%d %s = %s\n",code,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str);
                code++;
            }
            else{
    char* t = genvar();
    /* Strip trailing '=' to get the actual operator */
    char op_only[10];
    strncpy(op_only, (yyvsp[-1].str), sizeof(op_only)-1);
    op_only[sizeof(op_only)-1] = '\0';
    int op_len = strlen(op_only);
    if (op_len > 0 && op_only[op_len-1] == '=') op_only[op_len-1] = '\0';

    /* Get LHS variable type from symbol table */
    Symbol* lhs_sym = env_get(top, (yyvsp[-2].expr)->str);
    char lhs_type[100] = "";
    if(lhs_sym) strcpy(lhs_type, getBaseType(lhs_sym->type));

    /* Determine promoted type for the operation */
    char lhs_base[100], rhs_base[100];
    strcpy(lhs_base, lhs_type[0] ? lhs_type : (yyvsp[-2].expr)->type);
    strcpy(rhs_base, getBaseType((yyvsp[0].expr)->type));
    char* promoted = promoteType(lhs_base, rhs_base);

    /* Bitwise ops: warn if used on float/double */
    if((strcmp(op_only,"&")==0||strcmp(op_only,"|")==0||strcmp(op_only,"^")==0||
        strcmp(op_only,"<<")==0||strcmp(op_only,">>")==0) &&
       (isFloatingType(lhs_base)||isFloatingType(rhs_base))){
        e=1;
        sprintf(err+strlen(err),
            "Error: Bitwise operator '%s=' cannot be applied to float/double types\n", op_only);
    } else {
        /* Cast LHS up to promoted type if needed */
        char lhs_str[100]; strcpy(lhs_str, (yyvsp[-2].expr)->str);
        if(strcmp(lhs_base, promoted) != 0){
            char* cl = genvar();
            sprintf(imcode[code],"%d %s = (%s) %s\n",code,cl,promoted,(yyvsp[-2].expr)->str); code++;
            strcpy(lhs_str, cl);
        }

        /* Cast RHS up to promoted type if needed */
        char rhs_str[100]; strcpy(rhs_str, (yyvsp[0].expr)->str);
        if(strcmp(rhs_base, promoted) != 0){
            char* cr = genvar();
            sprintf(imcode[code],"%d %s = (%s) %s\n",code,cr,promoted,(yyvsp[0].expr)->str); code++;
            strcpy(rhs_str, cr);
        }

        /* Perform the operation */
        sprintf(imcode[code],"%d %s = %s %s %s\n",code,t,lhs_str,op_only,rhs_str); code++;

        /* Cast result back to LHS type if it differs from promoted type */
        if(strcmp(lhs_base, promoted) != 0){
            /* narrowing: warn */
            sprintf(err+strlen(err),
                "Warning: Implicit narrowing conversion from %s to %s in compound assignment to '%s'\n",
                promoted, lhs_base, (yyvsp[-2].expr)->str);
            char* cback = genvar();
            sprintf(imcode[code],"%d %s = (%s) %s\n",code,cback,lhs_base,t); code++;
            sprintf(imcode[code],"%d %s = %s\n",code,(yyvsp[-2].expr)->str,cback); code++;
        } else {
            sprintf(imcode[code],"%d %s = %s\n",code,(yyvsp[-2].expr)->str,t); code++;
        }
    }
}
            (yyval.b) = createBoolNode();
        }
    }
    if (!(yyvsp[-2].expr)->lv){e=1;strcat(err,"L value not assignable\n");}
}
#line 5747 "y.tab.c"
    break;

  case 107: /* EXPR: SIZEOF '(' IDEN ')'  */
#line 3591 "parser.y"
                          {
    if(!e){
        (yyval.expr) = createExpr();
        Env* temp = top; int found = 0; int var_size = 4;
        while(temp){
            Symbol* sym = get(temp->table, (yyvsp[-1].str));
            if(sym){
                found = 1;
                char* base_type = getBaseType(sym->type);
                if(strcmp(base_type,"char")==0) var_size=1;
                else if(strcmp(base_type,"short")==0) var_size=2;
                else if(strcmp(base_type,"int")==0||strcmp(base_type,"float")==0) var_size=4;
                else if(strcmp(base_type,"long")==0||strcmp(base_type,"double")==0) var_size=8;
                if(sym->dim_count > 0){
                    int total_elements = 1;
                    for(int i = 0; i < sym->dim_count; i++) total_elements *= sym->dimensions[i];
                    var_size *= total_elements;
                }
                break;
            }
            temp = temp->prev;
        }
        if(!found){ e=1; sprintf(err+strlen(err), "%s is not declared in scope\n", (yyvsp[-1].str)); }
        sprintf((yyval.expr)->str, "%d", var_size);
        strcpy((yyval.expr)->type, "int");
        (yyval.expr)->lv = 0;
    }
}
#line 5780 "y.tab.c"
    break;

  case 108: /* EXPR: EXPR '+' EXPR  */
#line 3619 "parser.y"
                {
    if (!e){
        (yyval.expr) = createExpr();
        if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), '+', (yyval.expr))) {
        } else {
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType((yyvsp[-2].expr),(yyvsp[0].expr),ct1,ct2,(yyval.expr)->type);
            if(strcmp(ct1,"")){strcpy((yyvsp[-2].expr)->str,ct1);}
            if(strcmp(ct2,"")){strcpy((yyvsp[0].expr)->str,ct2);}
            char* t = genvar(); strcpy((yyval.expr)->str,t);
            sprintf(imcode[code],"%d %s = %s + %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
        }
        (yyval.expr)->lv=0;
    }
}
#line 5801 "y.tab.c"
    break;

  case 109: /* EXPR: EXPR '-' EXPR  */
#line 3635 "parser.y"
                {
    if (!e){
        (yyval.expr) = createExpr();
        if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), '-', (yyval.expr))) {
        } else {
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType((yyvsp[-2].expr),(yyvsp[0].expr),ct1,ct2,(yyval.expr)->type);
            if(strcmp(ct1,"")){strcpy((yyvsp[-2].expr)->str,ct1);}
            if(strcmp(ct2,"")){strcpy((yyvsp[0].expr)->str,ct2);}
            char* t = genvar(); strcpy((yyval.expr)->str,t);
            sprintf(imcode[code],"%d %s = %s - %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
        }
        (yyval.expr)->lv=0;
    }
}
#line 5822 "y.tab.c"
    break;

  case 110: /* EXPR: EXPR '*' EXPR  */
#line 3651 "parser.y"
                {
    if (!e){
        (yyval.expr) = createExpr();
        if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), '*', (yyval.expr))) {
        } else {
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType((yyvsp[-2].expr),(yyvsp[0].expr),ct1,ct2,(yyval.expr)->type);
            if(strcmp(ct1,"")){strcpy((yyvsp[-2].expr)->str,ct1);}
            if(strcmp(ct2,"")){strcpy((yyvsp[0].expr)->str,ct2);}
            char* t = genvar(); strcpy((yyval.expr)->str,t);
            sprintf(imcode[code],"%d %s = %s * %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
        }
        (yyval.expr)->lv=0;
    }
}
#line 5843 "y.tab.c"
    break;

  case 111: /* EXPR: EXPR '/' EXPR  */
#line 3667 "parser.y"
                {
    if (!e){
        /*  division by zero check */
        if(isLiteral((yyvsp[0].expr)->str) && atof((yyvsp[0].expr)->str) == 0.0){
            e=1; sprintf(err+strlen(err),"Division by zero\n");
        }
        (yyval.expr) = createExpr();
        if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), '/', (yyval.expr))) {
        } else {
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType((yyvsp[-2].expr),(yyvsp[0].expr),ct1,ct2,(yyval.expr)->type);
            if(strcmp(ct1,"")){strcpy((yyvsp[-2].expr)->str,ct1);}
            if(strcmp(ct2,"")){strcpy((yyvsp[0].expr)->str,ct2);}
            char* t = genvar(); strcpy((yyval.expr)->str,t);
            sprintf(imcode[code],"%d %s = %s / %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
        }
        (yyval.expr)->lv=0;
    }
}
#line 5868 "y.tab.c"
    break;

  case 112: /* EXPR: EXPR '%' EXPR  */
#line 3687 "parser.y"
                {
    if (!e){
        /* modulo by zero check*/
        if(isLiteral((yyvsp[0].expr)->str) && atoi((yyvsp[0].expr)->str) == 0){
            e=1; sprintf(err+strlen(err),"Modulo by zero\n");
        }
        (yyval.expr) = createExpr();
        if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), '%', (yyval.expr))) {
        } else {
            if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){
                e=1;
                sprintf(err+strlen(err),"invalid operands to binary %% (float/double)\n");
            }
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType((yyvsp[-2].expr),(yyvsp[0].expr),ct1,ct2,(yyval.expr)->type);
            if(strcmp(ct1,"")){strcpy((yyvsp[-2].expr)->str,ct1);}
            if(strcmp(ct2,"")){strcpy((yyvsp[0].expr)->str,ct2);}
            char* t = genvar(); strcpy((yyval.expr)->str,t);
            sprintf(imcode[code],"%d %s = %s %% %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
        }
        (yyval.expr)->lv=0;
    }
}
#line 5897 "y.tab.c"
    break;

  case 113: /* EXPR: '(' EXPR ')'  */
#line 3711 "parser.y"
               {if (!e){(yyval.expr) = createExpr();strcpy((yyval.expr)->str,(yyvsp[-1].expr)->str); strcpy((yyval.expr)->type,(yyvsp[-1].expr)->type);(yyval.expr)->lv=(yyvsp[-1].expr)->lv;}}
#line 5903 "y.tab.c"
    break;

  case 114: /* EXPR: EXPR OP '$'  */
#line 3712 "parser.y"
                     {e=1;strcpy(err,"Missing operand");yyerrok;}
#line 5909 "y.tab.c"
    break;

  case 115: /* EXPR: EXPR BAND EXPR  */
#line 3713 "parser.y"
                     {
        if (!e){ 
            (yyval.expr) = createExpr();
            if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), '&', (yyval.expr))) { (yyval.expr)->lv = 0; } else {
                if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){ e=1; sprintf(err+strlen(err),"invalid operands to bitwise & (float/double)\n"); }
                char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
                char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
                checkType((yyvsp[-2].expr),(yyvsp[0].expr),ct1,ct2,(yyval.expr)->type);
                if(strcmp(ct1,"")){strcpy((yyvsp[-2].expr)->str,ct1);}
                if(strcmp(ct2,"")){strcpy((yyvsp[0].expr)->str,ct2);}
                char* t = genvar(); strcpy((yyval.expr)->str,t);
                sprintf(imcode[code],"%d %s = %s & %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
            }
            (yyval.expr)->lv=0;
        }
    }
#line 5930 "y.tab.c"
    break;

  case 116: /* EXPR: EXPR BOR EXPR  */
#line 3729 "parser.y"
                    {
        if (!e){ 
            (yyval.expr) = createExpr();
            if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), '|', (yyval.expr))) { (yyval.expr)->lv = 0; } else {
                if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){ e=1; sprintf(err+strlen(err),"invalid operands to bitwise | (float/double)\n"); }
                char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
                char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
                checkType((yyvsp[-2].expr),(yyvsp[0].expr),ct1,ct2,(yyval.expr)->type);
                if(strcmp(ct1,"")){strcpy((yyvsp[-2].expr)->str,ct1);}
                if(strcmp(ct2,"")){strcpy((yyvsp[0].expr)->str,ct2);}
                char* t = genvar(); strcpy((yyval.expr)->str,t);
                sprintf(imcode[code],"%d %s = %s | %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
            }
            (yyval.expr)->lv=0;
        }
    }
#line 5951 "y.tab.c"
    break;

  case 117: /* EXPR: EXPR BXOR EXPR  */
#line 3745 "parser.y"
                     {
        if (!e){ 
            (yyval.expr) = createExpr();
            if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), '^', (yyval.expr))) { (yyval.expr)->lv = 0; } else {
                if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){ e=1; sprintf(err+strlen(err),"invalid operands to bitwise ^ (float/double)\n"); }
                char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
                char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
                checkType((yyvsp[-2].expr),(yyvsp[0].expr),ct1,ct2,(yyval.expr)->type);
                if(strcmp(ct1,"")){strcpy((yyvsp[-2].expr)->str,ct1);}
                if(strcmp(ct2,"")){strcpy((yyvsp[0].expr)->str,ct2);}
                char* t = genvar(); strcpy((yyval.expr)->str,t);
                sprintf(imcode[code],"%d %s = %s ^ %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
            }
            (yyval.expr)->lv=0;
        }
    }
#line 5972 "y.tab.c"
    break;

  case 118: /* EXPR: EXPR LSHIFT EXPR  */
#line 3761 "parser.y"
                       {
        if (!e){ 
            (yyval.expr) = createExpr();
            if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), 'l', (yyval.expr))) { (yyval.expr)->lv = 0; } else {
                if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){ e=1; sprintf(err+strlen(err),"invalid operands to << (float/double)\n"); }
                strcpy((yyval.expr)->type,(yyvsp[-2].expr)->type);
                char* t = genvar(); strcpy((yyval.expr)->str,t);
                sprintf(imcode[code],"%d %s = %s << %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
            }
            (yyval.expr)->lv=0;
        }
    }
#line 5989 "y.tab.c"
    break;

  case 119: /* EXPR: EXPR RSHIFT EXPR  */
#line 3773 "parser.y"
                       {
        if (!e){ 
            (yyval.expr) = createExpr();
            if (tryConstantFold((yyvsp[-2].expr), (yyvsp[0].expr), 'r', (yyval.expr))) { (yyval.expr)->lv = 0; } else {
                if (isFloatingType((yyvsp[-2].expr)->type) || isFloatingType((yyvsp[0].expr)->type)){ e=1; sprintf(err+strlen(err),"invalid operands to >> (float/double)\n"); }
                strcpy((yyval.expr)->type,(yyvsp[-2].expr)->type);
                char* t = genvar(); strcpy((yyval.expr)->str,t);
                sprintf(imcode[code],"%d %s = %s >> %s\n",code,t,(yyvsp[-2].expr)->str,(yyvsp[0].expr)->str); code++;
            }
            (yyval.expr)->lv=0;
        }
    }
#line 6006 "y.tab.c"
    break;

  case 120: /* EXPR: BNOT EXPR  */
#line 3785 "parser.y"
                {
    if (!e){
        if (isFloatingType((yyvsp[0].expr)->type)){ e=1; sprintf(err+strlen(err),"invalid operand to bitwise ~ (float/double)\n"); }
        (yyval.expr) = createExpr();
        strcpy((yyval.expr)->type,(yyvsp[0].expr)->type);
        if (isNumericConstant((yyvsp[0].expr)->str)) {
            int val = (int)atof((yyvsp[0].expr)->str);
            sprintf((yyval.expr)->str, "%d", ~val);
            (yyval.expr)->lv = 0;
        } else {
            char* t = genvar(); strcpy((yyval.expr)->str,t);
            sprintf(imcode[code],"%d %s = ~ %s\n",code,t,(yyvsp[0].expr)->str); code++;
            (yyval.expr)->lv=0;
        }
    }
}
#line 6027 "y.tab.c"
    break;

  case 121: /* EXPR: FUNCALL  */
#line 3801 "parser.y"
              {(yyval.expr) = (yyvsp[0].expr);}
#line 6033 "y.tab.c"
    break;

  case 122: /* EXPR: TERM  */
#line 3802 "parser.y"
           {(yyval.expr) = (yyvsp[0].expr);}
#line 6039 "y.tab.c"
    break;

  case 123: /* FUNCALL: CALL IDEN '(' ARGLIST ')'  */
#line 3805 "parser.y"
                                   {
            if(!e){
                Function* f = findFunction((yyvsp[-3].str));
                if(f == NULL){
                    e=1;
                    sprintf(err+strlen(err),"Function %s not declared\n",(yyvsp[-3].str));
                    (yyval.expr) = createExpr();
                } else {
                    int arg_count = 0;
                    struct Expr* arg = (yyvsp[-1].expr);
                    Param* param = f->params;

                    /* argument type checking */
                    while(arg && param){
                        arg_count++;
                        if(!isTypeCompatible(param->type, arg->type)){
                            sprintf(err+strlen(err),"Warning: Argument %d type mismatch in call to %s: expected %s, got %s\n",
                                    arg_count, (yyvsp[-3].str), param->type, arg->type);
                        }
                        arg = arg->next;
                        param = param->next;
                    }
                    /* count any extra args */
                    while(arg){ arg_count++; arg = arg->next; }

                    if(arg_count != f->param_count){
                        e=1;
                        sprintf(err+strlen(err),"Function %s expects %d arguments, got %d\n",
                                (yyvsp[-3].str), f->param_count, arg_count);
                    }

                    struct Expr* reversed = NULL;
                    arg = (yyvsp[-1].expr);
                    while(arg){
                        struct Expr* temp = createExpr();
                        strcpy(temp->str, arg->str);
                        strcpy(temp->type, arg->type);
                        temp->lv = arg->lv;
                        temp->next = reversed;
                        reversed = temp;
                        arg = arg->next;
                    }
                    arg = reversed;
                    while(arg){
                        sprintf(imcode[code], "%d PushParam %s\n", code, arg->str);
                        code++;
                        arg = arg->next;
                    }

                   (yyval.expr) = createExpr();
                    strcpy((yyval.expr)->type, f->return_type);
                    (yyval.expr)->lv = 0;
                    if(strcmp(f->return_type, "void") == 0) {
                        e = 1;
                        sprintf(err+strlen(err), "Cannot use return value of void function '%s'\n", (yyvsp[-3].str));
                        sprintf(imcode[code], "%d Call %s\n", code, (yyvsp[-3].str));
                        code++;
                        strcpy((yyval.expr)->str, "__void__");
                    } else {
                        char* ret_var = genvar();
                        sprintf(imcode[code], "%d %s = Call %s\n", code, ret_var, (yyvsp[-3].str));
                        code++;
                        strcpy((yyval.expr)->str, ret_var);
                    }

                }
            }
        }
#line 6112 "y.tab.c"
    break;

  case 124: /* FUNCALL: CALL IDEN '(' ')'  */
#line 3873 "parser.y"
                    {
            if(!e){
                Function* f = findFunction((yyvsp[-2].str));
                if(f == NULL){
                    e=1;
                    sprintf(err+strlen(err),"Function %s not declared\n",(yyvsp[-2].str));
                    (yyval.expr) = createExpr();
                } else {
                    if(f->param_count != 0){
                        e=1;
                        sprintf(err+strlen(err),"Function %s expects %d arguments, got 0\n",
                                (yyvsp[-2].str), f->param_count);
                    }

                  (yyval.expr) = createExpr();
                    strcpy((yyval.expr)->type, f->return_type);
                    (yyval.expr)->lv = 0;
                    if(strcmp(f->return_type, "void") == 0) {
                        e = 1;
                        sprintf(err+strlen(err), "Cannot use return value of void function '%s'\n", (yyvsp[-2].str));
                        sprintf(imcode[code], "%d Call %s\n", code, (yyvsp[-2].str));
                        code++;
                        strcpy((yyval.expr)->str, "__void__");
                    } else {
                        char* ret_var = genvar();
                        sprintf(imcode[code], "%d %s = Call %s\n", code, ret_var, (yyvsp[-2].str));
                        code++;
                        strcpy((yyval.expr)->str, ret_var);
                    }

                }
            }
        }
#line 6150 "y.tab.c"
    break;

  case 125: /* ARGLIST: EXPR ',' ARGLIST  */
#line 3907 "parser.y"
                          {
            if(!e){ (yyval.expr) = (yyvsp[-2].expr); (yyval.expr)->next = (yyvsp[0].expr); }
        }
#line 6158 "y.tab.c"
    break;

  case 126: /* ARGLIST: EXPR  */
#line 3910 "parser.y"
               {
            if(!e){ (yyval.expr) = (yyvsp[0].expr); (yyval.expr)->next = NULL; }
        }
#line 6166 "y.tab.c"
    break;

  case 137: /* SUBSCRIPTS: '[' EXPR ']'  */
#line 3916 "parser.y"
                         {
                if (!e) {
                        (yyval.sub) = createSubscript();
                        sprintf((yyval.sub)->indices, "[%s]", (yyvsp[-1].expr)->str);
                        (yyval.sub)->count = 1;
                }
        }
#line 6178 "y.tab.c"
    break;

  case 138: /* SUBSCRIPTS: SUBSCRIPTS '[' EXPR ']'  */
#line 3923 "parser.y"
                                  {
                if (!e) {
                        (yyval.sub) = createSubscript();
                        sprintf((yyval.sub)->indices, "%s[%s]", (yyvsp[-3].sub)->indices, (yyvsp[-1].expr)->str);
                        (yyval.sub)->count = (yyvsp[-3].sub)->count + 1;
                }
        }
#line 6190 "y.tab.c"
    break;

  case 139: /* TERM: STRING  */
#line 3932 "parser.y"
             {
    (yyval.expr) = createExpr();
    strcpy((yyval.expr)->str, (yyvsp[0].str));
    strcpy((yyval.expr)->type, "char*");
    (yyval.expr)->str_len = strlen((yyvsp[0].str)) - 2 + 1;
    (yyval.expr)->lv = 0;
}
#line 6202 "y.tab.c"
    break;

  case 140: /* TERM: '(' TYPE ')' EXPR  */
#line 3939 "parser.y"
                    {
    if(!e){
        (yyval.expr) = createExpr();
        char clean_type[100]; strcpy(clean_type, (yyvsp[-2].type)->str);
        if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));
        if (isNumericConstant((yyvsp[0].expr)->str)) {
            double val = atof((yyvsp[0].expr)->str);
            if (strcmp(clean_type,"int")==0||strcmp(clean_type,"short")==0||
                strcmp(clean_type,"long")==0||strcmp(clean_type,"char")==0)
                sprintf((yyval.expr)->str, "%d", (int)val);
            else if (strcmp(clean_type,"float")==0||strcmp(clean_type,"double")==0)
                sprintf((yyval.expr)->str, "%g", val);
            else sprintf((yyval.expr)->str,"(%s)%s",clean_type,(yyvsp[0].expr)->str);
            strcpy((yyval.expr)->type, clean_type);
            (yyval.expr)->lv = 0;
        } else {
            sprintf((yyval.expr)->str, "(%s)%s", clean_type, (yyvsp[0].expr)->str);
            strcpy((yyval.expr)->type, clean_type);
            (yyval.expr)->lv = 0;
        }
    }
}
#line 6229 "y.tab.c"
    break;

  case 141: /* TERM: UN OPR IDEN B  */
#line 3961 "parser.y"
                {
    (yyval.expr) = createExpr();  
    if (strcmp((yyvsp[-3].str),"-")){
        Env* temp = top; int found=0;
        while(temp){
            if (get(temp->table,(yyvsp[-1].str))){
                found = 1;
                Symbol* t = get(temp->table,(yyvsp[-1].str));
                /*  const check on pre-inc/dec */
                if(strstr(t->type,"const")!=NULL){ e=1; sprintf(err+strlen(err),"Cannot modify const variable %s\n",(yyvsp[-1].str)); }
                strcpy((yyval.expr)->type,t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",(yyvsp[-1].str)); e=1; }
        char*t2=genvar();
        sprintf(imcode[code],"%d %s = %s %c 1\n",code,t2,(yyvsp[-1].str),(yyvsp[-2].str)[0]);code++;
        sprintf(imcode[code],"%d %s = %s\n",code,(yyvsp[-1].str),t2);code++;
        strcpy((yyval.expr)->str,t2);
    } else {
        if (!strcmp((yyvsp[-2].str),"--")){e=1;strcpy(err,"--- not allowed");}
        Env* temp = top; int found=0;
        while(temp){
            if (get(temp->table,(yyvsp[-1].str))){
                found = 1;
                Symbol* t = get(temp->table,(yyvsp[-1].str));
                if(strstr(t->type,"const")!=NULL){ e=1; sprintf(err+strlen(err),"Cannot modify const variable %s\n",(yyvsp[-1].str)); }
                strcpy((yyval.expr)->type,t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",(yyvsp[-1].str)); e=1; }
        else {
            char*t=genvar();char*t2=genvar();
            sprintf(imcode[code],"%d %s = %s %c 1\n",code,t,(yyvsp[-1].str),(yyvsp[-2].str)[0]);code++;
            sprintf(imcode[code],"%d %s = %s\n",code,(yyvsp[-1].str),t);code++;
            sprintf(imcode[code],"%d %s = - %s\n",code,t2,t);code++;
            strcpy((yyval.expr)->str,t2);
        }
    }
    (yyval.expr)->lv = 0;
}
#line 6278 "y.tab.c"
    break;

  case 142: /* TERM: UN IDEN OPR B  */
#line 4005 "parser.y"
                {
    (yyval.expr) = createExpr();  
    if (strcmp((yyvsp[-3].str),"-")){
        char*t = genvar();char*t2=genvar();
        sprintf(imcode[code],"%d %s = %s\n",code,t,(yyvsp[-2].str));code++;
        sprintf(imcode[code],"%d %s = %s %c 1\n",code,t2,(yyvsp[-2].str),(yyvsp[-1].str)[0]);code++;
        sprintf(imcode[code],"%d %s = %s\n",code,(yyvsp[-2].str),t2);code++;
        strcpy((yyval.expr)->str,t);
        Env* temp = top; int found=0;
        while(temp){
            if (get(temp->table,(yyvsp[-2].str))){
                found = 1;
                Symbol* t = get(temp->table,(yyvsp[-2].str));
                /*  const check on post-inc/dec */
                if(strstr(t->type,"const")!=NULL){ e=1; sprintf(err+strlen(err),"Cannot modify const variable %s\n",(yyvsp[-2].str)); }
                strcpy((yyval.expr)->type,t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",(yyvsp[-2].str)); e=1; }
    } else {
        char* t = genvar();char* t1 = genvar();char *t3 = genvar();
        sprintf(imcode[code],"%d %s = %s\n",code,t,(yyvsp[-2].str));code++;
        sprintf(imcode[code],"%d %s = %s %c 1\n",code,t1,(yyvsp[-2].str),(yyvsp[-1].str)[0]);code++;
        sprintf(imcode[code],"%d %s = %s\n",code,(yyvsp[-2].str),t1);code++;
        sprintf(imcode[code],"%d %s = -%s\n",code,t3,t);code++;
        strcpy((yyval.expr)->str,t3);
        Env* temp = top; int found=0;
        while(temp){
            if (get(temp->table,(yyvsp[-2].str))){
                found = 1;
                Symbol* t = get(temp->table,(yyvsp[-2].str));
                if(strstr(t->type,"const")!=NULL){ e=1; sprintf(err+strlen(err),"Cannot modify const variable %s\n",(yyvsp[-2].str)); }
                strcpy((yyval.expr)->type,t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",(yyvsp[-2].str)); e=1; }
    }
    (yyval.expr)->lv=0;
}
#line 6326 "y.tab.c"
    break;

  case 143: /* TERM: UN NUM C  */
#line 4048 "parser.y"
           {
    (yyval.expr) = createExpr();  
    if (!strcmp((yyvsp[-2].str),"-")) {
        char negated[100]; sprintf(negated, "-%s", (yyvsp[-1].str));
        strcpy((yyval.expr)->str, negated);
    } else {
        strcpy((yyval.expr)->str, (yyvsp[-1].str));
    }
    if (checkfloat((yyvsp[-1].str))) strcpy((yyval.expr)->type,"float");
    else strcpy((yyval.expr)->type,"int");
    (yyval.expr)->lv = 0;
}
#line 6343 "y.tab.c"
    break;

  case 144: /* TERM: UN IDEN SUBSCRIPTS  */
#line 4060 "parser.y"
                     {
    (yyval.expr) = createExpr();  
    Env* temp = top; int found=0; Symbol* sym_found = NULL;
    while(temp){
        if (get(temp->table,(yyvsp[-1].str))){
            found = 1;
            sym_found = get(temp->table,(yyvsp[-1].str));
            strcpy((yyval.expr)->type, sym_found->type);
            break;
        }
        temp = temp->prev;
    }
    if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",(yyvsp[-1].str)); e=1; }
    if (sym_found) {
        char* base_type = getBaseType(sym_found->type);
        strcpy((yyval.expr)->type, base_type);
    }
    if (sym_found && sym_found->dim_count > 0) {
        char* offset_var = calculateArrayOffset(sym_found, (yyvsp[0].sub), (yyvsp[-1].str));
        if (!strcmp((yyvsp[-2].str),"-")) {
            char* t = genvar(); strcpy((yyval.expr)->str, t);
            sprintf(imcode[code],"%d %s = - %s[%s]\n", code, t, (yyvsp[-1].str), offset_var); code++;
            (yyval.expr)->lv=0;
        } else {
            sprintf((yyval.expr)->str, "%s[%s]", (yyvsp[-1].str), offset_var);
            (yyval.expr)->lv=1;
        }
    } else {
        if (!strcmp((yyvsp[-2].str),"-")) {
            char* t = genvar(); strcpy((yyval.expr)->str, t);
            sprintf(imcode[code],"%d %s = - %s%s\n",code,t,(yyvsp[-1].str),(yyvsp[0].sub)->indices); code++;
            (yyval.expr)->lv=0;
        } else {
            sprintf((yyval.expr)->str, "%s%s", (yyvsp[-1].str), (yyvsp[0].sub)->indices);
            (yyval.expr)->lv=1;
        }
    }
}
#line 6386 "y.tab.c"
    break;

  case 145: /* TERM: UN IDEN C  */
#line 4098 "parser.y"
            {
    (yyval.expr) = createExpr();  
    if (!strcmp((yyvsp[-2].str),"-")) {
        char* t = genvar(); strcpy((yyval.expr)->str,t);
        sprintf(imcode[code],"%d %s = - %s\n",code,t,(yyvsp[-1].str)); code++;
        (yyval.expr)->lv=0;
    } else {
        strcpy((yyval.expr)->str,(yyvsp[-1].str));
        (yyval.expr)->lv=1;
    }
    Env* temp = top; int found=0;
    while(temp){
        if (get(temp->table,(yyvsp[-1].str))){
            found = 1;
            Symbol* t = get(temp->table,(yyvsp[-1].str));
            strcpy((yyval.expr)->type,t->type);
            break;
        }
        temp = temp->prev;
    }
    if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",(yyvsp[-1].str)); e=1; }
}
#line 6413 "y.tab.c"
    break;

  case 146: /* TERM: UN CHARR C  */
#line 4120 "parser.y"
             {
    (yyval.expr) = createExpr();
    if (!strcmp((yyvsp[-2].str),"-")) {
        char* t = genvar(); strcpy((yyval.expr)->str, t);
        sprintf(imcode[code], "%d %s = - %s\n", code, t, (yyvsp[-1].str)); code++;
        (yyval.expr)->lv = 0;
    } else {
        strcpy((yyval.expr)->str, (yyvsp[-1].str));
        (yyval.expr)->lv = 0;
    }
    strcpy((yyval.expr)->type, "char");
}
#line 6430 "y.tab.c"
    break;

  case 147: /* TERM: UN INC NUM  */
#line 4132 "parser.y"
             {e=1;strcpy(err,"cannot increment a constant value");}
#line 6436 "y.tab.c"
    break;

  case 148: /* TERM: UN DEC NUM  */
#line 4133 "parser.y"
             {e=1;strcpy(err,"cannot decrement a constant value");}
#line 6442 "y.tab.c"
    break;

  case 149: /* TERM: UN NUM INC  */
#line 4134 "parser.y"
             {e=1;strcpy(err,"cannot increment a constant value");}
#line 6448 "y.tab.c"
    break;

  case 150: /* TERM: UN NUM DEC  */
#line 4135 "parser.y"
             {e=1;strcpy(err,"cannot decrement a constant value");}
#line 6454 "y.tab.c"
    break;

  case 151: /* OPR: INC  */
#line 4138 "parser.y"
             {strcpy((yyval.str),(yyvsp[0].str));}
#line 6460 "y.tab.c"
    break;

  case 152: /* OPR: DEC  */
#line 4138 "parser.y"
                                   {strcpy((yyval.str),(yyvsp[0].str));}
#line 6466 "y.tab.c"
    break;

  case 153: /* B: OPR  */
#line 4140 "parser.y"
        {e=1;strcpy(err,"expression is not assignable");}
#line 6472 "y.tab.c"
    break;

  case 154: /* B: IDEN  */
#line 4141 "parser.y"
         {e=1;strcpy(err,"missing operator");}
#line 6478 "y.tab.c"
    break;

  case 155: /* B: NUM  */
#line 4142 "parser.y"
        {e=1;strcpy(err,"missing operator");}
#line 6484 "y.tab.c"
    break;

  case 157: /* C: IDEN  */
#line 4144 "parser.y"
         {e=1;strcpy(err,"missing operator");}
#line 6490 "y.tab.c"
    break;

  case 158: /* C: NUM  */
#line 4145 "parser.y"
        {e=1;strcpy(err,"missing operator");}
#line 6496 "y.tab.c"
    break;

  case 160: /* UN: '-'  */
#line 4147 "parser.y"
         {strcpy((yyval.str),"-");}
#line 6502 "y.tab.c"
    break;

  case 161: /* UN: '+'  */
#line 4147 "parser.y"
                                  {strcpy((yyval.str),"+");}
#line 6508 "y.tab.c"
    break;

  case 162: /* UN: %empty  */
#line 4147 "parser.y"
                                                      {strcpy((yyval.str),"");}
#line 6514 "y.tab.c"
    break;


#line 6518 "y.tab.c"

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

#line 4148 "parser.y"


char* genvar(){
    char *re = (char*)malloc(sizeof(char)*100);
    sprintf(re,"t%d",label);
    label++;
    return re;
}

int yyerror(char* msg){ return 0; }


void generateSymbolTableDOT() {
    FILE* dot = fopen("symbol_table.dot", "w");
    fprintf(dot, "digraph SymbolTable {\n");
        fprintf(dot, "  node [shape=record, style=filled, fillcolor=lightblue];\n");
    for (int i = 0; i < env_count; i++) {
        fprintf(dot, "  scope%d [label=\"{Scope %d|", i, i);
        
        Table* table = envs[i]->table;
        int first = 1;
        for (int j = 0; j < table->size; j++) {
            TableEntry* entry = table->buckets[j];
            while (entry) {
                if (!first) fprintf(dot, "|");
                fprintf(dot, "%s : %s - %d", 
                    entry->value->name, 
                    entry->value->type, 
                    entry->value->offset);
                   
                first = 0;
                entry = entry->next;
            }
        }
        fprintf(dot, "}\"];\n");
        
        // Connect to parent scope
        if (envs[i]->prev != NULL) {
            for (int k = 0; k < i; k++) {
                if (envs[k] == envs[i]->prev) {
                    fprintf(dot, "  scope%d -> scope%d;\n", i, k);
                    break;
                }
            }
        }
    }
    
    fprintf(dot, "}\n");
    fclose(dot);
}

void generateTACFlowDOT() {
    FILE* dot = fopen("tac_flow.dot", "w");
    fprintf(dot, "digraph TAC {\n");
    //fprintf(dot, "  node [shape=box];\n");
        fprintf(dot, "  node [shape=box, style=filled, fillcolor=lightblue];\n");

    for (int i = 0; i < code; i++) {
        //if (strstr(imcode[i], "// DEAD") != NULL) continue;
        
        // Escape quotes in labels
        char label[10000];
        strcpy(label, imcode[i]);
        char* newline = strchr(label, '\n');
        if (newline) *newline = '\0';
        
        fprintf(dot, "  n%d [label=\"%s\"];\n",  i, label);
        
        //  FIX: Check if this is a conditional jump (has both "if" and "goto")
        int is_conditional = 0;
        if (strstr(imcode[i], "if") != NULL && strstr(imcode[i], "goto") != NULL) {
            char* if_ptr = strstr(imcode[i], "if");
            char* goto_ptr = strstr(imcode[i], "goto");
            if (if_ptr < goto_ptr) {
                is_conditional = 1;
            }
        }
        
        // Draw edges for control flow
        if (is_conditional) {
            //  CONDITIONAL JUMP: Draw both true and false edges
            char* goto_ptr = strstr(imcode[i], "goto");
            char* ptr = goto_ptr + 4;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            
            if (isdigit(*ptr)) {
                int target = atoi(ptr);
                // True branch (goto target)
                fprintf(dot, "  n%d -> n%d [label=\"true\", color=green];\n", i, target);
            }
            
            //  FIX: False branch (fall-through to next line)
            if (i + 1 < code) {
                fprintf(dot, "  n%d -> n%d [label=\"false\", color=red];\n", i, i+1);
            }
        }
        else if (strstr(imcode[i], "goto") != NULL) {
            //  UNCONDITIONAL JUMP: Only goto edge, no fall-through
            char* goto_ptr = strstr(imcode[i], "goto");
            char* ptr = goto_ptr + 4;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            
            if (isdigit(*ptr)) {
                int target = atoi(ptr);
                fprintf(dot, "  n%d -> n%d [label=\"goto\"];\n", i, target);
            }
        }
        else if (strstr(imcode[i], "Return") == NULL &&
                 strstr(imcode[i], "BeginFunc") == NULL &&
                 strstr(imcode[i], "EndFunc") == NULL) {
            //  SEQUENTIAL FLOW: Normal statement, fall-through to next
            if (i + 1 < code) {
                fprintf(dot, "  n%d -> n%d [style=dashed];\n", i, i+1);
            }
        }
    }
    
    fprintf(dot, "}\n");
    fclose(dot);
}



// Function to identify basic block leaders
void identifyBasicBlocks() {
    int is_leader[10000] = {0};
    
    // Rule 1: First instruction is a leader
    is_leader[0] = 1;
    
    // Rule 2: Any target of a jump is a leader
    // Rule 3: Any instruction immediately following a jump is a leader
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        
        // Check for goto statements
        if (strstr(imcode[i], "goto") != NULL) {
            char* goto_ptr = strstr(imcode[i], "goto");
            char* ptr = goto_ptr + 4;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            
            if (isdigit(*ptr)) {
                int target = atoi(ptr);
                if (target >= 0 && target < code) {
                    is_leader[target] = 1;  // Target is a leader
                }
            }
            
            // Instruction after goto is a leader
            if (i + 1 < code) {
                is_leader[i + 1] = 1;
            }
        }
        
        // BeginFunc and EndFunc boundaries
        if (strstr(imcode[i], "BeginFunc") != NULL) {
            if (i + 1 < code) is_leader[i + 1] = 1;
        }
        if (strstr(imcode[i], "EndFunc") != NULL) {
            if (i + 1 < code) is_leader[i + 1] = 1;
        }
    }
    
    // Build basic blocks
    int current_start = -1;
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        
        if (is_leader[i]) {
            // End previous block
            if (current_start != -1) {
                BasicBlock* bb = (BasicBlock*)malloc(sizeof(BasicBlock));
                bb->start_line = current_start;
                bb->end_line = i - 1;
                bb->block_id = block_count++;
                bb->next = blocks;
                blocks = bb;
            }
            current_start = i;
        }
    }
    
    // End last block
    if (current_start != -1) {
        BasicBlock* bb = (BasicBlock*)malloc(sizeof(BasicBlock));
        bb->start_line = current_start;
        bb->end_line = code - 1;
        bb->block_id = block_count++;
        bb->next = blocks;
        blocks = bb;
    }
    
}

// Function to find which block a line belongs to
int getBlockForLine(int line) {
    BasicBlock* bb = blocks;
    while (bb) {
        if (line >= bb->start_line && line <= bb->end_line) {
            return bb->block_id;
        }
        bb = bb->next;
    }
    return -1;
}

// Enhanced DOT generation with basic blocks
void generateTACFlowWithBlocks() {
    identifyBasicBlocks();
    
    FILE* dot = fopen("tac_flow_blocks.dot", "w");
    fprintf(dot, "digraph TAC_Blocks {\n");
    fprintf(dot, "  node [shape=record, style=filled, fillcolor=lightblue];\n");
    fprintf(dot, "  rankdir=TB;\n\n");
    
    // Generate one node per basic block
    BasicBlock* bb = blocks;
    while (bb) {
        fprintf(dot, "  bb%d [label=\"{<b>Block %d|", bb->block_id, bb->block_id);
        
        // Add all instructions in this block
        int first = 1;
        for (int i = bb->start_line; i <= bb->end_line && i < code; i++) {
            //if (strstr(imcode[i], "// DEAD") != NULL) continue;
            
            char label[10000];
            strcpy(label, imcode[i]);
            
            // Escape special characters for DOT
            char escaped[10000];
            int j = 0, k = 0;
            while (label[j] != '\0' && label[j] != '\n') {
                if (label[j] == '|' || label[j] == '{' || label[j] == '}' || 
                    label[j] == '<' || label[j] == '>') {
                    escaped[k++] = '\\';
                }
                escaped[k++] = label[j++];
            }
            escaped[k] = '\0';
            
            if (!first) fprintf(dot, "\\l");  // Left-aligned line break
            //fprintf(dot, "%d: %s", i, escaped);
            fprintf(dot, "%s", escaped);
            first = 0;
        }
        
        fprintf(dot, "\\l}\"];\n");
        bb = bb->next;
    }
    
    fprintf(dot, "\n  // Control flow edges\n");
    
    // Draw edges between basic blocks
    for (int i = 0; i < code; i++) {
        //if (strstr(imcode[i], "// DEAD") != NULL) continue;
        
        int current_block = getBlockForLine(i);
        if (current_block == -1) continue;
        
        // Check if this is the last instruction in its block
        bb = blocks;
        int is_block_end = 0;
        while (bb) {
            if (bb->block_id == current_block && i == bb->end_line) {
                is_block_end = 1;
                break;
            }
            bb = bb->next;
        }
        
        if (!is_block_end) continue;  // Only process block-ending instructions
        
        // Check for conditional jump
        int is_conditional = 0;
        if (strstr(imcode[i], "if") != NULL && strstr(imcode[i], "goto") != NULL) {
            char* if_ptr = strstr(imcode[i], "if");
            char* goto_ptr = strstr(imcode[i], "goto");
            if (if_ptr < goto_ptr) {
                is_conditional = 1;
            }
        }
        
        if (is_conditional) {
            // True branch (goto target)
            char* goto_ptr = strstr(imcode[i], "goto");
            char* ptr = goto_ptr + 4;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            
            if (isdigit(*ptr)) {
                int target = atoi(ptr);
                int target_block = getBlockForLine(target);
                if (target_block != -1) {
                    fprintf(dot, "  bb%d -> bb%d [label=\"T\", color=green, penwidth=2];\n", 
                            current_block, target_block);
                }
            }
            
            // False branch (fall-through)
            if (i + 1 < code) {
                int next_block = getBlockForLine(i + 1);
                if (next_block != -1 && next_block != current_block) {
                    fprintf(dot, "  bb%d -> bb%d [label=\"F\", color=red, penwidth=2];\n", 
                            current_block, next_block);
                }
            }
        }
        else if (strstr(imcode[i], "goto") != NULL) {
            // Unconditional jump
            char* goto_ptr = strstr(imcode[i], "goto");
            char* ptr = goto_ptr + 4;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            
            if (isdigit(*ptr)) {
                int target = atoi(ptr);
                int target_block = getBlockForLine(target);
                if (target_block != -1) {
                    fprintf(dot, "  bb%d -> bb%d [label=\"goto\", penwidth=2];\n", 
                            current_block, target_block);
                }
            }
        }
        else if (strstr(imcode[i], "Return") == NULL &&
                 strstr(imcode[i], "EndFunc") == NULL) {
            // Sequential flow
            if (i + 1 < code) {
                int next_block = getBlockForLine(i + 1);
                if (next_block != -1 && next_block != current_block) {
                    fprintf(dot, "  bb%d -> bb%d [style=dashed, color=gray];\n", 
                            current_block, next_block);
                }
            }
        }
    }
    
    fprintf(dot, "}\n");
    fclose(dot);
}

// Add statistics function
void printBasicBlockStats() {
    printf("\n=== Basic Block Statistics ===\n");
    printf("Total blocks: %d\n", block_count);
    
    BasicBlock* bb = blocks;
    while (bb) {
        int inst_count = 0;
        for (int i = bb->start_line; i <= bb->end_line && i < code; i++) {
            if (strstr(imcode[i], "// DEAD") == NULL) {
                inst_count++;
            }
        }
        printf("Block %d: Lines %d-%d (%d instructions)\n", 
               bb->block_id, bb->start_line, bb->end_line, inst_count);
        bb = bb->next;
    }
}

/*
void generateCallGraphDOT() {
    FILE* dot = fopen("call_graph.dot", "w");
    fprintf(dot, "digraph CallGraph {\n");
    fprintf(dot, "  node [shape=record, style=filled];\n");
    fprintf(dot, "  rankdir=TB;\n");
    fprintf(dot, "  concentrate=true;\n\n");
    
    // Track edges with call site information
    typedef struct CallEdge {
        char caller[100];
        char callee[100];
        int line_number;
        int call_count;
        struct CallEdge* next;
    } CallEdge;
    
    CallEdge* edges = NULL;
    char current_func[100] = "";
    
    // Calculate function metrics
    typedef struct FuncMetrics {
        char name[100];
        int tac_lines;
        int start_line;
        int end_line;
        int call_count;  // How many times this function is called
        struct FuncMetrics* next;
    } FuncMetrics;
    
    FuncMetrics* metrics = NULL;
    
    // Scan TAC for function boundaries, calls, and metrics
    for (int i = 0; i < code; i++) {
        if (strstr(imcode[i], "// DEAD") != NULL) continue;
        
        // Track current function and calculate lines
        if (strstr(imcode[i], "BeginFunc") != NULL) {
            int param_count;
            sscanf(imcode[i], "%*d BeginFunc %s %d", current_func, &param_count);
            
            // Create metrics entry
            FuncMetrics* m = malloc(sizeof(FuncMetrics));
            strcpy(m->name, current_func);
            m->start_line = i;
            m->tac_lines = 0;
            m->call_count = 0;
            m->next = metrics;
            metrics = m;
        }
        
        if (strstr(imcode[i], "EndFunc") != NULL && strcmp(current_func, "") != 0) {
            // Update end line and count TAC instructions
            FuncMetrics* m = metrics;
            while (m) {
                if (strcmp(m->name, current_func) == 0) {
                    m->end_line = i;
                    // Count non-dead, non-control instructions
                    for (int j = m->start_line; j <= m->end_line; j++) {
                        if (strstr(imcode[j], "// DEAD") == NULL &&
                            strstr(imcode[j], "BeginFunc") == NULL &&
                            strstr(imcode[j], "EndFunc") == NULL &&
                            strstr(imcode[j], "PopParam") == NULL) {
                            m->tac_lines++;
                        }
                    }
                    break;
                }
                m = m->next;
            }
        }
        
        // Track function calls with line numbers
        if (strstr(imcode[i], "Call") != NULL && strcmp(current_func, "") != 0) {
            char callee[100];
            if (sscanf(imcode[i], "%*d %*s = Call %s", callee) == 1 ||
                sscanf(imcode[i], "%*d Call %s", callee) == 1) {
                
                // Increment callee's call count
                FuncMetrics* m = metrics;
                while (m) {
                    if (strcmp(m->name, callee) == 0) {
                        m->call_count++;
                        break;
                    }
                    m = m->next;
                }
                
                // Check if edge already exists
                CallEdge* check = edges;
                int exists = 0;
                while (check) {
                    if (strcmp(check->caller, current_func) == 0 &&
                        strcmp(check->callee, callee) == 0) {
                        check->call_count++;
                        exists = 1;
                        break;
                    }
                    check = check->next;
                }
                
                if (!exists) {
                    CallEdge* edge = malloc(sizeof(CallEdge));
                    strcpy(edge->caller, current_func);
                    strcpy(edge->callee, callee);
                    edge->line_number = i;
                    edge->call_count = 1;
                    edge->next = edges;
                    edges = edge;
                }
            }
        }
    }
    
    // Helper function to get color based on return type
    const char* getColorForType(const char* type) {
        if (strcmp(type, "void") == 0) return "lightgray";
        if (strcmp(type, "int") == 0) return "lightblue";
        if (strcmp(type, "float") == 0 || strcmp(type, "double") == 0) return "lightyellow";
        if (strcmp(type, "char") == 0) return "lightgreen";
        return "white";
    }
    
    // Draw function nodes with detailed information
    Function* f = func_list;
    while (f) {
        // Find metrics for this function
        FuncMetrics* m = metrics;
        int tac_lines = 0;
        int total_calls = 0;
        while (m) {
            if (strcmp(m->name, f->name) == 0) {
                tac_lines = m->tac_lines;
                total_calls = m->call_count;
                break;
            }
            m = m->next;
        }
        
        // Build parameter string
        char param_str[500] = "";
        if (f->param_count > 0) {
            Param* p = f->params;
            int first = 1;
            while (p) {
                if (!first) strcat(param_str, ", ");
                char temp[100];
                sprintf(temp, "%s %s", p->type, p->name);
                strcat(param_str, temp);
                first = 0;
                p = p->next;
            }
        } else {
            strcpy(param_str, "void");
        }
        
        // Determine complexity badge
        const char* complexity;
        if (tac_lines < 5) complexity = "Simple";
        else if (tac_lines < 20) complexity = "Medium";
        else complexity = "Complex";
        
        // Generate node with all information
        fprintf(dot, "  \"%s\" [fillcolor=\"%s\", label=<\n", 
                f->name, getColorForType(f->return_type));
        fprintf(dot, "    <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n");
        fprintf(dot, "      <TR><TD BGCOLOR=\"navy\"><FONT COLOR=\"white\"><B>%s</B></FONT></TD></TR>\n", 
                f->name);
        fprintf(dot, "      <TR><TD ALIGN=\"LEFT\"><B>Returns:</B> %s</TD></TR>\n", 
                f->return_type);
        fprintf(dot, "      <TR><TD ALIGN=\"LEFT\"><B>Params:</B> %s</TD></TR>\n", 
                param_str);
        fprintf(dot, "      <TR><TD ALIGN=\"LEFT\"><B>TAC Lines:</B> %d (%s)</TD></TR>\n", 
                tac_lines, complexity);
        fprintf(dot, "      <TR><TD ALIGN=\"LEFT\"><B>Called:</B> %d time%s</TD></TR>\n", 
                total_calls, total_calls == 1 ? "" : "s");
        fprintf(dot, "    </TABLE>\n");
        fprintf(dot, "  >];\n\n");
        
        f = f->next;
    }
    
    fprintf(dot, "\n  // Call relationships\n");
    
    // Draw edges with call information
    CallEdge* e = edges;
    while (e) {
        // Determine edge style based on call frequency
        const char* color = "black";
        int penwidth = 1;
        
        if (e->call_count > 5) {
            penwidth = 3;
            color = "red";
        } else if (e->call_count > 2) {
            penwidth = 2;
            color = "orange";
        }
        
        fprintf(dot, "  \"%s\" -> \"%s\" [label=\"%d call%s\\n@line %d\", "
                     "color=\"%s\", penwidth=%d];\n",
                e->caller, e->callee, 
                e->call_count, e->call_count == 1 ? "" : "s",
                e->line_number,
                color, penwidth);
        e = e->next;
    }
    
    // Add legend (FIXED - removed HR tag)
    fprintf(dot, "\n  // Legend\n");
    fprintf(dot, "  legend [shape=none, margin=0, label=<\n");
    fprintf(dot, "    <TABLE BORDER=\"1\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n");
    fprintf(dot, "      <TR><TD COLSPAN=\"2\" BGCOLOR=\"black\"><FONT COLOR=\"white\"><B>Legend</B></FONT></TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightblue\">int</TD><TD>Integer return type</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightyellow\">float/double</TD><TD>Floating-point return</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightgreen\">char</TD><TD>Character return type</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightgray\">void</TD><TD>No return value</TD></TR>\n");
    fprintf(dot, "      <TR><TD COLSPAN=\"2\" BGCOLOR=\"gray\"></TD></TR>\n");  // ← FIXED: Separator row
    fprintf(dot, "      <TR><TD>Black edge</TD><TD>Called 1-2 times</TD></TR>\n");
    fprintf(dot, "      <TR><TD><FONT COLOR=\"orange\">Orange edge</FONT></TD><TD>Called 3-5 times</TD></TR>\n");
    fprintf(dot, "      <TR><TD><FONT COLOR=\"red\">Red edge</FONT></TD><TD>Called 6+ times</TD></TR>\n");
    fprintf(dot, "    </TABLE>\n");
    fprintf(dot, "  >];\n");
    
    fprintf(dot, "}\n");
    fclose(dot);
    
    
    // Print statistics
    printf("\n=== Call Graph Statistics ===\n");
    printf("Total functions: ");
    int func_count = 0;
    f = func_list;
    while (f) { func_count++; f = f->next; }
    printf("%d\n", func_count);
    
    printf("\nMost called functions:\n");
    FuncMetrics* m = metrics;
    while (m) {
        if (m->call_count > 0) {
            printf("  %s: called %d time%s\n", 
                   m->name, m->call_count, m->call_count == 1 ? "" : "s");
        }
        m = m->next;
    }
    
    printf("\nFunction complexity:\n");
    m = metrics;
    while (m) {
        const char* complexity;
        if (m->tac_lines < 5) complexity = "Simple";
        else if (m->tac_lines < 20) complexity = "Medium";
        else complexity = "Complex";
        
        printf("  %s: %d TAC lines (%s)\n", m->name, m->tac_lines, complexity);
        m = m->next;
    }
    printf("=============================\n");
}

*/


void generateCallGraphDOT() {
    FILE* dot = fopen("call_graph.dot", "w");
    fprintf(dot, "digraph CallGraph {\n");
    fprintf(dot, "  node [shape=record, style=filled, fillcolor=lightblue];\n");
    fprintf(dot, "  rankdir=TB;\n");
    fprintf(dot, "  concentrate=true;\n\n");
    
    // Track edges with call site information
    typedef struct CallEdge {
        char caller[100];
        char callee[100];
        int line_number;
        int call_count;
        struct CallEdge* next;
    } CallEdge;
    
    CallEdge* edges = NULL;
    char current_func[100] = "";
    
    // Calculate function metrics
    typedef struct FuncMetrics {
        char name[100];
        int tac_lines;
        int start_line;
        int end_line;
        int call_count;  // How many times this function is called
        struct FuncMetrics* next;
    } FuncMetrics;
    
    FuncMetrics* metrics = NULL;
    
    // Scan TAC for function boundaries, calls, and metrics
    for (int i = 0; i < code; i++) {
        //if (strstr(imcode[i], "// DEAD") != NULL) continue;
        
        // Track current function and calculate lines
        if (strstr(imcode[i], "BeginFunc") != NULL) {
            int param_count;
            sscanf(imcode[i], "%*d BeginFunc %s %d", current_func, &param_count);
            
            // Create metrics entry
            FuncMetrics* m = malloc(sizeof(FuncMetrics));
            strcpy(m->name, current_func);
            m->start_line = i;
            m->tac_lines = 0;
            m->call_count = 0;
            m->next = metrics;
            metrics = m;
        }
        
        if (strstr(imcode[i], "EndFunc") != NULL && strcmp(current_func, "") != 0) {
            // Update end line and count TAC instructions
            FuncMetrics* m = metrics;
            while (m) {
                if (strcmp(m->name, current_func) == 0) {
                    m->end_line = i;
                    // Count non-dead, non-control instructions
                    for (int j = m->start_line; j <= m->end_line; j++) {
                        if (strstr(imcode[j], "// DEAD") == NULL &&
                            strstr(imcode[j], "BeginFunc") == NULL &&
                            strstr(imcode[j], "EndFunc") == NULL &&
                            strstr(imcode[j], "PopParam") == NULL) {
                            m->tac_lines++;
                        }
                    }
                    break;
                }
                m = m->next;
            }
        }
        
        // Track function calls with line numbers
        if (strstr(imcode[i], "Call") != NULL && strcmp(current_func, "") != 0) {
            char callee[100];
            if (sscanf(imcode[i], "%*d %*s = Call %s", callee) == 1 ||
                sscanf(imcode[i], "%*d Call %s", callee) == 1) {
                
                // Increment callee's call count
                FuncMetrics* m = metrics;
                while (m) {
                    if (strcmp(m->name, callee) == 0) {
                        m->call_count++;
                        break;
                    }
                    m = m->next;
                }
                
                // Check if edge already exists
                CallEdge* check = edges;
                int exists = 0;
                while (check) {
                    if (strcmp(check->caller, current_func) == 0 &&
                        strcmp(check->callee, callee) == 0) {
                        check->call_count++;
                        exists = 1;
                        break;
                    }
                    check = check->next;
                }
                
                if (!exists) {
                    CallEdge* edge = malloc(sizeof(CallEdge));
                    strcpy(edge->caller, current_func);
                    strcpy(edge->callee, callee);
                    edge->line_number = i;
                    edge->call_count = 1;
                    edge->next = edges;
                    edges = edge;
                }
            }
        }
    }
    
    //  UPDATED: Helper function to get color based on return type (ALL TYPES)
    const char* getColorForType(const char* type) {
        if (strcmp(type, "void") == 0) return "lightgray";
        if (strcmp(type, "int") == 0) return "lightyellow";
        if (strcmp(type, "short") == 0) return "lightcyan";
        if (strcmp(type, "long") == 0) return "lightskyblue";
        if (strcmp(type, "float") == 0) return "lightyellow";
        if (strcmp(type, "double") == 0) return "wheat";
        if (strcmp(type, "char") == 0) return "lightgreen";
        if (strcmp(type, "bool") == 0) return "lightpink";
        return "white";
    }
    
    // Draw function nodes with detailed information
    Function* f = func_list;
    while (f) {
        // Find metrics for this function
        FuncMetrics* m = metrics;
        int tac_lines = 0;
        int total_calls = 0;
        while (m) {
            if (strcmp(m->name, f->name) == 0) {
                tac_lines = m->tac_lines;
                total_calls = m->call_count;
                break;
            }
            m = m->next;
        }
        
        // Build parameter string
        char param_str[500] = "";
        if (f->param_count > 0) {
            Param* p = f->params;
            int first = 1;
            while (p) {
                if (!first) strcat(param_str, ", ");
                char temp[100];
                sprintf(temp, "%s %s", p->type, p->name);
                strcat(param_str, temp);
                first = 0;
                p = p->next;
            }
        } else {
            strcpy(param_str, "void");
        }
        
        // Determine complexity badge
        const char* complexity;
        if (tac_lines < 5) complexity = "Simple";
        else if (tac_lines < 20) complexity = "Medium";
        else complexity = "Complex";
        
        // Generate node with all information
        fprintf(dot, "  \"%s\" [fillcolor=lightblue, label=<\n", 
                f->name);
        fprintf(dot, "    <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n");
        fprintf(dot, "      <TR><TD BGCOLOR=\"lightblue\"><FONT COLOR=\"black\"><B>%s</B></FONT></TD></TR>\n", 
                f->name);
        fprintf(dot, "      <TR><TD ALIGN=\"LEFT\"><B>Returns:</B> %s</TD></TR>\n", 
                f->return_type);
        fprintf(dot, "      <TR><TD ALIGN=\"LEFT\"><B>Params:</B> %s</TD></TR>\n", 
                param_str);
        fprintf(dot, "      <TR><TD ALIGN=\"LEFT\"><B>TAC Lines:</B> %d (%s)</TD></TR>\n", 
                tac_lines, complexity);
        fprintf(dot, "      <TR><TD ALIGN=\"LEFT\"><B>Called:</B> %d time%s</TD></TR>\n", 
                total_calls, total_calls == 1 ? "" : "s");
        fprintf(dot, "    </TABLE>\n");
        fprintf(dot, "  >];\n\n");
        
        f = f->next;
    }
    
    fprintf(dot, "\n  // Call relationships\n");
    
    // Draw edges with call information
    CallEdge* e = edges;
    while (e) {
        // Determine edge style based on call frequency
        const char* color = "black";
        int penwidth = 1;
        
        if (e->call_count > 5) {
            penwidth = 3;
            color = "red";
        } else if (e->call_count > 2) {
            penwidth = 2;
            color = "orange";
        }
        
        fprintf(dot, "  \"%s\" -> \"%s\" [label=\"%d call%s\\n@line %d\", "
                     "color=\"%s\", penwidth=%d];\n",
                e->caller, e->callee, 
                e->call_count, e->call_count == 1 ? "" : "s",
                e->line_number,
                color, penwidth);
        e = e->next;
    }
    
    //  UPDATED: Add complete legend with ALL return types
    fprintf(dot, "\n  // Legend\n");
    fprintf(dot, "  legend [shape=none, margin=0, label=<\n");
    fprintf(dot, "    <TABLE BORDER=\"1\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n");
   /* fprintf(dot, "      <TR><TD COLSPAN=\"2\" BGCOLOR=\"black\"><FONT COLOR=\"white\"><B>Return Type Legend</B></FONT></TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightblue\">int</TD><TD>Integer (32-bit)</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightcyan\">short</TD><TD>Short integer (16-bit)</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightskyblue\">long</TD><TD>Long integer (64-bit)</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightyellow\">float</TD><TD>Float (32-bit)</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"wheat\">double</TD><TD>Double (64-bit)</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightgreen\">char</TD><TD>Character (8-bit)</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightpink\">bool</TD><TD>Boolean</TD></TR>\n");
    fprintf(dot, "      <TR><TD BGCOLOR=\"lightgray\">void</TD><TD>No return value</TD></TR>\n");*/
    fprintf(dot, "      <TR><TD>Black edge</TD><TD>Called 1-2 times</TD></TR>\n");
    fprintf(dot, "      <TR><TD><FONT COLOR=\"orange\">Orange edge</FONT></TD><TD>Called 3-5 times</TD></TR>\n");
    fprintf(dot, "      <TR><TD><FONT COLOR=\"red\">Red edge</FONT></TD><TD>Called 6+ times</TD></TR>\n");
    fprintf(dot, "    </TABLE>\n");
    fprintf(dot, "  >];\n");
    
    fprintf(dot, "}\n");
    fclose(dot);
    
    
    // Print statistics
    printf("\n=== Call Graph Statistics ===\n");
    printf("Total functions: ");
    int func_count = 0;
    f = func_list;
    while (f) { func_count++; f = f->next; }
    printf("%d\n", func_count);
    
    printf("\nMost called functions:\n");
    FuncMetrics* m = metrics;
    while (m) {
        if (m->call_count > 0) {
            printf("  %s: called %d time%s\n", 
                   m->name, m->call_count, m->call_count == 1 ? "" : "s");
        }
        m = m->next;
    }
    
    printf("\nFunction complexity:\n");
    m = metrics;
    while (m) {
        const char* complexity;
        if (m->tac_lines < 5) complexity = "Simple";
        else if (m->tac_lines < 20) complexity = "Medium";
        else complexity = "Complex";
        
        printf("  %s: %d TAC lines (%s)\n", m->name, m->tac_lines, complexity);
        m = m->next;
    }
    printf("=============================\n");
}

void generateAllImages() {
    system("dot -Tpng tac_flow.dot -o tac_flow.png 2>/dev/null");
    system("dot -Tpng tac_flow_blocks.dot -o tac_flow_blocks.png 2>/dev/null");
    //system("dot -Tpng call_graph.dot -o call_graph.png 2>/dev/null");
    //system("dot -Tpng symbol_table.dot -o symbol_table.png 2>/dev/null");
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Enter your code \n");
        yyin = stdin;
        memset(imcode, 0, sizeof(imcode));
        yyparse();
    } else {
        FILE* f = fopen(argv[1], "r");
        if (!f) {
            printf("Error: Cannot open file %s\n", argv[1]);
            return 1;
        }

        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);
        char* raw = (char*)malloc(fsize + 2);
        if (!raw) { fclose(f); return 1; }
        fread(raw, 1, fsize, f);
        raw[fsize] = '\0';
        fclose(f);

        /* Preprocess:
           - strip carriage returns (\r) so Windows files work
           - ensure a space before and after every '$' so "2$" lexes as NUM + '$'
           - ensure file ends with a newline so flex sees a clean EOF */
        char* clean = (char*)malloc(fsize * 3 + 4);
        if (!clean) { free(raw); return 1; }
        int j = 0;
        for (int i = 0; i < (int)fsize; i++) {
            if (raw[i] == '\r') continue;              /* strip \r */
            if (raw[i] == '$') {
                if (j > 0 && clean[j-1] != ' ' && clean[j-1] != '\t' && clean[j-1] != '\n')
                    clean[j++] = ' ';                  /* space before $ */
                clean[j++] = '$';
                clean[j++] = ' ';                      /* space after $ */
            } else {
                clean[j++] = raw[i];
            }
        }
        if (j == 0 || clean[j-1] != '\n') clean[j++] = '\n'; /* trailing newline */
        clean[j] = '\0';
        free(raw);

        memset(imcode, 0, sizeof(imcode));
        yy_scan_string(clean);  /* feed preprocessed source to lexer */
        yyparse();
        free(clean);
    }

    remove("output.tac");
    if (!e) {
        /* Write TAC file */
        FILE* tac_file = fopen("output.tac", "w");
        if (tac_file) {
            for (int i = 0; i < code; i++)
                fprintf(tac_file, "%s", imcode[i]);
            fclose(tac_file);
        }
       // generateSymbolTableDOT();
        generateTACFlowDOT();
        generateTACFlowWithBlocks();
       // generateCallGraphDOT();
        generateAllImages();
    }
    return 0;
}
