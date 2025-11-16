%code requires {
    #include "ast.h"
    #include "valor.h"
}

%define parse.error verbose

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *msg);
int yylex(void);

char *num_to_str(int n) {
    char buf[256];
    snprintf(buf, sizeof(buf), "%d", n);
    return strdup(buf);
}

char *join2(const char *a, const char *b) {
    size_t la = a ? strlen(a) : 0;
    size_t lb = b ? strlen(b) : 0;
    char *r = malloc(la + lb + 2);
    if (!r) return NULL;
    r[0] = '\0';
    if (a) strcat(r, a);
    strcat(r, "|");
    if (b) strcat(r, b);
    return r;
}
%}

%union {
    int num;
    char *str;
    Nodo *ast;
}

%token <num> NUMBER
%token <str> ID STRING
%token CRIAR_PEDIDO ADICIONAR_ITEM REMOVER_ITEM ATUALIZAR_STATUS CONSULTAR_P DESEJA_ADICIONAR PRINT
%token IF ELSE WHILE FOR TO
%token LBRACE RBRACE LPAREN RPAREN COMMA SEMI
%token EQ NE LE GE LT GT ATRIB
%token AND OR ADD_ASSIGN

%left OR
%left AND
%nonassoc EQ NE
%nonassoc LT LE GT GE
%left '+' '-'
%left '*' '/'

%type <ast> programa stmt stmt_list expr

%%

programa:
    stmt_list { ast_raiz = $1; }
    ;

stmt_list:
      stmt_list stmt  { $$ = $1; bloco_add($$, $2); }
    | stmt            { $$ = nodo_bloco(); bloco_add($$, $1); }
    ;

stmt:
      CRIAR_PEDIDO LPAREN NUMBER COMMA STRING COMMA STRING RPAREN SEMI
            { $$ = nodo_comando(C_CRIAR_PEDIDO, num_to_str($3), join2($5,$7)); }

    | ADICIONAR_ITEM LPAREN NUMBER COMMA STRING COMMA NUMBER RPAREN SEMI
            { $$ = nodo_comando(C_ADICIONAR_ITEM, num_to_str($3), join2($5, num_to_str($7))); }

    | REMOVER_ITEM LPAREN NUMBER COMMA STRING RPAREN SEMI
            { $$ = nodo_comando(C_REMOVER_ITEM, num_to_str($3), $5); }

    | ATUALIZAR_STATUS LPAREN NUMBER COMMA STRING RPAREN SEMI
            { $$ = nodo_comando(C_ATUALIZAR_STATUS, num_to_str($3), $5); }

    | CONSULTAR_P LPAREN NUMBER RPAREN SEMI
            { $$ = nodo_comando(C_CONSULTAR_P, num_to_str($3), NULL); }

    | CONSULTAR_P LPAREN NUMBER COMMA STRING RPAREN SEMI
            { $$ = nodo_comando(C_CONSULTAR_P, num_to_str($3), $5); }

    | PRINT LPAREN expr RPAREN SEMI
            { $$ = nodo_print($3); }

    | ID ATRIB expr SEMI
            { $$ = nodo_atrib($1, $3); }

    | ID ADD_ASSIGN expr SEMI
            {
                Nodo *leftvar = nodo_var($1);
                Nodo *sum = nodo_bin('+', leftvar, $3);
                $$ = nodo_atrib($1, sum);
            }

    | DESEJA_ADICIONAR LPAREN STRING RPAREN SEMI
            { $$ = nodo_comando(C_DESEJA_ADICIONAR, $3, NULL); }

    | IF expr LBRACE stmt_list RBRACE
            { $$ = nodo_if($2, $4); }

    | WHILE expr LBRACE stmt_list RBRACE
            { $$ = nodo_while($2, $4); }

    | FOR ID ATRIB NUMBER TO NUMBER LBRACE stmt_list RBRACE
            { $$ = nodo_for($2, $4, $6, $8); }
    ;

expr:
      NUMBER        { $$ = nodo_valor((Valor){ .tipo = T_NUMBER, .num = $1 }); }
    | STRING        { $$ = nodo_valor((Valor){ .tipo = T_STRING, .sval = $1 }); }
    | ID            { $$ = nodo_var($1); }

    | expr '+' expr { $$ = nodo_bin('+', $1, $3); }
    | expr '-' expr { $$ = nodo_bin('-', $1, $3); }
    | expr '*' expr { $$ = nodo_bin('*', $1, $3); }
    | expr '/' expr { $$ = nodo_bin('/', $1, $3); }

    | expr EQ expr  { $$ = nodo_bin(OP_EQ, $1, $3); }
    | expr NE expr  { $$ = nodo_bin(OP_NE, $1, $3); }
    | expr LT expr  { $$ = nodo_bin(OP_LT, $1, $3); }
    | expr LE expr  { $$ = nodo_bin(OP_LE, $1, $3); }
    | expr GT expr  { $$ = nodo_bin(OP_GT, $1, $3); }
    | expr GE expr  { $$ = nodo_bin(OP_GE, $1, $3); }

    | expr AND expr { $$ = nodo_bin(OP_AND, $1, $3); }
    | expr OR  expr { $$ = nodo_bin(OP_OR, $1, $3); }

    | LPAREN expr RPAREN { $$ = $2; }

    | CONSULTAR_P LPAREN expr RPAREN
        { $$ = nodo_consultar_expr($3, NULL); }
    | CONSULTAR_P LPAREN expr COMMA expr RPAREN
        { $$ = nodo_consultar_expr($3, $5); }
    ;

%%
