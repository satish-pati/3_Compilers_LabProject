%{
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

%}

%union{
    char str[1000];
    struct Expr* expr;
    struct Type* type;
    struct Decl* decl;
}

%token <str> IDEN NUM INT FLOAT CHAR DOUBLE SHORT LONG VOID BOOL CONST
%token <str> CHARR STRING
%token <str> PASN MASN DASN SASN
%token <str> INC DEC
%token <str> BAND BOR BXOR BNOT LSHIFT RSHIFT
%token <str> BANDASN BORASN BXORASN LSHIFTASN RSHIFTASN
%token <str> DEFAULT BREAK CONTINUE
%token <str> AND OR
%token <str> EQ NE LT GT LE GE
%token <str> TR FL
%token <str> IF ELSE
%token <str> FUNCTION RETURN CALL PRINT INPUT
%token SIZEOF
%token <str> WHILE DO FOR
%token <str> SWITCH CASE
%token MEOF

%type <str> ASSGN UN INCRDECR
%type <type> TYPE BASE_TYPE INDEX
%type <decl> DECLLIST PARAMLIST
%type <expr> EXPR TERM INIT_LIST ARGLIST FUNCALL CASE_EXPR 
//SUBSCRIPTS
%nonassoc error
%nonassoc PASN MASN DASN SASN BANDASN BORASN BXORASN LSHIFTASN RSHIFTASN
%right '?' ':'
%left OR
%left AND
%nonassoc '!'
%left LT GT LE GE EQ NE
%left BOR
%left BXOR
%left BAND
%left '+' '-'
%left '*' '/' '%'
%left LSHIFT RSHIFT
%nonassoc BNOT
%nonassoc INC DEC
%nonassoc '(' ')'
%nonassoc UMINUS ELSE IDEN
%nonassoc '$'

%%

S: {top = create_env(top, 0);} PROGRAM MEOF {
        if (e) {
            printf("%s\nRejected -> %s\n", buffer, err);
        } else {
            printf("%s\nAccepted\n", buffer);
            print_all_envs();
        }
        YYACCEPT;
    }
    | MEOF { YYACCEPT; }
    | error MEOF {
        e = 1;
        strcpy(err, "Invalid Statements");
        printf("%s\nRejected -> %s\n", buffer, err);
        YYACCEPT;
    };

PROGRAM: FUNDECL PROGRAM
    | A PROGRAM
    | FUNDECL
    | A;

