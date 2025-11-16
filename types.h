#ifndef TYPES_H
#define TYPES_H

typedef struct Bloco Bloco;

typedef struct InfoConsulta {
    int id_pedido;
    char* campo;
    char* valor_comparacao;
} InfoConsulta;

#endif