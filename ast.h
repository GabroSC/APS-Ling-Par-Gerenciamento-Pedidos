#ifndef AST_H
#define AST_H

#include "valor.h"

typedef enum {
    N_PROGRAMA,
    N_BLOCO,
    N_COMANDO,
    N_EXP_BIN,
    N_EXP_VALOR,
    N_VAR,
    N_WHILE,
    N_IF,
    N_ATRIB,
    N_FOR,
    N_PRINT,
    N_CONSULTAR_EXPR
} NodoTipo;

typedef enum {
    C_CRIAR_PEDIDO,
    C_ADICIONAR_ITEM,
    C_REMOVER_ITEM,
    C_ATUALIZAR_STATUS,
    C_CONSULTAR_P,
    C_DESEJA_ADICIONAR
} TipoComando;

typedef struct Nodo Nodo;

struct Nodo {
    NodoTipo tipo;
    union {
        struct { Nodo **lista; int count; } bloco;
        struct { TipoComando tipo; char *arg1; char *arg2; } comando;
        struct { Nodo *condicao; Nodo *corpo; } while_n;
        struct { Nodo *cond; Nodo *entao; } if_n;
        struct { char *nome; Nodo *expr; } atrib;
        struct { int operador; Nodo *a; Nodo *b; } exp_bin;
        struct { Valor v; } exp_valor;
        struct { char *nome; } var;
        struct { char *nome; int from; int to; Nodo *body; } for_n;
        struct { Nodo *expr; } print_n;
        struct {
            Nodo *pedido_id;
            Nodo *campo; // pode ser NULL
        } consultar_expr;
    } u;
};

/* Funções para criar nós */
Nodo *nodo_bloco();
void bloco_add(Nodo *bloco, Nodo *cmd);
Nodo *nodo_var(char *nome);
Nodo *nodo_valor(Valor v);
Nodo *nodo_bin(int op, Nodo *a, Nodo *b);
Nodo *nodo_comando(TipoComando t, char *a1, char *a2);
Nodo *nodo_while(Nodo *cond, Nodo *corpo);
Nodo *nodo_if(Nodo *cond, Nodo *entao);
Nodo *nodo_atrib(char *nome, Nodo *expr);
Nodo *nodo_for(char *nome, int from, int to, Nodo *body);
Nodo *nodo_print(Nodo *expr);
Nodo *nodo_consultar_expr(Nodo *pedido_id, Nodo *campo);
extern Nodo *ast_raiz;

#endif
