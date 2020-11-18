#ifndef CRYPTBASELIB_H
#define CRYPTBASELIB_H 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>

#define BEGIN_COLOR_FUNCTION "\033[01;32m"
#define END_COLOR_FUNCTION "\033[0m"

#define BEGIN_COLOR_DATE "\033[01;32m"
#define END_COLOR_DATE "\033[0m"

#define RANGE 1000

typedef enum boolean {False, True} boolean;

struct BSGS_valid_X
{
    long long x;
    boolean valid;
};

void quickSort(long long **numbers, long long left, long long right);
int isPrime(long long p);

struct BSGS_valid_X BSGS(long long y, long long a, long long p);
//Baby-step Giant-step;

long long DHKG();									 
//Diffie-Hellman Key Generation;

long long FME(long long x, long long y, long long n);	
//Fast modulo exponentiation;

long long GEA_GCD(long long a, long long b, long long *x, long long *y); 
//Generalized Euclidean algorithm for finding the greatest common divisor;

#endif