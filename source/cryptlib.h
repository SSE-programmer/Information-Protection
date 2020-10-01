#ifndef CRYPTLIB_H
#define CRYPTLIB_H 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>

#define RANGE 1000

void quickSort(long long **numbers, long long left, long long right);
int isPrime(long long p);

long long BSGS(long long y, long long a, long long p);
//Baby-step Giant-step;

long long DHKG();									 
//Diffie-Hellman Key Generation;

long long FME(long long x, long long y, long long n);	
//Fast modulo exponentiation;

long long GEA_GCD(long long a, long long b, long long *x, long long *y); 
//Generalized Euclidean algorithm for finding the greatest common divisor;

#endif