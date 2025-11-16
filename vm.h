#ifndef VM_H
#define VM_H

#include "ast.h"
#include "valor.h"

typedef struct { char nome[64]; Valor v; } RegistroVar;

extern RegistroVar tabela_vars[256];
extern int tabela_count;

Valor vm_obter_variavel(const char *nome);
void vm_definir_variavel(const char *nome, Valor v);
Valor vm_exec(Nodo *n);
Valor vm_exec_comando(int tipo, char *a1, char *a2);
Valor vm_comparar(int operador, Valor a, Valor b);
Valor vm_criar_pedido(int id, const char* cliente, const char* data);
Valor vm_adicionar_item(int id, const char* item, int quantidade);
Valor vm_remover_item(int id, const char* item);
Valor vm_atualizar_status(int id, const char* status);
Valor vm_consultar_pedido(int id, const char* campo);
Valor vm_deseja_adicionar_mais_itens(const char* resposta);
Valor vm_enviar_mensagem(const char* msg);

#endif
