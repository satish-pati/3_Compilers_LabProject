
%{
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
    e->str[0] = '\0';      
    e->type[0] = '\0';     
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


%}
%union{
        char str[1000];
        struct BoolNode* b;
        struct Expr *expr;
        int addr;
        struct Type* type;
        struct Decl* decl;
        struct Subscript* sub;
}
%nonassoc error
%nonassoc PASN MASN DASN SASN
%right '?' ':'
%left OR
%left AND
%nonassoc '!'
%left LT GT LE GE EQ NE
%left BOR
%left BXOR
%left BAND
%left '+' '-'
%left '/' '*' '%'
%left LSHIFT RSHIFT
%nonassoc BNOT
%nonassoc INC DEC
%nonassoc '(' ')'
%nonassoc UMINUS ELSE IDEN
%nonassoc '$'
%token <str> BREAK CONTINUE FOR DO IDEN NUM PASN MASN DASN SASN INC DEC LT GT LE GE NE OR AND EQ IF ELSE TR FL WHILE INT FLOAT CHAR CHARR
%token MEOF
%token <str> CONST
%token SWITCH CASE DEFAULT
%type <str> ASSGN UN OPR
%token <str> GOTO
%token <str> STRING BOOL
%type <expr>  EXPR TERM
%type <b> BOOLEXPR STMNTS A ASNEXPR NN 
%type <addr> M
%type <type> TYPE INDEX
%type <decl> DECLLIST
%type <sub> SUBSCRIPTS
%token <str> PRINT INPUT
%token <str> SIZEOF 
%token BANDASN BORASN BXORASN LSHIFTASN RSHIFTASN
%token <str> SHORT LONG DOUBLE VOID
%token <str> BAND BOR BXOR BNOT LSHIFT RSHIFT
%token <str> FUNCTION RETURN CALL
%type <expr> INIT_LIST
%type <decl> PARAMLIST
%type <expr> ARGLIST FUNCALL
%type <b> PROGRAM FUNDECL
%type <b>  CASE_LIST CASE_ITEM
%type <expr> CASE_EXPR

%%


S:      {top = create_env(top,0);} PROGRAM M MEOF{
        if (e){
                        printf("%s\nRejected \n%s \nCould not generate Three Address Code / Storage Layout\n",buffer,err);
                        e=0;err[0]="\0";buffer[0]='\0';}
                else {
                        backpatch($2->N,$3);
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
        | MEOF{YYACCEPT;}
        | error MEOF{e=1;strcpy(err,"Invalid Statements");
                printf("%s \nRejected -> %s \nCould not generate Three Address Code / Storage Layout\n",buffer,err);
                YYACCEPT;};


PROGRAM: FUNDECL PROGRAM {
            if(!e){
                $$ = createBoolNode();
                $$->N = merge($1->N, $2->N);
            }
        }        
        | STMNTS {$$ = $1;}
        | FUNDECL {
            if(!e){
                $$ = createBoolNode();
                $$->N = $1->N;
            }
        };


FUNDECL: FUNCTION TYPE IDEN '(' PARAMLIST ')' {
    if(!e){
        /*"" duplicate parameter check "" */
        if(checkDuplicateParams($5)){
            e = 1;
            sprintf(err+strlen(err), "Duplicate parameter names in function %s\n", $3);
        }

        //Function* f = createFunction($3, $2->str);
        char clean_ret_type[100];
strcpy(clean_ret_type, $2->str);
if(clean_ret_type[0] == '@') memmove(clean_ret_type, clean_ret_type+1, strlen(clean_ret_type));
Function* f = createFunction($3, clean_ret_type);  // stores "void", "int", "float" etc.
        f->start_label = code;

        struct Decl* p = $5;
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

        if(findFunction($3) != NULL){
            e=1;
            sprintf(err+strlen(err),"Redeclaration of function %s\n",$3);
        } else {
            addFunction(f);
        }

        sprintf(imcode[code], "%d BeginFunc %s %d\n", code, $3, pcount);
        code++;

        /*"" set current function context for return checking "" */
        char* ret_type_clean = $2->str;
        if(ret_type_clean[0]=='@') ret_type_clean++;
        strcpy(current_function, $3);
        strcpy(current_return_type, ret_type_clean);
        in_function = 1;
        has_return_statement = 0;

        top = create_env(top, offset);
        offset = 0;

        p = $5;
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
} '{' STMNTS '}' {
    if(!e){
    if(strcmp(current_return_type, "void") != 0 && !has_return_statement) {
    e = 1;
    sprintf(err + strlen(err), "Error: Non-void function '%s' must return a value of type %s\n", 
            current_function, current_return_type);
}

        sprintf(imcode[code], "%d EndFunc %s\n", code, $3);
        code++;

        $$ = createBoolNode();
        $$->N = $9->N;

        top = top->prev;
        if(!top) offset = 0;
        else offset = top->prev_offset;

        /*"" reset function context "" */
        in_function = 0;
        current_function[0] = '\0';
        current_return_type[0] = '\0';
    }
};

PARAMLIST: TYPE IDEN ',' PARAMLIST {
            if(!e){
                $$ = createDecl($2);
                char clean_type[100];
                strcpy(clean_type, $1->str);
                if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));
                strcpy($$->type, clean_type);
                $$->next = $4;
            }
        }
        | TYPE IDEN {
            if(!e){
                $$ = createDecl($2);
                char clean_type[100];
                strcpy(clean_type, $1->str);
                if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));
                strcpy($$->type, clean_type);
            }
        }
        | {$$ = NULL;};


A:  SWITCH '(' EXPR ')' '{' { 
       if(!e) {
           strcpy(current_switch_var, $3->str);
           saveoffset = offset;
           /*"" track switch depth "" */
           switch_depth++;
       }
   } CASE_LIST '}' {
       if(!e) {
           $$ = createBoolNode();
           backpatch($7->N, code);
           backpatch($7->B, code);
           offset = saveoffset;
           /*   restore switch depth  */
           switch_depth--;
       }
   }
