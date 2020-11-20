#include "blind_signature.h"

long long r, n; //потом поместить в файл;

int init_server()
{
    system("rm -rf blind_signature/open_keys/");
    system("rm -rf blind_signature/close_keys/");
    system("rm -rf blind_signature/list_of_voters/");
    system("mkdir -p blind_signature");
    system("mkdir -p blind_signature/open_keys");
    system("mkdir -p blind_signature/close_keys");
    system("mkdir -p blind_signature/list_of_voters");
    system("cd ..");

    FILE *open_keys_file = fopen("blind_signature/open_keys/keys.bin", "wb");
    FILE *close_keys_file = fopen("blind_signature/close_keys/keys.bin", "wb");

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

    fseek(open_keys_file, 0, SEEK_SET);

    fwrite(&D, sizeof(long long), 1, open_keys_file);
    fwrite(&N, sizeof(long long), 1, open_keys_file);

    fseek(close_keys_file, 0, SEEK_SET);

    fwrite(&C, sizeof(long long), 1, close_keys_file);

    fclose(open_keys_file);
    fclose(close_keys_file);

    return 0;
}

int client(int id_client)
{
    SHA512_CTX ctx;
    unsigned char sha512[SHA512_HASH_LENGTH];

    FILE *keys_file = open_file("blind_signature/open_keys/keys.bin");

    unsigned int rnd = rand() % 4294967295;
    int v = 0;

    long long D, N;

    fseek(keys_file, 0, SEEK_SET);
    fread(&D, sizeof(long long), 1, keys_file);
    fread(&N, sizeof(long long), 1, keys_file);

    /* Случайно заполняем ответы в бюллетене */
    /* В данном случае 16 вопросов - 16 бит */
    for (int i = 0; i < NUMBER_OF_QUESTIONS; i++)
    {
        int answer = rand() % 2;

        v <<= 1;
        v |= answer;
    }

    /* 12 бит на дату */
    v <<= 12;
    v |= 2111;

    /* 3 бита на другие данные */
    v <<= 3;
    v |= rand() % 0x7;

    /* n = rnd v v */
    n = rnd;
    n <<= 32;
    n |= v;

    do
    {

        r = rand() % N;

    } while (isPrime(r) < 1);

    SHA512_Init(&ctx);
    SHA512_Update(&ctx, &n, 8);
    SHA512_Final(sha512, &ctx);

    long long hash[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        hash[i] = (long long)sha512[i];
        hash[i] *= FME(r, D, N);
    }

    server(hash, id_client);

    fclose(keys_file);

    return 0;
}

int server(long long *sha512_h, int id_client)
{
    FILE *close_keys_file = open_file("blind_signature/close_keys/keys.bin");

    long long C;

    fseek(close_keys_file, 0, SEEK_SET);
    fread(&C, sizeof(long long), 1, close_keys_file);

    fclose(close_keys_file);

    FILE *open_keys_file = open_file("blind_signature/open_keys/keys.bin");

    long long D, N;

    fseek(open_keys_file, 0, SEEK_SET);
    fread(&D, sizeof(long long), 1, open_keys_file);
    fread(&N, sizeof(long long), 1, open_keys_file);

    fclose(open_keys_file);

    /* Отмечаем, что клиент получил бюллетень, если он еще его не получал; иначе выдаем ошибку */
    FILE *list_file = fopen("blind_signature/list_of_voters/list.bin", "rb");

    int temp_id_client;

    if (list_file != NULL)
    {
        while (fread(&temp_id_client, sizeof(int), 1, list_file) == 1)
        {
            if (temp_id_client == id_client)
            {
                printf("Ошибка: бюллетень не будет обработан - вы уже проголосовали!;\n");

                fclose(list_file);

                return -1;
            }
        }
    }

    list_file = fopen("blind_signature/list_of_voters/list.bin", "ab+");

    fwrite(&id_client, sizeof(int), 1, list_file);

    fclose(list_file);

    long long s[SHA512_HASH_LENGTH];

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        s[i] = FME(sha512_h[i], C, N);
    }

    client2(s);

    return 0;
}

int client2(long long *s_server)
{
    FILE *open_keys_file = open_file("blind_signature/open_keys/keys.bin");

    long long D, N;

    fseek(open_keys_file, 0, SEEK_SET);
    fread(&D, sizeof(long long), 1, open_keys_file);
    fread(&N, sizeof(long long), 1, open_keys_file);

    fclose(open_keys_file);

    long long s[SHA512_HASH_LENGTH];

    long long x1, x2, r_1;

    GEA_GCD(N, r, &x1, &x2);
    r_1 = x2 > 0 ? x2 : x2 + N;

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        s[i] = mod(s_server[i] * r_1, N);
    }

    server2(n, s);

    return 0;
}

int server2(long long n, long long *s)
{
    SHA512_CTX ctx;
    unsigned char sha512[SHA512_HASH_LENGTH];

    SHA512_Init(&ctx);
    SHA512_Update(&ctx, &n, 8);
    SHA512_Final(sha512, &ctx);

    FILE *open_keys_file = open_file("blind_signature/open_keys/keys.bin");

    long long D, N;

    fseek(open_keys_file, 0, SEEK_SET);
    fread(&D, sizeof(long long), 1, open_keys_file);
    fread(&N, sizeof(long long), 1, open_keys_file);

    fclose(open_keys_file);

    for (int i = 0; i < SHA512_HASH_LENGTH; i++)
    {
        if ((long long)sha512[i] != FME(s[i], D, N))
        {
            printf("Бюллетень некорректен!!!\n");

            return -1;
        }
    }

    printf("Ваш бюллетень учтен, спасибо =)\n");

    return 0;
}