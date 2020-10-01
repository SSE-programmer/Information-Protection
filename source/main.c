#include <stdio.h>
#include "cryptlib.h"

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

    x = rand() % 1000000000;
    y = rand() % 1000000000;
    n = rand() % 1000000000;

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

    long long y = rand() % 1000000000,
              a = rand() % (1000000000 - 2) + 2,
              p;

    do
    {
        p = rand() % 1000000000;
    } while (isPrime(p) < 1);

    printf("y = %lld, a = %lld, p = %lld\n", y, a, p);

    printf("Шаг младенца, шаг великана: %lld;\n", BSGS(y, a, p));

    return 0;
}

int main()
{
    output_DataAndTime();

    srand(time(NULL));

    test_FastPow();

    test_GreatestCommonDivisor();

    test_DiffieHellmanOpenKey();

    test_BabyStepGiantStep();

    printf("\n");

    return 0;
}