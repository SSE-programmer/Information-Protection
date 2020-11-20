#include "digital_signature.h"

int digital_signature_RSA(char *filename)
{
    SHA512_CTX ctx;
    unsigned char sha512[SHA512_HASH_LENGTH];

    system("rm -rf signature/file.sig");
    system("rm -rf keys/rsa_keys.bin");

    system("mkdir -p signature");
    system("mkdir -p keys");
    FILE *file = open_file(filename);

    FILE *keys_file = fopen("keys/rsa_keys.bin", "wb");

    FILE *sig = fopen("signature/file.sig", "wb");

    fseek(file, 0, SEEK_END);

    long file_length = ftell(file);

    fseek(file, 0, SEEK_SET);

    SHA512_Init(&ctx);

    for (long i = 0; !feof(file) && i < file_length; i++)
    {
        char one_byte[1];
        one_byte[0] = fgetc(file);
        SHA512_Update(&ctx, one_byte, 1);
    }

    SHA512_Final(sha512, &ctx);

    printf("SHA512 HASH:\n");
    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
        printf("%02x", sha512[i]);
    printf("\n");

    long long p, q, N, f, D, C;

    do
    {

        p = rand() % 10000;

    } while (isPrime(p) < 1);

    do
    {

        q = rand() % 10000;

    } while (isPrime(q) < 1);

    N = p * q;

    f = (p - 1) * (q - 1);

    long long x, y;

    do
    {

        D = rand() % f;

    } while (isPrime(D) < 1 || GEA_GCD(f, D, &x, &y) > 1);

    C = y > 0 ? y : y + f;

    fseek(keys_file, 0, SEEK_SET);
    fwrite(&D, sizeof(long long), 1, keys_file);
    fwrite(&N, sizeof(long long), 1, keys_file);

    long long Y[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        Y[i] = FME((long long)sha512[i], C, N);
    }
    printf("\n");

    fseek(file, 0, SEEK_SET);

    fwrite(Y, sizeof(long long), SHA512_HASH_LENGTH, sig);

    fclose(sig);
    fclose(keys_file);
    fclose(file);

    return 0;
}

int test_digital_signature_RSA(char *filename, char *sig_filename)
{
    SHA512_CTX ctx;
    unsigned char sha512[SHA512_HASH_LENGTH];
    unsigned char test_sha512[SHA512_HASH_LENGTH];

    FILE *file = open_file(filename);
    FILE *sig = open_file(sig_filename);

    FILE *keys_file = open_file("keys/rsa_keys.bin");

    long long D, N;

    fseek(keys_file, 0, SEEK_SET);
    fread(&D, sizeof(long long), 1, keys_file);
    fread(&N, sizeof(long long), 1, keys_file);

    fseek(file, 0, SEEK_END);

    long file_length = ftell(file);

    fseek(file, 0, SEEK_SET);

    SHA512_Init(&ctx);

    for (long i = 0; !feof(file) && i < file_length; i++)
    {
        char one_byte[1];
        one_byte[0] = fgetc(file);
        SHA512_Update(&ctx, one_byte, 1);
    }

    SHA512_Final(sha512, &ctx);

    printf("SHA512 HASH:\n");
    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
        printf("%02x", sha512[i]);
    printf("\n");

    int flag = 1;

    long long S[SHA512_HASH_LENGTH];

    fseek(sig, 0, SEEK_SET);
    fread(S, sizeof(long long), SHA512_HASH_LENGTH, sig);

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        S[i] = FME(S[i], D, N);

        test_sha512[i] = (unsigned char)S[i];

        if (sha512[i] != test_sha512[i])
        {
            flag = 0;
        }
    }

    printf("\n");

    if (flag == 1)
    {
        printf("%sФайл является подлинным.%s\n", BEGIN_COLOR_FUNCTION, END_COLOR_FUNCTION);
    }

    fseek(file, 0, SEEK_SET);

    fclose(sig);
    fclose(file);

    return 0;
}

