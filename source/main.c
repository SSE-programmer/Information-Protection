#include <stdio.h>
#include "cryptbaselib.h"
#include "encryptionlib.h"

#define BEGIN_COLOR_FUNCTION "\033[01;32m"
#define END_COLOR_FUNCTION "\033[0m"

#define BEGIN_COLOR_DATE "\033[01;32m"
#define END_COLOR_DATE "\033[0m"

int output_DataAndTime()
{
    printf("\n%sВЫВОД ПРОИЗВЕДЕН:\n%s%s \n\033[6m%s%s%s\n",
           BEGIN_COLOR_DATE, __DATE__, END_COLOR_DATE, BEGIN_COLOR_DATE, __TIME__, END_COLOR_DATE);
    return 0;
}

/*Возведение числа в степень по модулю*/
int test_FastPow()
{
    printf("\n%sФункция:%s %s\n", BEGIN_COLOR_FUNCTION, __FUNCTION__, END_COLOR_FUNCTION);

    long long x, y, n;

    /*  x = rand() % 1000000000;
    y = rand() % 1000000000;
    n = rand() % 1000000000;*/

    x = 7;
    y = 8;
    n = 12;

    printf("Результат возведения числа %lld в степень %lld по модулю %lld: %lld;\n", x, y, n, FME(x, y, n));

    return 0;
}

int test_GreatestCommonDivisor()
{
    printf("\n%sФункция:%s %s\n", BEGIN_COLOR_FUNCTION, __FUNCTION__, END_COLOR_FUNCTION);

    long long a, b, x, y;

    a = rand() % 1000000000;
    b = rand() % 1000000000;

    long long GCD = GEA_GCD(a, b, &x, &y);

    printf("Наибольший общий делитель чисел %lld и %lld: %lld;\n", a, b, GCD);
    printf("Неизвестные X и Y из уравнения: %lld и %lld;\n", x, y);

    return 0;
}

int test_DiffieHellmanOpenKey()
{
    printf("\n%sФункция:%s %s\n", BEGIN_COLOR_FUNCTION, __FUNCTION__, END_COLOR_FUNCTION);

    printf("Открытый ключ двух абонентов: %lld;\n", DHKG());

    return 0;
}

int test_BabyStepGiantStep()
{
    printf("\n%sФункция:%s %s\n", BEGIN_COLOR_FUNCTION, __FUNCTION__, END_COLOR_FUNCTION);

    long long y,
        a = rand() % (1000000000 - 2) + 2,
        p;

    do
    {
        p = rand() % 1000000000;
    } while (isPrime(p) < 1);

    y = rand() % p;

    printf("y = %lld, a = %lld, p = %lld\n", y, a, p);

    struct BSGS_valid_X result;

    result = BSGS(y, a, p);

    if (result.valid)
    {
        printf("Шаг младенца, шаг великана: %lld;\n", result.x);
    }
    else
    {
        printf("Шаг младенца, шаг великана: решение не найдено;");
    }

    return 0;
}

int main(int argc, char **argv)
{
    output_DataAndTime();

    srand(time(NULL));

    while (1)
    {
        int item;

        printf("Выберите действие:\n");
        printf("\t1. Показать результат работы базовых функций;\n");
        printf("\t2. Зашифровать файл;\n");
        printf("\t3. Расшифровать файл;\n");
        printf("\t4. Выйти.\n");

        scanf("%d", &item);

        if (item == 4)
            break;

        switch (item)
        {
        case 1:
        {
            test_FastPow();

            test_GreatestCommonDivisor();

            test_DiffieHellmanOpenKey();

            test_BabyStepGiantStep();

            break;
        }

        case 2:
        {
            system("mkdir -p encrypt");

            char *filename;

            filename = malloc(sizeof(char) * FILENAME_LENGTH);

            printf("Введите путь к файлу: \n");
            scanf("%s", filename);

            printf("Выберите метод шифрования:\n");
            printf("\t1. Шифр Шамира;\n");
            printf("\t2. Шифр Эль-Гамаля;\n");
            printf("\t3. Шифр RSA;\n");
            printf("\t4. Шифр Вернама;\n");

            int encode_type;

            scanf("%d", &encode_type);

            switch (encode_type)
            {
            case 1:
            {
                encryption_Shamirs_secret_sharing(filename);

                break;
            }

            case 2:
            {
                encryption_ElGamal(filename);

                break;
            }

            case 3:
            {
                encryption_RSA(filename);

                break;
            }

            case 4:
            {
                encryption_Vernam(filename);

                break;
            }
            }
        }

        case 3:
            break;

        default:
            break;
        }

        printf("\n");
    }

    return 0;
}