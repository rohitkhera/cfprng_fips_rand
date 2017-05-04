/*************************************************************************
*    R Khera 5/3/17 
*   ANSI C wrapper code for 
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
*   Tested wth openssl-fips-2.0.11 and openssl-1.0.1r
*   This file is compiled into a shared object that should 
*   be placed in the caller's LD_LIBRARY_PATH (see 
*  Makefile for compiler versions, fipsld, ld.so / c flags etc.
*************************************************************************/    




#include "cf_fips_rand.h"


/* 
   args: 
   1) unsigned char* - unsigned char buffer to hold the seed
   2) int - seed length (this should be equal to the length 
      of the buffer
*/

void explicit_seed(unsigned char* buf, int len) {

  ;  

}
