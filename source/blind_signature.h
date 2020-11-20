#ifndef BLIND_SIGNATURE_H
#define BLIND_SIGNATURE_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <time.h>
#include <openssl/sha.h>

#include "cryptbaselib.h"

#define SHA512_HASH_LENGTH 64
#define NUMBER_OF_QUESTIONS 16

int init_server();
int client(int id_client);
int server(long long *sha512_h, int id_client);
int client2(long long *s_server);
int server2(long long n, long long *s);

#endif