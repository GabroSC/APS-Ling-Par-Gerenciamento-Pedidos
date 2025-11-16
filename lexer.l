%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static int streq_ci(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}
%}

%option noyywrap
%option yylineno

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*

%%

\'([^\'\n]*)\' {
    yylval.str = strdup(yytext+1);
    yylval.str[strlen(yylval.str)-1] = '\0';
    return STRING;
}

\"([^\"\n]*)\" {
    yylval.str = strdup(yytext+1);
    yylval.str[strlen(yylval.str)-1] = '\0';
    return STRING;
}

{DIGIT}+ {
    yylval.num = atoi(yytext);
    return NUMBER;
}

{ID} {
    if (streq_ci(yytext, "criar_pedido")) return CRIAR_PEDIDO;
    if (streq_ci(yytext, "adicionar_item")) return ADICIONAR_ITEM;
    if (streq_ci(yytext, "remover_item")) return REMOVER_ITEM;
    if (streq_ci(yytext, "atualizar_status")) return ATUALIZAR_STATUS;
    if (streq_ci(yytext, "consultar_pedido") || streq_ci(yytext, "consultar_p")) return CONSULTAR_P;
    if (streq_ci(yytext, "deseja_adicionar_mais_itens") || streq_ci(yytext, "deseja_adicionar")) return DESEJA_ADICIONAR;
    if (streq_ci(yytext, "print")) return PRINT;
    if (streq_ci(yytext, "if")) return IF;
    if (streq_ci(yytext, "else")) return ELSE;
    if (streq_ci(yytext, "while")) return WHILE;
    if (streq_ci(yytext, "for")) return FOR;
    if (streq_ci(yytext, "to")) return TO;

    if (streq_ci(yytext, "aberto")) { yylval.str = strdup("aberto"); return STRING; }
    if (streq_ci(yytext, "concluido")) { yylval.str = strdup("concluido"); return STRING; }
    if (streq_ci(yytext, "cancelado")) { yylval.str = strdup("cancelado"); return STRING; }

    yylval.str = strdup(yytext);
    return ID;
}

"==" { return EQ; }
"!=" { return NE; }
"<=" { return LE; }
">=" { return GE; }
"&&" { return AND; }
"||" { return OR; }
"<" { return LT; }
">" { return GT; }
"\+=" { return ADD_ASSIGN; }
"=" { return ATRIB; }

"(" { return LPAREN; }
")" { return RPAREN; }
"{" { return LBRACE; }
"}" { return RBRACE; }
"," { return COMMA; }
";" { return SEMI; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }

[ \t\r\n]+ { }
. { return yytext[0]; }

%%
