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

#define L4_DIGITS 4
#define L4_MAXNUM 10000  /* 10^L4_DIGITS */

/**********************************************************************/

void l4_print(int c[], int l, char *s)
{
    int i;
    int sp = 1, ep;

    int cols = 80;
    char *env_columns = getenv("COLUMNS");

    if (env_columns != NULL) {
        cols = atoi(env_columns);
        if (cols < 40) cols = 40;
    }

    printf("%3s %3d.", s, c[0]);
    for (i = 1; i < l; i++) {
        printf("%0*d ", L4_DIGITS, c[i]);
        if (i % ((cols-30)/(L4_DIGITS+1)) == 0 && i != l-1) {
            ep = i * L4_DIGITS;
            printf("(%4d-%4d)\n", sp, ep);
            printf("        ");
            sp = ep + 1;
        }
    }
    ep = (l-1) * L4_DIGITS;
    printf("(%4d-%4d)\n", sp, ep);
}

void l4_add(int c[], int a[], int b[], int l)
{
    int i, cy = 0;
    for (i = l; i >= 0; i--) {
        c[i] = a[i] + b[i] + cy;
        if (c[i] < L4_MAXNUM) {
            cy = 0;
        } else {
            c[i] = c[i] - L4_MAXNUM;
            cy = 1;
        }
    }
}

void l4_sub(int c[], int a[], int b[], int l)
{
    int i, br = 0;
    for (i = l; i >= 0; i--) {
        c[i] = a[i] - b[i] - br;
        if (c[i] >= 0) {
            br = 0;
        } else {
            c[i] = c[i] + L4_MAXNUM;
            br = 1;
        }
    }
}

void l4_div(int c[], int a[], int b, int l)
{
    int i, d, rem = 0;
    for (i = 0; i <= l; i++){
        d = a[i];
        c[i] = (d + rem) / b;
        rem = ((d + rem) % b) * L4_MAXNUM;
    }
}

int l4_eps(int a[], int l)
{
    int i;
    for (i = 0; i < l; i++) {
        if (a[i] != 0) {
            return 0;
        }
    }
    if (a[l] >= 1) {
        return 0;
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
    extern int optind;

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

int machin(int pi[], int k)
{
    int *a, *b, *c;
    int m;
    int i;

    a  = (int *)calloc(k+3, sizeof(int));
    b  = (int *)calloc(k+3, sizeof(int));
    c  = (int *)calloc(k+3, sizeof(int));
    if (a == NULL || b == NULL || c == NULL) {
        fprintf(stderr, "Error: Can't allocate memory.\n");
        exit(1);
    }

    /* Machin's Formula */
    a[0] = 16;
    l4_div(a, a, 5, k+1);            /* a = 16 / 5  */
    b[0] = 4;
    l4_div(b, b, 239, k+1);          /* b = 4 / 239 */
    l4_sub(c, a, b, k+1);            /* c = a - b   */
    m = 1;
    i = 0;
    do {
        if (i % 2 == 0) {
            l4_add(pi, pi, c, k+1);  /* pi = pi + (-1)^i * (a - b) */
        } else {
            l4_sub(pi, pi, c, k+1);
        }

        i++;
        if (verbose_mode) {
            printf("%dth:\n", i);
            l4_print(c, k+1, i % 2 ? "+" : "-");
            l4_print(pi, k+1, "");
            printf("\n");
        }

        l4_div(a, a, 5*5, k+1);      /* a = a / 5^2   */
        l4_div(b, b, 239*239, k+1);  /* b = b / 239^2 */
        l4_sub(c, a, b, k+1);
        m += 2;
        l4_div(c, c, m, k+1);        /* c = (a - b) / (2*i+1) */
    } while (!l4_eps(c, k));

    free(a);
    free(b);
    free(c);

    return i;
}

int main(int argc, char *argv[])
{
    int *pi;
    int k, n, digits;
    int i;
    int opts;

    program_name = argv[0];
    opts = parse_options(argc, argv);
    argc -= opts;
    argv += opts;

    digits = 1000;
    if (argc != 0) digits = atoi(argv[0]);

    k = (digits + L4_DIGITS - 1) / L4_DIGITS + 1;
    n = (k - 1) * L4_DIGITS;
    if (digits != n) {
        fprintf(stderr, "Warning: %d is normalized to %d.\n", digits, n);
    }

    pi = (int *)calloc(k+3, sizeof(int));
    if (pi == NULL) {
        fprintf(stderr, "Error: Can't allocate memory.\n");
        exit(1);
    }

    timer_start();
    i = machin(pi, k);
    timer_stop();

    if (!silent_mode) {
        printf("pi:\n");
        l4_print(pi, k, "");
        printf("\n");
    }

    free(pi);

    printf("%d digits: %s seconds, %d iterations.\n", n, timer_str(), i);

    return 0;
}
