//
//  payload_demo.c
//  AES_codedecode
//
//  Created by Andreas (Admin) on 28.06.13.
//  Copyright (c) 2013 Andreas (Admin). All rights reserved.
//

/*
*   Demo C Program Develop for Master Thesis
*   "Data transmission using QR Codes. Leveraging a consumer trend for medical communication."
*   Donau Universität Krems, Institut für IT im Gesundheitswesen Konstanz
*
*   Purpose: provinding the functionality to AES encrypt, base64 endcode, base64 decode and AES decrypt 
*            of a string (payload) used as part of an URL for the transmission of a diagnostics PT result
*            into a PHR (personal health record)
*
*   For AES en/de-cryption the following public code was used: 
*            Byte-oriented AES-256 implementation. aes256.c, Ilya O. Levin,
*            http://www.literatecode.com
*   For base64 en/de-coding the following public code was used:
*            Base64EncoderDecoder in C, Ansi-style, base64.h, Sam Ernest Kumar, 
*            http://knol2share.blogspot.de/2011/07/base64-encoding-and-decoding-in-c.html
*    
*   Input values, to be entered when asked via console:
*
*       - str: any string to encode-encrypt (e.g. payload as part of URL)
*       - secretkey: AES256 encrption key (length must be 256/8 bit = 32 characters)
*/

#include <stdlib.h>
#include <stdio.h>
#include "aes256.h"
#include "base64.h"

#define DUMP(s, i, buf, sz)  {printf(s);                   \
                              for (i = 0; i < (sz);i++)    \
                                  printf("%02x ", buf[i]); \
                              printf("\n");}


int main (int argc, char *argv[])
{
    /* Variables for aes256 */
    
    aes256_context ctx;
    uint8_t key[32];
    uint8_t buf[16];
    
    uint8_t text[352];
    uint8_t textout[352];
    int i, j, k, a, lestr, rest, textlen;
    char secretkey[32];
    char str[480];
    
    /* Variables for base64 */
    
    int rc = 0;
    // unsigned char input[BUFFFERLEN + 1] = "";
    uint8_t inputb64[BUFFFERLEN + 1];
    char encodedoutput[BUFFFERLEN + 1] = "";
    unsigned char decodedoutput[BUFFFERLEN + 1] = "";
    
    /* START WITH DEMO APPLICATION */
    
    /* Get input string (e.g. URL payload), if less than a multiple of 16, 
       then the string is filled up to 16  with '#' as delimiter */
    
    // printf("I-1.) Enter any string to encode (maximum length: 30*16 = 480 chars): ");
    
    // scanf("%s",str);
    
    char *p;
    
    printf ("I-1.) Enter any string to encode, max %ld characters:\n", sizeof(str));
    
    if (fgets(str, sizeof(str), stdin) != NULL)
    {
        /*
         *  Now test for, and remove that newline character
         */
        if ((p = strchr(str, '\n')) != NULL)
            *p = '\0';
        
        printf ("You entered: >%s<\n", str);
    }
    
    printf("------\n");
    
    lestr = (int)strlen(str);
    printf("Length of input string: %i\n", lestr);
    rest = 0;
    if (lestr % 16){rest = 16 - (lestr % 16);}
    printf("Residuals characters to multiple of 16 char: %i\n", rest);
    
    for (k=0; k<rest; k++) {
        a = (int)strlen(str);
        str[a] = '#';
        str[a+1] = '\0';
    }
    
    printf("Calculate ASCII values as HEX of each character of given string, and filled up (with '#') to multiple of 16 chars (for AES256)\n");
    
    for (k = 0; k<strlen(str); k++){
        a = str[k];
        text[k] = a;
    };
    textlen = (int)strlen(str);

    DUMP("Input string values (hex): ", i, text, textlen);
    printf("------\n");
    
    /* End Preparation of input string */

    /* AES256 START */

    printf("I-2.) Enter secret key (maximum length: 256/8 Bit = 32 chars): ");
    scanf("%s",secretkey);

    printf("------\n");
    printf("AES256 key (string):");

    for (k = 0; k<32; k++){
        key[k] = (unsigned char)secretkey[k];
        printf("%c", secretkey[k]);
    };
    printf("\n");
    
    DUMP("AES256 key (hex):   ", i, key, sizeof(key));
    printf("------\n");

    aes256_init(&ctx, key);
    
    for (k=0; k<(textlen/16); k++) {
        for (i=0; i<16; i++) {
            buf[i] = text[k*16+i];
        }
        
        aes256_encrypt_ecb(&ctx, buf);
    
        // printf("%i ", k+1);
        // DUMP("AES256 encrypted text (hex): ", i, buf, sizeof(buf));
        // printf("...\n");
        
        for (j = 0; j<16; j++){
            // input[j+k*16] = (unsigned char)buf[j];
            inputb64[j+k*16] = buf[j];
        };
    }
    printf("Running AES encryption for %i",k);
    printf(" time(s) (in blocks of 16 bytes)\n");
    
    printf("------\n");

    printf("O-A. Output of AES256 encryption is input to BASE64 encoding: ");
    DUMP(" ", i, inputb64, textlen);
    
    /* BASE64 START */
    
    printf("------\n");
    
    rc = Base64Encode(inputb64, encodedoutput, textlen, BUFFFERLEN, 0);

    printf("O-B. BASE64 encoded value (ASCII): %s\n", encodedoutput);
    
    printf("\n");
    
    printf("**************************************************\n");
    
    printf("NOW: going backwards - decoding and decryption ...\n");
           
    printf("**************************************************\n");
    
    rc = Base64Decode(encodedoutput, decodedoutput, BUFFFERLEN, 0);
    
    printf("\n");
    
    DUMP("O-C. BASE64 decoded value (hex): ", i, decodedoutput, textlen);
    
    /* BASE64 END */
    
    printf("------\n");
    
    printf("Output of BASE64 decoded text is input to AES256 decryption ... \n");
    
    aes256_init(&ctx, key);
    
    for (k=0; k<(textlen/16); k++) {
        for (i=0; i<16; i++) {
            buf[i] = decodedoutput[k*16+i];
        }
        
        aes256_decrypt_ecb(&ctx, buf);
    
        // printf("%i ", k+1);
        // DUMP("AES256 decrypted text (hex): ", i, buf, sizeof(buf));
        // printf("...\n");
        
        for (j = 0; j<16; j++){
            textout[j+k*16] = (unsigned char)buf[j];
        };
    }
    
    printf("------\n");
    printf("O-D. AES256 decrypted text (string): ");
    
    for (k = 0; k< textlen; k++){
        printf("%c", textout[k]);
    };

    printf("\n------\n");
    aes256_done(&ctx);
    
    /* AES256 END */

    return 0;
} /* main */
