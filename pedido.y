/* pedido.y — Gramática Bison para a DSL de pedidos (data opcional em criar_pedido) */

%code top {
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  union YYSTYPE;
  typedef union YYSTYPE YYSTYPE;

  int  yylex(YYSTYPE *yylval_param, void *scanner);
  void yyerror(void *scanner, const char *s);
  int  yyget_lineno(void *scanner);
}

%define api.pure full
%define parse.error verbose

%parse-param { void *scanner }
%lex-param   { void *scanner }

%union {
  int    ival;
  char*  sval;
}

%token <ival> NUM
%token <sval> ID
%token <sval> STRING

%token CRIAR_PEDIDO ADICIONAR_ITEM REMOVER_ITEM ATUALIZAR_STATUS CONSULTAR_PEDIDO
%token DESEJA_MAIS PRINT
%token IF FOR WHILE TO
%token EQ NE LT GT LE GE ASSIGN
%token LPAREN RPAREN LBRACE RBRACE COMMA SEMICOL

%type  <sval> opt_campo

%start programa

%%

programa
  : %empty
  | programa comando
  ;

comando
  : criar_pedido    opt_semicol
  | adicionar_item  opt_semicol
  | remover_item    opt_semicol
  | atualizar_status opt_semicol
  | consultar_pedido opt_semicol
  | deseja_mais     opt_semicol
  | print_cmd
  | atribuicao      opt_semicol
  | condicional
  | loop_for
  | loop_while
  ;

opt_semicol
  : %empty
  | SEMICOL
  ;

/* ===== criar_pedido: aceita 2 ou 3 argumentos =====
   criar_pedido(NUM, STRING)
   criar_pedido(NUM, STRING, STRING)
*/
criar_pedido
  : CRIAR_PEDIDO LPAREN NUM COMMA STRING COMMA STRING RPAREN
    {
      printf("criar_pedido(id=%d, cliente=\"%s\", data=\"%s\")\n", $3, $5, $7);
      free($5); free($7);
    }
  ;

adicionar_item
  : ADICIONAR_ITEM LPAREN NUM COMMA STRING COMMA NUM RPAREN
    { printf("adicionar_item(pedido=%d, item=\"%s\", qtd=%d)\n", $3, $5, $7); free($5); }
  ;

remover_item
  : REMOVER_ITEM LPAREN NUM COMMA STRING RPAREN
    { printf("remover_item(pedido=%d, item=\"%s\")\n", $3, $5); free($5); }
  ;

atualizar_status
  : ATUALIZAR_STATUS LPAREN NUM COMMA STRING RPAREN
    { printf("atualizar_status(pedido=%d, status=\"%s\")\n", $3, $5); free($5); }
  ;

/* consultar_pedido(123) ou consultar_pedido(123, "status") */
consultar_pedido
  : CONSULTAR_PEDIDO LPAREN NUM opt_campo RPAREN
    { if ($4) { printf("consultar_pedido(%d, %s)\n", $3, $4); free($4); }
      else    { printf("consultar_pedido(%d)\n", $3); } }
  ;

opt_campo
  : %empty                  { $$ = NULL; }
  | COMMA STRING            { $$ = $2; }
  ;

deseja_mais
  : DESEJA_MAIS LPAREN STRING RPAREN
    { printf("deseja_adicionar_mais_itens(\"%s\")\n", $3); free($3); }
  ;

print_cmd
  : PRINT LPAREN expressao RPAREN SEMICOL
    { printf("print(...)\n"); }
  ;

atribuicao
  : ID ASSIGN expressao
    { printf("atribuir %s = <expr>\n", $1); free($1); }
  ;

condicional
  : IF condicao bloco
    { printf("if <cond> { ... }\n"); }
  ;

loop_for
  : FOR ID ASSIGN NUM TO NUM bloco
    { printf("for %s=%d to %d { ... }\n", $2, $4, $6); free($2); }
  ;

loop_while
  : WHILE condicao bloco
    { printf("while <cond> { ... }\n"); }
  ;

bloco
  : LBRACE lista_comandos RBRACE
  ;

lista_comandos
  : comando
  | lista_comandos comando
  ;

condicao
  : expressao EQ expressao
  | expressao NE expressao
  | expressao LT expressao
  | expressao GT expressao
  | expressao LE expressao
  | expressao GE expressao
  ;

expressao
  : NUM
  | STRING               { free($1); }
  | ID                   { free($1); }
  | consultar_pedido
  | deseja_mais
  ;

%%

void yyerror(void *scanner, const char *s) {
  int lineno = yyget_lineno(scanner);
  if (lineno <= 0) lineno = 1;
  fprintf(stderr, "[ERRO SINTÁTICO] %s (linha %d)\n", s, lineno);
}
