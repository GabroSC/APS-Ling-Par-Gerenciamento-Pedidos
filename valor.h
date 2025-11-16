#ifndef VALOR_H
#define VALOR_H

typedef enum { T_VOID, T_NUMBER, T_STRING, T_BOOL } TipoValor;
/* include logical ops OP_AND/OP_OR for expressions */
typedef enum { OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE, OP_AND, OP_OR } OperadorComparativo;

typedef struct { TipoValor tipo; char *sval; int num; int b; } Valor;

Valor criar_valor_string(const char *s);
Valor criar_valor_num(int n);
Valor criar_valor_bool(int b);
Valor criar_valor_void(void);

#endif
