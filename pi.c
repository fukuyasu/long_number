/*
 * Calculating the numerical value of PI using Machin's formula.
 * Author: FUKUYASU Naoki <fukuyasu@sys.wakayama-u.ac.jp>
 */

/*
 * 本プログラムは，文献[1]のpp.86-89で紹介されているプログラムを参考に，
 * 授業用に作成したものです．
 * 本プログラムでは，マチンの公式と呼ばれるarctan(x)を用いた公式を利用
 * し，マクローリン級数により，必要な桁数の値が収束するまで計算を繰り返す
 * ことによって円周率の値を求めています．
 *
 * 参考文献：
 *   [1] 河西朝雄：改定C言語によるはじめてのアルゴリズム入門，技術評論社．
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <libgen.h>
#include "longnumber.h"

/**********************************************************************/

static struct timeval tv1, tv2;

void timer_start(void)
{
    gettimeofday(&tv1, NULL);
}

void timer_stop(void)
{
    gettimeofday(&tv2, NULL);
}

char *timer_str(void)
{
    int x, y;
    static char diff_str[20] = "";

    x = tv2.tv_sec - tv1.tv_sec;
    y = tv2.tv_usec - tv1.tv_usec;
    if (y < 0) {
        x--;
        y += 1000000;
    }
    y /= 1000;
    sprintf(diff_str, "%d.%03d", x, y);

    return diff_str;
}

/**********************************************************************/

static char *program_name = "";
static int silent_mode = 0;
static int verbose_mode = 0;
static int dsize = 4;

void usage(void)
{
    fprintf(stderr, "usage: %s [-svh][-d <num>] <digits>\n", program_name);
    fprintf(stderr, "            -s: silent mode.\n");
    fprintf(stderr, "            -v: verbose mode.\n");
    fprintf(stderr, "            -h: print usage.\n");
    fprintf(stderr, "            -d <num>: size for each digit [1-9].\n");
}

int parse_options(int argc, char *argv[])
{
    int ch;
#if defined(__CYGWIN__)
    extern int __declspec(dllimport) optind;
    extern char __declspec(dllimport) *optarg;
#else /* !defined(__CYGWIN__) */
    extern int optind;
    extern char *optarg;
#endif /* defined(__CYGWIN__) */

    while ((ch = getopt(argc, argv, "d:svh")) != -1) {
        switch (ch) {
        case 'd':
            dsize = atoi(optarg);
            break;
        case 's':
            silent_mode = 1;
            verbose_mode = 0;
            break;
        case 'v':
            verbose_mode = 1;
            silent_mode = 0;
            break;
        case 'h':
        case '?':
        default:
            usage();
            exit(1);
        }
    }

    return optind;
}

/**********************************************************************/

int machin(LongNumber pi, size_t k)
{
    LongNumber a, b, c;
    unsigned int m;
    int i;

    /* Machin's Formula */
    a = ln_create(k, 16, 5);        /* a = 16 / 5  */
    b = ln_create(k, 4, 239);       /* b = 4 / 239 */
    c = ln_create(k, 0, 1);
    ln_sub(c, a, b, k);           /* c = a - b   */

    m = 1;
    i = 0;
    do {
        if (i % 2 == 0) {
            ln_add(pi, pi, c, k);  /* pi = pi + (-1)^i * (a - b) */
        } else {
            ln_sub(pi, pi, c, k);
        }

        i++;
        if (verbose_mode) {
            printf("%dth:\n", i);
            ln_print(c, k+1, i % 2 ? "+" : "-");
            ln_print(pi, k+1, "");
            printf("\n");
        }

        ln_div(a, a, 5*5, k);      /* a = a / 5^2   */
        ln_div(b, b, 239*239, k);  /* b = b / 239^2 */
        ln_sub(c, a, b, k);
        m += 2;
        ln_div(c, c, m, k);        /* c = (a - b) / (2*i+1) */
    } while (!ln_eps(c, k));

    ln_free(a);
    ln_free(b);
    ln_free(c);

    return i;
}

int main(int argc, char *argv[])
{
    LongNumber pi;
    int digits, n;
    size_t k;
    int i;
    int opts;

    program_name = basename(argv[0]);
    opts = parse_options(argc, argv);
    argc -= opts;
    argv += opts;

    digits = 1000;
    if (argc != 0) digits = atoi(argv[0]);

    dsize = ln_init(dsize);

    k = (digits + dsize - 1) / dsize + 1;
    n = (k - 1) * dsize;
    if (digits != n) {
        fprintf(stderr, "Warning: %d is normalized to %d.\n", digits, n);
    }

    pi = ln_create(k, 0, 1);

    timer_start();
    i = machin(pi, k);
    timer_stop();

    if (!silent_mode) {
        printf("pi:\n");
        ln_print(pi, k, "");
        printf("\n");
    }

    ln_free(pi);

    printf("%d digits: %s seconds, %d iterations.\n", n, timer_str(), i);

    return 0;
}
