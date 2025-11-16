#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

RegistroVar tabela_vars[256];
int tabela_count = 0;

Valor vm_obter_variavel(const char *nome) {
    for (int i = 0; i < tabela_count; i++) {
        if (strcmp(tabela_vars[i].nome, nome) == 0) {
            return tabela_vars[i].v;
        }
    }
    return criar_valor_void();
}

void vm_definir_variavel(const char *nome, Valor v) {
    for (int i = 0; i < tabela_count; i++) {
        if (strcmp(tabela_vars[i].nome, nome) == 0) {
            tabela_vars[i].v = v;
            return;
        }
    }
    if (tabela_count < 256) {
        strncpy(tabela_vars[tabela_count].nome, nome, 63);
        tabela_vars[tabela_count].nome[63] = '\0';
        tabela_vars[tabela_count].v = v;
        tabela_count++;
    }
}

/* Support: arithmetic and comparison */
Valor vm_comparar(int operador, Valor a, Valor b) {
    Valor res = criar_valor_bool(0);
    if (a.tipo == T_NUMBER && b.tipo == T_NUMBER) {
        switch (operador) {
            case OP_EQ: res.b = (a.num == b.num); break;
            case OP_NE: res.b = (a.num != b.num); break;
            case OP_LT: res.b = (a.num < b.num); break;
            case OP_LE: res.b = (a.num <= b.num); break;
            case OP_GT: res.b = (a.num > b.num); break;
            case OP_GE: res.b = (a.num >= b.num); break;
            default: res.b = 0; break;
        }
        return res;
    }
    if (a.tipo == T_STRING && b.tipo == T_STRING) {
        int cmp = strcmp(a.sval, b.sval);
        switch (operador) {
            case OP_EQ: res.b = (cmp == 0); break;
            case OP_NE: res.b = (cmp != 0); break;
            default: res.b = 0; break;
        }
        return res;
    }
    res.b = 0;
    return res;
}

/* Small in-memory pedido DB (demo) */
typedef struct Item { char id[64]; int q; } Item;
typedef struct Pedido { int id; char cliente[128]; char data[16]; char status[32]; Item items[64]; int item_count; } Pedido;
static Pedido pedidos[256]; static int pedidos_count = 0;

static Pedido *find_pedido(int id) {
    for (int i = 0; i < pedidos_count; i++) if (pedidos[i].id == id) return &pedidos[i];
    return NULL;
}

Valor vm_criar_pedido(int id, const char* cliente, const char* data) {
    Pedido *p = find_pedido(id);
    if (!p) {
        if (pedidos_count < 256) {
            pedidos[pedidos_count].id = id;
            strncpy(pedidos[pedidos_count].cliente, cliente ? cliente : "(null)", 127);
            pedidos[pedidos_count].cliente[127] = '\0';
            strncpy(pedidos[pedidos_count].data, data ? data : "(null)", 15);
            pedidos[pedidos_count].data[15] = '\0';
            strncpy(pedidos[pedidos_count].status, "aberto", 31);
            pedidos[pedidos_count].status[31] = '\0';
            pedidos[pedidos_count].item_count = 0;
            pedidos_count++;
            printf("Pedido %d criado.\n", id);
        }
    } else {
        printf("Pedido %d já existe.\n", id);
    }
    return criar_valor_void();
}

Valor vm_adicionar_item(int id, const char* item, int quantidade) {
    Pedido *p = find_pedido(id);
    if (!p) { printf("Pedido %d não encontrado.\n", id); return criar_valor_void(); }
    if (p->item_count < 64) {
        strncpy(p->items[p->item_count].id, item ? item : "(null)", 63);
        p->items[p->item_count].id[63] = '\0';
        p->items[p->item_count].q = quantidade;
        p->item_count++;
        printf("Adicionado %s x%d ao pedido %d\n", item ? item : "(null)", quantidade, id);
    }
    return criar_valor_void();
}

Valor vm_remover_item(int id, const char* item) {
    Pedido *p = find_pedido(id);
    if (!p) { printf("Pedido %d não encontrado.\n", id); return criar_valor_void(); }
    for (int i = 0; i < p->item_count; i++) {
        if (strcmp(p->items[i].id, item) == 0) {
            for (int j = i; j+1 < p->item_count; j++) p->items[j] = p->items[j+1];
            p->item_count--;
            printf("Removido %s do pedido %d\n", item, id);
            break;
        }
    }
    return criar_valor_void();
}

