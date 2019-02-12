#if !defined(_LONG_NUMBER_H)
#define _LONG_NUMBER_H

typedef struct {
    unsigned int int_d;
    unsigned int *frac_d;
    /* size_t frac_len; */
    /* size_t digits; */
    /* unsigned int max_value; */
} LongNumber;

int ln_init(int digits);
LongNumber *ln_create(size_t l, unsigned int x, unsigned int y);
void ln_free(LongNumber *ln);
void ln_print(LongNumber *c, size_t l, char *s);
void ln_add(LongNumber *c, LongNumber *a, LongNumber *b, size_t l);
void ln_sub(LongNumber *c, LongNumber *a, LongNumber *b, size_t l);
void ln_div(LongNumber *c, LongNumber *a, unsigned int b, size_t l);
int ln_eps(LongNumber *a, size_t l);

#endif /* !defined(_LONG_NUMBER_H) */
