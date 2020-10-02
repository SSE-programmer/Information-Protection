#include "cryptlib.h"

void quickSort(long long **as, long long left, long long right) //& 0x7FFFFFFF
{
	long long pivot, pivot1;
	long long l_hold = left;
	long long r_hold = right;

	pivot = as[left][0];
	pivot1 = as[left][1];

	while (left < right)
	{
		while ((as[right][0] >= pivot) && (left < right))
			right--;
		if (left != right)
		{
			as[left][0] = as[right][0];
			as[left][1] = as[right][1];
			left++;
		}
		while ((as[left][0] <= pivot) && (left < right))
			left++;
		if (left != right)
		{
			as[right][0] = as[left][0];
			as[right][1] = as[left][1];
			right--;
		}
	}
	as[left][0] = pivot;
	as[left][1] = pivot1;
	pivot = left;
	left = l_hold;
	right = r_hold;
	if (left < pivot)
		quickSort(as, left, pivot - 1);
	if (right > pivot)
		quickSort(as, pivot + 1, right);
}

int isPrime(long long p)
{
	if (p <= 1)
		return -1;

	int b = (int)pow(p, 0.5);

	for (int i = 2; i <= b; ++i)
	{
		if ((p % i) == 0)
		{
			return -1;
		}
	}

	return 1;
}

/*Прим. y = a^x % p; */
struct BSGS_valid_X BSGS(long long y, long long a, long long p)
{
	long long m = (long long)sqrt(p) + 1; /* K == M */

	long long x = 0;

	long long **U = (long long **)malloc(sizeof(long long *) * (long long)m * 2);

	for (long long i = 0; i < m * 2; i++)
	{
		U[i] = (long long *)malloc(sizeof(long long) * 2);
	}

	for (long long i = 0; i < m; i++)
	{
		U[i][0] = (long long)y * FME(a, i, p) % p;
		U[i][1] = i;  //+1 для второго ряда? 
		U[m + i][0] = FME(a, (long long)m * (i + 1), p);
		U[m + i][1] = (i + 1) | 0x8000000000000000;

	}

	quickSort(U, 0, (long long)m * 2 - 1);

	int flag = 0;

	for (long long i = 0; i < (long long)m * 2 - 1; i++)
	{
		if ((U[i][0] == U[i + 1][0]) && ((U[i][1] & 0x8000000000000000) != (U[i + 1][1] & 0x8000000000000000)))
		{
			flag = 1;

			long long l, j;

			if ((U[i][1] & 0x8000000000000000) == 0)
			{
				j = U[i][1] & 0x7FFFFFFFFFFFFFFF;
				l = U[i + 1][1] & 0x7FFFFFFFFFFFFFFF;
			}
			else
			{
				j = U[i + 1][1] & 0x7FFFFFFFFFFFFFFF;
				l = U[i][1] & 0x7FFFFFFFFFFFFFFF;
			}

			x = l * m - j;

			break;
		}
	}

	long long Y = FME(a, x, p);
	
	if (flag)
	{
		printf("Y = %lld\n", Y);
	}
	
	struct BSGS_valid_X result;

	if (flag)
	{
		result.valid = true;
		result.x = x;
	}
	else
	{
		result.valid = false;
		result.x = -1;
	}

	return result;
}

long long DHKG()
{
	long long p, q, g;

	do
	{
		do
		{
			q = rand() % (1000000000 / 2);
		} while (isPrime(q) < 1);

		p = q * 2 + 1;
	} while (isPrime(p) < 1);

	do
	{
		g = rand() % (p - 1) + 1;
	} while (FME(g, q, p) == 1);

	long long Xa = rand() % 1000000000,
			  Xb = rand() % 1000000000;

	printf("P =%lld, Q = %lld, G = %lld, Xa = %lld, Xb = %lld\n", p, q, g, Xa, Xb);

	long long Ya = FME(g, Xa, p),
			  Yb = FME(g, Xb, p);

	long long Zab = FME(Yb, Xa, p),
			  Zba = FME(Ya, Xb, p);

	if (Zab == Zba)
	{
		return Zab;
	}

	printf("Произошла ошибка, т.к. открытый ключ двух абонентов не совпадает;\n");

	return -1;
}

long long FME(long long a, long long x, long long p)
{
	long long y = 1,
			  s = a;

	while (x)
	{
		if (x & 1)
		{
			y = (long long)y * (long long)s % (long long)p;
		}
		s = (long long)s * (long long)s % (long long)p;
		x >>= 1;
	}

	return y;
}

long long GEA_GCD(long long a, long long b, long long *x, long long *y)
{
	a = abs(a);
	b = abs(b);

	if (a < b)
	{
		a += b;
		b = a - b;
		a -= b;
	}

	long long q = 0;

	long long *U = calloc(3, sizeof(long long)),
			  *V = calloc(3, sizeof(long long)),
			  *T = calloc(3, sizeof(long long));

	U[0] = a;
	U[1] = 1;
	U[2] = 0;

	V[0] = b;
	V[1] = 0;
	V[2] = 1;

	while (V[0] != 0)
	{
		q = (long long)U[0] / (long long)V[0];

		T[0] = (long long)U[0] % (long long)V[0];
		T[1] = U[1] - (long long)V[1] * (long long)q;
		T[2] = U[2] - (long long)V[2] * (long long)q;

		for (int i = 0; i < 3; i++)
		{
			U[i] = V[i];
			V[i] = T[i];
		}
	}

	long long GCD = U[0];
	*x = U[1];
	*y = U[2];

	free(U);
	free(V);
	free(T);

	return GCD;
}