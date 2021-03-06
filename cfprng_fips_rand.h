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

#ifndef HEADER_CF_FIPS_RAND_H
# define HEADER_CF_FIPS_RAND_H  

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h> 
#include <string.h>
#include <assert.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bio.h>

#undef CFPRNG_MAX_RAND_BYTES
#define CFPRNG_MAX_RAND_BYTES 1024

  /* ERROR CODES */

#undef CFPRNG_SUCCESS
#define CFPRNG_SUCCESS 0x00U

#undef  CFPRNG_ERR
#define CFPRNG_ERR 0x01U

/*
typedef struct 
{
  int type, flags;
} DRBG_LIST;


static DRBG_LIST drbg_types[] =
  {
    {NID_sha1, 0},
    {NID_sha224, 0},
    {NID_sha256, 0},
    {NID_sha384, 0},
    {NID_sha512, 0},
    {NID_hmacWithSHA1, 0},
    {NID_hmacWithSHA224, 0},
    {NID_hmacWithSHA256, 0},
    {NID_hmacWithSHA384, 0},
    {NID_hmacWithSHA512, 0},
    {NID_aes_128_ctr, 0},
    {NID_aes_192_ctr, 0},
    {NID_aes_256_ctr, 0},
    {NID_aes_128_ctr, DRBG_FLAG_CTR_USE_DF},
    {NID_aes_192_ctr, DRBG_FLAG_CTR_USE_DF},
    {NID_aes_256_ctr, DRBG_FLAG_CTR_USE_DF},
    {(NID_X9_62_prime256v1 << 16)|NID_sha1, 0},
    {(NID_X9_62_prime256v1 << 16)|NID_sha224, 0},
    {(NID_X9_62_prime256v1 << 16)|NID_sha256, 0},
    {(NID_X9_62_prime256v1 << 16)|NID_sha384, 0},
    {(NID_X9_62_prime256v1 << 16)|NID_sha512, 0},
    {(NID_secp384r1 << 16)|NID_sha224, 0},
    {(NID_secp384r1 << 16)|NID_sha256, 0},
    {(NID_secp384r1 << 16)|NID_sha384, 0},
    {(NID_secp384r1 << 16)|NID_sha512, 0},
    {(NID_secp521r1 << 16)|NID_sha256, 0},
    {(NID_secp521r1 << 16)|NID_sha384, 0},
    {(NID_secp521r1 << 16)|NID_sha512, 0},
    {0, 0}
  };

*/

static char fips_label[] = "@(#)FIPS approved RAND"; 

/* 
   args: 
   1) unsigned char* - unsigned char buffer to hold the seed
   2) int - seed length (this should be equal to the length 
      of the buffer
      Not thread safe
   RetVal : CFPRNG_ERR, CFPRNG_SUCCESS
   Currently not implemented
*/

  int cfprng_explicit_seed(unsigned char* buf, int len);


/* 
   Lifted from <openssl>/crypto/rand/randtest.c
   args: None
   Return val : CFPRNG_ERR, CFPRNG_SUCCESS
*/

  int cfprng_fips_tests();

/*
  Currently not implemented
*/

  int cfrprng_set_rand_method();

/* 
   args: (1) unsigned char* - buf of max len CFPRNG_MAX_RAND_BYTES
         (2) int - length of buffer
   Return val : CFPRNG_ERR, CFPRNG_SUCCESS
*/

  int cfprng_nist_rand(unsigned char* buf, int len);

/* 
   Same as above but calls FIPS_mode_set(1)
   args: (1) unsigned char* - buf of max len CFPRNG_MAX_RAND_BYTES
         (2) int - length of buffer
   Return val : CFPRNG_ERR, CFPRNG_SUCCESS
*/

  int cfprng_fips_rand(unsigned char* buf, int len);

  /*    
    args: (1) unsigned char* - buf of max len CFPRNG_MAX_RAND_BYTES
    (2) int - length of buffer
   Return val : none

  */
  void cfprng_pretty_print(unsigned char* buf, int len);

  void cfopenssl_log_err(char* filename, int linenum, char *msg);

  void cfopenssl_log_info(char* filename, int linenum, char *msg);  

  
#ifdef  __cplusplus
}
#endif

#endif                          /* !HEADER_CF_FIPS_RAND_H */