| BREAK '$' {
    if (!e) {
        /*  break must be inside loop or switch */
        if(loop_depth == 0 && switch_depth == 0){
            e = 1;
            sprintf(err+strlen(err), "break statement not within loop or switch\n");
        }
        $$ = createBoolNode();
        $$->B = createNode(code);
        sprintf(imcode[code], "%d goto ", code);
        code++;
    }
}
| CONTINUE '$' {
    if (!e) {
        /*  continue must be inside a loop  */
        if(loop_depth == 0){
            e = 1;
            sprintf(err+strlen(err), "continue statement not within a loop\n");
        }
        $$ = createBoolNode();
        $$->C = createNode(code);
        sprintf(imcode[code], "%d goto ", code);
        code++;
    }
}
| RETURN EXPR '$' {
    if(!e){
        has_return_statement = 1;
        if(!in_function){
            e = 1;
            sprintf(err+strlen(err), "return statement outside function\n");
        } else if(strcmp(current_return_type,"void")==0){
            e = 1;
            sprintf(err+strlen(err), "void function '%s' should not return a value\n", current_function);
        } else if(!isTypeCompatible(current_return_type, $2->type)){
            e = 1;
            sprintf(err+strlen(err), "Return type mismatch in function '%s': expected %s, got %s\n",
                    current_function, current_return_type, $2->type);
        }
         char ret_val[200];
        strcpy(ret_val, $2->str);
        char* ret_base = getBaseType(current_return_type);
        char* expr_base = getBaseType($2->type);
        if (!e && strlen(ret_base) > 0 && strcmp(ret_base, expr_base) != 0
               && getTypeRank(ret_base) > 0 && getTypeRank(expr_base) > 0) {
            /* generate:  t = (target_type) expr  then  Return t  */
            char* tmp = genvar();
            sprintf(imcode[code], "%d %s = (%s) %s\n", code, tmp, ret_base, $2->str);
            code++;
            strcpy(ret_val, tmp);
        }
                sprintf(imcode[code], "%d Return %s\n", code, ret_val);
        code++;
        $$ = createBoolNode();
    }
}
| RETURN '$' {
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
        $$ = createBoolNode();
    }
}
|PRINT '(' EXPR ')' '$' {
    if (!e) {
        char* base_type = getBaseType($3->type);
        if(strstr($3->type, "char[") != NULL)
            sprintf(imcode[code], "%d printstring %s\n", code, $3->str);
        else if(strcmp(base_type, "char") == 0)
            sprintf(imcode[code], "%d printchar %s\n", code, $3->str);
        else if(strcmp(base_type, "float") == 0 || strcmp(base_type, "double") == 0)
            sprintf(imcode[code], "%d printfloat %s\n", code, $3->str);
        else
            sprintf(imcode[code], "%d printint %s\n", code, $3->str);
        code++;
        $$ = createBoolNode();
    }
}
|INPUT '(' EXPR ')' '$' {
    if (!e) {
        if(!$3->lv) {
            e = 1;
            sprintf(err + strlen(err), "Input requires a variable, not an expression\n");
        } 
        else {
        char* base_type = getBaseType($3->type);
        if(strstr($3->type, "char[") != NULL)
            sprintf(imcode[code], "%d inputstring %s\n", code, $3->str);
        else if(strcmp(base_type, "char") == 0)
            sprintf(imcode[code], "%d inputchar %s\n", code, $3->str);
        else if(strcmp(base_type, "float") == 0 || strcmp(base_type, "double") == 0)
            sprintf(imcode[code], "%d inputfloat %s\n", code, $3->str);
        else
            sprintf(imcode[code], "%d inputint %s\n", code, $3->str);
        code++;
        $$ = createBoolNode();
    }
}
}
| ASNEXPR '$' {if (!e){$$ = $1;}}
        | ASNEXPR error MEOF{strcat(err,"$ missing\n");yyerrok;e=1;
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT;}
        | IF '(' BOOLEXPR ')' M  A {if (!e){backpatch($3->T,$5);
                                                                                $$ = createBoolNode();
                                                                                $$->N = merge($3->F,$6->N);
                                                                                $$->B = $6->B;
                                                                                $$->C = $6->C;
                                                                                }}
        | IF '(' BOOLEXPR ')' M A ELSE NN M A {if (!e){
                backpatch($3->T,$5);
                backpatch($3->F,$9);
                $$ = createBoolNode();
                $$->N = merge(merge($6->N,$8->N),$10->N);
                $$->B = merge($6->B, $10->B);
                $$->C = merge($6->C, $10->C);
        }}
        | EXPR error MEOF{{strcat(err,"$ missing");yyerrok;e=1;}
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT;}
        | IF BOOLEXPR ')' M A ELSE NN M A{{strcat(err,"missing (\n");e=1;}}

        | WHILE M '(' BOOLEXPR ')' M {loop_depth++;} A {if (!e){
    loop_depth--;
    backpatch($8->N,$2);
    backpatch($8->C, $2);
    backpatch($4->T,$6);
    sprintf(imcode[code],"%d goto %d\n",code,$2);
    code++;
    backpatch($8->B, code);
    $$ = createBoolNode();
    $$->N = $4->F;
}}
        | WHILE M  BOOLEXPR ')' M A{{strcat(err,"missing (\n");e=1;}}

        | DO M {loop_depth++;} A WHILE M '(' BOOLEXPR ')' '$' {
    if (!e) {
        loop_depth--;
        backpatch($4->N, $6); 
        backpatch($4->C, $6); 
        backpatch($8->T, $2);
        backpatch($4->B, code);
        backpatch($8->F, code);
        $$ = createBoolNode();
        $$->N = NULL;
    }
}
        | FOR '(' ASNEXPR '$' M BOOLEXPR '$' M ASNEXPR 
    { 
        sprintf(imcode[code], "%d goto %d\n", code, $5); 
        code++; 
    } 
  ')' M {loop_depth++;} A 
    {
        if (!e) {
            loop_depth--;
            backpatch($6->T, $12);
            backpatch($14->N, $8);
            backpatch($14->C, $8);
            sprintf(imcode[code], "%d goto %d\n", code, $8);
            code++;
            backpatch($14->B, code);
            $$ = createBoolNode();
            $$->N = $6->F;
        }
    }
|'{' {top = create_env(top,offset);offset=0;} STMNTS '}' {if (!e) {
                                                $$ = createBoolNode();
                                                $$->N = $3->N;
                                                $$->B = $3->B;
                                                $$->C = $3->C;
                                                top = top->prev;
                                                if (!top) offset =0;
                                                else offset = top->prev_offset;
                                                }}
        | '{' '}' {if (!e){$$=createBoolNode();}}
        | EXPR '$'{if (!e) {$$=createBoolNode();}}
        | DECLSTATEMENT {
            if (!e){$$=createBoolNode();}
            } 
 ;

