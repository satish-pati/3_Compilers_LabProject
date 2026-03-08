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
    memset(f, 0, sizeof(Function));
    strcpy(f->name, name);
    strcpy(f->return_type, ret_type);
    f->params = NULL;
    f->param_count = 0;
    f->start_label = 0;
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
}*/



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
        // For long, we can't really check with double precision, but we can try
        if (fabs(value) > 9.223372e18) {
            sprintf(err+strlen(err), 
                "Warning: Value %.0f may be out of range for long\n", value);
            has_warning = 1;
        }
    }
    
    return !has_warning;  // Return 1 if no warning, 0 if warning issued
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



#line 1056 "y.tab.c"

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
#line 986 "parser.y"

        char str[1000];
        struct BoolNode* b;
        struct Expr *expr;
        int addr;
        struct Type* type;
        struct Decl* decl;
        struct Subscript* sub;

#line 1241 "y.tab.c"

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
#define YYLAST   1099

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  160
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  306

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
       0,  1042,  1042,  1042,  1060,  1061,  1066,  1072,  1073,  1081,
    1081,  1172,  1182,  1191,  1194,  1194,  1211,  1224,  1237,  1268,
    1284,  1299,  1320,  1321,  1324,  1330,  1338,  1341,  1343,  1343,
    1354,  1356,  1356,  1369,  1373,  1368,  1387,  1387,  1396,  1397,
    1398,  1403,  1411,  1415,  1415,  1429,  1437,  1445,  1454,  1765,
    1770,  1773,  1775,  1787,  1802,  1812,  1829,  1848,  1860,  1876,
    1893,  1908,  1926,  1938,  1951,  1952,  1953,  1954,  1955,  1956,
    1957,  1958,  1959,  1968,  1974,  1981,  1982,  1983,  1984,  1985,
    1986,  1987,  1988,  1989,  1990,  1993,  1998,  2003,  2006,  2009,
    2014,  2019,  2024,  2029,  2034,  2039,  2045,  2053,  2054,  2061,
    2062,  2063,  2064,  2065,  2067,  2179,  2207,  2223,  2239,  2255,
    2275,  2299,  2300,  2301,  2317,  2333,  2349,  2361,  2373,  2389,
    2390,  2393,  2461,  2495,  2498,  2502,  2502,  2502,  2502,  2502,
    2502,  2502,  2502,  2502,  2502,  2504,  2511,  2518,  2525,  2547,
    2591,  2634,  2646,  2684,  2706,  2718,  2719,  2720,  2721,  2724,
    2724,  2726,  2727,  2728,  2729,  2730,  2731,  2732,  2733,  2733,
    2733
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

