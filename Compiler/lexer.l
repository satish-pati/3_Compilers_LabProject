%{
#include<stdio.h>
#include<string.h>
#include"y.tab.h"
char buffer[10240]="";
void append(const char* str){
        if (strlen(buffer)+strlen(str)<10240){
                strcat(buffer,str);
        }
}
%}

iden [[:alpha:]_][[:alnum:]_]*
num [[:digit:]]+\.?[[:digit:]]*
num2 [[:digit:]]*\.?[[:digit:]]+
ws [\n\t ]
%%
"#".*                      {}
"'''"([^']|'[^']|''[^'])*"'''"  {}  // ''' multi-line comment '''
"\"\"\""([^"]|"[^"]|""[^"])*"\"\"\""  {}  // """ multi-line comment """
"def"   {append(yytext);strcpy(yylval.str,yytext);return DEFAULT;}
"exit"     {append(yytext);strcpy(yylval.str,yytext);return BREAK;}
"skip"  {append(yytext);strcpy(yylval.str,yytext);return CONTINUE;}
@short  {append(yytext);strcpy(yylval.str,yytext);return SHORT;}
@long   {append(yytext);strcpy(yylval.str,yytext);return LONG;}
@double {append(yytext);strcpy(yylval.str,yytext);return DOUBLE;}
input {append(yytext);strcpy(yylval.str,yytext);return INPUT;}
"begin" {append(yytext);return '{';}
"end"   {append(yytext);return '}';}
"bitand"  {append(yytext);strcpy(yylval.str,yytext);return BAND;}
"bitor"  {append(yytext);strcpy(yylval.str,yytext);return BOR;}
"bitxor"  {append(yytext);strcpy(yylval.str,yytext);return BXOR;}
"bitnot"  {append(yytext);strcpy(yylval.str,yytext);return BNOT;}
"lshift" {append(yytext);strcpy(yylval.str,yytext);return LSHIFT;}
"rshift" {append(yytext);strcpy(yylval.str,yytext);return RSHIFT;}
"++" {append(yytext);strcpy(yylval.str,yytext);return INC;}
"--" {append(yytext);strcpy(yylval.str,yytext);return DEC;}
"+=" {append(yytext);strcpy(yylval.str,yytext);return PASN;}
"-=" {append(yytext);strcpy(yylval.str,yytext);return MASN;}
"*=" {append(yytext);strcpy(yylval.str,yytext);return SASN;}
"/=" {append(yytext);strcpy(yylval.str,yytext);return DASN;}
"bitand=" {append(yytext);strcpy(yylval.str,yytext);return BANDASN;}
"bitor=" {append(yytext);strcpy(yylval.str,yytext);return BORASN;}
"bitxor=" {append(yytext);strcpy(yylval.str,yytext);return BXORASN;}
"lshift=" {append(yytext);strcpy(yylval.str,yytext);return LSHIFTASN;}
"rshift=" {append(yytext);strcpy(yylval.str,yytext);return RSHIFTASN;}
"and" {append(yytext);strcpy(yylval.str,"&&");return AND;}
"or"  {append(yytext);strcpy(yylval.str,"||");return OR;}
"not" {append(yytext);return '!';}
"==" {append(yytext);strcpy(yylval.str,"==");return EQ;}
"!=" {append(yytext);strcpy(yylval.str,"!=");return NE;}
"<" {append(yytext);strcpy(yylval.str,"<");return LT;}
">" {append(yytext);strcpy(yylval.str,">");return GT;}
"<=" {append(yytext);strcpy(yylval.str,"<=");return LE;}
">=" {append(yytext);strcpy(yylval.str,">=");return GE;}
"eq" {append(yytext);strcpy(yylval.str,"==");return EQ;}
"ne" {append(yytext);strcpy(yylval.str,"!=");return NE;}
"lt" {append(yytext);strcpy(yylval.str,"<");return LT;}
"gt" {append(yytext);strcpy(yylval.str,">");return GT;}
"le" {append(yytext);strcpy(yylval.str,"<=");return LE;}
"ge" {append(yytext);strcpy(yylval.str,">=");return GE;}
true {append(yytext);strcpy(yylval.str,yytext);return TR;}
false {append(yytext);strcpy(yylval.str,yytext);return FL;}
if   {append(yytext);strcpy(yylval.str,yytext);return IF;}
else {append(yytext);strcpy(yylval.str,yytext);return ELSE;}
@int {append(yytext);strcpy(yylval.str,yytext);return INT;}
@void {append(yytext);strcpy(yylval.str,yytext);return VOID;}
@float {append(yytext);strcpy(yylval.str,yytext);return FLOAT;}
@char {append(yytext);strcpy(yylval.str,yytext);return CHAR;}
func {append(yytext);strcpy(yylval.str,yytext);return FUNCTION;}
ret  {append(yytext);strcpy(yylval.str,yytext);return RETURN;}
call     {append(yytext);strcpy(yylval.str,yytext);return CALL;}
show {append(yytext);strcpy(yylval.str,yytext);return PRINT;}
"getsizeof" {append(yytext);return SIZEOF;}
loop {append(yytext);strcpy(yylval.str,yytext);return WHILE;}
repeat {append(yytext);strcpy(yylval.str,yytext);return DO;}
is    {append(yytext);return '=';}
iterate  {append(yytext);strcpy(yylval.str,yytext);return FOR;}
choose   {append(yytext);strcpy(yylval.str,yytext);return SWITCH;}
option   {append(yytext);strcpy(yylval.str,yytext);return CASE;}
when   {append(yytext);strcpy(yylval.str,yytext);return IF;}
otherwise {append(yytext);strcpy(yylval.str,yytext);return ELSE;}
fixed {append(yytext);strcpy(yylval.str,yytext);return CONST;}
\"([^\"]|\\\")*\"  {
    append(yytext);
    strcpy(yylval.str, yytext);  
    return STRING;
}

hex[0-9a-fA-F]+  {
    append(yytext);
    long val = strtol(yytext+3, NULL, 16);
    sprintf(yylval.str, "%ld", val);
    return NUM;
}

oct[0-7]+  {
    append(yytext);
    long val = strtol(yytext+3, NULL, 8);
    sprintf(yylval.str, "%ld", val);
    return NUM;
}

bin[01]+  {
    append(yytext);
    long val = strtol(yytext+3, NULL, 2);
    sprintf(yylval.str, "%ld", val);
    return NUM;
}
({num}|{num2}) {append(yytext);strcpy(yylval.str,yytext);return NUM;}
{iden} {append(yytext);strcpy(yylval.str,yytext);return IDEN;}
{ws} {append(yytext);}
<<EOF>> {return MEOF;}
'\\n' {
    append(yytext);
    strcpy(yylval.str, "'\\n'");  
    return CHARR;
}
'\\t' {
    append(yytext);
    strcpy(yylval.str, "'\\t'");  
    return CHARR;
}
'\\0' {
    append(yytext);
    strcpy(yylval.str, "'\\0'");  
    return CHARR;
}
'[^']' { 
    append(yytext);  
    strcpy(yylval.str, yytext); 
    return CHARR; 
}
. {append(yytext);strcpy(yylval.str,yytext);return yytext[0];}

%%

