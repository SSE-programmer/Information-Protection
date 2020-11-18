#include <stdio.h>
#include "cryptbaselib.h"
#include "encryptionlib.h"
#include "digital_signature.h"
#include "mental_poker.h"

int output_DataAndTime()
{
    printf("\n%sВЫВОД ПРОИЗВЕДЕН:\n%s%s \n\033[6m%s%s%s\n",
           BEGIN_COLOR_DATE, __DATE__, END_COLOR_DATE, BEGIN_COLOR_DATE, __TIME__, END_COLOR_DATE);
    return 0;
}

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
        printf("\t2. Зашифровать/расшифровать файл;\n");
        printf("\t3. Подписать файл;\n");
        printf("\t4. Проверить цифровую подпись;\n");
        printf("\t5. Ментальный покер;\n");
        printf("\t6. Выйти.\n");

        scanf("%d", &item);

        if (item == 6)
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

            break;
        }

        case 3:
        {
            char *filename;

            filename = malloc(sizeof(char) * FILENAME_LENGTH);

            printf("Введите путь к файлу: \n");
            scanf("%s", filename);

            printf("Выберите метод электронной подписи:\n");
            printf("\t1. RSA;\n");
            printf("\t2. Эль-Гамаля;\n");
            printf("\t3. ГОСТ;\n");

            int encode_type;

            scanf("%d", &encode_type);

            switch (encode_type)
            {
            case 1:
            {
                digital_signature_RSA(filename);

                break;
            }

            case 2:
            {
                digital_signature_ElGamal(filename);

                break;
            }

            case 3:
            {
                digital_signature_GOST(filename);

                break;
            }

            default:
                break;
            }

            break;
        }

        case 4:
        {
            char *filename, *sig_filename;

            filename = malloc(sizeof(char) * FILENAME_LENGTH);
            sig_filename = malloc(sizeof(char) * FILENAME_LENGTH);

            printf("Введите путь к файлу: \n");
            scanf("%s", filename);
            printf("Введите путь к электронной подписи: \n");
            scanf("%s", sig_filename);

            printf("Выберите метод электронной подписи:\n");
            printf("\t1. RSA;\n");
            printf("\t2. Эль-Гамаля;\n");
            printf("\t3. ГОСТ;\n");

            int encode_type;

            scanf("%d", &encode_type);

            switch (encode_type)
            {
            case 1:
            {
                test_digital_signature_RSA(filename, sig_filename);

                break;
            }

            case 2:
            {
                test_digital_signature_ElGamal(filename, sig_filename);

                break;
            }

            case 3:
            {
                test_digital_signature_GOST(filename, sig_filename);

                break;
            }

            default:
                break;
            }

            break;
        }

        case 5:
        {
            start_poker();
        }

        default:
            break;
        }

        printf("\n");
    }

    return 0;
}