#define YYPACT_NINF (-137)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-135)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     337,   -38,  -137,    12,   557,  -137,  -137,  -137,  -137,   708,
       4,    25,    31,  -137,   447,  -137,  -137,  -137,  -137,  1036,
      50,  -137,  -137,    58,    64,    83,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,    89,  1036,   767,   100,    32,
    -137,   489,  -137,  -137,   121,    98,    43,   271,  -137,  -137,
     248,   132,   302,  -137,  -137,   651,  -137,   516,   684,  -137,
    -137,    41,   956,   584,  -137,    89,    89,    89,   127,  -137,
     129,  -137,   903,   138,  -137,   625,   113,  -137,  -137,    77,
      46,   625,   116,  -137,   134,   776,   778,   787,   797,   799,
    -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
     808,   849,   870,   872,   881,  -137,    89,   171,    15,   296,
     162,   169,    35,   172,    89,  -137,   174,   405,   625,   684,
    -137,   126,   948,  -137,  -137,  -137,    89,    89,    89,    89,
      89,    89,   684,   139,   985,   992,   999,   180,   191,  -137,
     444,   125,  -137,   121,    89,   184,    33,   166,  -137,  -137,
    -137,  -137,    16,    16,    48,    48,    48,   167,  1026,   115,
    -137,  -137,  1020,  -137,  -137,  -137,  -137,  -137,    89,   136,
     298,  -137,  -137,  -137,  -137,  -137,  -137,  -137,   298,  -137,
    -137,   175,   168,   216,   625,   516,   516,  1020,  1020,  1020,
    1020,  1020,  1020,   250,  -137,   133,   201,   209,  -137,  1036,
    -137,   156,   214,  -137,  -137,   232,   151,   121,    26,  -137,
      13,    89,  -137,  -137,  -137,  -137,  -137,   516,  -137,  -137,
     625,   194,   200,  -137,   216,   625,  -137,  -137,  -137,   233,
     225,    89,  -137,   121,   170,  -137,    89,   411,  -137,   188,
      17,   239,   219,  -137,  -137,  -137,    87,  -137,   186,  -137,
    -137,  -137,   189,   424,   121,  -137,  -137,   516,  -137,  -137,
     625,     8,   260,   196,   212,   202,  1036,   213,    89,  -137,
     651,   272,  -137,   625,  -137,  -137,  -137,   285,  -137,  -137,
      87,   625,  -137,   121,  -137,  -137,   276,   625,  -137,  -137,
     625,  -137,   228,  -137,   287,  -137,  -137,  -137,   -31,  -137,
    -137,   625,  -137,    -2,   625,  -137
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     4,     0,   160,     5,     1,   159,   158,   160,
       0,     0,     0,    97,   160,    97,    64,    65,    67,     0,
       0,   137,    66,     0,     0,     0,    99,   100,   101,   102,
     103,    68,    69,    70,    71,   160,     0,   160,     0,    36,
      97,   160,    97,    40,     0,    97,     0,     0,   119,   120,
       0,     0,     0,    16,    17,   160,    31,   160,   160,    95,
      96,     0,     0,   160,    72,   160,   160,   160,     0,   118,
       0,    19,     0,     0,    38,   160,     0,     6,    74,    54,
       0,   160,     0,    22,     0,   160,   160,   160,   160,   160,
      39,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     160,   160,   160,   160,   160,    75,   160,     0,   157,   157,
     149,   150,   157,     0,   160,   111,     0,     0,   160,   160,
      87,     0,     0,    97,    97,    97,   160,   160,   160,   160,
     160,   160,   160,     0,     0,     0,     0,     0,     0,    18,
     160,    97,     3,     0,   160,     0,    57,     0,    48,    73,
      23,    26,   106,   107,   109,   108,   110,   113,   114,   115,
     116,   117,   104,   112,   155,   156,   149,   150,   160,   142,
     154,   143,   147,   148,   141,   145,   146,   144,   154,   138,
      97,     0,     0,    97,   160,   160,   160,    89,    90,    93,
      94,    92,    91,     0,    97,     0,     0,     0,   105,    13,
     122,   124,     0,    37,    52,    55,     0,     0,   160,    49,
       0,   160,   152,   153,   151,   140,   139,   160,    97,    88,
     160,     0,    85,    86,    97,   160,    14,    20,    21,     0,
       0,   160,   121,     0,    62,    53,   160,    60,   135,     0,
       0,     0,    24,    98,    28,    30,     0,     9,    12,   123,
      56,    63,     0,    51,     0,   136,    97,   160,    98,    97,
     160,     0,     0,     0,    97,     0,    13,    58,   160,    61,
     160,     0,    97,   160,    29,    46,    47,     0,    97,    15,
       0,   160,    11,     0,    50,    33,     0,   160,    27,    97,
     160,    41,    97,    59,     0,    32,    25,    43,    97,    10,
      97,   160,    34,    97,   160,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,  -137,   262,  -137,  -137,    40,   -70,  -137,  -137,
    -137,  -137,  -137,  -137,    39,  -137,  -137,  -137,  -137,    45,
    -136,    86,    -6,   -74,  -137,   -49,   -13,    67,   -51,     1,
    -137,   101,  -137,  -137,  -137,   -45,   153,   -47,  -137
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,    40,    41,   265,   229,    42,   246,   260,
     118,   294,   304,    75,   263,   264,   301,   277,    43,   252,
      80,   146,    44,    45,   106,    61,    81,   259,    46,    47,
      48,   202,   107,   169,    49,   214,   215,   171,    50
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      56,   141,    63,    51,   116,   113,     5,   204,   120,   121,
      52,   149,     6,    64,   133,    62,   -45,   -45,    53,    85,
      86,    87,    88,    89,    87,    88,    89,    76,   275,    78,
      70,   256,     7,     8,   164,   165,    69,     9,    72,    54,
     166,   167,    55,   -45,    82,   -44,   -44,   147,   181,   124,
     125,   276,    51,   123,   164,   165,   117,    83,    62,   122,
     148,    65,   174,   170,    62,   177,   134,   135,   136,    66,
     182,   235,   -44,   124,   125,    67,    21,   100,   101,   102,
      25,   103,   104,   193,   103,   104,   152,   153,   154,   155,
     156,   238,   168,    35,    68,     7,     8,   250,    38,   236,
       9,   157,   158,   159,   160,   161,    74,   162,   207,   208,
     184,   185,   186,    51,   221,   179,   103,   104,   269,    73,
     122,    85,    86,    87,    88,    89,    51,   187,   188,   189,
     190,   191,   192,   122,   261,   262,   222,   223,   183,    21,
      79,   201,    -7,    25,   114,   205,   137,   293,   138,   140,
     242,   194,   143,   144,   145,   245,    35,   142,   124,   125,
     150,    38,    85,    86,    87,    88,    89,   217,   240,   210,
     220,   124,   125,    85,    86,    87,    88,    89,   151,   100,
     219,   225,   175,   103,   104,   163,    62,    62,   180,   176,
     274,   178,   198,   230,    85,    86,    87,    88,    89,   203,
     124,   125,   199,   288,   206,   241,   226,   292,   271,   237,
     209,   244,   239,   211,   218,   227,   298,   296,    62,   285,
     100,   101,   102,   228,   103,   104,   232,   303,   -88,   234,
     243,   231,   201,   125,   305,   103,   104,   253,    85,    86,
      87,    88,    89,   270,   248,   247,   273,   145,   -88,   -88,
     257,   280,   100,   101,   102,   258,   103,   104,    62,   287,
     230,   266,   224,   267,   278,   290,   255,   108,   109,   253,
     279,   117,    84,   110,   111,   281,   297,    85,    86,    87,
      88,    89,   124,   125,   286,    90,   -42,   302,   283,   289,
     295,   112,    91,    92,    93,    94,   100,   101,   102,   300,
     103,   104,   299,    77,   124,   125,   282,   233,    85,    86,
      87,    88,    89,   284,   115,   164,   165,   212,   213,   291,
     251,   172,   173,   166,   167,   272,    95,    96,    97,    98,
      99,   216,   249,     0,     0,   100,   101,   102,     1,   103,
     104,     0,     0,    -2,    -2,     0,     0,   105,    -2,     0,
       0,     0,    -2,    -2,    -2,    -2,    -2,    -2,     0,     0,
       0,     0,    -2,    -2,     0,     0,   100,   101,   102,     0,
     103,   104,    -2,     0,     0,     0,    -2,    -2,    -2,    -2,
      -2,     2,    -2,    -2,     0,     0,     0,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,     0,     0,     0,    -2,     0,     0,    -2,    -2,    -2,
      -2,    85,    86,    87,    88,    89,     0,    85,    86,    87,
      88,    89,     0,     0,     0,     0,    91,    92,    93,    94,
      85,    86,    87,    88,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,    57,     7,     8,     9,   200,     0,    58,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
     101,   102,     0,   103,   104,   100,   101,   102,     0,   103,
     104,   105,     0,     0,    59,    60,   254,     0,   100,   101,
     102,     0,   103,   104,    21,     7,     8,    21,    25,   268,
       9,    25,     0,     0,    10,    11,    12,    13,     0,     0,
       0,    35,     0,     0,    35,     0,    38,     0,     0,    38,
       0,    57,     7,     8,    14,     0,     0,   119,    15,    16,
      17,    18,     0,    -8,    19,    20,     0,     0,     0,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    59,    60,     0,    35,     0,     0,    36,
      37,    38,    39,     7,     8,     0,    21,     0,     9,     0,
      25,     0,    10,    11,    12,    13,     0,     0,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    38,    57,
       7,     8,    14,     0,     0,   132,    15,    16,    17,    18,
       0,     0,    19,    20,     0,     0,     0,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    59,    60,     0,    35,     0,     0,    36,    37,    38,
      39,     7,     8,     0,    21,     0,     9,     0,    25,     0,
      10,    11,    12,    13,     0,     0,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    38,     7,     8,     0,
      14,     0,     9,     0,    15,    16,    17,    18,     0,     0,
      19,    20,     0,     0,     0,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    57,
       7,     8,    35,     0,     0,   119,    37,    38,    39,     0,
       0,    21,     0,     0,     0,    25,    26,    27,    28,    29,
      30,     0,     0,     0,     7,     8,     0,     0,    35,     9,
       0,    59,    60,    38,    16,    17,    18,     0,     0,    19,
       0,     0,     0,     0,    21,    22,     0,     0,    25,     0,
       0,     0,     0,     0,    31,    32,    33,    34,    16,    17,
      18,    35,     0,    19,     0,     0,    38,     0,    21,    22,
       0,     0,    25,     0,     0,     0,     0,     0,    31,    32,
      33,    34,     0,     7,     8,    35,     0,     0,     9,     0,
      38,    71,     7,     8,     7,     8,     0,     9,     0,     9,
    -125,     0,  -126,     7,     8,     0,     0,     0,     9,     0,
       0,  -128,     0,     7,     8,     7,     8,     0,     9,     0,
       9,  -127,     0,  -129,     7,     8,     0,    21,     0,     9,
       0,    25,  -130,     0,     0,     0,    21,     0,    21,     0,
      25,     0,    25,     0,    35,     0,     0,    21,     0,    38,
       0,    25,     0,    35,     0,    35,     0,    21,    38,    21,
      38,    25,     0,    25,    35,     7,     8,     0,    21,    38,
       9,     0,    25,  -131,    35,     0,    35,     0,     0,    38,
       0,    38,     0,     0,     0,    35,     7,     8,     7,     8,
      38,     9,     0,     9,  -132,     0,  -133,     7,     8,     0,
       0,     0,     9,     0,     0,  -134,     0,     0,     0,    21,
       0,     0,     0,    25,     0,     0,     0,     0,     0,    85,
      86,    87,    88,    89,     0,     0,    35,   139,     0,     0,
      21,    38,    21,     0,    25,     0,    25,     0,     0,     0,
       0,    21,     0,     0,     0,    25,     0,    35,     0,    35,
       0,     0,    38,     0,    38,     0,     0,     0,    35,     0,
       0,     0,     0,    38,    85,    86,    87,    88,    89,     0,
     115,     0,    85,    86,    87,    88,    89,   100,   101,   102,
       0,   103,   104,     0,     0,   126,   127,   128,   129,   130,
       0,     0,   131,   126,   127,   128,   129,   130,     0,     0,
     131,    85,    86,    87,    88,    89,     0,   195,    85,    86,
      87,    88,    89,     0,   196,    85,    86,    87,    88,    89,
       0,   197,   100,   101,   102,     0,   103,   104,     0,     0,
     100,   101,   102,     0,   103,   104,    85,    86,    87,    88,
      89,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
     101,   102,     0,   103,   104,     0,   100,   101,   102,     0,
     103,   104,     0,   100,   101,   102,     0,   103,   104,     0,
       0,     0,     0,     0,     0,     0,    16,    17,    18,     0,
       0,    19,     0,     0,   100,   101,   102,    22,   103,   104,
     100,     0,   102,     0,   103,   104,    31,    32,    33,    34
};