FUNDECL: FUNCTION TYPE IDEN '(' PARAMLIST ')' {
        if(!e){
            // Check for duplicate parameter names
            if(checkDuplicateParams($5)) {
                e = 1;
                sprintf(err + strlen(err), "Duplicate parameter names in function %s\n", $3);
            }
            
            // FIX: Strip @ prefix from return type
            char* ret_type = $2->str;
            if(ret_type[0] == '@') {
                ret_type++;  // Skip the @ character
            }
            
            Function* f = createFunction($3, ret_type);  // NOW USES CLEAN TYPE
            
            struct Decl* p = $5;
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
            
            if(findFunction($3) != NULL){
                e = 1;
                sprintf(err + strlen(err), "Redeclaration of function %s\n", $3);
            } else {
                addFunction(f);
            }
            
            printf("Function: %s (returns %s, %d parameters)\n", $3, ret_type, pcount);
            
            // Set current function context (ALSO FIX HERE)
            strcpy(current_function, $3);
            strcpy(current_return_type, ret_type);  // USE CLEAN TYPE
            in_function = 1;
            has_return_statement = 0;
            
            // Create new environment for function scope
            top = create_env(top, offset);
            offset = 0;
            
            // Add parameters to symbol table
            p = $5;
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
    } '{' STMNTS '}' {
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
    };

PARAMLIST: TYPE IDEN ',' PARAMLIST {
        if(!e){
            $$ = createDecl($2);
            strcpy($$->type, $1->str);
            $$->next = $4;
        }
    }
    | TYPE IDEN {
        if(!e){
            $$ = createDecl($2);
            strcpy($$->type, $1->str);
        }
    }
    | {$$ = NULL;};

STMNTS: STMNTS A
    | A;

A: SWITCH '(' EXPR ')' '{' {
        saveoffset = offset;
        switch_depth++;
        top = create_env(top, offset);  // CREATE NEW SCOPE FOR SWITCH
        offset = 0;
    } CASE_LIST '}' {
        top = top->prev;  // RESTORE PREVIOUS SCOPE
        if (!top) offset = 0;
        else offset = saveoffset;
        switch_depth--;
    }
    | BREAK '$' {
        if(!e && loop_depth == 0 && switch_depth == 0) {
            e = 1;
            sprintf(err + strlen(err), "break statement not within loop or switch\n");
        }
    }
    | CONTINUE '$' {
        if(!e && loop_depth == 0) {  // Only check loop_depth, NOT switch_depth
            e = 1;
            sprintf(err + strlen(err), "continue statement not within loop\n");
        }
    }
    | RETURN EXPR '$' {
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
                    if(!isTypeCompatible(current_return_type, $2->type)) {
                        e = 1;
                        sprintf(err + strlen(err), "Return type mismatch in function '%s': expected %s, got %s\n", 
                                current_function, current_return_type, $2->type);
                    }
                }
                printf("Return statement with value of type: %s\n", $2->type);
            }
        }
    }
    | RETURN '$' {
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
    | PRINT '(' EXPR ')' '$' {
        printf("Print: type=%s\n", $3->type);
    }
    | INPUT '(' EXPR ')' '$' {
    if(!e) {
        if(!$3->lv) {
            e = 1;
            sprintf(err + strlen(err), "Input requires a variable, not an expression\n");
        } else {
            printf("Input: type=%s\n", $3->type);
        }
    }
}
    | ASNEXPR '$'
    | ASNEXPR error MEOF {
        strcat(err, "Syntax error (missing $ or invalid statement)\n");
        yyerrok;
        e = 1;
        printf("%s\nRejected -> %s\n", buffer, err);
        YYACCEPT;
    }
    | IF '(' BOOLEXPR ')' A
    | IF '(' BOOLEXPR ')' A ELSE A
    | EXPR error MEOF {
            strcat(err, "Syntax error (missing $ or unknown keyword — did you mean @int/@float etc?)\n");
        yyerrok;
        e = 1;
        printf("%s\nRejected -> %s\n", buffer, err);
        YYACCEPT;
    }
    | IF BOOLEXPR ')' A ELSE A {
        strcat(err, "missing (\n");
        e = 1;
    }
    | WHILE '(' BOOLEXPR ')' {loop_depth++;} A {loop_depth--;}
    | WHILE BOOLEXPR ')' A {
        strcat(err, "missing (\n");
        e = 1;
    }
    | DO {loop_depth++;} A WHILE '(' BOOLEXPR ')' '$' {loop_depth--;}
    | FOR '(' ASNEXPR '$' BOOLEXPR '$' ASNEXPR ')' {loop_depth++;} A {loop_depth--;}
    | '{' {top = create_env(top, offset); offset = 0;} STMNTS '}' {
        top = top->prev;
        if (!top) offset = 0;
        else offset = top->prev_offset;
    }
    | '{' '}'
    | EXPR '$'
    | DECLSTATEMENT;

CASE_LIST: CASE_ITEM CASE_LIST
    | CASE_ITEM;

CASE_ITEM: CASE CASE_EXPR ':' STMNTS
    | DEFAULT ':' STMNTS;

CASE_EXPR: NUM {
        if(!e) {
            $$ = createExpr();
            strcpy($$->str, $1);
            if(checkfloat($1)) {
                strcpy($$->type, "float");
            } else {
                strcpy($$->type, "int");
            }
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
        struct Decl* temp = $2;
        while(temp){
            if (temp->re){
                e = 1;
                Symbol* s = get(top->table, temp->key);
                if (strcmp(s->type, $1->str) == 0){
                    sprintf(err + strlen(err), "Redeclaration of %s\n", s->name);
                } else {
                    sprintf(err + strlen(err), "Conflicting types for %s\n", s->name);
                }
            }
            
            if (strcmp(temp->type, "") == 0){
                Symbol* s = get(top->table, temp->key);
                s->offset = offset;
                offset += $1->size;
                
                char* type_to_store = $1->str;
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
                offset += (temp->size * $1->size);
                
                char* base_type_str = $1->str;
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
                strcpy(base_type, $1->str);
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
    | TYPE DECLLIST error MEOF {
       strcat(err, "Syntax error in declaration (  missing $)\n");
        yyerrok;
        e = 1;
        printf("%s\nRejected -> %s\n", buffer, err);
        YYACCEPT;
    };

INIT_LIST: EXPR ',' INIT_LIST {
        if(!e){
            $$ = $1;
            $$->next = $3;
        }
    }
    | EXPR {
        if(!e){
            $$ = $1;
            $$->next = NULL;
        }
    };

DECLLIST: IDEN ',' DECLLIST {
        if (get(top->table, $1) == NULL){
            Symbol* s = createSymbol($1);
            put(top->table, $1, s);
            $$ = createDecl($1);
            $$->next = $3;
        } else {
            $$ = createDecl($1);
            strcpy($$->type, "");
            $$->re = 1;
        }
        strcpy($$->lt, "u");
    }
    | IDEN INDEX ',' DECLLIST {
        if (get(top->table, $1) == NULL){
            Symbol* s = createSymbol($1);
            put(top->table, $1, s);
            $$ = createDecl($1);
            $$->next = $4;
            strcpy($$->type, $2->str);
            $$->size = $2->size;
        } else {
            $$ = createDecl($1);
            strcpy($$->type, "");
            $$->re = 1;
        }
        strcpy($$->lt, "u");
    }
    | IDEN {
        if (get(top->table, $1) == NULL){
            Symbol* s = createSymbol($1);
            put(top->table, $1, s);
            $$ = createDecl($1);
        } else {
            $$ = createDecl($1);
            strcpy($$->type, "");
            $$->re = 1;
        }
        strcpy($$->lt, "u");
    }
    | IDEN '=' EXPR {
        if (get(top->table, $1) == NULL){
            Symbol* s = createSymbol($1);
            put(top->table, $1, s);
            $$ = createDecl($1);
            strcpy($$->lt, $3->type);
            strcpy($$->op, $3->str);
            $$->is_literal = isLiteral($3->str);
        } else {
            $$ = createDecl($1);
            strcpy($$->type, "");
            strcpy($$->lt, $3->type);
            strcpy($$->op, $3->str);
            $$->is_literal = isLiteral($3->str);
            $$->re = 1;
        }
    }
    | IDEN '=' EXPR ',' DECLLIST {
        if (get(top->table, $1) == NULL){
            Symbol* s = createSymbol($1);
            put(top->table, $1, s);
            $$ = createDecl($1);
            strcpy($$->lt, $3->type);
            strcpy($$->op, $3->str);
            $$->is_literal = isLiteral($3->str);
            $$->next = $5;
        } else {
            $$ = createDecl($1);
            strcpy($$->type, "");
            strcpy($$->lt, $3->type);
            strcpy($$->op, $3->str);
            $$->is_literal = isLiteral($3->str);
            $$->re = 1;
            $$->next = $5;
        }
    }
    | IDEN INDEX {
        if (get(top->table, $1) == NULL){
            Symbol* s = createSymbol($1);
            put(top->table, $1, s);
            $$ = createDecl($1);
            strcpy($$->type, $2->str);
            $$->size = $2->size;
        } else {
            $$ = createDecl($1);
            strcpy($$->type, "");
            $$->re = 1;
        }
        strcpy($$->lt, "u");
    }
    | IDEN INDEX '=' '{' INIT_LIST '}' {
        if (get(top->table, $1) == NULL){
            Symbol* s = createSymbol($1);
            put(top->table, $1, s);
            $$ = createDecl($1);
            strcpy($$->type, $2->str);
            $$->size = $2->size;
            strcpy($$->lt, "array_init");
            
            char init_str[1000] = "";
            struct Expr* e = $5;
            int idx = 0;
            while(e){
                if(idx > 0) strcat(init_str, ",");
                strcat(init_str, e->str);
                e = e->next;
                idx++;
            }
            strcpy($$->op, init_str);
        } else {
            $$ = createDecl($1);
            strcpy($$->type, "");
            $$->re = 1;
        }
    }
    | IDEN INDEX '=' '{' INIT_LIST '}' ',' DECLLIST {
        if (get(top->table, $1) == NULL){
            Symbol* s = createSymbol($1);
            put(top->table, $1, s);
            $$ = createDecl($1);
            strcpy($$->type, $2->str);
            $$->size = $2->size;
            $$->next = $8;
            strcpy($$->lt, "array_init");
            
            char init_str[1000] = "";
            struct Expr* e = $5;
            int idx = 0;
            while(e){
                if(idx > 0) strcat(init_str, ",");
                strcat(init_str, e->str);
                e = e->next;
                idx++;
            }
            strcpy($$->op, init_str);
        } else {
            $$ = createDecl($1);
            strcpy($$->type, "");
            $$->re = 1;
            $$->next = $8;
        }
    };

INDEX: '[' NUM ']' {
        $$ = createType();
        int arr_size = atoi($2);
        
        // Check for negative or zero array size
        if(arr_size <= 0) {
            e = 1;
            sprintf(err + strlen(err), "Array size must be positive (got %d)\n", arr_size);
        }
        
        $$->size = arr_size;
        sprintf($$->str, "array %d ", arr_size);
        
        if (checkfloat($2)){
            e = 1;
            sprintf(err + strlen(err), "Array index cannot be float\n");
        }
    }
    | '[' NUM ']' INDEX {
        $$ = createType();
        int arr_size = atoi($2);
        
        // Check for negative or zero array size
        if(arr_size <= 0) {
            e = 1;
            sprintf(err + strlen(err), "Array size must be positive (got %d)\n", arr_size);
        }
        
        $$->size = $4->size * arr_size;
        sprintf($$->str, "array %d %s", arr_size, $4->str);
        
        if (checkfloat($2)){
            e = 1;
            sprintf(err + strlen(err), "Array index cannot be float\n");
        }
    };

TYPE: BASE_TYPE {
        $$ = $1;
    }
   | CONST BASE_TYPE {
    $$ = $2;
    char base[100];
    strcpy(base, $2->str);
    if (base[0] == '@') memmove(base, base+1, strlen(base));
    sprintf($$->str, "const %s", base);
};

BASE_TYPE: INT {$$ = createType(); strcpy($$->str, $1); $$->size = 4;}
    | FLOAT {$$ = createType(); strcpy($$->str, $1); $$->size = 4;}
    | BOOL {$$ = createType(); strcpy($$->str, $1); $$->size = 1;}
    | CHAR {$$ = createType(); strcpy($$->str, $1); $$->size = 1;}
    | SHORT {$$ = createType(); strcpy($$->str, $1); $$->size = 2;}
    | LONG {$$ = createType(); strcpy($$->str, $1); $$->size = 8;}
    | DOUBLE {$$ = createType(); strcpy($$->str, $1); $$->size = 8;}
    | VOID {$$ = createType(); strcpy($$->str, $1); $$->size = 0;};

ASSGN: '=' { strcpy($$, "="); }
    | PASN { strcpy($$, $1); }
    | MASN { strcpy($$, $1); }
    | DASN { strcpy($$, $1); }
    | SASN { strcpy($$, $1); }
    | BANDASN { strcpy($$, $1); }
    | BORASN { strcpy($$, $1); }
    | BXORASN { strcpy($$, $1); }
    | LSHIFTASN { strcpy($$, $1); }
    | RSHIFTASN { strcpy($$, $1); };

BOOLEXPR: BOOLEXPR OR BOOLEXPR
    | BOOLEXPR AND BOOLEXPR
    | '!' BOOLEXPR
    | '(' BOOLEXPR ')'
    | EXPR LT EXPR
    | EXPR GT EXPR
    | EXPR EQ EXPR
    | EXPR NE EXPR
    | EXPR LE EXPR
    | EXPR GE EXPR
    | TR
    | FL;

ASNEXPR: EXPR ASSGN EXPR {
        if (!e && $1->lv){
            Symbol* sym = env_get(top, $1->str);
            if(sym && strstr(sym->type, "const") != NULL){
                e = 1;
                sprintf(err + strlen(err), "Cannot assign to const variable %s\n", $1->str);
            }
            checkTypeAssign($1, $3);
            printf("Assignment: %s = %s (types: %s = %s)\n", $1->str, $3->str, $1->type, $3->type);
        }
        if (!$1->lv){
            e = 1;
            strcat(err, "L value not assignable\n");
        }
    };

EXPR: SIZEOF '(' IDEN ')' {
        if(!e){
            $$ = createExpr();
            Env* temp = top;
            int found = 0;
            int var_size = 4;
            
            while(temp){
                Symbol* sym = get(temp->table, $3);
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
                sprintf(err + strlen(err), "%s is not declared in scope\n", $3);
            }
            
            sprintf($$->str, "%d", var_size);
            strcpy($$->type, "int");
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "0");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR '+' EXPR {
        if (!e){
            $$ = createExpr();
            char result_type[100];
            checkType($1, $3, result_type);
            strcpy($$->type, result_type);
            sprintf($$->str, "(%s + %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR '-' EXPR {
        if (!e){
            $$ = createExpr();
            char result_type[100];
            checkType($1, $3, result_type);
            strcpy($$->type, result_type);
            sprintf($$->str, "(%s - %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR '*' EXPR {
        if (!e){
            $$ = createExpr();
            char result_type[100];
            checkType($1, $3, result_type);
            strcpy($$->type, result_type);
            sprintf($$->str, "(%s * %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR '/' EXPR {
        if (!e){
            // Check for division by zero for numeric constants
            if(isLiteral($3->str)) {
                double divisor = atof($3->str);
                if(divisor == 0.0) {
                    e = 1;
                    sprintf(err + strlen(err), "Division by zero\n");
                }
            }
            
            $$ = createExpr();
            char result_type[100];
            checkType($1, $3, result_type);
            strcpy($$->type, result_type);
            sprintf($$->str, "(%s / %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR '%' EXPR {
        if (!e){
            if (isFloatingType($1->type) || isFloatingType($3->type)){
                e = 1;
                sprintf(err + strlen(err), "Invalid operands to binary %% (float/double)\n");
            }
            
            // Check for modulo by zero for numeric constants
            if(isLiteral($3->str)) {
                int divisor = atoi($3->str);
                if(divisor == 0) {
                    e = 1;
                    sprintf(err + strlen(err), "Modulo by zero\n");
                }
            }
            
            $$ = createExpr();
            char result_type[100];
            checkType($1, $3, result_type);
            strcpy($$->type, result_type);
            sprintf($$->str, "(%s %% %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | '(' EXPR ')' {
        if (!e){
            $$ = createExpr();
            strcpy($$->str, $2->str);
            strcpy($$->type, $2->type);
            $$->lv = $2->lv;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR BAND EXPR {
        if (!e){ 
            if (isFloatingType($1->type) || isFloatingType($3->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to bitwise & (float/double)\n");
            }
            $$ = createExpr();
            char result_type[100];
            checkType($1, $3, result_type);
            strcpy($$->type, result_type);
            sprintf($$->str, "(%s & %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR BOR EXPR {
        if (!e){ 
            if (isFloatingType($1->type) || isFloatingType($3->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to bitwise | (float/double)\n");
            }
            $$ = createExpr();
            char result_type[100];
            checkType($1, $3, result_type);
            strcpy($$->type, result_type);
            sprintf($$->str, "(%s | %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR BXOR EXPR {
        if (!e){ 
            if (isFloatingType($1->type) || isFloatingType($3->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to bitwise ^ (float/double)\n");
            }
            $$ = createExpr();
            char result_type[100];
            checkType($1, $3, result_type);
            strcpy($$->type, result_type);
            sprintf($$->str, "(%s ^ %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR LSHIFT EXPR {
        if (!e){ 
            if (isFloatingType($1->type) || isFloatingType($3->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to << (float/double)\n");
            }
            $$ = createExpr();
            strcpy($$->type, $1->type);
            sprintf($$->str, "(%s << %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | EXPR RSHIFT EXPR {
        if (!e){ 
            if (isFloatingType($1->type) || isFloatingType($3->type)){
                e = 1; 
                sprintf(err + strlen(err), "Invalid operands to >> (float/double)\n");
            }
            $$ = createExpr();
            strcpy($$->type, $1->type);
            sprintf($$->str, "(%s >> %s)", $1->str, $3->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | BNOT EXPR {
        if (!e){
            if (isFloatingType($2->type)){
                e = 1;
                sprintf(err + strlen(err), "Invalid operand to bitwise ~ (float/double)\n");
            }
            $$ = createExpr();
            strcpy($$->type, $2->type);
            sprintf($$->str, "(~%s)", $2->str);
            $$->lv = 0;
        } else {
            $$ = createExpr();
            strcpy($$->str, "error");
            strcpy($$->type, "int");
            $$->lv = 0;
        }
    }
    | FUNCALL {$$ = $1;}
    | TERM {$$ = $1;};

FUNCALL: CALL IDEN '(' ARGLIST ')' {
        if(!e){
            Function* f = findFunction($2);
            if(f == NULL){
                e = 1;
                sprintf(err + strlen(err), "Function %s not declared\n", $2);
                $$ = createExpr();
                strcpy($$->type, "int");  // Default type to prevent segfault
                strcpy($$->str, $2);
                $$->lv = 0;
            } else {
                int arg_count = 0;
                struct Expr* arg = $4;
                Param* param = f->params;
                
                // Count arguments and check types
                while(arg && param){
                    arg_count++;
                    
                    // Type check arguments
                    if(!isTypeCompatible(param->type, arg->type)) {
                        printf("Warning: Argument %d type mismatch in call to %s: expected %s, got %s\n",
                               arg_count, $2, param->type, arg->type);
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
                            $2, f->param_count, arg_count);
                }
                
                $$ = createExpr();
                sprintf($$->str, "%s(...)", $2);
                strcpy($$->type, f->return_type);
                $$->lv = 0;

                if(strcmp(f->return_type, "void") == 0) {
    e = 1;
    sprintf(err + strlen(err), "Cannot use return value of void function '%s'\n", $2);
}

                
                printf("Function call: %s (returns %s)\n", $2, f->return_type);
            }
        } else {
            // If already in error state, still create expr to prevent NULL pointer
            $$ = createExpr();
            strcpy($$->type, "int");
            strcpy($$->str, "error");
            $$->lv = 0;
        }
    }
    | CALL IDEN '(' ')' {
        if(!e){
            Function* f = findFunction($2);
            if(f == NULL){
                e = 1;
                sprintf(err + strlen(err), "Function %s not declared\n", $2);
                $$ = createExpr();
                strcpy($$->type, "int");  // Default type to prevent segfault
                strcpy($$->str, $2);
                $$->lv = 0;
            } else {
                if(f->param_count != 0){
                    e = 1;
                    sprintf(err + strlen(err), "Function %s expects %d arguments, got 0\n",
                            $2, f->param_count);
                }
                
                $$ = createExpr();
                sprintf($$->str, "%s()", $2);
                strcpy($$->type, f->return_type);
                $$->lv = 0;
                
                 if(strcmp(f->return_type, "void") == 0) {
    e = 1;
    sprintf(err + strlen(err), "Cannot use return value of void function '%s'\n", $2);
}
                printf("Function call: %s (no args, returns %s)\n", $2, f->return_type);
            }
        } else {
            // If already in error state, still create expr to prevent NULL pointer
            $$ = createExpr();
            strcpy($$->type, "int");
            strcpy($$->str, "error");
            $$->lv = 0;
        }
    };

ARGLIST: EXPR ',' ARGLIST {
        if(!e){
            $$ = $1;
            $$->next = $3;
        }
    }
    | EXPR {
        if(!e){
            $$ = $1;
            $$->next = NULL;
        }
    };
/*SUBSCRIPTS: '[' EXPR ']' {
        // Store the index expression for bounds checking
        $$ = $2;
    }
    | SUBSCRIPTS '[' EXPR ']' {
        // For multi-dimensional arrays, we'd need to track multiple indices
        // For now, just use the last one
        $$ = $3;
    };
*/
TERM: STRING {
        $$ = createExpr();
        strcpy($$->str, $1);
        strcpy($$->type, "char*");
        $$->str_len = strlen($1) - 2 + 1;
        $$->lv = 0;
    }
    | '(' TYPE ')' EXPR {
    if(!e){
        if($4 == NULL) {
            e = 1;
            sprintf(err + strlen(err), "Invalid cast expression\n");
            $$ = createExpr();
            strcpy($$->type, $2->str);
        } else {
            $$ = createExpr();
            sprintf($$->str, "(%s)%s", $2->str, $4->str);
            strcpy($$->type, $2->str);
            $$->lv = 0;
        }
    } else {
        $$ = createExpr();
        strcpy($$->type, "int");
        strcpy($$->str, "error");
        $$->lv = 0;
    }
}
    | UN INCRDECR IDEN {
        $$ = createExpr();
        Env* temp = top;
        int found = 0;
        while(temp){
            if (get(temp->table, $3)){
                found = 1;
                Symbol* t = get(temp->table, $3);
                
                // Check if const
                if(strstr(t->type, "const") != NULL) {
                    e = 1;
                    sprintf(err + strlen(err), "Cannot modify const variable %s\n", $3);
                }
                
                strcpy($$->type, t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){
            sprintf(err + strlen(err), "%s is not declared in scope\n", $3);
            e = 1;
        }
        sprintf($$->str, "%s%s%s", $1, $2, $3);
        $$->lv = 0;
    }
    | UN IDEN INCRDECR {
        $$ = createExpr();
        Env* temp = top;
        int found = 0;
        while(temp){
            if (get(temp->table, $2)){
                found = 1;
                Symbol* t = get(temp->table, $2);
                
                // Check if const
                if(strstr(t->type, "const") != NULL) {
                    e = 1;
                    sprintf(err + strlen(err), "Cannot modify const variable %s\n", $2);
                }
                
                strcpy($$->type, t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){
            sprintf(err + strlen(err), "%s is not declared in scope\n", $2);
            e = 1;
        }
        sprintf($$->str, "%s%s%s", $1, $2, $3);
        $$->lv = 0;
    }
    | UN NUM {
    $$ = createExpr();
    sprintf($$->str, "%s%s", $1, $2);
    
    char* num = $2;
    
    if(checkfloat(num)) {
        // Has decimal point
        // Check if it needs double precision (more than 6-7 significant digits)
        // Simple rule: tag as "double" by default (like real C)
        strcpy($$->type, "double");
    } else {
        // Integer — check range
        long long val = atoll(num);
        if($1[0] == '-') val = -val;
        
        if(val >= -128 && val <= 127)
            strcpy($$->type, "int");   // could be char but default int
        else if(val >= -32768 && val <= 32767)
            strcpy($$->type, "int");
        else if(val >= -2147483648LL && val <= 2147483647LL)
            strcpy($$->type, "int");
        else
            strcpy($$->type, "long");  // too big for int
    }
    $$->lv = 0;
}
    | UN IDEN '[' EXPR ']' {
    $$ = createExpr();
    Env* temp = top;
    int found = 0;
    Symbol* sym_found = NULL;
    
    while(temp){
        if (get(temp->table, $2)){
            found = 1;
            sym_found = get(temp->table, $2);
            char* base_type = getBaseType(sym_found->type);
            strcpy($$->type, base_type);
            
            // BOUNDS CHECKING: Check if index is a constant
            if(isLiteral($4->str)) {
                int index = atoi($4->str);
                
                // Extract array size from type (e.g., "int[3]" -> 3)
                char* type_str = sym_found->type;
                char* bracket_start = strchr(type_str, '[');
                if(bracket_start) {
                    int array_size = atoi(bracket_start + 1);
                    
                    if(index < 0) {
                        e = 1;
                        sprintf(err + strlen(err), 
                                "Array index %d is negative for array %s\n", 
                                index, $2);
                    } else if(index >= array_size) {
                        e = 1;
                        sprintf(err + strlen(err), 
                                "Array index %d out of bounds for array %s[%d] (valid range: 0-%d)\n", 
                                index, $2, array_size, array_size - 1);
                    }
                }
            }
            
            break;
        }
        temp = temp->prev;
    }
    
    if (!found){
        sprintf(err + strlen(err), "%s is not declared in scope\n", $2);
        e = 1;
    }
    
    sprintf($$->str, "%s%s[%s]", $1, $2, $4->str);
    $$->lv = 1;
}
| UN IDEN '[' EXPR ']' '[' EXPR ']' {
    $$ = createExpr();
    Env* temp = top;
    int found = 0;
    Symbol* sym_found = NULL;
    
    while(temp){
        if (get(temp->table, $2)){
            found = 1;
            sym_found = get(temp->table, $2);
            char* base_type = getBaseType(sym_found->type);
            strcpy($$->type, base_type);
            
            // Bounds checking for multi-dimensional array
            if(sym_found->dim_count >= 2) {
                if(isLiteral($4->str)) {
                    int idx1 = atoi($4->str);
                    if(idx1 < 0 || idx1 >= sym_found->dimensions[0]) {
                        e = 1;
                        sprintf(err + strlen(err), 
                                "First index %d out of bounds for array %s (valid: 0-%d)\n", 
                                idx1, $2, sym_found->dimensions[0] - 1);
                    }
                }
                
                if(isLiteral($7->str)) {
                    int idx2 = atoi($7->str);
                    if(idx2 < 0 || idx2 >= sym_found->dimensions[1]) {
                        e = 1;
                        sprintf(err + strlen(err), 
                                "Second index %d out of bounds for array %s (valid: 0-%d)\n", 
                                idx2, $2, sym_found->dimensions[1] - 1);
                    }
                }
            }
            
            break;
        }
        temp = temp->prev;
    }
    
    if (!found){
        sprintf(err + strlen(err), "%s is not declared in scope\n", $2);
        e = 1;
    }
    
    sprintf($$->str, "%s%s[%s][%s]", $1, $2, $4->str, $7->str);
    $$->lv = 1;
}
    | UN IDEN {
        $$ = createExpr();
        Env* temp = top;
        int found = 0;
        while(temp){
            if (get(temp->table, $2)){
                found = 1;
                Symbol* t = get(temp->table, $2);
                strcpy($$->type, t->type);
                break;
            }
            temp = temp->prev;
        }
        if (!found){
            sprintf(err + strlen(err), "%s is not declared in scope\n", $2);
            e = 1;
        }
        sprintf($$->str, "%s%s", $1, $2);
        $$->lv = 1;
    }
    | UN CHARR {
        $$ = createExpr();
        sprintf($$->str, "%s%s", $1, $2);
        strcpy($$->type, "char");
        $$->lv = 0;
    }
    | UN INC NUM {
        e = 1;
        sprintf(err + strlen(err), "Cannot increment a constant value\n");
        $$ = createExpr();
        strcpy($$->str, "error");
        strcpy($$->type, "int");
        $$->lv = 0;
    }
    | UN DEC NUM {
        e = 1;
        sprintf(err + strlen(err), "Cannot decrement a constant value\n");
        $$ = createExpr();
        strcpy($$->str, "error");
        strcpy($$->type, "int");
        $$->lv = 0;
    }
    | UN NUM INC {
        e = 1;
        sprintf(err + strlen(err), "Cannot increment a constant value\n");
        $$ = createExpr();
        strcpy($$->str, "error");
        strcpy($$->type, "int");
        $$->lv = 0;
    }
    | UN NUM DEC {
        e = 1;
        sprintf(err + strlen(err), "Cannot decrement a constant value\n");
        $$ = createExpr();
        strcpy($$->str, "error");
        strcpy($$->type, "int");
        $$->lv = 0;
    };

INCRDECR: INC { strcpy($$, $1); }
    | DEC { strcpy($$, $1); };

UN: '-' { strcpy($$, "-"); }
    | '+' { strcpy($$, "+"); }
    | { strcpy($$, ""); };

%%

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