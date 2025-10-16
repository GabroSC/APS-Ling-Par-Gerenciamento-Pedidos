#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pedido.tab.h"
#include "lex.yy.h"

/* Lê stdin inteiro para memória */
static unsigned char* read_stdin(size_t *out_len) {
    size_t cap = 8192, len = 0;
    unsigned char *buf = (unsigned char*)malloc(cap);
    if (!buf) return NULL;
    int c;
    while ((c = fgetc(stdin)) != EOF) {
        if (len == cap) {
            cap *= 2;
            unsigned char *tmp = (unsigned char*)realloc(buf, cap);
            if (!tmp) { free(buf); return NULL; }
            buf = tmp;
        }
        buf[len++] = (unsigned char)c;
    }
    *out_len = len;
    return buf;
}

/* Normaliza: remove BOM; converte NBSP (C2 A0)->' '; vírgula full-width (EF BC 8C)->','; aspas curvas -> ASCII */
static unsigned char* normalize_utf8(const unsigned char *in, size_t len, size_t *out_len) {
    unsigned char *out = (unsigned char*)malloc(len + 1);
    if (!out) return NULL;
    size_t i = 0, j = 0;

    /* Remove BOM (EF BB BF) no início */
    if (len >= 3 && in[0] == 0xEF && in[1] == 0xBB && in[2] == 0xBF) i = 3;

    while (i < len) {
        unsigned char b = in[i];

        /* NBSP U+00A0 -> espaço */
        if (i + 1 < len && in[i] == 0xC2 && in[i+1] == 0xA0) { out[j++] = ' '; i += 2; continue; }

        /* vírgula full-width U+FF0C -> ',' */
        if (i + 2 < len && in[i] == 0xEF && in[i+1] == 0xBC && in[i+2] == 0x8C) { out[j++] = ','; i += 3; continue; }

        /* aspas curvas simples U+2018/U+2019 -> '\'' */
        if (i + 2 < len && in[i] == 0xE2 && in[i+1] == 0x80 && (in[i+2] == 0x98 || in[i+2] == 0x99)) {
            out[j++] = '\'';
            i += 3; continue;
        }

        /* aspas curvas duplas U+201C/U+201D -> '"' */
        if (i + 2 < len && in[i] == 0xE2 && in[i+1] == 0x80 && (in[i+2] == 0x9C || in[i+2] == 0x9D)) {
            out[j++] = '"';
            i += 3; continue;
        }

        /* CR -> nada (normaliza EOL para LF) */
        if (b == '\r') { i++; continue; }

        /* default: copia byte */
        out[j++] = b; i++;
    }
    out[j] = 0;
    *out_len = j;
    return out;
}

int yyparse(void *scanner);
void yyerror(void *scanner, const char *s);

int main(void) {
    size_t in_len = 0, norm_len = 0;
    unsigned char *raw = read_stdin(&in_len);
    if (!raw) { fprintf(stderr, "Falha ao ler stdin\n"); return 1; }

    unsigned char *norm = normalize_utf8(raw, in_len, &norm_len);
    free(raw);
    if (!norm) { fprintf(stderr, "Falha ao normalizar entrada\n"); return 1; }

    yyscan_t scanner;
    yylex_init(&scanner);

    /* Faz o Flex ler da memória normalizada */
    YY_BUFFER_STATE buf = yy_scan_bytes((const char*)norm, (int)norm_len, scanner);

    int ret = yyparse(scanner);

    yy_delete_buffer(buf, scanner);
    yylex_destroy(scanner);
    free(norm);

    if (ret == 0) {
        printf("[OK] Análise sintática concluída sem erros.\n");
        return 0;
    } else {
        fprintf(stderr, "[FALHA] Erros na análise.\n");
        return 1;
    }
}