static const yytype_int16 yycheck[] =
{
      13,    75,    15,     9,    55,    50,    44,   143,    57,    58,
       9,    81,     0,    19,    63,    14,    47,    48,    14,     6,
       7,     8,     9,    10,     8,     9,    10,    40,    20,    42,
      36,    14,     6,     7,    19,    20,    35,    11,    37,    14,
      25,    26,    11,    74,     1,    47,    48,     1,   118,    32,
      33,    43,    58,    12,    19,    20,    55,    14,    57,    58,
      14,    11,   109,   108,    63,   112,    65,    66,    67,    11,
     119,   207,    74,    32,    33,    11,    50,    64,    65,    66,
      54,    68,    69,   132,    68,    69,    85,    86,    87,    88,
      89,    78,    77,    67,    11,     6,     7,   233,    72,    73,
      11,   100,   101,   102,   103,   104,    74,   106,    75,    76,
     123,   124,   125,   119,   184,   114,    68,    69,   254,    19,
     119,     6,     7,     8,     9,    10,   132,   126,   127,   128,
     129,   130,   131,   132,    47,    48,   185,   186,    12,    50,
      19,   140,    44,    54,    12,   144,    19,   283,    19,    11,
     220,    12,    75,    76,    77,   225,    67,    44,    32,    33,
      44,    72,     6,     7,     8,     9,    10,   180,   217,   168,
     183,    32,    33,     6,     7,     8,     9,    10,    44,    64,
      12,   194,    20,    68,    69,    14,   185,   186,    14,    20,
     260,    19,    12,   199,     6,     7,     8,     9,    10,    74,
      32,    33,    11,   273,    20,   218,    73,   281,   257,   208,
      44,   224,   211,    77,    39,    14,   290,   287,   217,   270,
      64,    65,    66,    14,    68,    69,    12,   301,    12,    78,
      36,    75,   231,    33,   304,    68,    69,   236,     6,     7,
       8,     9,    10,   256,    19,    12,   259,    77,    32,    33,
      11,   264,    64,    65,    66,    36,    68,    69,   257,   272,
     266,    75,    12,    74,     4,   278,    78,    19,    20,   268,
      74,   270,     1,    25,    26,    73,   289,     6,     7,     8,
       9,    10,    32,    33,    12,    14,    74,   300,    75,     4,
      14,    43,    21,    22,    23,    24,    64,    65,    66,    12,
      68,    69,    74,    41,    32,    33,   266,    75,     6,     7,
       8,     9,    10,   268,    12,    19,    20,    19,    20,   280,
     234,    25,    26,    25,    26,   258,    55,    56,    57,    58,
      59,   178,   231,    -1,    -1,    64,    65,    66,     1,    68,
      69,    -1,    -1,     6,     7,    -1,    -1,    76,    11,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    64,    65,    66,    -1,
      68,    69,    35,    -1,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    -1,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    67,    -1,    -1,    70,    71,    72,
      73,     6,     7,     8,     9,    10,    -1,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    -1,    21,    22,    23,    24,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       6,     7,     5,     6,     7,    11,    12,    -1,    11,    -1,
      55,    56,    57,    58,    59,    -1,    -1,    -1,    -1,    64,
      65,    66,    -1,    68,    69,    64,    65,    66,    -1,    68,
      69,    76,    -1,    -1,    37,    38,    75,    -1,    64,    65,
      66,    -1,    68,    69,    50,     6,     7,    50,    54,    75,
      11,    54,    -1,    -1,    15,    16,    17,    18,    -1,    -1,
      -1,    67,    -1,    -1,    67,    -1,    72,    -1,    -1,    72,
      -1,     5,     6,     7,    35,    -1,    -1,    11,    39,    40,
      41,    42,    -1,    44,    45,    46,    -1,    -1,    -1,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    37,    38,    -1,    67,    -1,    -1,    70,
      71,    72,    73,     6,     7,    -1,    50,    -1,    11,    -1,
      54,    -1,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    72,     5,
       6,     7,    35,    -1,    -1,    11,    39,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    37,    38,    -1,    67,    -1,    -1,    70,    71,    72,
      73,     6,     7,    -1,    50,    -1,    11,    -1,    54,    -1,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    72,     6,     7,    -1,
      35,    -1,    11,    -1,    39,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,     5,
       6,     7,    67,    -1,    -1,    11,    71,    72,    73,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    56,    57,    58,
      59,    -1,    -1,    -1,     6,     7,    -1,    -1,    67,    11,
      -1,    37,    38,    72,    40,    41,    42,    -1,    -1,    45,
      -1,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    60,    61,    62,    63,    40,    41,
      42,    67,    -1,    45,    -1,    -1,    72,    -1,    50,    51,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,     6,     7,    67,    -1,    -1,    11,    -1,
      72,    14,     6,     7,     6,     7,    -1,    11,    -1,    11,
      14,    -1,    14,     6,     7,    -1,    -1,    -1,    11,    -1,
      -1,    14,    -1,     6,     7,     6,     7,    -1,    11,    -1,
      11,    14,    -1,    14,     6,     7,    -1,    50,    -1,    11,
      -1,    54,    14,    -1,    -1,    -1,    50,    -1,    50,    -1,
      54,    -1,    54,    -1,    67,    -1,    -1,    50,    -1,    72,
      -1,    54,    -1,    67,    -1,    67,    -1,    50,    72,    50,
      72,    54,    -1,    54,    67,     6,     7,    -1,    50,    72,
      11,    -1,    54,    14,    67,    -1,    67,    -1,    -1,    72,
      -1,    72,    -1,    -1,    -1,    67,     6,     7,     6,     7,
      72,    11,    -1,    11,    14,    -1,    14,     6,     7,    -1,
      -1,    -1,    11,    -1,    -1,    14,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    10,    -1,    -1,    67,    14,    -1,    -1,
      50,    72,    50,    -1,    54,    -1,    54,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    67,    -1,    67,
      -1,    -1,    72,    -1,    72,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    72,     6,     7,     8,     9,    10,    -1,
      12,    -1,     6,     7,     8,     9,    10,    64,    65,    66,
      -1,    68,    69,    -1,    -1,    27,    28,    29,    30,    31,
      -1,    -1,    34,    27,    28,    29,    30,    31,    -1,    -1,
      34,     6,     7,     8,     9,    10,    -1,    12,     6,     7,
       8,     9,    10,    -1,    12,     6,     7,     8,     9,    10,
      -1,    12,    64,    65,    66,    -1,    68,    69,    -1,    -1,
      64,    65,    66,    -1,    68,    69,     6,     7,     8,     9,
      10,    -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    -1,    68,    69,    -1,    64,    65,    66,    -1,
      68,    69,    -1,    64,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    -1,    -1,    64,    65,    66,    51,    68,    69,
      64,    -1,    66,    -1,    68,    69,    60,    61,    62,    63
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
     117,   101,   108,    14,    14,    11,   105,     5,    11,    37,
      38,   104,   108,   105,   101,    11,    11,    11,    11,   108,
     101,    14,   108,    19,    74,    92,   105,    82,   105,    19,
      99,   105,     1,    14,     1,     6,     7,     8,     9,    10,
      14,    21,    22,    23,    24,    55,    56,    57,    58,    59,
      64,    65,    66,    68,    69,    76,   103,   111,    19,    20,
      25,    26,    43,   114,    12,    12,   107,   108,    89,    11,
     104,   104,   108,    12,    32,    33,    27,    28,    29,    30,
      31,    34,    11,   104,   108,   108,   108,    19,    19,    14,
      11,   102,    44,    75,    76,    77,   100,     1,    14,    86,
      44,    44,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,    14,    19,    20,    25,    26,    77,   112,
     114,   116,    25,    26,   116,    20,    20,   116,    19,   108,
      14,    86,   104,    12,   105,   105,   105,   108,   108,   108,
     108,   108,   108,   104,    12,    12,    12,    12,    12,    11,
      12,   108,   110,    74,    99,   108,    20,    75,    76,    44,
     108,    77,    19,    20,   114,   115,   115,   105,    39,    12,
     105,    86,   104,   104,    12,   105,    73,    14,    14,    85,
     101,    75,    12,    75,    78,    99,    73,   108,    78,   108,
     104,   105,    86,    36,   105,    86,    87,    12,    19,   110,
      99,   100,    98,   108,    75,    78,    14,    11,    36,   106,
      88,    47,    48,    93,    94,    84,    75,    74,    75,    99,
     105,   104,   106,   105,    86,    20,    43,    96,     4,    74,
     105,    73,    85,    75,    98,   107,    12,   105,    86,     4,
     105,    93,   102,    99,    90,    14,    86,   105,   102,    74,
      12,    95,   105,   102,    91,    86
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    79,    81,    80,    80,    80,    82,    82,    82,    84,
      83,    85,    85,    85,    87,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    88,    86,
      86,    89,    86,    90,    91,    86,    92,    86,    86,    86,
      86,    93,    93,    95,    94,    94,    96,    96,    97,    97,
      98,    98,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,   100,   100,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   102,   102,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   105,   106,   107,
     107,   107,   107,   107,   107,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   109,   109,   110,   110,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   112,   112,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   114,
     114,   115,   115,   115,   115,   116,   116,   116,   117,   117,
     117
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     4,     1,     2,     2,     1,     1,     0,
      10,     4,     2,     0,     0,     8,     2,     2,     3,     2,
       5,     5,     2,     3,     6,    10,     3,     9,     0,     8,
       6,     0,    10,     0,     0,    14,     0,     4,     2,     2,
       1,     3,     1,     0,     6,     4,     1,     1,     3,     4,
       3,     1,     3,     4,     1,     3,     5,     2,     6,     8,
       4,     6,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     2,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     0,     0,     1,
       1,     1,     1,     1,     3,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     1,
       1,     5,     4,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     1,     4,     4,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     0,     1,     1,     0,     1,     1,
       0
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
#line 1042 "parser.y"
        {top = create_env(top,0);}
#line 2704 "y.tab.c"
    break;

  case 3: /* S: $@1 PROGRAM M MEOF  */
#line 1042 "parser.y"
                                                 {
        if (e){
                        printf("%s\nRejected \n%s \nCould not generate Three Address Code / Storage Layout\n",buffer,err);
                        e=0;err[0]="\0";buffer[0]='\0';}
                else {
                        backpatch((yyvsp[-2].b)->N,(yyvsp[-1].addr));
                                                printf("Source code :\n");
                        printf("%s\nAccepted -> Generated Three Address code:\n",buffer);
                        for (int i=0;i<code;i++){
                                printf("%s",imcode[i]);
                        }

                            if (strlen(err) > 0) {
        printf("\n=== Warnings ===\n%s", err);
    }

                        print_all_envs(top);
                }YYACCEPT;}
#line 2727 "y.tab.c"
    break;

  case 4: /* S: MEOF  */
#line 1060 "parser.y"
              {YYACCEPT;}
#line 2733 "y.tab.c"
    break;

  case 5: /* S: error MEOF  */
#line 1061 "parser.y"
                    {e=1;strcpy(err,"Invalid Statements");
                printf("%s \nRejected -> %s \nCould not generate Three Address Code / Storage Layout\n",buffer,err);
                YYACCEPT;}
#line 2741 "y.tab.c"
    break;

  case 6: /* PROGRAM: FUNDECL PROGRAM  */
#line 1066 "parser.y"
                         {
            if(!e){
                (yyval.b) = createBoolNode();
                (yyval.b)->N = merge((yyvsp[-1].b)->N, (yyvsp[0].b)->N);
            }
        }
#line 2752 "y.tab.c"
    break;

  case 7: /* PROGRAM: STMNTS  */
#line 1072 "parser.y"
                 {(yyval.b) = (yyvsp[0].b);}
#line 2758 "y.tab.c"
    break;

  case 8: /* PROGRAM: FUNDECL  */
#line 1073 "parser.y"
                  {
            if(!e){
                (yyval.b) = createBoolNode();
                (yyval.b)->N = (yyvsp[0].b)->N;
            }
        }
#line 2769 "y.tab.c"
    break;

  case 9: /* $@2: %empty  */
#line 1081 "parser.y"
                                              {
    if(!e){
        /*"" duplicate parameter check "" */
        if(checkDuplicateParams((yyvsp[-1].decl))){
            e = 1;
            sprintf(err+strlen(err), "Duplicate parameter names in function %s\n", (yyvsp[-3].str));
        }

        //Function* f = createFunction($3, $2->str);
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

        /*"" set current function context for return checking "" */
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
#line 2841 "y.tab.c"
    break;

  case 10: /* FUNDECL: FUNCTION TYPE IDEN '(' PARAMLIST ')' $@2 '{' STMNTS '}'  */
#line 1147 "parser.y"
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

        /*"" reset function context "" */
        in_function = 0;
        current_function[0] = '\0';
        current_return_type[0] = '\0';
    }
}
#line 2870 "y.tab.c"
    break;

  case 11: /* PARAMLIST: TYPE IDEN ',' PARAMLIST  */
#line 1172 "parser.y"
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
#line 2885 "y.tab.c"
    break;

  case 12: /* PARAMLIST: TYPE IDEN  */
#line 1182 "parser.y"
                    {
            if(!e){
                (yyval.decl) = createDecl((yyvsp[0].str));
                char clean_type[100];
                strcpy(clean_type, (yyvsp[-1].type)->str);
                if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));
                strcpy((yyval.decl)->type, clean_type);
            }
        }
