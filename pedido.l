%option noyywrap
%option nodefault
%option yylineno
%option reentrant
%option bison-bridge
%option header-file="lex.yy.h"
%option noinput
%option nounput

%x COMMENT

%{
  #include "pedido.tab.h"
  #include <stdlib.h>
  #include <string.h>

  static char *duptext_(const char *s, size_t n) {
    char *p = (char*)malloc(n + 1);
    if (!p) return NULL;
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
  }
%}

%%

[ \t\r\n]+                  ;
\/\/[^\n]*                  ;

"criar_pedido"              { return CRIAR_PEDIDO; }
"adicionar_item"            { return ADICIONAR_ITEM; }
"remover_item"              { return REMOVER_ITEM; }
"atualizar_status"          { return ATUALIZAR_STATUS; }
"consultar_pedido"          { return CONSULTAR_PEDIDO; }
"deseja_adicionar_mais_itens" { return DESEJA_MAIS; }
"print"                     { return PRINT; }

"if"                        { return IF; }
"for"                       { return FOR; }
"while"                     { return WHILE; }
"to"                        { return TO; }

"=="                        { return EQ; }
"!="                        { return NE; }
"<="                        { return LE; }
">="                        { return GE; }
"<"                         { return LT; }
">"                         { return GT; }
"="                         { return ASSIGN; }

"("                         { return LPAREN; }
")"                         { return RPAREN; }
"{"                         { return LBRACE; }
"}"                         { return RBRACE; }
","                         { return COMMA; }
";"                         { return SEMICOL; }

[0-9]+                      { yylval->ival = atoi(yytext); return NUM; }

\'([^\'\\\n]|\\.)*\'        { yylval->sval = duptext_(yytext + 1, yyleng - 2); return STRING; }
\"([^\"\\\n]|\\.)*\"        { yylval->sval = duptext_(yytext + 1, yyleng - 2); return STRING; }

[a-zA-Z_][a-zA-Z0-9_]*      { yylval->sval = duptext_(yytext, yyleng); return ID; }

.                           { return yytext[0]; }

%%
