//
//  main.c
//  AES_codedecode
//
//  Created by Andreas (Admin) on 15.06.13.
//  Copyright (c) 2013 Andreas (Admin). All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include "aes256.h"

#define DUMP(s, i, buf, sz)  {printf(s);                   \
for (i = 0; i < (sz);i++)    \
printf("%02x ", buf[i]); \
printf("\n");}

//int main (int argc, char *argv[])
int main ()
{
    aes256_context ctx;
    uint8_t key[32];
    uint8_t buf[16], i;
    
    /* put a test vector */
    for (i = 0; i < sizeof(buf);i++) buf[i] = i * 16 + i;
    for (i = 0; i < sizeof(key);i++) key[i] = i;
    
    DUMP("txt: ", i, buf, sizeof(buf));
    DUMP("key: ", i, key, sizeof(key));
    printf("---\n");
    
    aes256_init(&ctx, key);
    aes256_encrypt_ecb(&ctx, buf);
    
    DUMP("enc: ", i, buf, sizeof(buf));
    printf("tst: 8e a2 b7 ca 51 67 45 bf ea fc 49 90 4b 49 60 89\n");
    
    aes256_init(&ctx, key);
    aes256_decrypt_ecb(&ctx, buf);
    DUMP("dec: ", i, buf, sizeof(buf));
    
    aes256_done(&ctx);
    
    return 0;
} /* main */
