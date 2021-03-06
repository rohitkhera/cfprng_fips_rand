/************************************************************************
*    R Khera 10/31/2017
*   ANSI C wrapper code for 
*   (1)  
*   generating pseudo random  bytes 
*   Using NIST SP 800-90A algos with the OpenSSL FIPS module 
*   The goal in CF is to only use 
*   a PRNG specified in NIST SP 800-90A 
*   Enabling OpenSSL FIPS mode only allows algorithms 
*   specified in SP 800-90A and ANS X9.31
*   However, we would prefer to just use algorithms 
*   from SP800-90A over ANS X9.31
*   Among the SP800-90A specified algorithms, we 
*   must avoid "Dual_EC_DRBG". 
*   (2)
*   RSA 4K key gen and cert gen 
*   (3) 
*   AES GCM 128
*   Tested wth openssl-fips-2.0.11 and openssl-1.0.1r
*   This file is compiled into a shared object that should 
*   be placed in the caller's LD_LIBRARY_PATH 
*  Apple LLVM version 8.1.0 (clang-802.0.42)
*  Target: x86_64-apple-darwin16.5.0
* Thread model: posix
* Also tested with gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1) 
*  Also see Makefile for more on compiler versions, 
*  fipsld, ld.so / c flags etc.
*
*************************************************************************/    

/* some FIPS 140-1 random number test */
/* taken from openssl-1.0.1h <openssl>/crpyto/rand/randtest.c */

#include "cfprng_fips_rand.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>

//#include <openssl/e_os.h>


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
      cfopenssl_log_err(__FILE__,__LINE__,"init failed, the rand method is not properly installed");
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
      cfopenssl_log_err(__FILE__,__LINE__,"test 1 failed");      
      err++;
    }
	
    cfopenssl_log_info(__FILE__,__LINE__,"test 1 done");      	

    /* test 2 */
#ifdef undef
    d = 0;
    for (i = 0; i < 16; i++)
        d += n2[i] * n2[i];
    d = d * 16.0 / 5000.0 - 5000.0;
    if (!((1.03 < d) && (d < 57.4))) {
      cfopenssl_log_err(__FILE__,__LINE__,"test 2 failed");      	      
      err++;
    }
#endif
    d = 0;
    for (i = 0; i < 16; i++)
        d += n2[i] * n2[i];
    d = (d * 8) / 25 - 500000;
    if (!((103 < d) && (d < 5740))) {
      cfopenssl_log_err(__FILE__,__LINE__,"test 2 failed");      	            
      err++;
    }
    cfopenssl_log_info(__FILE__,__LINE__,"test 2 done");      	                

    /* test 3 */
    for (i = 0; i < 2; i++) {
        if (!((2267 < runs[i][0]) && (runs[i][0] < 2733))) {

	  cfopenssl_log_err(__FILE__,__LINE__,"test 3 failed");      	                	  
	  err++;
        }
        if (!((1079 < runs[i][1]) && (runs[i][1] < 1421))) {
	  cfopenssl_log_err(__FILE__,__LINE__,"test 3 failed");      	
	  err++;
        }
        if (!((502 < runs[i][2]) && (runs[i][2] < 748))) {
	  cfopenssl_log_err(__FILE__,__LINE__,"test 3 failed");      	        
	  err++;
        }
        if (!((223 < runs[i][3]) && (runs[i][3] < 402))) {
	  cfopenssl_log_err(__FILE__,__LINE__,"test 3 failed");      	                	  	  	  	  
	  err++;
        }
        if (!((90 < runs[i][4]) && (runs[i][4] < 223))) {
	  cfopenssl_log_err(__FILE__,__LINE__,"test 3 failed");      	                	  	  	  	  	  
	  err++;
        }
        if (!((90 < runs[i][5]) && (runs[i][5] < 223))) {
	  cfopenssl_log_err(__FILE__,__LINE__,"test 3 failed");      	                	  	  	  	  	  	  
	  err++;
        }
    }

    cfopenssl_log_info(__FILE__,__LINE__,"test 3 done");      	                	  	  	  	  	  	      

    /* test 4 */
    if (runs[0][33] != 0) {
      cfopenssl_log_err(__FILE__,__LINE__,"test 4 failed");      	                	  	  	  	  	  	     
      err++;
    }
    if (runs[1][33] != 0) {
      cfopenssl_log_err(__FILE__,__LINE__,"test 4 failed");      	                	  	  	  	  	  	     
      err++;
    }

    cfopenssl_log_info(__FILE__,__LINE__,"test 4 done");      	                	  	  	  	  	  	         

 err:
    err = ((err) ? 1 : 0);

    return (err);
}
