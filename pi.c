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

#if !defined(LN_DIGITS)
#define LN_DIGITS 4  /* max 9 digits with unsigned int */
#endif /* !defined(LN_DIGITS) */

/**********************************************************************/

static unsigned long long LN_MAXNUM = 1U;

static void ln_init_maxnum(void)
{
    if (LN_MAXNUM == 1) {
        int i;
        for (i = 0; i < LN_DIGITS; i++) {
            LN_MAXNUM *= 10U;
        }
    }
}

void ln_init(void)
{
    ln_init_maxnum();
}

void ln_print(unsigned int c[], size_t l, char *s)
{
    int i;
    size_t sp = 1, ep;

    int cols = 80;
    char *env_columns = getenv("COLUMNS");

    if (env_columns != NULL) {
        cols = atoi(env_columns);
        if (cols < 40) cols = 40;
    }

    printf("%3s %3u.", s, c[0]);
    for (i = 1; i < l; i++) {
        printf("%0*u ", LN_DIGITS, c[i]);
        if (i % ((cols-30)/(LN_DIGITS+1)) == 0 && i != l-1) {
            ep = i * LN_DIGITS;
            printf("(%4zu-%4zu)\n", sp, ep);
            printf("        ");
            sp = ep + 1;
        }
    }
    ep = (l-1) * LN_DIGITS;
    printf("(%4zu-%4zu)\n", sp, ep);
}

void ln_add(unsigned int c[], unsigned int a[], unsigned int b[], size_t l)
{
    int i;
    int cy = 0;
    for (i = l; i >= 0; i--) {
        c[i] = a[i] + b[i] + cy;  /* 0 <= c[i] <= 2*LN_MAXNUM - 1 */
        if (c[i] < LN_MAXNUM) {
            cy = 0;
        } else {
            c[i] -= LN_MAXNUM;
            cy = 1;
        }
    }
}

void ln_sub(unsigned int c[], unsigned int a[], unsigned int b[], size_t l)
{
    int i;
    int br = 0;
    for (i = l; i >= 0; i--) {
        if (a[i] < b[i] + br) { /* b[i] + br <= LN_MAXNUM */
            c[i] = a[i] + (LN_MAXNUM - b[i] - br);
            br = 1;
        } else {
            c[i] = a[i] - b[i] - br;
            br = 0;
        }
    }
}

void ln_div(unsigned int c[], unsigned int a[], unsigned int b, size_t l)
{
    int i;
    unsigned int rem = 0;
    unsigned long long d;
    for (i = 0; i <= l; i++){
        d = a[i] + rem * LN_MAXNUM; /* 0 <= d <= b * LN_MAXNUM - 1 */
        c[i] = d / b;               /* 0 <= c[i] <= LN_MAXNUM - 1 */
        rem = d % b;                /* 0 <= rem <= b-1 */
    }
}

int ln_eps(unsigned int a[], size_t l)
{
    int i;
    for (i = 0; i <= l; i++) {
        if (a[i] != 0) {
            return 0;
        }
    }
    return 1;
}

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

void usage(void)
{
    fprintf(stderr, "usage: %s [-svh] <digits>\n", program_name);
    fprintf(stderr, "            -s: silent mode.\n");
    fprintf(stderr, "            -v: verbose mode.\n");
    fprintf(stderr, "            -h: print usage.\n");
}

int parse_options(int argc, char *argv[])
{
    int ch;
#if defined(__CYGWIN__)
    extern int __declspec(dllimport) optind;
#else /* !defined(__CYGWIN__) */
    extern int optind;
#endif /* defined(__CYGWIN__) */

    while ((ch = getopt(argc, argv, "svh")) != -1) {
        switch (ch) {
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

int machin(int pi[], size_t k)
{
    unsigned int *a, *b, *c;
    unsigned int m;
    int i;

    a  = (unsigned int *)calloc(k+3, sizeof(unsigned int));
    b  = (unsigned int *)calloc(k+3, sizeof(unsigned int));
    c  = (unsigned int *)calloc(k+3, sizeof(unsigned int));
    if (a == NULL || b == NULL || c == NULL) {
        fprintf(stderr, "Error: Can't allocate memory.\n");
        exit(1);
    }

    /* Machin's Formula */
    a[0] = 16;
    ln_div(a, a, 5, k+1);            /* a = 16 / 5  */
    b[0] = 4;
    ln_div(b, b, 239, k+1);          /* b = 4 / 239 */
    ln_sub(c, a, b, k+1);            /* c = a - b   */
    m = 1;
    i = 0;
    do {
        if (i % 2 == 0) {
            ln_add(pi, pi, c, k+1);  /* pi = pi + (-1)^i * (a - b) */
        } else {
            ln_sub(pi, pi, c, k+1);
        }

        i++;
        if (verbose_mode) {
            printf("%dth:\n", i);
            ln_print(c, k+1, i % 2 ? "+" : "-");
            ln_print(pi, k+1, "");
            printf("\n");
        }

        ln_div(a, a, 5*5, k+1);      /* a = a / 5^2   */
        ln_div(b, b, 239*239, k+1);  /* b = b / 239^2 */
        ln_sub(c, a, b, k+1);
        m += 2;
        ln_div(c, c, m, k+1);        /* c = (a - b) / (2*i+1) */
    } while (!ln_eps(c, k));

    free(a);
    free(b);
    free(c);

    return i;
}

int main(int argc, char *argv[])
{
    unsigned int *pi;
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

    k = (digits + LN_DIGITS - 1) / LN_DIGITS + 1;
    n = (k - 1) * LN_DIGITS;
    if (digits != n) {
        fprintf(stderr, "Warning: %d is normalized to %d.\n", digits, n);
    }

    ln_init();

    pi = (unsigned int *)calloc(k+3, sizeof(unsigned int));
    if (pi == NULL) {
        fprintf(stderr, "Error: Can't allocate memory.\n");
        exit(1);
    }

    timer_start();
    i = machin(pi, k);
    timer_stop();

    if (!silent_mode) {
        printf("pi:\n");
        ln_print(pi, k, "");
        printf("\n");
    }

    free(pi);

    printf("%d digits: %s seconds, %d iterations.\n", n, timer_str(), i);

    return 0;
}
