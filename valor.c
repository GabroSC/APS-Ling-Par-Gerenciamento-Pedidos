#include <string.h>
#include <stdlib.h>
#include "valor.h"

Valor criar_valor_string(const char *s) { Valor v; v.tipo = T_STRING; v.sval = strdup(s); return v; }
Valor criar_valor_num(int n) { Valor v; v.tipo = T_NUMBER; v.num = n; return v; }
Valor criar_valor_bool(int b) { Valor v; v.tipo = T_BOOL; v.b = b; return v; }
Valor criar_valor_void(void) { Valor v; v.tipo = T_VOID; return v; }
