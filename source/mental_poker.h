#ifndef MENTAL_POKER_H
#define MENTAL_POKER_H 

#include "cryptbaselib.h"

#define DECK_SIZE 52
#define MIN_NUMBER_OF_PLAYERS 2
#define MAX_NUMBER_OF_PLAYERS 23
#define NUMBER_OF_SUIT 4
#define NUMBER_OF_CARD_VALUE 13
#define NUMBER_OF_PLAYER_CARDS 2
#define NUMBER_OF_CARDS_ON_THE_TABLE 5

const static char SUIT[NUMBER_OF_SUIT][4] = 
{
    {"♥"}, {"♦"}, {"♠"}, {"♣"}
};

const static char CARD_VALUE[NUMBER_OF_CARD_VALUE][3] = 
{
    {"T"}, {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"10"}, {"J"}, {"Q"}, {"K"}
};

int shuffle(long long *deck, int N);
int print_card(long long card);
int distribution_of_cards(int number_of_players);
int start_poker();

#endif