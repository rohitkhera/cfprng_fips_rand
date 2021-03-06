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
  
 /* generic header for cf_nist_rand and cf_fips_rand functions */

#include "cfprng_fips_rand.h"

int cfprng_nist_rand(unsigned char* buf, int len)
{ 

  if(len > CFPRNG_MAX_RAND_BYTES) {
    cfopenssl_log_err(__FILE__,__LINE__,"length exceeds CFPRNG_MAX_RAND_BYTES");
    return CFPRNG_ERR;
  }

  /* phase this out  since by default RAND_bytes() 
   * should seed from the LRNG entropy pools */

  int rc = RAND_load_file("/dev/urandom", 32);
  if(rc != 32) {
    cfopenssl_log_err(__FILE__,__LINE__,"could not open /dev/urandom");    
    return CFPRNG_ERR;
  }

#ifdef CFOPENSSL_LOG_LEVEL_INFO
    cfprng_pretty_print(buf,len);
#endif

  if (!RAND_bytes(buf, len)) {
    cfopenssl_log_err(__FILE__,__LINE__,"RAND_bytes() fail");        
    return CFPRNG_ERR;
  }
  else {
#ifdef CFOPENSSL_LOG_LEVEL_INFO
    cfprng_pretty_print(buf,len);
#endif
    return CFPRNG_SUCCESS;
  }

}



