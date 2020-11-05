#ifndef DIGITAL_SIGNATURE_H
#define DIGITAL_SIGNATURE_H 

#include "encryptionlib.h"
#include "cryptbaselib.h"
#include <openssl/sha.h>

#define SHA512_HASH_LENGTH 64

#define BEGIN_COLOR_FUNCTION "\033[01;32m"
#define END_COLOR_FUNCTION "\033[0m"

long long mod(long long x, long long p);

int digital_signature_RSA(char *filename);
int digital_signature_ElGamal(char *filename);
int digital_signature_GOST(char *filename);

int test_digital_signature_RSA(char *filename, char *sig_filename);
int test_digital_signature_ElGamal(char *filename, char *sig_filename);
int test_digital_signature_GOST(char *filename, char *sig_filename); 

#endif