Valor vm_atualizar_status(int id, const char* status) {
    Pedido *p = find_pedido(id);
    if (!p) { printf("Pedido %d não encontrado.\n", id); return criar_valor_void(); }
    strncpy(p->status, status ? status : "(null)", 31);
    p->status[31] = '\0';
    printf("Pedido %d status atualizado para %s\n", id, status ? status : "(null)");
    return criar_valor_void();
}

Valor vm_consultar_pedido(int id, const char* campo) {
    Pedido *p = find_pedido(id);
    if (!p) { return criar_valor_string("(pedido-nao-encontrado)"); }
    if (!campo) {
        char buf[256];
        snprintf(buf, sizeof(buf), "pedido(id=%d, cliente=%s, status=%s)", p->id, p->cliente, p->status);
        return criar_valor_string(buf);
    }
    if (strcmp(campo, "status") == 0) return criar_valor_string(p->status);
    if (strcmp(campo, "cliente") == 0) return criar_valor_string(p->cliente);
    if (strcmp(campo, "data_criacao") == 0) return criar_valor_string(p->data);
    if (strcmp(campo, "itens") == 0) {
        char buf[512] = "";
        for (int i = 0; i < p->item_count; i++) {
            char tmp[256];
            snprintf(tmp, sizeof(tmp), "%s:x%d ", p->items[i].id, p->items[i].q);
            strncat(buf, tmp, sizeof(buf)-strlen(buf)-1);
        }
        return criar_valor_string(buf);
    }
    return criar_valor_string("(campo-desconhecido)");
}

Valor vm_deseja_adicionar_mais_itens(const char* resposta) {
    return criar_valor_string(resposta ? resposta : "");
}

Valor vm_enviar_mensagem(const char* msg) {
    printf("Mensagem: %s\n", msg ? msg : "(null)");
    return criar_valor_void();
}

/* vm_exec_comando: arg2 may contain 'x|y' pairs when parser joined values */
Valor vm_exec_comando(int tipo, char *a1, char *a2) {
    if (!a1 && !a2) return criar_valor_void();

    switch (tipo) {
        case C_CRIAR_PEDIDO: {
            int id = atoi(a1);
            char *client = NULL, *date = NULL;
            if (a2) {
                char *sep = strchr(a2, '|');
                if (sep) {
                    *sep = '\0'; client = a2; date = sep+1;
                } else client = a2;
            }
            return vm_criar_pedido(id, client, date);
        }
        case C_ADICIONAR_ITEM: {
            int id = atoi(a1);
            char *item = NULL; int qty = 1;
            if (a2) {
                char *sep = strchr(a2, '|');
                if (sep) {
                    *sep = '\0'; item = a2; qty = atoi(sep+1);
                } else item = a2;
            }
            return vm_adicionar_item(id, item, qty);
        }
        case C_REMOVER_ITEM:
            return vm_remover_item(atoi(a1), a2);
        case C_ATUALIZAR_STATUS:
            return vm_atualizar_status(atoi(a1), a2);
        case C_CONSULTAR_P:
            return vm_consultar_pedido(atoi(a1), a2);
        case C_DESEJA_ADICIONAR:
            return vm_deseja_adicionar_mais_itens(a1);
        default:
            printf("Comando desconhecido.\n");
            return criar_valor_void();
    }
}

