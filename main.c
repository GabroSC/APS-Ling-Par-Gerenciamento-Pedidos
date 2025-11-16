#include <stdio.h>
#include "parser.tab.h"
#include "ast.h"
#include "vm.h"

extern Nodo *ast_raiz;

/* variáveis do scanner/parser */
extern char *yytext;
extern int yylineno;

int main(int argc, char **argv) {
    yyparse();
    if (ast_raiz) vm_exec(ast_raiz);
    return 0;
}

void yyerror(const char *msg) {
    fprintf(stderr, "Erro de sintaxe: %s (linha %d, token: \"%s\")\n", msg, yylineno, yytext ? yytext : "(null)");
}