#line 2899 "y.tab.c"
    break;

  case 13: /* PARAMLIST: %empty  */
#line 1191 "parser.y"
          {(yyval.decl) = NULL;}
#line 2905 "y.tab.c"
    break;

  case 14: /* $@3: %empty  */
#line 1194 "parser.y"
                            { 
       if(!e) {
           strcpy(current_switch_var, (yyvsp[-2].expr)->str);
           saveoffset = offset;
           /*"" track switch depth "" */
           switch_depth++;
       }
   }
#line 2918 "y.tab.c"
    break;

  case 15: /* A: SWITCH '(' EXPR ')' '{' $@3 CASE_LIST '}'  */
#line 1201 "parser.y"
                   {
       if(!e) {
           (yyval.b) = createBoolNode();
           backpatch((yyvsp[-1].b)->N, code);
           backpatch((yyvsp[-1].b)->B, code);
           offset = saveoffset;
           /*   restore switch depth  */
           switch_depth--;
       }
   }
#line 2933 "y.tab.c"
    break;

  case 16: /* A: BREAK '$'  */
#line 1211 "parser.y"
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
#line 2951 "y.tab.c"
    break;

  case 17: /* A: CONTINUE '$'  */
#line 1224 "parser.y"
               {
    if (!e) {
        /*  continue must be inside a loop  */
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
#line 2969 "y.tab.c"
    break;

  case 18: /* A: RETURN EXPR '$'  */
#line 1237 "parser.y"
                  {
    if(!e){
        has_return_statement = 1;
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
#line 3005 "y.tab.c"
    break;

  case 19: /* A: RETURN '$'  */
#line 1268 "parser.y"
             {
    if(!e){
        has_return_statement = 1;
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
#line 3026 "y.tab.c"
    break;

  case 20: /* A: PRINT '(' EXPR ')' '$'  */
#line 1284 "parser.y"
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
#line 3046 "y.tab.c"
    break;

  case 21: /* A: INPUT '(' EXPR ')' '$'  */
#line 1299 "parser.y"
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
#line 3072 "y.tab.c"
    break;

  case 22: /* A: ASNEXPR '$'  */
#line 1320 "parser.y"
              {if (!e){(yyval.b) = (yyvsp[-1].b);}}
#line 3078 "y.tab.c"
    break;

  case 23: /* A: ASNEXPR error MEOF  */
#line 1321 "parser.y"
                            {strcat(err,"$ missing\n");yyerrok;e=1;
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT;}
#line 3086 "y.tab.c"
    break;

  case 24: /* A: IF '(' BOOLEXPR ')' M A  */
#line 1324 "parser.y"
                                   {if (!e){backpatch((yyvsp[-3].b)->T,(yyvsp[-1].addr));
                                                                                (yyval.b) = createBoolNode();
                                                                                (yyval.b)->N = merge((yyvsp[-3].b)->F,(yyvsp[0].b)->N);
                                                                                (yyval.b)->B = (yyvsp[0].b)->B;
                                                                                (yyval.b)->C = (yyvsp[0].b)->C;
                                                                                }}
#line 3097 "y.tab.c"
    break;

  case 25: /* A: IF '(' BOOLEXPR ')' M A ELSE NN M A  */
#line 1330 "parser.y"
                                              {if (!e){
                backpatch((yyvsp[-7].b)->T,(yyvsp[-5].addr));
                backpatch((yyvsp[-7].b)->F,(yyvsp[-1].addr));
                (yyval.b) = createBoolNode();
                (yyval.b)->N = merge(merge((yyvsp[-4].b)->N,(yyvsp[-2].b)->N),(yyvsp[0].b)->N);
                (yyval.b)->B = merge((yyvsp[-4].b)->B, (yyvsp[0].b)->B);
                (yyval.b)->C = merge((yyvsp[-4].b)->C, (yyvsp[0].b)->C);
        }}
#line 3110 "y.tab.c"
    break;

  case 26: /* A: EXPR error MEOF  */
#line 1338 "parser.y"
                         {{strcat(err,"$ missing");yyerrok;e=1;}
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT;}
#line 3118 "y.tab.c"
    break;

  case 27: /* A: IF BOOLEXPR ')' M A ELSE NN M A  */
#line 1341 "parser.y"
                                         {{strcat(err,"missing (\n");e=1;}}
#line 3124 "y.tab.c"
    break;

  case 28: /* $@4: %empty  */
#line 1343 "parser.y"
                                     {loop_depth++;}
#line 3130 "y.tab.c"
    break;

  case 29: /* A: WHILE M '(' BOOLEXPR ')' M $@4 A  */
#line 1343 "parser.y"
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
#line 3146 "y.tab.c"
    break;

  case 30: /* A: WHILE M BOOLEXPR ')' M A  */
#line 1354 "parser.y"
                                   {{strcat(err,"missing (\n");e=1;}}
#line 3152 "y.tab.c"
    break;

  case 31: /* $@5: %empty  */
#line 1356 "parser.y"
               {loop_depth++;}
#line 3158 "y.tab.c"
    break;

  case 32: /* A: DO M $@5 A WHILE M '(' BOOLEXPR ')' '$'  */
#line 1356 "parser.y"
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
#line 3175 "y.tab.c"
    break;

  case 33: /* $@6: %empty  */
#line 1369 "parser.y"
    { 
        sprintf(imcode[code], "%d goto %d\n", code, (yyvsp[-4].addr)); 
        code++; 
    }
#line 3184 "y.tab.c"
    break;

  case 34: /* $@7: %empty  */
#line 1373 "parser.y"
        {loop_depth++;}
#line 3190 "y.tab.c"
    break;

  case 35: /* A: FOR '(' ASNEXPR '$' M BOOLEXPR '$' M ASNEXPR $@6 ')' M $@7 A  */
#line 1374 "parser.y"
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
#line 3208 "y.tab.c"
    break;

  case 36: /* $@8: %empty  */
#line 1387 "parser.y"
     {top = create_env(top,offset);offset=0;}
#line 3214 "y.tab.c"
    break;

  case 37: /* A: '{' $@8 STMNTS '}'  */
#line 1387 "parser.y"
                                                         {if (!e) {
                                                (yyval.b) = createBoolNode();
                                                (yyval.b)->N = (yyvsp[-1].b)->N;
                                                (yyval.b)->B = (yyvsp[-1].b)->B;
                                                (yyval.b)->C = (yyvsp[-1].b)->C;
                                                top = top->prev;
                                                if (!top) offset =0;
                                                else offset = top->prev_offset;
                                                }}
#line 3228 "y.tab.c"
    break;

  case 38: /* A: '{' '}'  */
#line 1396 "parser.y"
                  {if (!e){(yyval.b)=createBoolNode();}}
#line 3234 "y.tab.c"
    break;

  case 39: /* A: EXPR '$'  */
#line 1397 "parser.y"
                  {if (!e) {(yyval.b)=createBoolNode();}}
#line 3240 "y.tab.c"
    break;

  case 40: /* A: DECLSTATEMENT  */
#line 1398 "parser.y"
                        {
            if (!e){(yyval.b)=createBoolNode();}
            }
#line 3248 "y.tab.c"
    break;

  case 41: /* CASE_LIST: CASE_ITEM M CASE_LIST  */
#line 1403 "parser.y"
                                 {
       if(!e) {
           backpatch((yyvsp[-2].b)->N, (yyvsp[-1].addr));
           (yyval.b) = createBoolNode();
           (yyval.b)->N = (yyvsp[0].b)->N;
           (yyval.b)->B = merge((yyvsp[-2].b)->B, (yyvsp[0].b)->B);
       }
   }
#line 3261 "y.tab.c"
    break;

  case 42: /* CASE_LIST: CASE_ITEM  */
#line 1411 "parser.y"
               {
       if(!e) { (yyval.b) = (yyvsp[0].b); }
   }
#line 3269 "y.tab.c"
    break;

  case 43: /* @9: %empty  */
#line 1415 "parser.y"
                                   {
       if(!e) {
           sprintf(imcode[code], "%d if %s != %s goto ", code, 
                   current_switch_var, (yyvsp[-2].expr)->str);
           (yyval.addr) = code;
           code++;
       }
   }
#line 3282 "y.tab.c"
    break;

  case 44: /* CASE_ITEM: CASE CASE_EXPR ':' M @9 STMNTS  */
#line 1422 "parser.y"
            {
       if(!e) {
           (yyval.b) = createBoolNode();
           (yyval.b)->N = createNode((yyvsp[-1].addr));
           (yyval.b)->B = (yyvsp[0].b)->B;
       }
   }
#line 3294 "y.tab.c"
    break;

  case 45: /* CASE_ITEM: DEFAULT ':' M STMNTS  */
#line 1429 "parser.y"
                          {
       if(!e) {
           (yyval.b) = createBoolNode();
           (yyval.b)->N = NULL;
           (yyval.b)->B = (yyvsp[0].b)->B;
       }
   }
#line 3306 "y.tab.c"
    break;

  case 46: /* CASE_EXPR: NUM  */
#line 1437 "parser.y"
                  {
       if(!e) {
           (yyval.expr) = createExpr();
           strcpy((yyval.expr)->str, (yyvsp[0].str));
           if(checkfloat((yyvsp[0].str))) strcpy((yyval.expr)->type, "float");
           else strcpy((yyval.expr)->type, "int");
       }
   }
#line 3319 "y.tab.c"
    break;

  case 47: /* CASE_EXPR: CHARR  */
#line 1445 "parser.y"
           {
       if(!e) {
           (yyval.expr) = createExpr();
           strcpy((yyval.expr)->str, (yyvsp[0].str));
           strcpy((yyval.expr)->type, "char");
       }
   }
#line 3331 "y.tab.c"
    break;

  case 48: /* DECLSTATEMENT: TYPE DECLLIST '$'  */
#line 1454 "parser.y"
                                 {
    if(!e){
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

                /*  Check each array element for range overflow */
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
                    /*  Enhanced type checking and range validation */
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
}
#line 3647 "y.tab.c"
    break;

  case 49: /* DECLSTATEMENT: TYPE DECLLIST error MEOF  */
#line 1765 "parser.y"
                           {{strcat(err,"$ missing\n");yyerrok;e=1;}
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT; }
#line 3655 "y.tab.c"
    break;

  case 50: /* INIT_LIST: EXPR ',' INIT_LIST  */
#line 1770 "parser.y"
                              {
        if(!e){ (yyval.expr) = (yyvsp[-2].expr); (yyval.expr)->next = (yyvsp[0].expr); }
    }
#line 3663 "y.tab.c"
    break;

  case 51: /* INIT_LIST: EXPR  */
#line 1773 "parser.y"
           { if(!e){ (yyval.expr) = (yyvsp[0].expr); (yyval.expr)->next = NULL; } }
#line 3669 "y.tab.c"
    break;

  case 52: /* DECLLIST: IDEN ',' DECLLIST  */
#line 1775 "parser.y"
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
#line 3686 "y.tab.c"
    break;

  case 53: /* DECLLIST: IDEN INDEX ',' DECLLIST  */
#line 1787 "parser.y"
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
#line 3706 "y.tab.c"
    break;

  case 54: /* DECLLIST: IDEN  */
#line 1802 "parser.y"
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
#line 3721 "y.tab.c"
    break;

  case 55: /* DECLLIST: IDEN '=' EXPR  */
#line 1812 "parser.y"
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
#line 3743 "y.tab.c"
    break;

  case 56: /* DECLLIST: IDEN '=' EXPR ',' DECLLIST  */
#line 1829 "parser.y"
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
#line 3767 "y.tab.c"
    break;

  case 57: /* DECLLIST: IDEN INDEX  */
#line 1848 "parser.y"
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
#line 3784 "y.tab.c"
    break;

  case 58: /* DECLLIST: IDEN INDEX '=' '{' INIT_LIST '}'  */
#line 1860 "parser.y"
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
#line 3805 "y.tab.c"
    break;

  case 59: /* DECLLIST: IDEN INDEX '=' '{' INIT_LIST '}' ',' DECLLIST  */
#line 1876 "parser.y"
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
#line 3827 "y.tab.c"
    break;

  case 60: /* DECLLIST: IDEN INDEX '=' EXPR  */
#line 1893 "parser.y"
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
#line 3847 "y.tab.c"
    break;

  case 61: /* DECLLIST: IDEN INDEX '=' EXPR ',' DECLLIST  */
#line 1908 "parser.y"
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
#line 3868 "y.tab.c"
    break;

  case 62: /* INDEX: '[' NUM ']'  */
#line 1926 "parser.y"
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
#line 3885 "y.tab.c"
    break;

  case 63: /* INDEX: '[' NUM ']' INDEX  */
#line 1938 "parser.y"
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
#line 3902 "y.tab.c"
    break;

  case 64: /* TYPE: INT  */
#line 1951 "parser.y"
          {(yyval.type) = createType(); strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=4;}
#line 3908 "y.tab.c"
    break;

  case 65: /* TYPE: FLOAT  */
#line 1952 "parser.y"
                 {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=4;}
#line 3914 "y.tab.c"
    break;

  case 66: /* TYPE: BOOL  */
#line 1953 "parser.y"
           {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=1;}
#line 3920 "y.tab.c"
    break;

  case 67: /* TYPE: CHAR  */
#line 1954 "parser.y"
               {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=1;}
#line 3926 "y.tab.c"
    break;

  case 68: /* TYPE: SHORT  */
#line 1955 "parser.y"
            {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=2;}
#line 3932 "y.tab.c"
    break;

  case 69: /* TYPE: LONG  */
#line 1956 "parser.y"
           {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=8;}
#line 3938 "y.tab.c"
    break;

  case 70: /* TYPE: DOUBLE  */
#line 1957 "parser.y"
             {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=8;}
#line 3944 "y.tab.c"
    break;

  case 71: /* TYPE: VOID  */
#line 1958 "parser.y"
           {(yyval.type) = createType();strcpy((yyval.type)->str,(yyvsp[0].str));(yyval.type)->size=0;}
#line 3950 "y.tab.c"
    break;

  case 72: /* TYPE: CONST TYPE  */
#line 1959 "parser.y"
                 {
    (yyval.type) = (yyvsp[0].type);
    char base[100];
    strcpy(base, (yyvsp[0].type)->str);
    if (base[0] == '@') memmove(base, base+1, strlen(base));
    sprintf((yyval.type)->str, "const %s", base);
}
#line 3962 "y.tab.c"
    break;

  case 73: /* STMNTS: STMNTS M A  */
#line 1968 "parser.y"
                   {if (!e){backpatch((yyvsp[-2].b)->N,(yyvsp[-1].addr));
                                        (yyval.b) = createBoolNode();
                                        (yyval.b)->N = (yyvsp[0].b)->N;
                                        (yyval.b)->B = merge((yyvsp[-2].b)->B, (yyvsp[0].b)->B);
                                        (yyval.b)->C = merge((yyvsp[-2].b)->C, (yyvsp[0].b)->C);
}}
#line 3973 "y.tab.c"
    break;

  case 74: /* STMNTS: A M  */
#line 1974 "parser.y"
             {if (!e){(yyval.b) = createBoolNode();
                (yyval.b)->N = (yyvsp[-1].b)->N;
                (yyval.b)->B = (yyvsp[-1].b)->B;
                (yyval.b)->C = (yyvsp[-1].b)->C;
}}
#line 3983 "y.tab.c"
    break;

  case 75: /* ASSGN: '='  */
#line 1981 "parser.y"
           {strcpy((yyval.str),"=");}
#line 3989 "y.tab.c"
    break;

  case 76: /* ASSGN: PASN  */
#line 1982 "parser.y"
                {strcpy((yyval.str),(yyvsp[0].str));}
#line 3995 "y.tab.c"
    break;

  case 77: /* ASSGN: MASN  */
#line 1983 "parser.y"
                {strcpy((yyval.str),(yyvsp[0].str));}
#line 4001 "y.tab.c"
    break;

  case 78: /* ASSGN: DASN  */
#line 1984 "parser.y"
            {strcpy((yyval.str),(yyvsp[0].str));}
#line 4007 "y.tab.c"
    break;

  case 79: /* ASSGN: SASN  */
#line 1985 "parser.y"
            {strcpy((yyval.str),(yyvsp[0].str));}
#line 4013 "y.tab.c"
    break;

  case 80: /* ASSGN: BANDASN  */
#line 1986 "parser.y"
               {strcpy((yyval.str),"&=");}
#line 4019 "y.tab.c"
    break;

  case 81: /* ASSGN: BORASN  */
#line 1987 "parser.y"
               {strcpy((yyval.str),"|=");}
#line 4025 "y.tab.c"
    break;

  case 82: /* ASSGN: BXORASN  */
#line 1988 "parser.y"
               {strcpy((yyval.str),"^=");}
#line 4031 "y.tab.c"
    break;

  case 83: /* ASSGN: LSHIFTASN  */
#line 1989 "parser.y"
                 {strcpy((yyval.str),"<<=");}
#line 4037 "y.tab.c"
    break;

  case 84: /* ASSGN: RSHIFTASN  */
#line 1990 "parser.y"
                 {strcpy((yyval.str),">>=");}
#line 4043 "y.tab.c"
    break;

  case 85: /* BOOLEXPR: BOOLEXPR OR M BOOLEXPR  */
#line 1993 "parser.y"
                                {  if (!e){backpatch((yyvsp[-3].b)->F,(yyvsp[-1].addr));
                                                                (yyval.b) = createBoolNode();
                                                                (yyval.b)->T = merge((yyvsp[-3].b)->T,(yyvsp[0].b)->T);
                                                                (yyval.b)->F = (yyvsp[0].b)->F;
                                                         }}
#line 4053 "y.tab.c"
    break;

  case 86: /* BOOLEXPR: BOOLEXPR AND M BOOLEXPR  */
#line 1998 "parser.y"
                              { if (!e){backpatch((yyvsp[-3].b)->T,(yyvsp[-1].addr));
                                                                (yyval.b) = createBoolNode();
                                                                (yyval.b)->T = (yyvsp[0].b)->T;
                                                                (yyval.b)->F = merge((yyvsp[-3].b)->F,(yyvsp[0].b)->F);
                                                                }}
#line 4063 "y.tab.c"
    break;

  case 87: /* BOOLEXPR: '!' BOOLEXPR  */
#line 2003 "parser.y"
                       {
                if (!e){ (yyval.b) = createBoolNode(); (yyval.b)->T = (yyvsp[0].b)->F; (yyval.b)->F = (yyvsp[0].b)->T; }
        }
#line 4071 "y.tab.c"
    break;

  case 88: /* BOOLEXPR: '(' BOOLEXPR ')'  */
#line 2006 "parser.y"
                           {
                if (!e){ (yyval.b) = createBoolNode(); (yyval.b)->T = (yyvsp[-1].b)->T; (yyval.b)->F = (yyvsp[-1].b)->F; }
        }
#line 4079 "y.tab.c"
    break;

  case 89: /* BOOLEXPR: EXPR LT EXPR  */
#line 2009 "parser.y"
                        {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 4089 "y.tab.c"
    break;

  case 90: /* BOOLEXPR: EXPR GT EXPR  */
#line 2014 "parser.y"
                    {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 4099 "y.tab.c"
    break;

  case 91: /* BOOLEXPR: EXPR EQ EXPR  */
#line 2019 "parser.y"
                        {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 4109 "y.tab.c"
    break;

  case 92: /* BOOLEXPR: EXPR NE EXPR  */
#line 2024 "parser.y"
                    {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 4119 "y.tab.c"
    break;

  case 93: /* BOOLEXPR: EXPR LE EXPR  */
#line 2029 "parser.y"
                        {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 4129 "y.tab.c"
    break;

  case 94: /* BOOLEXPR: EXPR GE EXPR  */
#line 2034 "parser.y"
                    {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,(yyvsp[-2].expr)->str,(yyvsp[-1].str),(yyvsp[0].expr)->str);
                                                        (yyval.b) = createBoolNode();
                                                        (yyval.b)->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        (yyval.b)->F = createNode(code); code++;}}
#line 4139 "y.tab.c"
    break;

  case 95: /* BOOLEXPR: TR  */
#line 2039 "parser.y"
             {if (!e){
                (yyval.b) = createBoolNode();
                (yyval.b)->T = createNode(code);
                sprintf(imcode[code],"%d goto ",code);
                code++;
        }}
#line 4150 "y.tab.c"
    break;

  case 96: /* BOOLEXPR: FL  */
#line 2045 "parser.y"
             {if (!e){
                (yyval.b) = createBoolNode();
                (yyval.b)->F = createNode(code);
                sprintf(imcode[code],"%d goto ",code);
                code++;
        }}
#line 4161 "y.tab.c"
    break;

  case 97: /* M: %empty  */
#line 2053 "parser.y"
   {(yyval.addr)=code;}
#line 4167 "y.tab.c"
    break;

  case 98: /* NN: %empty  */
#line 2054 "parser.y"
    {(yyval.b)=createBoolNode();
        (yyval.b)->N = createNode(code);
        sprintf(imcode[code],"%d goto ",code);
        code++;
        }
#line 4177 "y.tab.c"
    break;

  case 99: /* ASNEXPR: BANDASN  */
#line 2061 "parser.y"
                 {strcpy((yyval.b), "&=");}
#line 4183 "y.tab.c"
    break;

  case 100: /* ASNEXPR: BORASN  */
#line 2062 "parser.y"
              {strcpy((yyval.b), "|=");}
#line 4189 "y.tab.c"
    break;

  case 101: /* ASNEXPR: BXORASN  */
#line 2063 "parser.y"
               {strcpy((yyval.b), "^=");}
#line 4195 "y.tab.c"
    break;

  case 102: /* ASNEXPR: LSHIFTASN  */
#line 2064 "parser.y"
                 {strcpy((yyval.b), "<<=");}
#line 4201 "y.tab.c"
    break;

  case 103: /* ASNEXPR: RSHIFTASN  */
#line 2065 "parser.y"
                 {strcpy((yyval.b), ">>=");}
#line 4207 "y.tab.c"
    break;

  case 104: /* ASNEXPR: EXPR ASSGN EXPR  */
#line 2067 "parser.y"
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
#line 4323 "y.tab.c"
    break;

  case 105: /* EXPR: SIZEOF '(' IDEN ')'  */
#line 2179 "parser.y"
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
#line 4356 "y.tab.c"
    break;

  case 106: /* EXPR: EXPR '+' EXPR  */
#line 2207 "parser.y"
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
#line 4377 "y.tab.c"
    break;

  case 107: /* EXPR: EXPR '-' EXPR  */
#line 2223 "parser.y"
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
#line 4398 "y.tab.c"
    break;

  case 108: /* EXPR: EXPR '*' EXPR  */
#line 2239 "parser.y"
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
#line 4419 "y.tab.c"
    break;

  case 109: /* EXPR: EXPR '/' EXPR  */
#line 2255 "parser.y"
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
#line 4444 "y.tab.c"
    break;

  case 110: /* EXPR: EXPR '%' EXPR  */
#line 2275 "parser.y"
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
#line 4473 "y.tab.c"
    break;

  case 111: /* EXPR: '(' EXPR ')'  */
#line 2299 "parser.y"
               {if (!e){(yyval.expr) = createExpr();strcpy((yyval.expr)->str,(yyvsp[-1].expr)->str); strcpy((yyval.expr)->type,(yyvsp[-1].expr)->type);(yyval.expr)->lv=(yyvsp[-1].expr)->lv;}}
#line 4479 "y.tab.c"
    break;

  case 112: /* EXPR: EXPR OP '$'  */
#line 2300 "parser.y"
                     {e=1;strcpy(err,"Missing operand");yyerrok;}
#line 4485 "y.tab.c"
    break;

  case 113: /* EXPR: EXPR BAND EXPR  */
#line 2301 "parser.y"
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
#line 4506 "y.tab.c"
    break;

  case 114: /* EXPR: EXPR BOR EXPR  */
#line 2317 "parser.y"
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
#line 4527 "y.tab.c"
    break;

  case 115: /* EXPR: EXPR BXOR EXPR  */
#line 2333 "parser.y"
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
#line 4548 "y.tab.c"
    break;

  case 116: /* EXPR: EXPR LSHIFT EXPR  */
#line 2349 "parser.y"
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
#line 4565 "y.tab.c"
    break;

  case 117: /* EXPR: EXPR RSHIFT EXPR  */
#line 2361 "parser.y"
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
#line 4582 "y.tab.c"
    break;

  case 118: /* EXPR: BNOT EXPR  */
#line 2373 "parser.y"
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
#line 4603 "y.tab.c"
    break;

  case 119: /* EXPR: FUNCALL  */
#line 2389 "parser.y"
              {(yyval.expr) = (yyvsp[0].expr);}
#line 4609 "y.tab.c"
    break;

  case 120: /* EXPR: TERM  */
#line 2390 "parser.y"
           {(yyval.expr) = (yyvsp[0].expr);}
#line 4615 "y.tab.c"
    break;

  case 121: /* FUNCALL: CALL IDEN '(' ARGLIST ')'  */
#line 2393 "parser.y"
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
#line 4688 "y.tab.c"
    break;

  case 122: /* FUNCALL: CALL IDEN '(' ')'  */
#line 2461 "parser.y"
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
#line 4726 "y.tab.c"
    break;

  case 123: /* ARGLIST: EXPR ',' ARGLIST  */
#line 2495 "parser.y"
                          {
            if(!e){ (yyval.expr) = (yyvsp[-2].expr); (yyval.expr)->next = (yyvsp[0].expr); }
        }
#line 4734 "y.tab.c"
    break;

  case 124: /* ARGLIST: EXPR  */
#line 2498 "parser.y"
               {
            if(!e){ (yyval.expr) = (yyvsp[0].expr); (yyval.expr)->next = NULL; }
        }
#line 4742 "y.tab.c"
    break;

  case 135: /* SUBSCRIPTS: '[' EXPR ']'  */
#line 2504 "parser.y"
                         {
                if (!e) {
                        (yyval.sub) = createSubscript();
                        sprintf((yyval.sub)->indices, "[%s]", (yyvsp[-1].expr)->str);
                        (yyval.sub)->count = 1;
                }
        }
#line 4754 "y.tab.c"
    break;

  case 136: /* SUBSCRIPTS: SUBSCRIPTS '[' EXPR ']'  */
#line 2511 "parser.y"
                                  {
                if (!e) {
                        (yyval.sub) = createSubscript();
                        sprintf((yyval.sub)->indices, "%s[%s]", (yyvsp[-3].sub)->indices, (yyvsp[-1].expr)->str);
                        (yyval.sub)->count = (yyvsp[-3].sub)->count + 1;
                }
        }
#line 4766 "y.tab.c"
    break;

  case 137: /* TERM: STRING  */
#line 2518 "parser.y"
             {
    (yyval.expr) = createExpr();
    strcpy((yyval.expr)->str, (yyvsp[0].str));
    strcpy((yyval.expr)->type, "char*");
    (yyval.expr)->str_len = strlen((yyvsp[0].str)) - 2 + 1;
    (yyval.expr)->lv = 0;
}
#line 4778 "y.tab.c"
    break;

  case 138: /* TERM: '(' TYPE ')' EXPR  */
#line 2525 "parser.y"
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
#line 4805 "y.tab.c"
    break;

  case 139: /* TERM: UN OPR IDEN B  */
#line 2547 "parser.y"
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
#line 4854 "y.tab.c"
    break;

  case 140: /* TERM: UN IDEN OPR B  */
#line 2591 "parser.y"
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
                /*  const check on post-inc/dec  */
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
#line 4902 "y.tab.c"
    break;

  case 141: /* TERM: UN NUM C  */
#line 2634 "parser.y"
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
#line 4919 "y.tab.c"
    break;

  case 142: /* TERM: UN IDEN SUBSCRIPTS  */
#line 2646 "parser.y"
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
#line 4962 "y.tab.c"
    break;

  case 143: /* TERM: UN IDEN C  */
#line 2684 "parser.y"
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
#line 4989 "y.tab.c"
    break;

  case 144: /* TERM: UN CHARR C  */
#line 2706 "parser.y"
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
#line 5006 "y.tab.c"
    break;

  case 145: /* TERM: UN INC NUM  */
#line 2718 "parser.y"
             {e=1;strcpy(err,"cannot increment a constant value");}
#line 5012 "y.tab.c"
    break;

  case 146: /* TERM: UN DEC NUM  */
#line 2719 "parser.y"
             {e=1;strcpy(err,"cannot decrement a constant value");}
#line 5018 "y.tab.c"
    break;

  case 147: /* TERM: UN NUM INC  */
#line 2720 "parser.y"
             {e=1;strcpy(err,"cannot increment a constant value");}
#line 5024 "y.tab.c"
    break;

  case 148: /* TERM: UN NUM DEC  */
#line 2721 "parser.y"
             {e=1;strcpy(err,"cannot decrement a constant value");}
#line 5030 "y.tab.c"
    break;

  case 149: /* OPR: INC  */
#line 2724 "parser.y"
             {strcpy((yyval.str),(yyvsp[0].str));}
#line 5036 "y.tab.c"
    break;

  case 150: /* OPR: DEC  */
#line 2724 "parser.y"
                                   {strcpy((yyval.str),(yyvsp[0].str));}
#line 5042 "y.tab.c"
    break;

  case 151: /* B: OPR  */
#line 2726 "parser.y"
        {e=1;strcpy(err,"expression is not assignable");}
#line 5048 "y.tab.c"
    break;

  case 152: /* B: IDEN  */
#line 2727 "parser.y"
         {e=1;strcpy(err,"missing operator");}
#line 5054 "y.tab.c"
    break;

  case 153: /* B: NUM  */
#line 2728 "parser.y"
        {e=1;strcpy(err,"missing operator");}
#line 5060 "y.tab.c"
    break;

  case 155: /* C: IDEN  */
#line 2730 "parser.y"
         {e=1;strcpy(err,"missing operator");}
#line 5066 "y.tab.c"
    break;

  case 156: /* C: NUM  */
#line 2731 "parser.y"
        {e=1;strcpy(err,"missing operator");}
#line 5072 "y.tab.c"
    break;

  case 158: /* UN: '-'  */
#line 2733 "parser.y"
         {strcpy((yyval.str),"-");}
#line 5078 "y.tab.c"
    break;

  case 159: /* UN: '+'  */
#line 2733 "parser.y"
                                  {strcpy((yyval.str),"+");}
#line 5084 "y.tab.c"
    break;

  case 160: /* UN: %empty  */
#line 2733 "parser.y"
                                                      {strcpy((yyval.str),"");}
#line 5090 "y.tab.c"
    break;


#line 5094 "y.tab.c"

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

#line 2734 "parser.y"

char* genvar(){
    char *re = (char*)malloc(sizeof(char)*100);
    sprintf(re,"t%d",label);
    label++;
    return re;
}

int yyerror(char* msg){ return 0; }

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
    memset(imcode, 0, sizeof(imcode));  
    yyin = fopen(argv[1],"r");
    yyparse();
    
    if (!e) {  
        // Write TAC files
        FILE* tac_file = fopen("output.tac", "w");
        if (tac_file) {
            for (int i = 0; i < code; i++) {
                fprintf(tac_file, "%s", imcode[i]);
            }
            fclose(tac_file);
        }
       
    } 
    if (argc >= 2 && yyin != stdin) {
        fclose(yyin);
    }
    return 0;
}
