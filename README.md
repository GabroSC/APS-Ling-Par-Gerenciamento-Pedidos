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
                      | <print>
                      | <atribuicao>
                      | <atribuicao_aumentada>;

<criar_pedido> = 'criar_pedido' '(' <id_pedido> ',' <cliente> ',' <data> ')';

<adicionar_item> = 'adicionar_item' '(' <id_pedido> ',' <id_item> ',' <quantidade> ')';

<remover_item> = 'remover_item' '(' <id_pedido> ',' <id_item> ')';

<atualizar_status> = 'atualizar_status' '(' <id_pedido> ',' <status> ')';

<consultar_pedido> = 'consultar_pedido' '(' <id_pedido> [',' <campo>] ')';

<print> = 'print' '(' <expressao> ')' ';' ;

<atribuicao> = <variavel> '=' <expressao> ';';

<atribuicao_aumentada> = <variavel> '+=' <expressao> ';';

<condicional> = 'if' <condicao> '{' <comando>+ '}';

<loop_for> = 'for' <variavel> '=' <numero> 'to' <numero> '{' <comando>+ '}';

<loop_while> = 'while' <condicao_while> '{' <comando>+ '}';

<deseja_adicionar_mais_itens> = 'deseja_adicionar_mais_itens' '(' <resposta> ')';

<condicao_while> = <expressao> <operador_logico> <expressao>;

<condicao> = <expressao> <operador_comparativo> <expressao>;

<operador_comparativo> = '==' | '!=' | '<' | '>' | '<=' | '>=';

<operador_logico> = '&&' | '||';

<expressao> = <id_pedido>
            | <quantidade>
            | <status>
            | <resposta>
            | <valor>
            | <expressao> '+' <expressao>
            | <expressao> '-' <expressao>
            | <expressao> '*' <expressao>
            | <expressao> '/' <expressao>
            | '(' <expressao> ')'
            | 'consultar_pedido' '(' <expressao> [',' <expressao>] ')'
            | <variavel>;

<resposta> = 'sim' | 'não';

<variavel> = <letra> { <letra> | <digito> }*;

<id_pedido> = <digito> { <digito> }*;

<id_item> = <letra> { <letra> | <digito> }*;

<cliente> = <letra> { <letra> | <digito> }*;

<data> = <digito> <digito> <digito> <digito> '-' <digito> <digito> '-' <digito> <digito>;

<status> = 'aberto' | 'concluido' | 'cancelado';

<quantidade> = <digito> { <digito> }*;

<campo> = 'cliente' | 'status' | 'itens' | 'data_criacao';

<valor> = <letra> { <letra> | <digito> }*;

<letra> = 'a'..'z' | 'A'..'Z';

<digito> = '0'..'9';

```

### Exemplo código

```
CRIAR_PEDIDO(123, "João Silva", "2025-09-18");
ADICIONAR_ITEM(123, "item001", 3);
ADICIONAR_ITEM(123, "item002", 2);

PRINT("Iniciando loop while...");

cond = 0;

WHILE CONSULTAR_P(123, "status") == "aberto" &&  cond < 3{
    cond += 1;
    ADICIONAR_ITEM(123, "item003", 1);

    resposta = DESEJA_ADICIONAR('sim');


    IF resposta == "não" {
        ATUALIZAR_STATUS(123, "concluido");
    }

    IF cond >= 3 {
        ATUALIZAR_STATUS(123, "concluido");
    }
}

pedido = CONSULTAR_P(123);
PRINT(pedido);
```

### Compilação e Execução
```
## Como compilar
Digite "make" no terminal

## Como executar
Digite "./interp < exemplo.ped" e "./interp < exemplo2.ped"

## Como limpar
Digite "make clean"

```