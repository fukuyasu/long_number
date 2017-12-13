/*
 * Calculating the numerical value of Napier's constant.
 * Author: FUKUYASU Naoki <fukuyasu@sys.wakayama-u.ac.jp>
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

int calc_napier(LongNumber e, size_t k)
{
    LongNumber a;
    int n;

    a = ln_create(k, 1, 1);        /* a = 1/0! */

    n = 0;
    do {
        ln_add(e, e, a, k);  /* e = e + 1/n! */

        n++;
        if (verbose_mode) {
            printf("%dth:\n", n);
            ln_print(a, k+1, "+");
            ln_print(e, k+1, "");
            printf("\n");
        }

        ln_div(a, a, n, k);      /* a = a/n (a = 1/n!) */
    } while (!ln_eps(a, k));

    ln_free(a);

    return n;
}

int main(int argc, char *argv[])
{
    LongNumber e;
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

    e = ln_create(k, 0, 1);

    timer_start();
    i = calc_napier(e, k);
    timer_stop();

    if (!silent_mode) {
        printf("e:\n");
        ln_print(e, k, "");
        printf("\n");
    }

    ln_free(e);

    printf("%d digits: %s seconds, %d iterations.\n", n, timer_str(), i);

    return 0;
}
