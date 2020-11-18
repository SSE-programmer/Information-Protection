#include "mental_poker.h"

int shuffle(long long *deck, int N)
{
    for (int i = N - 1; i >= 1; i--)
    {
        int j = rand() % (i + 1);

        int tmp = deck[j];
        deck[j] = deck[i];
        deck[i] = tmp;
    }

    return 0;
}

int print_card(long long card)
{
    int suit = card >> 8;
    int value = card & 0xF;
    printf("[%s%s]\n", SUIT[suit], CARD_VALUE[value]);

    return 0;
}

int distribution_of_cards(int number_of_players)
{
    /*Создание колоды*/
    long long deck[DECK_SIZE];

    int card_index = 0;

    for (int c_v = 0; c_v < NUMBER_OF_CARD_VALUE; c_v++)
    {
        for (int s = 0; s < NUMBER_OF_SUIT; s++)
        {
            deck[card_index] = s;
            deck[card_index] <<= 8;
            deck[card_index] |= c_v;

            card_index++;
        }
    }

    /*Генерация большого простого числа*/
    long long p, q;

    do
    {
        do
        {
            q = rand() % (1000000000 / 2);
        } while (isPrime(q) < 1);

        p = q * 2 + 1;
    } while (isPrime(p) < 1);

    /*Шифрование и перемешивание колоды каждым игроком (и генерация закрытых ключей)*/
    long long *C, *D, x, y;

    C = malloc(sizeof(long long) * number_of_players);
    D = malloc(sizeof(long long) * number_of_players);

    for (int i = 0; i < number_of_players; i++)
    {
        do
        {

            D[i] = rand() % (p - 2) + 1;

        } while (isPrime(D[i]) < 1 || GEA_GCD(p - 1, D[i], &x, &y) > 1);

        C[i] = y > 0 ? y : y + p - 1;

        for (int j = 0; j < NUMBER_OF_SUIT * NUMBER_OF_CARD_VALUE; j++)
        {
            deck[j] = FME(deck[j], C[i], p);
        }

        shuffle(deck, NUMBER_OF_SUIT * NUMBER_OF_CARD_VALUE);
    }

    /*Раздача карт*/
    long long *player_deck;

    player_deck = malloc(sizeof(long long) * number_of_players * NUMBER_OF_PLAYER_CARDS);

    for (int i = 0; i < number_of_players; i++)
    {
        int v = 0;

        while (v < NUMBER_OF_PLAYER_CARDS)
        {
            player_deck[i * NUMBER_OF_PLAYER_CARDS + v] = deck[i * NUMBER_OF_PLAYER_CARDS + v];
            v++;
        }
    }

    long long cards_on_the_table[NUMBER_OF_CARDS_ON_THE_TABLE];

    for (int i = 0; i < NUMBER_OF_CARDS_ON_THE_TABLE; i++)
    {
        cards_on_the_table[i] = deck[number_of_players * NUMBER_OF_PLAYER_CARDS + i];
    }

    /*Расшифровка карт на столе (Не важно, в каком порядке)*/
    for (int i = 0; i < number_of_players; i++)
    {
        for (int j = 0; j < NUMBER_OF_CARDS_ON_THE_TABLE; j++)
        {
            cards_on_the_table[j] = FME(cards_on_the_table[j], D[i], p);
        }
    }

    /*Расшифровка карт каждого игрока (Расшифровка владельцем карт в последнюю очередь)*/
    for (int i = 0; i < number_of_players; i++)
    {
        for (int j = 0; j < number_of_players; j++)
        {
            if (i != j)
            {
                int v = 0;

                while (v < NUMBER_OF_PLAYER_CARDS)
                {
                    player_deck[i * NUMBER_OF_PLAYER_CARDS + v] = FME(player_deck[i * NUMBER_OF_PLAYER_CARDS + v], D[j], p);
                    v++;
                }
            }
        }
        int v = 0;

        while (v < NUMBER_OF_PLAYER_CARDS)
        {
            player_deck[i * NUMBER_OF_PLAYER_CARDS + v] = FME(player_deck[i * NUMBER_OF_PLAYER_CARDS + v], D[i], p);
            v++;
        }
    }

    /* Вывод карт со стола*/
    printf("\n");
    printf("%sКарты на столе:%s\n", BEGIN_COLOR_FUNCTION, END_COLOR_FUNCTION);

    for (int i = 0; i < NUMBER_OF_CARDS_ON_THE_TABLE; i++)
    {
        print_card(cards_on_the_table[i]);
    }

    /* Вывод карт каждого игрока*/
    printf("\n");
    for (int i = 0; i < number_of_players; i++)
    {
        printf("\n");
        printf("%sКарты игрока #%d:%s\n", BEGIN_COLOR_FUNCTION, i + 1, END_COLOR_FUNCTION);
        int v = 0;

        while (v < NUMBER_OF_PLAYER_CARDS)
        {
            print_card(player_deck[i * NUMBER_OF_PLAYER_CARDS + v]);
            v++;
        }
    }

    /* Вывод оставшихся в колоде карт*/
    printf("\n");
    printf("%sОставшиеся карты в колоде:%s\n", BEGIN_COLOR_FUNCTION, END_COLOR_FUNCTION);

    for (int i = NUMBER_OF_CARDS_ON_THE_TABLE + number_of_players * NUMBER_OF_PLAYER_CARDS; i < NUMBER_OF_SUIT * NUMBER_OF_CARD_VALUE; i++)
    {
        for (int j = 0; j < number_of_players; j++)
        {
            deck[i] = FME(deck[i], D[j], p);
        }
        print_card(deck[i]);
    }

    return 0;
}

int start_poker()
{
    int number_of_players;

    printf("\n");
    printf("%sМЕНТАЛЬНЫЙ ПОКЕР%s\n", BEGIN_COLOR_FUNCTION, END_COLOR_FUNCTION);
    printf("Каждому игроку раздаются по 2 карты, 5 выкладываются на стол.\n");

    do
    {
        printf("Введите кол-во игроков(2-23): \n");
        scanf("%d", &number_of_players);
    } while (number_of_players < MIN_NUMBER_OF_PLAYERS || number_of_players > MAX_NUMBER_OF_PLAYERS);

    distribution_of_cards(number_of_players);

    return 0;
}