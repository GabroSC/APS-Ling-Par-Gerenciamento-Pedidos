#include <stdlib.h>
#include <string.h>
#include "ast.h"

Nodo *ast_raiz = NULL;

Nodo *nodo_bloco() {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_BLOCO;
    return n;
}

void bloco_add(Nodo *bloco, Nodo *cmd) {
    bloco->u.bloco.lista = realloc(bloco->u.bloco.lista, sizeof(Nodo*) * (bloco->u.bloco.count + 1));
    bloco->u.bloco.lista[bloco->u.bloco.count++] = cmd;
}

Nodo *nodo_var(char *nome) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_VAR;
    n->u.var.nome = strdup(nome);
    return n;
}

Nodo *nodo_valor(Valor v) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_EXP_VALOR;
    n->u.exp_valor.v = v;
    return n;
}

Nodo *nodo_bin(int op, Nodo *a, Nodo *b) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_EXP_BIN;
    n->u.exp_bin.operador = op;
    n->u.exp_bin.a = a;
    n->u.exp_bin.b = b;
    return n;
}

Nodo *nodo_comando(TipoComando t, char *a1, char *a2) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_COMANDO;
    n->u.comando.tipo = t;
    n->u.comando.arg1 = a1 ? strdup(a1) : NULL;
    n->u.comando.arg2 = a2 ? strdup(a2) : NULL;
    return n;
}

Nodo *nodo_while(Nodo *cond, Nodo *corpo) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_WHILE;
    n->u.while_n.condicao = cond;
    n->u.while_n.corpo = corpo;
    return n;
}

Nodo *nodo_if(Nodo *cond, Nodo *entao) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_IF;
    n->u.if_n.cond = cond;
    n->u.if_n.entao = entao;
    return n;
}

Nodo *nodo_atrib(char *nome, Nodo *expr) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_ATRIB;
    n->u.atrib.nome = strdup(nome);
    n->u.atrib.expr = expr;
    return n;
}

Nodo *nodo_for(char *nome, int from, int to, Nodo *body) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_FOR;
    n->u.for_n.nome = strdup(nome);
    n->u.for_n.from = from;
    n->u.for_n.to = to;
    n->u.for_n.body = body;
    return n;
}

Nodo *nodo_print(Nodo *expr) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_PRINT;
    n->u.print_n.expr = expr;
    return n;
}

Nodo *nodo_consultar_expr(Nodo *pedido_id, Nodo *campo) {
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = N_CONSULTAR_EXPR;
    n->u.consultar_expr.pedido_id = pedido_id;
    n->u.consultar_expr.campo = campo;
    return n;
}