CASE_LIST: CASE_ITEM M CASE_LIST {
       if(!e) {
           backpatch($1->N, $2);
           $$ = createBoolNode();
           $$->N = $3->N;
           $$->B = merge($1->B, $3->B);
       }
   }
   | CASE_ITEM {
       if(!e) { $$ = $1; }
   };

   CASE_ITEM: CASE CASE_EXPR ':' M {
       if(!e) {
           sprintf(imcode[code], "%d if %s != %s goto ", code, 
                   current_switch_var, $2->str);
           $<addr>$ = code;
           code++;
       }
   } STMNTS {
       if(!e) {
           $$ = createBoolNode();
           $$->N = createNode($<addr>5);
           $$->B = $6->B;
       }
   }
   | DEFAULT ':' M STMNTS {
       if(!e) {
           $$ = createBoolNode();
           $$->N = NULL;
           $$->B = $4->B;
       }
   };

   CASE_EXPR: NUM {
       if(!e) {
           $$ = createExpr();
           strcpy($$->str, $1);
           if(checkfloat($1)) strcpy($$->type, "float");
           else strcpy($$->type, "int");
       }
   }
   | CHARR {
       if(!e) {
           $$ = createExpr();
           strcpy($$->str, $1);
           strcpy($$->type, "char");
       }
   };