int digital_signature_ElGamal(char *filename)
{
    SHA512_CTX ctx;
    unsigned char sha512[SHA512_HASH_LENGTH];

    system("rm -rf signature/file.sig");
    system("rm -rf keys/eg_keys.bin");
    system("mkdir -p signature");
    system("mkdir -p keys");

    FILE *file = open_file(filename);

    FILE *keys_file = fopen("keys/eg_keys.bin", "wb");

    FILE *sig = fopen("signature/file.sig", "wb");

    fseek(file, 0, SEEK_END);

    long file_length = ftell(file);

    fseek(file, 0, SEEK_SET);

    long long p, q, g, x, y;

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

    SHA512_Init(&ctx);

    for (long i = 0; !feof(file) && i < file_length; i++)
    {
        char one_byte[1];
        one_byte[0] = fgetc(file);
        SHA512_Update(&ctx, one_byte, 1);
    }

    SHA512_Final(sha512, &ctx);

    printf("SHA512 HASH:\n");
    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
        printf("%02x", sha512[i]);
    printf("\n");

    long long H[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        H[i] = (long long)sha512[i];
    }

    x = rand() % (p - 2) + 1;
    y = FME(g, x, p);

    long long k, r, t, h, u, k_k;

    do
    {

        k = rand() % (p - 2) + 1;

    } while (isPrime(k) < 1 || GEA_GCD(p - 1, k, &t, &h) > 1);

    k_k = h > 0 ? h : h + p - 1;

    r = FME(g, k, p);

    fseek(keys_file, 0, SEEK_SET);
    fwrite(&p, sizeof(long long), 1, keys_file);
    fwrite(&g, sizeof(long long), 1, keys_file);
    fwrite(&y, sizeof(long long), 1, keys_file);

    long long s[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {

        u = (H[i] - x * r);
        u = mod(u, p - 1);
        s[i] = k_k * u;
        s[i] = mod(s[i], p - 1);
    }

    fseek(file, 0, SEEK_SET);

    fwrite(&r, sizeof(long long), 1, sig);
    fwrite(s, sizeof(long long), SHA512_HASH_LENGTH, sig);

    fclose(sig);
    fclose(keys_file);
    fclose(file);

    return 0;
}

int test_digital_signature_ElGamal(char *filename, char *sig_filename)
{
    SHA512_CTX ctx;
    unsigned char sha512[SHA512_HASH_LENGTH];

    FILE *file = open_file(filename);

    FILE *keys_file = open_file("keys/eg_keys.bin");

    FILE *sig = open_file(sig_filename);

    long long p, g, y;

    fseek(keys_file, 0, SEEK_SET);
    fread(&p, sizeof(long long), 1, keys_file);
    fread(&g, sizeof(long long), 1, keys_file);
    fread(&y, sizeof(long long), 1, keys_file);

    fseek(file, 0, SEEK_END);

    long file_length = ftell(file);

    fseek(file, 0, SEEK_SET);

    SHA512_Init(&ctx);

    for (long i = 0; !feof(file) && i < file_length; i++)
    {
        char one_byte[1];
        one_byte[0] = fgetc(file);
        SHA512_Update(&ctx, one_byte, 1);
    }

    SHA512_Final(sha512, &ctx);

    printf("SHA512 HASH:\n");
    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
        printf("%02x", sha512[i]);
    printf("\n");

    int flag = 1;

    long long r, s[SHA512_HASH_LENGTH];

    fseek(sig, 0, SEEK_SET);
    fread(&r, sizeof(long long), 1, sig);
    fread(s, sizeof(long long), SHA512_HASH_LENGTH, sig);

    long long H[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        H[i] = (long long)sha512[i];
    }

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        long long left_value, right_value;

        left_value = FME(y, r, p) * FME(r, s[i], p) % p;
        right_value = FME(g, H[i], p);

        if (left_value != right_value)
        {
            flag = 0;
        }
    }

    printf("\n");

    if (flag == 1)
    {
        printf("%sФайл является подлинным.%s\n", BEGIN_COLOR_FUNCTION, END_COLOR_FUNCTION);
    }

    fclose(sig);
    fclose(file);

    return 0;
}

