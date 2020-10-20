#include "encryptionlib.h"
#include "cryptbaselib.h"

FILE *open_file(const char *name)
{
	FILE *f;

	if ((f = fopen(name, "rb")) == NULL)
	{
		printf("Не удалось открыть файл\n");
		exit(201);
	}

	return f;
}

int close_file(FILE *name)
{
	fclose(name);

	return 0;
}

char *concat(char *s1, char *s2)
{
	char *result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

long long get_prime()
{
	long long number;

	do
	{

		number = rand() % (1000000000);

	} while (isPrime(number) < 1);

	return number;
}

void get_C_D(long long p, long long *C, long long *D)
{
	long long x, y;

	do
	{

		*C = rand() % p;

	} while (isPrime(*C) < 1 || GEA_GCD(p - 1, *C, &x, &y) > 1);

	*D = y > 0 ? y : y + p - 1;
}

int encryption_Shamirs_secret_sharing(char *filename)
{
	system("rm -rf temp/");
	system("mkdir -p temp");

	FILE *file = open_file(filename);

	FILE *keys_file = fopen("temp/keys.txt", "w+");

	FILE *f1 = fopen("temp/f1", "wb");
	FILE *f2 = fopen("temp/f2", "wb");
	FILE *f3 = fopen("temp/f3", "wb");
	FILE *f4 = fopen("temp/f4", "wb");

	fseek(file, 0, SEEK_END);

	long file_length = ftell(file);

	fseek(file, 0, SEEK_SET);

	long long p, Ca, Da, Cb, Db;

	p = get_prime();

	for (long i = 0; !feof(file) && i < file_length; i++)
	{
		long long one_byte = fgetc(file);

		get_C_D(p, &Ca, &Da);
		get_C_D(p, &Cb, &Db);

		if (i == 0)
		{
			fprintf(keys_file, "p = %lld\n", p);
		}

		fprintf(keys_file, "[%ld]Ca = %lld, Da = %lld, Cb = %lld, Db = %lld\n", i + 1, Ca, Da, Cb, Db);

		one_byte = FME(one_byte, Ca, p);
		fputc(one_byte, f1);

		one_byte = FME(one_byte, Cb, p);
		fputc(one_byte, f2);

		one_byte = FME(one_byte, Da, p);
		fputc(one_byte, f3);

		one_byte = FME(one_byte, Db, p);
		fputc(one_byte, f4);
	}

	fclose(f4);
	fclose(f3);
	fclose(f2);
	fclose(f1);
	fclose(keys_file);
	fclose(file);

	return 0;
}

int encryption_ElGamal(char *filename)
{
	system("rm -rf temp/");
	system("mkdir -p temp");

	FILE *file = open_file(filename);

	FILE *keys_file = fopen("temp/keys.txt", "w+");

	FILE *f1 = fopen("temp/f1", "wb");
	FILE *f2 = fopen("temp/f2", "wb");

	fseek(file, 0, SEEK_END);

	long file_length = ftell(file);

	fseek(file, 0, SEEK_SET);

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

	for (long i = 0; !feof(file) && i < file_length; i++)
	{
		long long one_byte = fgetc(file);

		long long Ca = rand() % (p - 1),
				  Cb = rand() % (p - 1);

		long long Da = FME(g, Ca, p),
				  Db = FME(g, Cb, p);

		if (i == 0)
		{
			fprintf(keys_file, "p = %lld, g = %lld\n", p, g);
		}

		fprintf(keys_file, "[%ld]Ca = %lld, Da = %lld, Cb = %lld, Db = %lld\n", i + 1, Ca, Da, Cb, Db);

		long long r, e, k = rand() % (p - 2);

		r = FME(g, k, p);
		e = one_byte * FME(Db, k, p) % p;

		fputc(e, f1);

		long m = e * FME(r, p - 1 - Cb, p) % p;
		fputc(m, f2);
	}

	fclose(f2);
	fclose(f1);
	fclose(keys_file);
	fclose(file);

	return 0;
}

int encryption_RSA(char *filename)
{
	system("rm -rf temp/");
	system("mkdir -p temp");

	FILE *file = open_file(filename);

	FILE *keys_file = fopen("temp/keys.txt", "w+");

	FILE *f1 = fopen("temp/f1", "wb");
	FILE *f2 = fopen("temp/f2", "wb");

	fseek(file, 0, SEEK_END);

	long file_length = ftell(file);

	fseek(file, 0, SEEK_SET);

	long long p, q, N, f, D, C;

	do
	{

		p = rand() % (int)sqrt(10000);

	} while (isPrime(p) < 1);

	do
	{

		q = rand() % (int)sqrt(10000);

	} while (isPrime(q) < 1);

	N = p * q;

	f = (p - 1) * (q - 1);

	for (long i = 0; !feof(file) && i < file_length; i++)
	{
		long long one_byte = fgetc(file);

		if (i == 0)
		{
			fprintf(keys_file, "N = %lld\n", N);
		}

		long long x, y;

		do
		{

			D = rand() % f;

		} while (isPrime(D) < 1 || GEA_GCD(f, D, &x, &y) > 1);

		C = y > 0 ? y : y + f;

		fprintf(keys_file, "[%ld]C = %lld, D = %lld\n", i + 1, C, D);

		long long e;

		e = FME(one_byte, D, N);
		fputc(e, f1);

		long m = FME(e, C, N);
		fputc(m, f2);
	}

	fclose(f2);
	fclose(f1);
	fclose(keys_file);
	fclose(file);

	return 0;
}

int encryption_Vernam(char *filename)
{
	system("rm -rf temp/");
	system("mkdir -p temp");

	FILE *file = open_file(filename);

	FILE *keys_file = fopen("temp/keys.txt", "w+");

	FILE *f1 = fopen("temp/f1", "wb");
	FILE *f2 = fopen("temp/f2", "wb");

	fseek(file, 0, SEEK_END);

	long file_length = ftell(file);

	fseek(file, 0, SEEK_SET);

	for (long i = 0; !feof(file) && i < file_length; i++)
	{
		unsigned char k, e, one_byte = fgetc(file);

		k = rand() % 256;

		fprintf(keys_file, "[%ld] k = %d\n", i + 1, k);

		e = one_byte ^ k;

		fputc(e, f1);

		one_byte = e ^ k;
		fputc(one_byte, f2);
	}

	fclose(f2);
	fclose(f1);
	fclose(keys_file);
	fclose(file);

	return 0;
}