#include <stdio.h>
#include "longnumber.h"

void test_ln_create01(void)
{
    LongNumber *a;
    a = ln_create(10, 0, 0);
    ln_print(a, 10+2, "c01");
    ln_free(a);
}

void test_ln_create02(void)
{
    LongNumber *a;
    a = ln_create(10, 1, 3);
    ln_print(a, 10+2, "c02");
    ln_free(a);
}

void test_ln_create03(void)
{
    LongNumber *a;
    a = ln_create(10, 2, 3);
    ln_print(a, 10+2, "c03");
    ln_free(a);
}

void test_ln_add01(void)
{
    LongNumber *a, *b;
    a = ln_create(10, 0, 0);
    b = ln_create(10, 0, 0);
    ln_add(b, b, a, 10);
    ln_print(b, 10+2, "a01");
    ln_free(a);
    ln_free(b);
}

void test_ln_add02(void)
{
    LongNumber *a, *b;
    a = ln_create(10, 1, 3);
    b = ln_create(10, 2, 3);
    ln_add(b, b, a, 10);
    ln_print(b, 10+2, "a02");
    ln_free(a);
    ln_free(b);
}

void test_ln_add03(void)
{
    LongNumber *a, *b;
    a = ln_create(10, 2, 3);
    b = ln_create(10, 2, 3);
    ln_add(b, b, a, 10);
    ln_print(b, 10+2, "a03");
    ln_free(a);
    ln_free(b);
}

int main(void)
{
    ln_init(4);
    test_ln_create01();
    test_ln_create02();
    test_ln_create03();
    test_ln_add01();
    test_ln_add02();
    test_ln_add03();

    ln_init(1);
    test_ln_create01();

    return 0;
}
