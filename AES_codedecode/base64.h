//
//  base64.h
//  AES_codedecode
//
//  Created by Andreas (Admin) on 28.06.13.
//  Copyright (c) 2013 Andreas (Admin). All rights reserved.
//

#ifndef AES_codedecode_base64_h
#define AES_codedecode_base64_h

/**
 * http://knol2share.blogspot.de/2011/07/base64-encoding-and-decoding-in-c.html
 */

/*
 ============================================================================
 Name        : Base64EncodeDecode.c
 Author      : Sam Ernest Kumar
 Version     :
 Copyright   :
 Description : Base64EncoderDecoder in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Macro definitions */
#define TABLELEN        63
#define BUFFFERLEN      480

#define ENCODERLEN      4
#define ENCODEROPLEN    0
#define ENCODERBLOCKLEN 3

#define PADDINGCHAR     '='

/* #define BASE64CHARSET   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
"abcdefghijklmnopqrstuvwxyz"\
"0123456789"\
"+/"; */

/* URL safe Variante: -_ anstelle von +/*/

#define BASE64CHARSET   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
 "abcdefghijklmnopqrstuvwxyz"\
 "0123456789"\
 "-_";

/* Function prototypes */
int Base64Encode(uint8_t *input, char *output, int inputlen, int oplen, int debug);
int encodeblock(uint8_t *input, char *output, int oplen, int deltas, int debug);
int Base64Decode(char *input, uint8_t *output, int oplen, int debug);
int decodeblock(char *input, uint8_t *output, int oplen, int debug, int oindex);
/* Its always better to move the macros and function prototypes to a header file */


int decodeblock(char *input, uint8_t *output, int oplen, int debug, int oindex){
    int rc = 0;
    uint8_t decodedstr[ENCODERLEN + 1] = "";
    
    decodedstr[0] = input[0] << 2 | input[1] >> 4;
    decodedstr[1] = input[1] << 4 | input[2] >> 2;
    decodedstr[2] = input[2] << 6 | input[3] >> 0;

    output[oindex-3] = decodedstr[0];
    output[oindex-2] = decodedstr[1];
    output[oindex-1] = decodedstr[2];
    
    if (debug){
        printf("%i ", oindex);
        printf("%02x ",decodedstr[0]);
        printf("%02x ",decodedstr[1]);
        printf("%02x ",decodedstr[2]);
        printf("\n");
    }
    
    return rc;
}

int Base64Decode(char *input, uint8_t *output, int oplen, int debug){
    char *charval = 0;
    char decoderinput[ENCODERLEN + 1] = "";
    char encodingtable[TABLELEN + 1] = BASE64CHARSET;
    int index = 0, asciival = 0, computeval = 0, iplen = 0, rc = 0, oindex = 3;
    
    iplen = (int)strlen(input);
    while(index < iplen){
        asciival = (int)input[index];
        if(asciival == PADDINGCHAR){
            rc = decodeblock(decoderinput, output, oplen, debug, oindex);
            break;
        }else{
            charval = strchr(encodingtable, asciival);
            if(charval){
                decoderinput[computeval] = charval - encodingtable;
                computeval = (computeval + 1) % 4;
                 if(computeval == 0){
                    rc = decodeblock(decoderinput, output, oplen, debug, oindex);
                    oindex=oindex+3;
                    decoderinput[0] = decoderinput[1] =
                    decoderinput[2] = decoderinput[3] = 0;
                 }
            }
        }
        index++;
    }
    
    return rc;
}

int encodeblock(uint8_t *input, char *output, int oplen, int deltas, int debug){
    int rc = 0;
    char encodedstr[ENCODERLEN + 1] = "";
    char encodingtabe[TABLELEN + 1] = BASE64CHARSET;
    
    encodedstr[0] = encodingtabe[ input[0] >> 2 ];
    encodedstr[1] = encodingtabe[ ((input[0] & 0x03) << 4) |
                                 ((input[1] & 0xf0) >> 4) ];
    encodedstr[2] = (deltas < 2 ? encodingtabe[ ((input[1] & 0x0f) << 2) |
                                                  ((input[2] & 0xc0) >> 6) ] : PADDINGCHAR);
    encodedstr[3] = (deltas < 1 ? encodingtabe[ input[2] & 0x3f ] : PADDINGCHAR);

    strncat(output, encodedstr, oplen-strlen(output));
    
    if(debug){
        printf("%s",encodedstr);
        printf("\n");
    }
    
    return rc;
}

int Base64Encode(uint8_t *input, char *output, int inputlen, int oplen, int debug){
    int rc = 0, del = 0;
    int index = 0, ipindex = 0, iplen = 0;
    uint8_t encoderinput[ENCODERBLOCKLEN + 1] = "";
    
    iplen = inputlen;
    
    while(ipindex < iplen){
        for(index = 0; index < 3; index++){
            if(ipindex < iplen){
                encoderinput[index] = input[ipindex];
            }else{
                encoderinput[index] = 0;
            }
            ipindex++;
        }

        del = ipindex - iplen;
        
        if(debug){
        printf("iplen = %i ,", iplen);
        printf("ipindex = %i ,", ipindex);
        printf("delta = %i ,", del);
        printf("%02x ", encoderinput[0]);
        printf("%02x ", encoderinput[1]);
        printf("%02x ", encoderinput[2]);
        printf(" => ");}
        

        
        rc = encodeblock(encoderinput, output, oplen, del, debug);
    }
    
    return rc;
}

#endif