int digital_signature_GOST(char *filename)
{
    SHA512_CTX ctx;
    unsigned char sha512[SHA512_HASH_LENGTH];

    system("rm -rf signature/file.sig");
    system("rm -rf keys/gost_keys.bin");
    system("mkdir -p signature");
    system("mkdir -p keys");

    FILE *file = open_file(filename);

    FILE *keys_file = fopen("keys/gost_keys.bin", "wb");

    FILE *sig = fopen("signature/file.sig", "wb");

    fseek(file, 0, SEEK_END);

    long file_length = ftell(file);

    fseek(file, 0, SEEK_SET);

    SHA512_Init(&ctx);

    for (long i = 0; !feof(file) && i < file_length; i++)
    {
        char one_byte[1];
        one_byte[0] = fgetc(file);
        SHA512_Update(&ctx, one_byte, 1);
    }

    SHA512_Final(sha512, &ctx);

    printf("SHA512 HASH:\n");
    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
        printf("%02x", sha512[i]);
    printf("\n");

    long long H[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        H[i] = (long long)sha512[i];
    }

    long long p, q, b;

    do
    {
        q = rand() % 32767;
        q |= 0x8000;
    } while (isPrime(q) < 1);

    do
    {
        b = rand() % 32767;
        p = q * b + 1;
    } while ((isPrime(p) < 1) || ((p & 0x40000000) < 0) || ((p & 0x80000000) > 0));

    long long g, a;

    do
    {
        g = rand() % (p - 2) + 1;
        a = FME(g, b, p);
    } while ((a <= 1) || (FME(a, q, p) != 1));

    long long x, y;

    x = rand() % (q - 1) + 1;

    y = FME(a, x, p);

    long long k, r[SHA512_HASH_LENGTH], s[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        do
        {
            k = rand() % (q - 1) + 1;
            r[i] = mod(FME(a, k, p), q);
            s[i] = mod(k * H[i] + x * r[i], q);
        } while (r[i] == 0 || s[i] == 0);
    }

    fseek(keys_file, 0, SEEK_SET);
    fwrite(&p, sizeof(long long), 1, keys_file);
    fwrite(&q, sizeof(long long), 1, keys_file);
    fwrite(&y, sizeof(long long), 1, keys_file);
    fwrite(&a, sizeof(long long), 1, keys_file);

    fseek(file, 0, SEEK_SET);
    fwrite(r, sizeof(long long), SHA512_HASH_LENGTH, sig);
    fwrite(s, sizeof(long long), SHA512_HASH_LENGTH, sig);

    fclose(sig);
    fclose(keys_file);
    fclose(file);

    return 0;
}

int test_digital_signature_GOST(char *filename, char *sig_filename)
{
    SHA512_CTX ctx;
    unsigned char sha512[SHA512_HASH_LENGTH];

    FILE *file = open_file(filename);

    FILE *keys_file = open_file("keys/gost_keys.bin");

    FILE *sig = open_file(sig_filename);

    long long p, q, y, a;
    
    fseek(keys_file, 0, SEEK_SET);
    fread(&p, sizeof(long long), 1, keys_file);
    fread(&q, sizeof(long long), 1, keys_file);
    fread(&y, sizeof(long long), 1, keys_file);
    fread(&a, sizeof(long long), 1, keys_file);

    fseek(file, 0, SEEK_END);

    long file_length = ftell(file);

    fseek(file, 0, SEEK_SET);

    SHA512_Init(&ctx);

    for (long i = 0; !feof(file) && i < file_length; i++)
    {
        char one_byte[1];
        one_byte[0] = fgetc(file);
        SHA512_Update(&ctx, one_byte, 1);
    }

    SHA512_Final(sha512, &ctx);

    printf("SHA512 HASH:\n");
    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
        printf("%02x", sha512[i]);
    printf("\n");

    long long r[SHA512_HASH_LENGTH], s[SHA512_HASH_LENGTH];

    fread(r, sizeof(long long), SHA512_HASH_LENGTH, sig);
    fread(s, sizeof(long long), SHA512_HASH_LENGTH, sig);

    long long H[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        H[i] = (long long)sha512[i];
    }

    int flag = 1;

    long long u1, u2, v;

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        if (r[i] <= 0 || r[i] >= q || s[i] <= 0 || s[i] >= q)
        {
            flag = 0;
        }

        long long x1, x2, h_1;

        GEA_GCD(q, H[i], &x1, &x2);
        h_1 = x2 > 0 ? x2 : x2 + q;

        u1 = mod(s[i] * h_1, q);
        u2 = mod(r[i] * -1 * h_1, q);

        v = mod(mod(FME(a, u1, p) * FME(y, u2, p), p), q);

        if (v != r[i])
        {
            flag = 0;
        }
    }

    printf("\n");

    if (flag == 1)
    {
        printf("%sФайл является подлинным.%s\n", BEGIN_COLOR_FUNCTION, END_COLOR_FUNCTION);
    }

    fclose(sig);
    fclose(file);

    return 0;
}