DECLSTATEMENT: TYPE DECLLIST '$' {
    if(!e){
        struct Decl* temp = $2;
        while(temp){
            char clean_type[100];
            strcpy(clean_type, $1->str);
            if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));

            if (temp->re){
                e = 1;
                Symbol* s = get(top->table, temp->key);
                char* decl_type = $1->str;
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
                offset+=$1->size;
                strcpy(s->type, clean_type);
                s->dim_count = 0;
            }
            else{
                Symbol* s = get(top->table,temp->key);
                s->offset = offset;
                offset+=(temp->size*$1->size);
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
| TYPE DECLLIST  error MEOF{{strcat(err,"$ missing\n");yyerrok;e=1;}
                                                        printf("%s\nRejected -> %s -> Could not generate Three Address Code / Storage Layout\n",buffer,err);
                                                        YYACCEPT; }
;

INIT_LIST: EXPR ',' INIT_LIST {
        if(!e){ $$ = $1; $$->next = $3; }
    }
    | EXPR { if(!e){ $$ = $1; $$->next = NULL; } };

DECLLIST: IDEN ',' DECLLIST {if (get(top->table,$1)==NULL){
                                                                Symbol* s = createSymbol($1);
                                                                put(top->table,$1,s);
                                                                $$ = createDecl($1);
                                                                $$->next = $3;
                                                        }
                                                        else{
                                                                $$ = createDecl($1);
                                                                strcpy($$->type,"");
                                                                $$->re =1;
                                                                }strcpy($$->lt,"u");
                                                }
        | IDEN INDEX ',' DECLLIST {
                                if (get(top->table,$1)==NULL){
                                        Symbol* s = createSymbol($1);
                                        put(top->table,$1,s);
                                        $$ = createDecl($1);
                                        $$->next = $4;
                                        strcpy($$->type,$2->str);
                                        $$->size = $2->size;
                                }
                                else{
                                        $$ = createDecl($1);
                                        strcpy($$->type,"");
                                        $$->re =1;
                                }strcpy($$->lt,"u");
        }
        | IDEN {if (get(top->table,$1)==NULL){
                                                Symbol* s = createSymbol($1);
                                                put(top->table,$1,s);
                                                $$ = createDecl($1);
                        }
                        else{
                                                $$ = createDecl($1);
                                                strcpy($$->type,"");
                                                $$->re = 1;
                                                }strcpy($$->lt,"u");}
        | IDEN '=' EXPR {
                                        if (get(top->table,$1)==NULL){
                                                Symbol* s = createSymbol($1);
                                                put(top->table,$1,s);
                                                $$ = createDecl($1);
                                                strcpy($$->lt,$3->type);
                                                strcpy($$->op,$3->str);
                                                $$->is_literal = isLiteral($3->str);
                                                }
                                        else{
                                                $$ = createDecl($1);
                                                strcpy($$->type,"");
                                                strcpy($$->lt,$3->type);
                                                strcpy($$->op,$3->str);
                                                $$->is_literal = isLiteral($3->str);
                                                $$->re=1;
                                        }}
 | IDEN '=' EXPR ',' DECLLIST {
                                        if (get(top->table,$1)==NULL){
                                                Symbol* s = createSymbol($1);
                                                put(top->table,$1,s);
                                                $$ = createDecl($1);
                                                strcpy($$->lt,$3->type);
                                                strcpy($$->op,$3->str);
                                                $$->is_literal = isLiteral($3->str);
                                                $$->next = $5;
                                                }
                                        else{
                                                $$ = createDecl($1);
                                                strcpy($$->type,"");
                                                strcpy($$->lt,$3->type);
                                                strcpy($$->op,$3->str);
                                                $$->is_literal = isLiteral($3->str);
                                                $$->re=1;
                                                $$->next = $5;
                                        }}
        | IDEN INDEX {if (get(top->table,$1)==NULL){
                                                Symbol* s = createSymbol($1);
                                                put(top->table,$1,s);
                                                $$ = createDecl($1);
                                                strcpy($$->type,$2->str);
                                                $$->size = $2->size;
                                                }
                                        else{
                                                $$ = createDecl($1);
                                                strcpy($$->type,"");
                                                $$->re=1;
                                        }strcpy($$->lt,"u");}
        | IDEN INDEX '=' '{' INIT_LIST '}' {
        if (get(top->table,$1)==NULL){
            Symbol* s = createSymbol($1);
            put(top->table,$1,s);
            $$ = createDecl($1);
            strcpy($$->type,$2->str);
            $$->size = $2->size;
            strcpy($$->lt,"array_init");
            char init_str[1000] = "";
            struct Expr* e = $5; int idx = 0;
            while(e){ if(idx>0) strcat(init_str,","); strcat(init_str,e->str); e=e->next; idx++; }
            strcpy($$->op, init_str);
        } else {
            $$ = createDecl($1); strcpy($$->type,""); $$->re=1;
        }
    }
    | IDEN INDEX '=' '{' INIT_LIST '}' ',' DECLLIST {
        if (get(top->table,$1)==NULL){
            Symbol* s = createSymbol($1);
            put(top->table,$1,s);
            $$ = createDecl($1);
            strcpy($$->type,$2->str);
            $$->size = $2->size;
            $$->next = $8;
            strcpy($$->lt,"array_init");
            char init_str[1000] = "";
            struct Expr* e = $5; int idx = 0;
            while(e){ if(idx>0) strcat(init_str,","); strcat(init_str,e->str); e=e->next; idx++; }
            strcpy($$->op, init_str);
        } else {
            $$ = createDecl($1); strcpy($$->type,""); $$->re=1; $$->next = $8;
        }
    }
        | IDEN INDEX '=' EXPR {
                if (get(top->table,$1)==NULL){
                    Symbol* s = createSymbol($1);
                    put(top->table,$1,s);
                    $$ = createDecl($1);
                    strcpy($$->type,$2->str);
                    $$->size = $2->size;
                    strcpy($$->lt, $4->type);
                    strcpy($$->op, $4->str);
                    if(strcmp($4->type, "char*") == 0) $$->str_len = $4->str_len;
                    $$->is_literal = 1;
                } else {
                    $$ = createDecl($1); strcpy($$->type,""); $$->re=1;
                }
        }
        | IDEN INDEX '=' EXPR ',' DECLLIST {
                if (get(top->table,$1)==NULL){
                    Symbol* s = createSymbol($1);
                    put(top->table,$1,s);
                    $$ = createDecl($1);
                    strcpy($$->type,$2->str);
                    $$->size = $2->size;
                    $$->next = $6;
                    strcpy($$->lt, $4->type);
                    strcpy($$->op, $4->str);
                    if(strcmp($4->type, "char*") == 0) $$->str_len = $4->str_len;
                    $$->is_literal = 1;
                } else {
                    $$ = createDecl($1); strcpy($$->type,""); $$->re=1; $$->next = $6;
                }
        };

/* added positive-size check */
INDEX: '[' NUM ']' {$$ = createType();
                    int arr_size = atoi($2);
                    /* positive size check*/
                    if(arr_size <= 0){
                        e=1;
                        sprintf(err+strlen(err),"Array size must be positive (got %d)\n", arr_size);
                    }
                    $$->size=arr_size;
                    sprintf($$->str,"array %d ", arr_size);
                    if (checkfloat($2)){
                            e=1;sprintf(err+strlen(err),"Array index cannot be float\n");
                    }}
        | '[' NUM ']' INDEX {$$ = createType();
                             int arr_size = atoi($2);
                             /* positive size check  */
                             if(arr_size <= 0){
                                 e=1;
                                 sprintf(err+strlen(err),"Array size must be positive (got %d)\n", arr_size);
                             }
                             $$->size=$4->size*arr_size;
                             sprintf($$->str,"array %d %s", arr_size, $4->str);
                             if (checkfloat($2)){
                                     e=1;sprintf(err+strlen(err),"Array index cannot be float\n");
                             }};

TYPE: INT {$$ = createType(); strcpy($$->str,$1);$$->size=4;}
        | FLOAT  {$$ = createType();strcpy($$->str,$1);$$->size=4;}
    | BOOL {$$ = createType();strcpy($$->str,$1);$$->size=1;}
        | CHAR {$$ = createType();strcpy($$->str,$1);$$->size=1;}
    | SHORT {$$ = createType();strcpy($$->str,$1);$$->size=2;}
    | LONG {$$ = createType();strcpy($$->str,$1);$$->size=8;}
    | DOUBLE {$$ = createType();strcpy($$->str,$1);$$->size=8;}
    | VOID {$$ = createType();strcpy($$->str,$1);$$->size=0;}
    | CONST TYPE {
    $$ = $2;
    char base[100];
    strcpy(base, $2->str);
    if (base[0] == '@') memmove(base, base+1, strlen(base));
    sprintf($$->str, "const %s", base);
};

  
STMNTS: STMNTS M A {if (!e){backpatch($1->N,$2);
                                        $$ = createBoolNode();
                                        $$->N = $3->N;
                                        $$->B = merge($1->B, $3->B);
                                        $$->C = merge($1->C, $3->C);
}}
        | A M{if (!e){$$ = createBoolNode();
                $$->N = $1->N;
                $$->B = $1->B;
                $$->C = $1->C;
}};


ASSGN: '=' {strcpy($$,"=");}
         | PASN {strcpy($$,$1);}
         | MASN {strcpy($$,$1);}
     | DASN {strcpy($$,$1);}
     | SASN {strcpy($$,$1);}
     | BANDASN {strcpy($$,"&=");}
     | BORASN  {strcpy($$,"|=");}
     | BXORASN {strcpy($$,"^=");}
     | LSHIFTASN {strcpy($$,"<<=");}
     | RSHIFTASN {strcpy($$,">>=");} ;

BOOLEXPR:
         BOOLEXPR OR M BOOLEXPR {  if (!e){backpatch($1->F,$3);
                                                                $$ = createBoolNode();
                                                                $$->T = merge($1->T,$4->T);
                                                                $$->F = $4->F;
                                                         }}
    | BOOLEXPR AND M BOOLEXPR { if (!e){backpatch($1->T,$3);
                                                                $$ = createBoolNode();
                                                                $$->T = $4->T;
                                                                $$->F = merge($1->F,$4->F);
                                                                }}
        | '!' BOOLEXPR {
                if (!e){ $$ = createBoolNode(); $$->T = $2->F; $$->F = $2->T; }
        }
        | '(' BOOLEXPR ')' {
                if (!e){ $$ = createBoolNode(); $$->T = $2->T; $$->F = $2->F; }
        }
        | EXPR LT EXPR  {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,$1->str,$2,$3->str);
                                                        $$ = createBoolNode();
                                                        $$->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        $$->F = createNode(code); code++;}}
    | EXPR GT EXPR  {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,$1->str,$2,$3->str);
                                                        $$ = createBoolNode();
                                                        $$->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        $$->F = createNode(code); code++;}}
        | EXPR EQ EXPR  {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,$1->str,$2,$3->str);
                                                        $$ = createBoolNode();
                                                        $$->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        $$->F = createNode(code); code++;}}
    | EXPR NE EXPR  {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,$1->str,$2,$3->str);
                                                        $$ = createBoolNode();
                                                        $$->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        $$->F = createNode(code); code++;}}
        | EXPR LE EXPR  {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,$1->str,$2,$3->str);
                                                        $$ = createBoolNode();
                                                        $$->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        $$->F = createNode(code); code++;}}
    | EXPR GE EXPR  {if(!e) {sprintf(imcode[code],"%d if %s %s %s goto ",code,$1->str,$2,$3->str);
                                                        $$ = createBoolNode();
                                                        $$->T = createNode(code); code++;
                                                        sprintf(imcode[code],"%d goto ",code);
                                                        $$->F = createNode(code); code++;}}
        | TR {if (!e){
                $$ = createBoolNode();
                $$->T = createNode(code);
                sprintf(imcode[code],"%d goto ",code);
                code++;
        }}
        | FL {if (!e){
                $$ = createBoolNode();
                $$->F = createNode(code);
                sprintf(imcode[code],"%d goto ",code);
                code++;
        }};


