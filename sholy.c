/*                 sholy - Light TODO list
		  Copyright (C)  2012  Sholy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA*/

#include "polarssl/aes.h"

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>

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

unsigned char* Encrypt(unsigned char* key,char* buf) {
  int i, j, u, v;
  aes_context ctx;
  u = 2; //5 >> 1; 101 + RSH 1 = 10 
  v = 0; //Encrypt
  aes_setkey_enc( &ctx, key, 128 + u * 64 );
  for( j = 0; j < 10000; j++ ) 
	aes_crypt_ecb( &ctx, v, buf, buf );
  return buf;
}

unsigned char* Decrypt(unsigned char* key,char* buf) {
  int i, j, u, v;
  aes_context ctx;
  u = 2; //4 >> 1; 100 + RSH 1 = 10 
  v = 1; //Decrypt
  aes_setkey_enc( &ctx, key, 128 + u * 64 );
  for( j = 0; j < 10000; j++ ) 
	aes_crypt_ecb( &ctx, v, buf, buf );
  return buf;
}

void help(char* argv) {
	printf( "usage:\n  %s <command>\n  - initdb - init empty database structure\n  - read - to read all\n  - write <msg> - add task\n  - test - testing AES encryption\n", argv );
}
int main(int argc, char *argv[]) {
  int encryption = 0;
  char* key;
  if ( argc < 2 ) {
	help( argv[0] );
	return 0;
    }
  else {
	if ( strcmp(argv[1],"--version") == 0 ) {
	  printf("v0.01\n");
	  return 0;
	  }
	if ( strcmp(argv[1],"--help") == 0 ) {
	  help( argv[0] );
	  return 0;
	  }
	else if  ( strcmp(argv[1],"test") == 0 ) {
	  RunTests(1);
	  return 0;
	  }
	else if ( strcmp(argv[1],"initdb") == 0 
			  || strcmp(argv[1],"read") == 0 
			  || strcmp(argv[1],"clean") == 0
			  || strcmp(argv[1],"rm") == 0
			  || strcmp(argv[1],"write") == 0 ) {
	  //Read config file here:
	  char line[256];
	  int linenum=0;
	  FILE *file = fopen("sholy.conf", "rw");
      if (file == NULL) {
        perror("Failed to open file sholy.conf");
        }
	  else {
	    while(fgets(line, 256, file) != NULL){
          char option[256], value[256];
          linenum++;
          if(line[0] == '#') continue;
          if(sscanf(line, "%s %s", option, value) != 2) {
            fprintf(stderr, "Syntax error, line %d\n", linenum);
            continue;
            }
		  if (  strcmp(option,"encryption") == 0 ) {
			encryption = ( (value[0] == '1') ? 1 : 0);
		    }
		  else if (  strcmp(option,"key") == 0 ) {
			key = value;
		    }
	      }
	    fclose(file);
	  }
	  int retval, x;
	  int q_cnt = 5,q_size = 150,ind = 0;
	  char **queries = (char**)malloc(sizeof(char*) * q_cnt);
	  for (x = 0; x < q_cnt; x++) {
		queries[x] = (char*)malloc(sizeof(char) * q_size);
	    }
	  sqlite3_stmt *stmt;
	  sqlite3 *handle;
	  retval = sqlite3_open("base.db3",&handle);
	  if(retval) {
		printf("Database connection failed\n");
		return -1;
	    }
	  if (  strcmp(argv[1],"initdb") == 0 ) {
		char create_table[100] = "CREATE TABLE IF NOT EXISTS TODO (id INTEGER PRIMARY KEY,text TEXT NOT NULL,enc INTEGER NOT NULL)";
		retval = sqlite3_exec(handle,create_table,0,0,0);
	    }
	  else if ( strcmp(argv[1],"write") == 0 ) {
	    if ( argc < 3) {
	 	  printf("write what?\n");
	      }
		else {
		  queries[ind++] = "SELECT MAX(id) FROM TODO GROUP BY (text)";
		  retval = sqlite3_prepare_v2(handle,queries[ind-1],-1,&stmt,0);
		  if(retval) {
			printf("Inserting data to DB Failed, run initdb first\n");
			return -1;
	        }
		  int last = 0;
		  while (sqlite3_step(stmt) == SQLITE_ROW) {
			last =  atoi(sqlite3_column_text(stmt, 0));
	        }
		  last++;
		  unsigned char key[32];
		  memset( key, 0, 32 );
          char enc[16];
		  if (encryption!=0) {
			memcpy( enc, Encrypt(key,argv[2]), 16 );
			sprintf(queries[ind++], "INSERT INTO TODO VALUES(%d,'%s',1)", last, enc);
		    }
		  else {
			sprintf(queries[ind++], "INSERT INTO TODO VALUES(%d,'%s',0)", last, argv[2]);
		    }
		  retval = sqlite3_exec(handle,queries[ind-1],0,0,0);
		  }
	    }
	  else if ( strcmp(argv[1],"clean") == 0 ) {
		queries[ind++] = "DELETE FROM TODO";
		retval = sqlite3_exec(handle,queries[ind-1],0,0,0);
	    }
	  else if ( strcmp(argv[1],"rm") == 0 ) {
	    if ( argc < 3) {
	 	  printf("rm what?\n");
	      }
		else {
		  sprintf(queries[ind++], "DELETE FROM TODO WHERE id = %s", argv[2]);
		  retval = sqlite3_exec(handle,queries[ind-1],0,0,0);
		  }
	    }
	  else if ( strcmp(argv[1],"read") == 0 ) {
		if ( argc > 2) {
		  sprintf(queries[ind++], "SELECT id, text FROM TODO WHERE id = %s", argv[2]);
		  }
		else {
	      queries[ind++] = "SELECT id, text, enc from TODO";
		  }
	    retval = sqlite3_prepare_v2(handle,queries[ind-1],-1,&stmt,0);
	    if(retval) {
		  printf("Selecting data from DB Failed, run initdb first\n");
		  return -1;
	      }
		printf("======================================================\n");
		unsigned char key[32];
		memset( key, 0, 32 );
        char dec[16];
		int encryption = 0;
		while (sqlite3_step(stmt) == SQLITE_ROW) {
		  printf("%s ", sqlite3_column_text(stmt, 0));
		  encryption =  atoi(sqlite3_column_text(stmt, 2));
		  if (encryption == 1) {
		    memcpy( dec, Decrypt(key, (char *)sqlite3_column_text(stmt, 1) ), 16 );
		    printf(" > %s\n", dec);
			}
		  else {
			printf(" | %s\n", sqlite3_column_text(stmt, 1));
		    }
	      }
        printf("======================================================\n");
	    }
	  sqlite3_close(handle);
	  }
    }
}
