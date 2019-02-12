#include <stdio.h>
#include <stdlib.h>
#include "longnumber.h"

static int LN_DIGITS;   /* max 9 digits with unsigned int */
static unsigned long long LN_MAXNUM = 1U;

int ln_init(int digits)
{
    if (digits < 1 || 9 < digits) {
        fprintf(stderr, "Warning: %d: invalid digit size.\n", digits);
        digits = 4;
    }
    LN_DIGITS = digits;
    if (LN_MAXNUM == 1) {
        int i;
        for (i = 0; i < LN_DIGITS; i++) {
            LN_MAXNUM *= 10U;
        }
    }
    return digits;
}

LongNumber *ln_create(size_t l, unsigned int x, unsigned int y)
{
    LongNumber *ln;
    ln = (LongNumber *)malloc(sizeof(LongNumber));
    if (ln == NULL) {
        fprintf(stderr, "Error: Can't allocate memory.\n");
        exit(1);
    }
    ln->frac_d = (unsigned int *)calloc(sizeof(unsigned int), l+1);
    if (ln->frac_d == NULL) {
        fprintf(stderr, "Error: Can't allocate memory.\n");
        exit(1);
    }
    ln->int_d = x;
    if (y > 1) {
        ln_div(ln, ln, y, l);
    }
    return ln;
}

void ln_free(LongNumber *ln)
{
    if (ln != NULL) {
        if (ln->frac_d != NULL) free(ln->frac_d);
        free(ln);
    }
}

void ln_print(LongNumber *c, size_t l, char *s)
{
    int i;
    size_t sp = 1, ep;

    int cols = 80;
    char *env_columns = getenv("COLUMNS");

    if (env_columns != NULL) {
        cols = atoi(env_columns);
        if (cols < 40) cols = 40;
    }

    printf("%3s %3u.", s, c->int_d);
    for (i = 0; i < l; i++) {
        printf("%0*u ", LN_DIGITS, c->frac_d[i]);
        if ((i+1) % ((cols-30)/(LN_DIGITS+1)) == 0 && i != l-1) {
            ep = i * LN_DIGITS;
            printf("(%4zu-%4zu)\n", sp, ep);
            printf("        ");
            sp = ep + 1;
        }
    }
    ep = (l-1) * LN_DIGITS;
    printf("(%4zu-%4zu)\n", sp, ep);
}

void ln_add(LongNumber *c, LongNumber *a, LongNumber *b, size_t l)
{
    int i;
    int cy = 0;
    for (i = l; i >= 0; i--) {
        c->frac_d[i] = a->frac_d[i] + b->frac_d[i] + cy;  /* 0 <= c->frac_d[i] <= 2*LN_MAXNUM - 1 */
        if (c->frac_d[i] < LN_MAXNUM) {
            cy = 0;
        } else {
            c->frac_d[i] -= LN_MAXNUM;
            cy = 1;
        }
    }
    c->int_d = a->int_d + b->int_d + cy;
}

void ln_sub(LongNumber *c, LongNumber *a, LongNumber *b, size_t l)
{
    int i;
    int br = 0;
    for (i = l; i >= 0; i--) {
        if (a->frac_d[i] < b->frac_d[i] + br) { /* b->frac_d[i] + br <= LN_MAXNUM */
            c->frac_d[i] = a->frac_d[i] + (LN_MAXNUM - b->frac_d[i] - br);
            br = 1;
        } else {
            c->frac_d[i] = a->frac_d[i] - b->frac_d[i] - br;
            br = 0;
        }
    }
    c->int_d = a->int_d - b->int_d - br;
}

void ln_div(LongNumber *c, LongNumber *a, unsigned int b, size_t l)
{
    int i;
    unsigned int rem = 0;
    unsigned long long d;
    d = a->int_d;
    c->int_d = d / b;
    rem = d % b;
    for (i = 0; i <= l; i++){
        d = a->frac_d[i] + rem * LN_MAXNUM; /* 0 <= d <= b * LN_MAXNUM - 1 */
        c->frac_d[i] = d / b;               /* 0 <= c->frac_d[i] <= LN_MAXNUM - 1 */
        rem = d % b;                        /* 0 <= rem <= b-1 */
    }
}

int ln_eps(LongNumber *a, size_t l)
{
    int i;
    if (a->int_d != 0) {
        return 0;
    }
    for (i = 0; i <= l; i++) {
        if (a->frac_d[i] != 0) {
            return 0;
        }
    }
    return 1;
}
