#ifndef DIGITAL_SIGNATURE_H
#define DIGITAL_SIGNATURE_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <openssl/sha.h>

#include "cryptbaselib.h"

#define SHA512_HASH_LENGTH 64

int digital_signature_RSA(char *filename);
int digital_signature_ElGamal(char *filename);
int digital_signature_GOST(char *filename);

int test_digital_signature_RSA(char *filename, char *sig_filename);
int test_digital_signature_ElGamal(char *filename, char *sig_filename);
int test_digital_signature_GOST(char *filename, char *sig_filename); 

#endif