M: {$$=code;};
NN: {$$=createBoolNode();
        $$->N = createNode(code);
        sprintf(imcode[code],"%d goto ",code);
        code++;
        };


ASNEXPR: BANDASN {strcpy($$, "&=");}
     | BORASN {strcpy($$, "|=");}
     | BXORASN {strcpy($$, "^=");}
     | LSHIFTASN {strcpy($$, "<<=");}
     | RSHIFTASN {strcpy($$, ">>=");}

|EXPR ASSGN EXPR {
    if (!e && $1->lv){
         if(strcmp($3->type, "void")==0){
            e=1;
            sprintf(err+strlen(err), "Cannot assign void expression to '%s'\n", $1->str);
        }
        Symbol* sym = env_get(top, $1->str);
        if(sym && strstr(sym->type, "const") != NULL){
            e=1;
            sprintf(err+strlen(err), "Cannot assign to const variable %s\n", $1->str);
        }

        if (strcmp($3->type, "char*") == 0 && 
            strstr($1->type, "char[") != NULL &&
            strlen($2)==1 && $2[0]=='=') {
            char* bracket = strchr($1->type, '[');
            if (bracket) {
                int array_size = atoi(bracket + 1);
                int string_len = $3->str_len;
                if (string_len > array_size) {
                    e = 1;
                    sprintf(err+strlen(err), "String literal too large for array (needs %d bytes, array is %d bytes)\n", string_len, array_size);
                } else {
                    char str_copy[1000]; strcpy(str_copy, $3->str);
                    int len = strlen(str_copy);
                    if(str_copy[0]=='"' && str_copy[len-1]=='"'){
                        str_copy[len-1]='\0';
                        char* str_content = str_copy + 1;
                        int idx = 0;
                        while(*str_content != '\0'){
                            sprintf(imcode[code], "%d %s[%d] = '%c'\n", code, $1->str, idx, *str_content);
                            code++; str_content++; idx++;
                        }
                        sprintf(imcode[code], "%d %s[%d] = '\\0'\n", code, $1->str, idx);
                        code++;
                    }
                }
            }
            $$ = createBoolNode();
        }
        else {
            if (strlen($2)==1){
                sprintf(imcode[code],"%d %s = %s\n",code,$1->str,$3->str);
                code++;
            }
            else{
    char* t = genvar();
    /* Strip trailing '=' to get the actual operator */
    char op_only[10];
    strncpy(op_only, $2, sizeof(op_only)-1);
    op_only[sizeof(op_only)-1] = '\0';
    int op_len = strlen(op_only);
    if (op_len > 0 && op_only[op_len-1] == '=') op_only[op_len-1] = '\0';

    /* Get LHS variable type from symbol table */
    Symbol* lhs_sym = env_get(top, $1->str);
    char lhs_type[100] = "";
    if(lhs_sym) strcpy(lhs_type, getBaseType(lhs_sym->type));

    /* Determine promoted type for the operation */
    char lhs_base[100], rhs_base[100];
    strcpy(lhs_base, lhs_type[0] ? lhs_type : $1->type);
    strcpy(rhs_base, getBaseType($3->type));
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
        char lhs_str[100]; strcpy(lhs_str, $1->str);
        if(strcmp(lhs_base, promoted) != 0){
            char* cl = genvar();
            sprintf(imcode[code],"%d %s = (%s) %s\n",code,cl,promoted,$1->str); code++;
            strcpy(lhs_str, cl);
        }

        /* Cast RHS up to promoted type if needed */
        char rhs_str[100]; strcpy(rhs_str, $3->str);
        if(strcmp(rhs_base, promoted) != 0){
            char* cr = genvar();
            sprintf(imcode[code],"%d %s = (%s) %s\n",code,cr,promoted,$3->str); code++;
            strcpy(rhs_str, cr);
        }

        /* Perform the operation */
        sprintf(imcode[code],"%d %s = %s %s %s\n",code,t,lhs_str,op_only,rhs_str); code++;

        /* Cast result back to LHS type if it differs from promoted type */
        if(strcmp(lhs_base, promoted) != 0){
            /* narrowing: warn */
            sprintf(err+strlen(err),
                "Warning: Implicit narrowing conversion from %s to %s in compound assignment to '%s'\n",
                promoted, lhs_base, $1->str);
            char* cback = genvar();
            sprintf(imcode[code],"%d %s = (%s) %s\n",code,cback,lhs_base,t); code++;
            sprintf(imcode[code],"%d %s = %s\n",code,$1->str,cback); code++;
        } else {
            sprintf(imcode[code],"%d %s = %s\n",code,$1->str,t); code++;
        }
    }
}
            $$ = createBoolNode();
        }
    }
    if (!$1->lv){e=1;strcat(err,"L value not assignable\n");}
}
/* EXPR: division/modulo by zero checks added */
EXPR: SIZEOF '(' IDEN ')' {
    if(!e){
        $$ = createExpr();
        Env* temp = top; int found = 0; int var_size = 4;
        while(temp){
            Symbol* sym = get(temp->table, $3);
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
        if(!found){ e=1; sprintf(err+strlen(err), "%s is not declared in scope\n", $3); }
        sprintf($$->str, "%d", var_size);
        strcpy($$->type, "int");
        $$->lv = 0;
    }
}
| EXPR '+' EXPR {
    if (!e){
        $$ = createExpr();
        if (tryConstantFold($1, $3, '+', $$)) {
        } else {
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType($1,$3,ct1,ct2,$$->type);
            if(strcmp(ct1,"")){strcpy($1->str,ct1);}
            if(strcmp(ct2,"")){strcpy($3->str,ct2);}
            char* t = genvar(); strcpy($$->str,t);
            sprintf(imcode[code],"%d %s = %s + %s\n",code,t,$1->str,$3->str); code++;
        }
        $$->lv=0;
    }
}
| EXPR '-' EXPR {
    if (!e){
        $$ = createExpr();
        if (tryConstantFold($1, $3, '-', $$)) {
        } else {
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType($1,$3,ct1,ct2,$$->type);
            if(strcmp(ct1,"")){strcpy($1->str,ct1);}
            if(strcmp(ct2,"")){strcpy($3->str,ct2);}
            char* t = genvar(); strcpy($$->str,t);
            sprintf(imcode[code],"%d %s = %s - %s\n",code,t,$1->str,$3->str); code++;
        }
        $$->lv=0;
    }
}
| EXPR '*' EXPR {
    if (!e){
        $$ = createExpr();
        if (tryConstantFold($1, $3, '*', $$)) {
        } else {
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType($1,$3,ct1,ct2,$$->type);
            if(strcmp(ct1,"")){strcpy($1->str,ct1);}
            if(strcmp(ct2,"")){strcpy($3->str,ct2);}
            char* t = genvar(); strcpy($$->str,t);
            sprintf(imcode[code],"%d %s = %s * %s\n",code,t,$1->str,$3->str); code++;
        }
        $$->lv=0;
    }
}
| EXPR '/' EXPR {
    if (!e){
        /*  division by zero check */
        if(isLiteral($3->str) && atof($3->str) == 0.0){
            e=1; sprintf(err+strlen(err),"Division by zero\n");
        }
        $$ = createExpr();
        if (tryConstantFold($1, $3, '/', $$)) {
        } else {
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType($1,$3,ct1,ct2,$$->type);
            if(strcmp(ct1,"")){strcpy($1->str,ct1);}
            if(strcmp(ct2,"")){strcpy($3->str,ct2);}
            char* t = genvar(); strcpy($$->str,t);
            sprintf(imcode[code],"%d %s = %s / %s\n",code,t,$1->str,$3->str); code++;
        }
        $$->lv=0;
    }
}
| EXPR '%' EXPR {
    if (!e){
        /* modulo by zero check*/
        if(isLiteral($3->str) && atoi($3->str) == 0){
            e=1; sprintf(err+strlen(err),"Modulo by zero\n");
        }
        $$ = createExpr();
        if (tryConstantFold($1, $3, '%', $$)) {
        } else {
            if (isFloatingType($1->type) || isFloatingType($3->type)){
                e=1;
                sprintf(err+strlen(err),"invalid operands to binary %% (float/double)\n");
            }
            char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
            char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
            checkType($1,$3,ct1,ct2,$$->type);
            if(strcmp(ct1,"")){strcpy($1->str,ct1);}
            if(strcmp(ct2,"")){strcpy($3->str,ct2);}
            char* t = genvar(); strcpy($$->str,t);
            sprintf(imcode[code],"%d %s = %s %% %s\n",code,t,$1->str,$3->str); code++;
        }
        $$->lv=0;
    }
}
|'(' EXPR ')'  {if (!e){$$ = createExpr();strcpy($$->str,$2->str); strcpy($$->type,$2->type);$$->lv=$2->lv;}}
        | EXPR OP '$'{e=1;strcpy(err,"Missing operand");yyerrok;}
    | EXPR BAND EXPR {
        if (!e){ 
            $$ = createExpr();
            if (tryConstantFold($1, $3, '&', $$)) { $$->lv = 0; } else {
                if (isFloatingType($1->type) || isFloatingType($3->type)){ e=1; sprintf(err+strlen(err),"invalid operands to bitwise & (float/double)\n"); }
                char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
                char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
                checkType($1,$3,ct1,ct2,$$->type);
                if(strcmp(ct1,"")){strcpy($1->str,ct1);}
                if(strcmp(ct2,"")){strcpy($3->str,ct2);}
                char* t = genvar(); strcpy($$->str,t);
                sprintf(imcode[code],"%d %s = %s & %s\n",code,t,$1->str,$3->str); code++;
            }
            $$->lv=0;
        }
    }
    | EXPR BOR EXPR {
        if (!e){ 
            $$ = createExpr();
            if (tryConstantFold($1, $3, '|', $$)) { $$->lv = 0; } else {
                if (isFloatingType($1->type) || isFloatingType($3->type)){ e=1; sprintf(err+strlen(err),"invalid operands to bitwise | (float/double)\n"); }
                char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
                char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
                checkType($1,$3,ct1,ct2,$$->type);
                if(strcmp(ct1,"")){strcpy($1->str,ct1);}
                if(strcmp(ct2,"")){strcpy($3->str,ct2);}
                char* t = genvar(); strcpy($$->str,t);
                sprintf(imcode[code],"%d %s = %s | %s\n",code,t,$1->str,$3->str); code++;
            }
            $$->lv=0;
        }
    }
    | EXPR BXOR EXPR {
        if (!e){ 
            $$ = createExpr();
            if (tryConstantFold($1, $3, '^', $$)) { $$->lv = 0; } else {
                if (isFloatingType($1->type) || isFloatingType($3->type)){ e=1; sprintf(err+strlen(err),"invalid operands to bitwise ^ (float/double)\n"); }
                char* ct1 = (char*)malloc(sizeof(char));strcpy(ct1,"");
                char* ct2 = (char*)malloc(sizeof(char));strcpy(ct2,"");
                checkType($1,$3,ct1,ct2,$$->type);
                if(strcmp(ct1,"")){strcpy($1->str,ct1);}
                if(strcmp(ct2,"")){strcpy($3->str,ct2);}
                char* t = genvar(); strcpy($$->str,t);
                sprintf(imcode[code],"%d %s = %s ^ %s\n",code,t,$1->str,$3->str); code++;
            }
            $$->lv=0;
        }
    }
    | EXPR LSHIFT EXPR {
        if (!e){ 
            $$ = createExpr();
            if (tryConstantFold($1, $3, 'l', $$)) { $$->lv = 0; } else {
                if (isFloatingType($1->type) || isFloatingType($3->type)){ e=1; sprintf(err+strlen(err),"invalid operands to << (float/double)\n"); }
                strcpy($$->type,$1->type);
                char* t = genvar(); strcpy($$->str,t);
                sprintf(imcode[code],"%d %s = %s << %s\n",code,t,$1->str,$3->str); code++;
            }
            $$->lv=0;
        }
    }
    | EXPR RSHIFT EXPR {
        if (!e){ 
            $$ = createExpr();
            if (tryConstantFold($1, $3, 'r', $$)) { $$->lv = 0; } else {
                if (isFloatingType($1->type) || isFloatingType($3->type)){ e=1; sprintf(err+strlen(err),"invalid operands to >> (float/double)\n"); }
                strcpy($$->type,$1->type);
                char* t = genvar(); strcpy($$->str,t);
                sprintf(imcode[code],"%d %s = %s >> %s\n",code,t,$1->str,$3->str); code++;
            }
            $$->lv=0;
        }
    }
    | BNOT EXPR {
    if (!e){
        if (isFloatingType($2->type)){ e=1; sprintf(err+strlen(err),"invalid operand to bitwise ~ (float/double)\n"); }
        $$ = createExpr();
        strcpy($$->type,$2->type);
        if (isNumericConstant($2->str)) {
            int val = (int)atof($2->str);
            sprintf($$->str, "%d", ~val);
            $$->lv = 0;
        } else {
            char* t = genvar(); strcpy($$->str,t);
            sprintf(imcode[code],"%d %s = ~ %s\n",code,t,$2->str); code++;
            $$->lv=0;
        }
    }
}
    | FUNCALL {$$ = $1;} 
    | TERM {$$ = $1;};


FUNCALL: CALL IDEN '(' ARGLIST ')' {
            if(!e){
                Function* f = findFunction($2);
                if(f == NULL){
                    e=1;
                    sprintf(err+strlen(err),"Function %s not declared\n",$2);
                    $$ = createExpr();
                } else {
                    int arg_count = 0;
                    struct Expr* arg = $4;
                    Param* param = f->params;

                    /* argument type checking */
                    while(arg && param){
                        arg_count++;
                        if(!isTypeCompatible(param->type, arg->type)){
                            sprintf(err+strlen(err),"Warning: Argument %d type mismatch in call to %s: expected %s, got %s\n",
                                    arg_count, $2, param->type, arg->type);
                        }
                        arg = arg->next;
                        param = param->next;
                    }
                    /* count any extra args */
                    while(arg){ arg_count++; arg = arg->next; }

                    if(arg_count != f->param_count){
                        e=1;
                        sprintf(err+strlen(err),"Function %s expects %d arguments, got %d\n",
                                $2, f->param_count, arg_count);
                    }

                    struct Expr* reversed = NULL;
                    arg = $4;
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

                      $$ = createExpr();
                    strcpy($$->type, f->return_type);
                    $$->lv = 0;
                    if(strcmp(f->return_type, "void") == 0) {
                        e = 1;
                        sprintf(err+strlen(err), "Cannot use return value of void function '%s'\n", $2);
                        sprintf(imcode[code], "%d Call %s\n", code, $2);
                        code++;
                        strcpy($$->str, "__void__");
                    } else {
                        char* ret_var = genvar();
                        sprintf(imcode[code], "%d %s = Call %s\n", code, ret_var, $2);
                        code++;
                        strcpy($$->str, ret_var);
                    }

                }
            }
        }
| CALL IDEN '(' ')' {
            if(!e){
                Function* f = findFunction($2);
                if(f == NULL){
                    e=1;
                    sprintf(err+strlen(err),"Function %s not declared\n",$2);
                    $$ = createExpr();
                } else {
                    if(f->param_count != 0){
                        e=1;
                        sprintf(err+strlen(err),"Function %s expects %d arguments, got 0\n",
                                $2, f->param_count);
                    }

$$ = createExpr();
                    strcpy($$->type, f->return_type);
                    $$->lv = 0;
                    if(strcmp(f->return_type, "void") == 0) {
                        e = 1;
                        sprintf(err+strlen(err), "Cannot use return value of void function '%s'\n", $2);
                        sprintf(imcode[code], "%d Call %s\n", code, $2);
                        code++;
                        strcpy($$->str, "__void__");
                    } else {
                        char* ret_var = genvar();
                        sprintf(imcode[code], "%d %s = Call %s\n", code, ret_var, $2);
                        code++;
                        strcpy($$->str, ret_var);
                    }

                }
            }
        };

ARGLIST: EXPR ',' ARGLIST {
            if(!e){ $$ = $1; $$->next = $3; }
        }
        | EXPR {
            if(!e){ $$ = $1; $$->next = NULL; }
        };

OP: '+' | '-' | '*' | '/' | '%'| BAND | BOR | BXOR | LSHIFT | RSHIFT;

SUBSCRIPTS: '[' EXPR ']' {
                if (!e) {
                        $$ = createSubscript();
                        sprintf($$->indices, "[%s]", $2->str);
                        $$->count = 1;
                }
        }
        | SUBSCRIPTS '[' EXPR ']' {
                if (!e) {
                        $$ = createSubscript();
                        sprintf($$->indices, "%s[%s]", $1->indices, $3->str);
                        $$->count = $1->count + 1;
                }
        };
TERM: STRING {
    $$ = createExpr();
    strcpy($$->str, $1);
    strcpy($$->type, "char*");
    $$->str_len = strlen($1) - 2 + 1;
    $$->lv = 0;
}
| '(' TYPE ')' EXPR {
    if(!e){
        $$ = createExpr();
        char clean_type[100]; strcpy(clean_type, $2->str);
        if (clean_type[0] == '@') memmove(clean_type, clean_type+1, strlen(clean_type));
        if (isNumericConstant($4->str)) {
            double val = atof($4->str);
            if (strcmp(clean_type,"int")==0||strcmp(clean_type,"short")==0||
                strcmp(clean_type,"long")==0||strcmp(clean_type,"char")==0)
                sprintf($$->str, "%d", (int)val);
            else if (strcmp(clean_type,"float")==0||strcmp(clean_type,"double")==0)
                sprintf($$->str, "%g", val);
            else sprintf($$->str,"(%s)%s",clean_type,$4->str);
            strcpy($$->type, clean_type);
            $$->lv = 0;
        } else {
            sprintf($$->str, "(%s)%s", clean_type, $4->str);
            strcpy($$->type, clean_type);
            $$->lv = 0;
        }
    }
}
| UN OPR IDEN B {
    $$ = createExpr();  
    if (strcmp($1,"-")){
        Env* temp = top; int found=0;
        while(temp){
            if (get(temp->table,$3)){
                found = 1;
                Symbol* t = get(temp->table,$3);
                /*  const check on pre-inc/dec */
                if(strstr(t->type,"const")!=NULL){ e=1; sprintf(err+strlen(err),"Cannot modify const variable %s\n",$3); }
                strcpy($$->type,t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",$3); e=1; }
        char*t2=genvar();
        sprintf(imcode[code],"%d %s = %s %c 1\n",code,t2,$3,$2[0]);code++;
        sprintf(imcode[code],"%d %s = %s\n",code,$3,t2);code++;
        strcpy($$->str,t2);
    } else {
        if (!strcmp($2,"--")){e=1;strcpy(err,"--- not allowed");}
        Env* temp = top; int found=0;
        while(temp){
            if (get(temp->table,$3)){
                found = 1;
                Symbol* t = get(temp->table,$3);
                if(strstr(t->type,"const")!=NULL){ e=1; sprintf(err+strlen(err),"Cannot modify const variable %s\n",$3); }
                strcpy($$->type,t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",$3); e=1; }
        else {
            char*t=genvar();char*t2=genvar();
            sprintf(imcode[code],"%d %s = %s %c 1\n",code,t,$3,$2[0]);code++;
            sprintf(imcode[code],"%d %s = %s\n",code,$3,t);code++;
            sprintf(imcode[code],"%d %s = - %s\n",code,t2,t);code++;
            strcpy($$->str,t2);
        }
    }
    $$->lv = 0;
}
| UN IDEN OPR B {
    $$ = createExpr();  
    if (strcmp($1,"-")){
        char*t = genvar();char*t2=genvar();
        sprintf(imcode[code],"%d %s = %s\n",code,t,$2);code++;
        sprintf(imcode[code],"%d %s = %s %c 1\n",code,t2,$2,$3[0]);code++;
        sprintf(imcode[code],"%d %s = %s\n",code,$2,t2);code++;
        strcpy($$->str,t);
        Env* temp = top; int found=0;
        while(temp){
            if (get(temp->table,$2)){
                found = 1;
                Symbol* t = get(temp->table,$2);
                /*  const check on post-inc/dec  */
                if(strstr(t->type,"const")!=NULL){ e=1; sprintf(err+strlen(err),"Cannot modify const variable %s\n",$2); }
                strcpy($$->type,t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",$2); e=1; }
    } else {
        char* t = genvar();char* t1 = genvar();char *t3 = genvar();
        sprintf(imcode[code],"%d %s = %s\n",code,t,$2);code++;
        sprintf(imcode[code],"%d %s = %s %c 1\n",code,t1,$2,$3[0]);code++;
        sprintf(imcode[code],"%d %s = %s\n",code,$2,t1);code++;
        sprintf(imcode[code],"%d %s = -%s\n",code,t3,t);code++;
        strcpy($$->str,t3);
        Env* temp = top; int found=0;
        while(temp){
            if (get(temp->table,$2)){
                found = 1;
                Symbol* t = get(temp->table,$2);
                if(strstr(t->type,"const")!=NULL){ e=1; sprintf(err+strlen(err),"Cannot modify const variable %s\n",$2); }
                strcpy($$->type,t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",$2); e=1; }
    }
    $$->lv=0;
}
| UN NUM C {
    $$ = createExpr();  
    if (!strcmp($1,"-")) {
        char negated[100]; sprintf(negated, "-%s", $2);
        strcpy($$->str, negated);
    } else {
        strcpy($$->str, $2);
    }
    if (checkfloat($2)) strcpy($$->type,"float");
    else strcpy($$->type,"int");
    $$->lv = 0;
}
| UN IDEN SUBSCRIPTS {
    $$ = createExpr();  
    Env* temp = top; int found=0; Symbol* sym_found = NULL;
    while(temp){
        if (get(temp->table,$2)){
            found = 1;
            sym_found = get(temp->table,$2);
            strcpy($$->type, sym_found->type);
            break;
        }
        temp = temp->prev;
    }
    if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",$2); e=1; }
    if (sym_found) {
        char* base_type = getBaseType(sym_found->type);
        strcpy($$->type, base_type);
    }
    if (sym_found && sym_found->dim_count > 0) {
        char* offset_var = calculateArrayOffset(sym_found, $3, $2);
        if (!strcmp($1,"-")) {
            char* t = genvar(); strcpy($$->str, t);
            sprintf(imcode[code],"%d %s = - %s[%s]\n", code, t, $2, offset_var); code++;
            $$->lv=0;
        } else {
            sprintf($$->str, "%s[%s]", $2, offset_var);
            $$->lv=1;
        }
    } else {
        if (!strcmp($1,"-")) {
            char* t = genvar(); strcpy($$->str, t);
            sprintf(imcode[code],"%d %s = - %s%s\n",code,t,$2,$3->indices); code++;
            $$->lv=0;
        } else {
            sprintf($$->str, "%s%s", $2, $3->indices);
            $$->lv=1;
        }
    }
}
| UN IDEN C {
    $$ = createExpr();  
    if (!strcmp($1,"-")) {
        char* t = genvar(); strcpy($$->str,t);
        sprintf(imcode[code],"%d %s = - %s\n",code,t,$2); code++;
        $$->lv=0;
    } else {
        strcpy($$->str,$2);
        $$->lv=1;
    }
    Env* temp = top; int found=0;
    while(temp){
        if (get(temp->table,$2)){
            found = 1;
            Symbol* t = get(temp->table,$2);
            strcpy($$->type,t->type);
            break;
        }
        temp = temp->prev;
    }
    if (!found){ sprintf(err+strlen(err),"%s is not declared in scope\n",$2); e=1; }
}
| UN CHARR C {
    $$ = createExpr();
    if (!strcmp($1,"-")) {
        char* t = genvar(); strcpy($$->str, t);
        sprintf(imcode[code], "%d %s = - %s\n", code, t, $2); code++;
        $$->lv = 0;
    } else {
        strcpy($$->str, $2);
        $$->lv = 0;
    }
    strcpy($$->type, "char");
}
| UN INC NUM {e=1;strcpy(err,"cannot increment a constant value");}
| UN DEC NUM {e=1;strcpy(err,"cannot decrement a constant value");}
| UN NUM INC {e=1;strcpy(err,"cannot increment a constant value");}
| UN NUM DEC {e=1;strcpy(err,"cannot decrement a constant value");}
;

    OPR: INC {strcpy($$,$1);}| DEC {strcpy($$,$1);};

B : OPR {e=1;strcpy(err,"expression is not assignable");}
  | IDEN {e=1;strcpy(err,"missing operator");}
  | NUM {e=1;strcpy(err,"missing operator");}
  |;
C : IDEN {e=1;strcpy(err,"missing operator");}
  | NUM {e=1;strcpy(err,"missing operator");}
  |;
UN : '-' {strcpy($$,"-");}  | '+' {strcpy($$,"+");} | {strcpy($$,"");} ;
%%
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