#include "cfprng_fips_rand.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>



//#include <openssl/e_os.h>

/* some FIPS 140-1 random number test */
/* taken from openssl-1.0.1h <openssl>/crpyto/rand/randtest.c */

int cfprng_fips_tests()
{
    unsigned char buf[2500];
    int i, j, k, s, sign, nsign, err = 0;
    unsigned long n1;
    unsigned long n2[16];
    unsigned long runs[2][34];

    /*
     * double d;
     */
    long d;

    /* suppress CFPRNG_MAX_RAND_BYTES check */
    i = RAND_bytes(buf, 2500);
    if (i < 0) {
#ifdef CFPRNG_LOG_LEVEL_ERR
      fprintf(stderr, "%s : ln %d :init failed, the rand method is not properly installed\n", __FILE__, __LINE__);
#endif
      err++;
      goto err;
    }

    n1 = 0;
    for (i = 0; i < 16; i++)
        n2[i] = 0;
    for (i = 0; i < 34; i++)
        runs[0][i] = runs[1][i] = 0;

    /* test 1 and 2 */
    sign = 0;
    nsign = 0;
    for (i = 0; i < 2500; i++) {
        j = buf[i];

        n2[j & 0x0f]++;
        n2[(j >> 4) & 0x0f]++;

        for (k = 0; k < 8; k++) {
            s = (j & 0x01);
            if (s == sign)
                nsign++;
            else {
                if (nsign > 34)
                    nsign = 34;
                if (nsign != 0) {
                    runs[sign][nsign - 1]++;
                    if (nsign > 6)
                        runs[sign][5]++;
                }
                sign = s;
                nsign = 1;
            }

            if (s)
                n1++;
            j >>= 1;
        }
    }
    if (nsign > 34)
        nsign = 34;
    if (nsign != 0)
        runs[sign][nsign - 1]++;

    /* test 1 */
    if (!((9654 < n1) && (n1 < 10346))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
      fprintf(stderr, "%s : ln %d : test 1 failed, X=%lu\n", __FILE__, __LINE__, n1);
#endif
      err++;
    }
#ifdef CFPRNG_LOG_LEVEL_INFO
    fprintf(stderr, "%s : ln %d : test 1 done\n", __FILE__, __LINE__);
#endif

    /* test 2 */
#ifdef undef
    d = 0;
    for (i = 0; i < 16; i++)
        d += n2[i] * n2[i];
    d = d * 16.0 / 5000.0 - 5000.0;
    if (!((1.03 < d) && (d < 57.4))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
      fprintf(stderr, "%s : ln %d : test 2 failed, X=%.2f\n", __FILE__, __LINE__);
#endif
        err++;
    }
#endif
    d = 0;
    for (i = 0; i < 16; i++)
        d += n2[i] * n2[i];
    d = (d * 8) / 25 - 500000;
    if (!((103 < d) && (d < 5740))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
      fprintf(stderr, "%s : ln %d :test 2 failed, X=%ld.%02ld\n", __FILE__, __LINE__, d / 100L, d % 100L);
#endif
        err++;
    }
#ifdef CFPRNG_LOG_LEVEL_INFO
    fprintf(stderr, "%s : ln %d :test 2 done\n", __FILE__, __LINE__);
#endif
    /* test 3 */
    for (i = 0; i < 2; i++) {
        if (!((2267 < runs[i][0]) && (runs[i][0] < 2733))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
	  fprintf(stderr, "%s : ln %d :test 3 failed, bit=%d run=%d num=%lu\n", __FILE__,  __LINE__,
                   i, 1, runs[i][0]);
#endif
            err++;
        }
        if (!((1079 < runs[i][1]) && (runs[i][1] < 1421))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
	  fprintf(stderr, "%s: ln %d : test 3 failed, bit=%d run=%d num=%lu\n", __FILE__, __LINE__,
                   i, 2, runs[i][1]);
#endif
            err++;
        }
        if (!((502 < runs[i][2]) && (runs[i][2] < 748))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
	  fprintf(stderr, "%s : ln %d :test 3 failed, bit=%d run=%d num=%lu\n", __FILE__, __LINE__,
                   i, 3, runs[i][2]);
#endif
            err++;
        }
        if (!((223 < runs[i][3]) && (runs[i][3] < 402))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
	  fprintf(stderr, "%s : ln %d :test 3 failed, bit=%d run=%d num=%lu\n", __FILE__, __LINE__,
                   i, 4, runs[i][3]);
#endif
            err++;
        }
        if (!((90 < runs[i][4]) && (runs[i][4] < 223))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
	  fprintf(stderr, "%s : ln %d :test 3 failed, bit=%d run=%d num=%lu\n",__FILE__, __LINE__,
                   i, 5, runs[i][4]);
#endif
            err++;
        }
        if (!((90 < runs[i][5]) && (runs[i][5] < 223))) {
#ifdef CFPRNG_LOG_LEVEL_ERR
	  fprintf(stderr, "%s : ln %d : test 3 failed, bit=%d run=%d num=%lu\n", __FILE__, __LINE__,
                   i, 6, runs[i][5]);
#endif
            err++;
        }
    }
#ifdef CFPRNG_LOG_LEVEL_INFO
    fprintf(stderr, "%s : ln %d : test 3 done\n", __FILE__, __LINE__);
#endif

    /* test 4 */
    if (runs[0][33] != 0) {
#ifdef CFPRNG_LOG_LEVEL_ERR
      fprintf(stderr, "%s : ln %d : test 4 failed, bit=%d run=%d num=%lu\n", __FILE__, __LINE__, 0, 34, runs[0][33]);
#endif
        err++;
    }
    if (runs[1][33] != 0) {
#ifdef CFPRNG_LOG_LEVEL_ERR
      fprintf(stderr, "%s : ln %d : test 4 failed, bit=%d run=%d num=%lu\n", __FILE__, __LINE__,1, 34, runs[1][33]);
#endif
        err++;
    }
#ifdef CFPRNG_LOG_LEVEL_INFO
    fprintf(stderr, "%s : ln %d :test 4 done\n",__FILE__, __LINE__);
#endif
 err:
    err = ((err) ? 1 : 0);

    return (err);
}