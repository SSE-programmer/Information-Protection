#ifndef ENCRYPTION_H
#define ENCRYPTION_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>

#define FILENAME_LENGTH 256

FILE *open_file(const char *name);
int close_file(FILE *name);
char* concat(char *s1, char *s2);

long long get_prime();
void get_C_D(long long p, long long *C, long long *D);

int encryption_Shamirs_secret_sharing(char *filename);
int encryption_ElGamal(char *filename);
int encryption_RSA(char *filename);
int encryption_Vernam(char *filename);

#endif