/* Execução geral da AST */
Valor vm_exec(Nodo *n) {
    if (!n) return criar_valor_void();

    switch (n->tipo) {
        case N_BLOCO: {
            Valor r = criar_valor_void();
            for (int i = 0; i < n->u.bloco.count; i++) r = vm_exec(n->u.bloco.lista[i]);
            return r;
        }
        case N_EXP_VALOR:
            return n->u.exp_valor.v;
        case N_VAR:
            return vm_obter_variavel(n->u.var.nome);
        case N_ATRIB: {
            Valor v = vm_exec(n->u.atrib.expr);
            vm_definir_variavel(n->u.atrib.nome, v);
            return v;
        }
        case N_EXP_BIN: {
            int op = n->u.exp_bin.operador;

            /* handle logical AND/OR with short-circuit */
            if (op == OP_AND || op == OP_OR) {
                Valor a = vm_exec(n->u.exp_bin.a);
                int aval = 0;
                if (a.tipo == T_BOOL) aval = a.b;
                else if (a.tipo == T_NUMBER) aval = (a.num != 0);
                else if (a.tipo == T_STRING) aval = (a.sval && a.sval[0] != '\0');

                if (op == OP_AND) {
                    if (!aval) return criar_valor_bool(0);
                    Valor b = vm_exec(n->u.exp_bin.b);
                    int bval = 0;
                    if (b.tipo == T_BOOL) bval = b.b;
                    else if (b.tipo == T_NUMBER) bval = (b.num != 0);
                    else if (b.tipo == T_STRING) bval = (b.sval && b.sval[0] != '\0');
                    return criar_valor_bool(bval);
                } else { /* OP_OR */
                    if (aval) return criar_valor_bool(1);
                    Valor b = vm_exec(n->u.exp_bin.b);
                    int bval = 0;
                    if (b.tipo == T_BOOL) bval = b.b;
                    else if (b.tipo == T_NUMBER) bval = (b.num != 0);
                    else if (b.tipo == T_STRING) bval = (b.sval && b.sval[0] != '\0');
                    return criar_valor_bool(bval);
                }
            }

            /* arithmetic and comparisons */
            Valor a = vm_exec(n->u.exp_bin.a);
            Valor b = vm_exec(n->u.exp_bin.b);

            if (op == '+' || op == '-' || op == '*' || op == '/') {
                if (a.tipo == T_NUMBER && b.tipo == T_NUMBER) {
                    if (op == '+') return criar_valor_num(a.num + b.num);
                    if (op == '-') return criar_valor_num(a.num - b.num);
                    if (op == '*') return criar_valor_num(a.num * b.num);
                    if (op == '/') return criar_valor_num(b.num != 0 ? a.num / b.num : 0);
                }
                return criar_valor_void();
            }

            return vm_comparar(op, a, b);
        }
        case N_IF: {
            Valor cond = vm_exec(n->u.if_n.cond);
            if (cond.tipo == T_BOOL && cond.b) return vm_exec(n->u.if_n.entao);
            return criar_valor_void();
        }
        case N_WHILE: {
            while (1) {
                Valor cond = vm_exec(n->u.while_n.condicao);
                if (!(cond.tipo == T_BOOL && cond.b)) break;
                vm_exec(n->u.while_n.corpo);
            }
            return criar_valor_void();
        }
        case N_FOR: {
            for (int i = n->u.for_n.from; i <= n->u.for_n.to; i++) {
                Valor v = criar_valor_num(i);
                vm_definir_variavel(n->u.for_n.nome, v);
                vm_exec(n->u.for_n.body);
            }
            return criar_valor_void();
        }
        case N_COMANDO:
            return vm_exec_comando(n->u.comando.tipo, n->u.comando.arg1, n->u.comando.arg2);
        case N_PRINT: {
            Valor v = vm_exec(n->u.print_n.expr);
            if (v.tipo == T_NUMBER) printf("%d\n", v.num);
            else if (v.tipo == T_STRING) printf("%s\n", v.sval);
            else if (v.tipo == T_BOOL) printf("%s\n", v.b ? "true" : "false");
            else printf("(void)\n");
            return criar_valor_void();
        }
        case N_CONSULTAR_EXPR: {
            Valor pidv = vm_exec(n->u.consultar_expr.pedido_id);
            int id = 0;
            if (pidv.tipo == T_NUMBER) id = pidv.num;
            else if (pidv.tipo == T_STRING) id = atoi(pidv.sval);
            else id = 0;

            if (n->u.consultar_expr.campo == NULL) {
                return vm_consultar_pedido(id, NULL);
            }

            Valor campo_v = vm_exec(n->u.consultar_expr.campo);
            const char *campo_s = NULL;
            char tmpbuf[256];
            if (campo_v.tipo == T_STRING) campo_s = campo_v.sval;
            else if (campo_v.tipo == T_NUMBER) { snprintf(tmpbuf, sizeof(tmpbuf), "%d", campo_v.num); campo_s = tmpbuf; }
            else if (campo_v.tipo == T_BOOL) { snprintf(tmpbuf, sizeof(tmpbuf), "%s", campo_v.b ? "true" : "false"); campo_s = tmpbuf; }
            else campo_s = NULL;

            return vm_consultar_pedido(id, campo_s);
        }
        default:
            return criar_valor_void();
    }
}
