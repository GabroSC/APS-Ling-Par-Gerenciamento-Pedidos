# APS-Ling-Par-Gerenciamento-Pedidos

### Descrição
Essa linguagem foi criada para ajudar no gerenciamento de pedidos.

### EBNF

```
<programa>           = <comando>* ;

<comando>             = <criar_pedido>
                      | <adicionar_item>
                      | <remover_item>
                      | <atualizar_status>
                      | <consultar_pedido>
                      | <condicional>
                      | <loop_for>
                      | <loop_while>
                      | <deseja_adicionar_mais_itens>
                      | <print>;

<criar_pedido> = 'criar_pedido' '(' <id_pedido> ',' <cliente> ',' <data> ')';

<adicionar_item> = 'adicionar_item' '(' <id_pedido> ',' <id_item> ',' <quantidade> ')';

<remover_item> = 'remover_item' '(' <id_pedido> ',' <id_item> ')';

<atualizar_status> = 'atualizar_status' '(' <id_pedido> ',' <status> ')';

<consultar_pedido> = 'consultar_pedido' '(' <id_pedido> [',' <campo>] ')';

<print> = 'print' '(' <expressao> ')' ';' ;

<condicional> = 'if' <condicao> '{' <comando>+ '}';

<loop_for> = 'for' <variavel> '=' <numero> 'to' <numero> '{' <comando>+ '}';

<loop_while> = 'while' <condicao_while> '{' <comando>+ '}';

<deseja_adicionar_mais_itens> = 'deseja_adicionar_mais_itens' '(' <resposta> ')';

<condicao_while> = <status> '==' 'aberto';

<condicao> = <expressao> <operador_comparativo> <expressao>;

<operador_comparativo> = '==' | '!=' | '<' | '>' | '<=' | '>=';

<expressao> = <id_pedido>
                      | <quantidade>
                      | <status>
                      | <valor>;

<resposta> = 'sim' | 'não';

<variavel> = <letra> { <letra> | <digito> }*;

<id_pedido> = <digito> { <digito> }*;

<id_item> = <letra> { <letra> | <digito> }*;

<cliente> = <letra> { <letra> | <digito> }*;

<data> = <digito> <digito> <digito> '-' <digito> <digito> '-' <digito> <digito>;

<status> = 'aberto' | 'concluido' | 'cancelado';

<quantidade> = <digito> { <digito> }*;

<campo> = 'cliente' | 'status' | 'itens' | 'data_criacao';  // Campos disponíveis para consulta

<criterio> = 'cliente' | 'status' | 'data';

<valor> = <letra> { <letra> | <digito> }*;

<letra> = 'a'..'z' | 'A'..'Z';

<digito> = '0'..'9';

```

### Exemplo código

```
criar_pedido(123, 'João Silva', '2025-09-18');

adicionar_item(123, 'item001', 3);

adicionar_item(123, 'item002', 2);

while consultar_pedido(123, "status") == 'aberto' {

    adicionar_item(123, 'item003', 1);
    
    resposta = deseja_adicionar_mais_itens('sim');
    
    if resposta == 'não' {
        atualizar_status(123, 'concluido');
    }
}

pedido = consultar_pedido(123);
print(pedido);

```

### Compilação e Execução
```
## Como compilar
Digite "make" no terminal

## Como executar
Digite "./pedido < exemplo.ped"

```