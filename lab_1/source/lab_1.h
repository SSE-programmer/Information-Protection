#ifndef LAB_1_H
#define LAB_1_H 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define RANGE 1000

void quickSort(int **numbers, int left, int right);
int isPrime(int p);
int BSGS();											//Baby-step Giant-step;
int DHKG();									//Diffie-Hellman Key Generation;
int FME(int x, int y, int n);				//Fast modulo exponentiation;
int GEA_GCD(int a, int b, int *x, int *y);	//Generalized Euclidean algorithm for finding the greatest common divisor;

#endif