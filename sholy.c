#include "polarssl/aes.h"
#include <stdio.h>
#include <string.h>
static const unsigned char aes_ecb_dec[3][16] = {
  { 0x44, 0x41, 0x6A, 0xC2, 0xD1, 0xF5, 0x3C, 0x58, 0x33, 0x03, 0x91, 0x7E, 0x6B, 0xE9, 0xEB, 0xE0 },
  { 0x48, 0xE3, 0x1E, 0x9E, 0x25, 0x67, 0x18, 0xF2, 0x92, 0x29, 0x31, 0x9C, 0x19, 0xF1, 0x5B, 0xA4 },
  { 0x05, 0x8C, 0xCF, 0xFD, 0xBB, 0xCB, 0x38, 0x2D, 0x1F, 0x6F, 0x56, 0x58, 0x5D, 0x8A, 0x4A, 0xDE }
}; //Testing Array Dec
static const unsigned char aes_ecb_enc[3][16] = {
  { 0xC3, 0x4C, 0x05, 0x2C, 0xC0, 0xDA, 0x8D, 0x73, 0x45, 0x1A, 0xFE, 0x5F, 0x03, 0xBE, 0x29, 0x7F },
  { 0xF3, 0xF6, 0x75, 0x2A, 0xE8, 0xD7, 0x83, 0x11, 0x38, 0xF0, 0x41, 0x56, 0x06, 0x31, 0xB1, 0x14 },
  { 0x8B, 0x79, 0xEE, 0xCC, 0x93, 0xA0, 0xEE, 0x5D, 0xFF, 0x30, 0xB4, 0xEA, 0x21, 0x63, 0x6D, 0xA4 }
}; //Testing Array Enc
int RunTests(int verbose) {
  int i, j, u, v;
  unsigned char key[32];
  unsigned char buf[64];
  aes_context ctx;
  memset( key, 0, 32 );
  for( i = 0; i < 6; i++ ) {
	u = i >> 1;
    v = i  & 1;
    if( verbose != 0 )
      printf( "  AES-ECB-%3d (%s): ", 128 + u * 64,
        ( v == AES_DECRYPT ) ? "dec" : "enc" );
      memset( buf, 0, 16 );
      if( v == AES_DECRYPT ) {
        aes_setkey_dec( &ctx, key, 128 + u * 64 );
        for( j = 0; j < 10000; j++ )
          aes_crypt_ecb( &ctx, v, buf, buf );
        if( memcmp( buf, aes_ecb_dec[u], 16 ) != 0 ) {
          if( verbose != 0 ) printf( "failed\n" );
          return( 1 );
		  }
	    }
      else {
        aes_setkey_enc( &ctx, key, 128 + u * 64 );
        for( j = 0; j < 10000; j++ )
          aes_crypt_ecb( &ctx, v, buf, buf );
        if( memcmp( buf, aes_ecb_enc[u], 16 ) != 0 ) {
          if( verbose != 0 ) printf( "failed\n" );
          return( 1 );
          }
        }
      if( verbose != 0 ) printf( "passed\n" );
    }
  return 0;
}
unsigned char* Encrypt(unsigned char* key,unsigned char* buf) {
  int i, j, u, v;
  aes_context ctx;
  u = 2; //5 >> 1; 101 + RSH 1 = 10 
  v = 1; //Encrypt
  aes_setkey_enc( &ctx, key, 128 + u * 64 );
  for( j = 0; j < 10000; j++ ) 
	aes_crypt_ecb( &ctx, v, buf, buf );
  return buf;
}
unsigned char* Decrypt(unsigned char* key,unsigned char* buf) {
  int i, j, u, v;
  aes_context ctx;
  u = 2; //4 >> 1; 100 + RSH 1 = 10 
  v = 0; //Decrypt
  aes_setkey_enc( &ctx, key, 128 + u * 64 );
  for( j = 0; j < 10000; j++ ) 
	aes_crypt_ecb( &ctx, v, buf, buf );
  return buf;
}
int main(void) {
  RunTests(1); // default tests
  printf("tests passed\n");
  unsigned char key[32];
  unsigned char buf[64];
  memset( key, 0, 32 );
  memset( buf, 0, 16 );
  Encrypt(key,buf);
  Decrypt(key,buf);
  printf("bye\n");
}
