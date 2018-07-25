/*
 * File:    blackjack.c
 * Project: Black Jack Game
 * Author:  Anna Stoneman
 * Version: 3.0 July 23, 2018 added comments and documentation
 *
 * Copyright (c) 2018, Anna Stoneman all rights reserved
 * Distribution without permission of the author is prohibited
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include "userAccount.h"
#include <string.h>


#define MAX_STRING_LEN 256

/* function prototypes */
int randomBetween(int lower, double upper);
int getCard(void);
int playGame(void);
int aceValue(void);
int initialCards(void);
int dealerScore(void);
int whoWon(void);

/* determines user and bet, calls playGame function, and adds/removes money from bank
 * depending whether user won
 */
int main(void)
{
    /* initializes variables */
    int winner = 0;
    int money = 0;
    int userInput = 0;
    int gamble = -1;
    int wrongValue = 0;
    char userName[MAX_STRING_LEN];
    /* determines if user wants to play */
    printf("\e[36mComputer: Play Blackjack? (1/0)\e[33m");
    scanf("%d", &userInput);
    /* if user does want to play continues game */
    if (userInput == 1) {
        printf("\e[32mDealer: Welcome to Black Jack!\n");
        /* determines user */
        printf("Dealer: What is your name?\e[33m ");
        scanf("%s", userName);
        printf("\e[32mDealer: Hello, \e[33m%s\e[32m!\e[0m\n", userName);
        /* tells user how much is in account */
        printf("\e[36mComputer: Your current balance is \e[33m$%d\e[36m.\n", bankAccount(userName));
        /* keeps playing until user quits */
        while(userInput == 1) {
            money = bankAccount(userName);
            /* user has to bet more than 0 and equal or less than their bank account */
            for(wrongValue = 0; (gamble > money) || (gamble < 0); wrongValue++) {
                printf("\e[32mDealer: How much do you want to bet?\e[33m");
                scanf("%d", &gamble);
                if (wrongValue > 0)
                    printf("\e[32mDealer: You can't do that, stupid.\n");
            }
            /* determines who won */
            winner = whoWon();
            /* if user won, adds their gamble to bank */
            if (winner == 1) {
                printf("\e[32mDealer: Congrats, you won!\n");
                money += gamble;
                printf("\e[36mComputer: Your current balance is \e[33m$%d\e[36m.\n", addMoney(userName, gamble, 1));
            /* if user ties, than does not touch bank */
            } else if (winner == 2) {
                printf("\e[32mDealer: It's a push (tie)!\n");
            /* if user lose, removes gamble from bank */
            } else {
                printf("\e[32mDealer: Haha I won!\n");
                printf("\e[36mComputer: Your current balance is \e[33m$%d\e[36m.\n", addMoney(userName, gamble, 0));
            } 
            /* checks if user still wants to play */
            printf("\e[36mComputer: Play Blackjack? (1/0)\e[33m");
            scanf("%d", &userInput);
            /* resets gamble variable so user has to enter a new one next round */
            gamble = -1;
        }
        /* checks balance of user */
        bankAccount(userName);
    }
    /* changes font color back to white */
    printf("\e[0m");
    return 0;
}

/* returns a random number given range */
int randomBetween(int lower, double upper)
{
    /* inits vars */
    int randomInt = 0;
    time_t t;
    /* seeds rng using time */
    srand((unsigned) time(&t));
    /* converts upper var from int to double */
    double rangeAsDouble = upper;
    /* determines random int */
    randomInt = rand() * rangeAsDouble / RAND_MAX + 1;
    return randomInt;
}

/* if user draws an ace - calls this function to determine if 1 or 11 */
int aceValue(void)
{
    /* inits ace var */
    int ace = 0;
    /* determines if user wants 1 or 11 */
    printf("\e[35mYou drew an \e[37mace\e[35m! Do you want it to be 11 or 1?\e[33m");
    scanf("%d", &ace);
    /* sets ace var to what user wants */
    if (ace == 1) {
        ace = 1;
    } else {
        ace = 11;
    }   
    return ace;
}

/* gets the value of the card drawn */
int getCard(void)
{
    /* inits card var */
    int card = 0;
    /* determines value of card. Since house does not matter, only uses 13 values.
     * 11, 12, and 13 are all face cards and given the value 10. Ace is 1.
     */
    card = randomBetween(1, 13);
    /* if card value is greater or equal to 11, assigns 10 to value */
    if (card >= 11) { 
        card = 10;
        printf("\e[35mYou drew a \e[37mface card\e[35m!\n");
    /* if card value is 1, calls ace function to see if 1 or 11 */
    } else if (card == 1) {
        card = aceValue();
    }
    /* if card value is 8, tells user card's value using correct grammar */
    if (card == 8) {
        printf("\e[35mYou drew an \e[37m%d\e[35m.\n", card);
    /* if nothing special, tells user the card's value */
    } else { 
        printf("\e[35mYou drew a \e[37m%d\e[35m.\n", card);
    }
    /* returns card's value */
    return card;
}

/* determines what the dealer's score is */
int dealerScore(void)
{
    /* inits vars */
    int score = 0;
    int card = 0;
    /* keeps drawing cards while score is under 17 */
    while (score < 17) {
        card = randomBetween(1,13);
        if (card >= 11)
            card = 10;
        /* determines what value ace should be */
        else if (card == 1) {
            if (score <= 10)
                card = 11;
            else
                card = 1;
        }
        /* prints what dealer drew so user can see (one at a time) */
        printf("dealer card: %d\n", card);
        score += card;
        sleep(1);
    }
    /* returns dealer's score */
    return score;
}

/* structure behind a single game */
int playGame(void)
{
    /* inits vars */
    int score = 0;
    int userInput = 0;
    /* gives user first two cards one at a time and adds to score */
    printf("\e[32mDealer: Here are your first two cards:\n");
    score += getCard();
    sleep(1);
    score += getCard();
    printf("\e[35mYour score is now \e[37m%d\e[35m.\n", score);
    /* asks if user wants another card */
    printf("\e[32mDealer: Another card? (1/0)\e[33m");
    scanf("%d", &userInput);
    /* if user does want another card, keeps asking until they don't or score goes over 21 */
    while ((userInput != 0) && (score < 21)) {
        score += getCard(); 
        printf("\e[35mYour score is now \e[37m%d\e[35m.\n", score);
        if (score < 21) {
            printf("\e[32mDealer: Another card? (1/0)\e[33m");
            
            scanf("%d", &userInput);
        }
    }
    /* prints final score and returns value */
    printf("\e[35mYour final score is \e[37m%d\e[35m.\e[0m\n", score);
    return score;
}

/* determines who won */
int whoWon(void)
{
    /* inits vars */
    int userScore = 0;
    int dScore = 0;
    /* gets user score from playGame function and dealer score from dealerScore function */
    userScore = playGame();
    dScore = dealerScore();
    /* prints the scores */
    printf("Your score: %d\n", userScore);
    printf("Dealer's score: %d\n", dScore);
    /* whoever's score is closer to 21 and below wins */
    if (((userScore > dScore) && (userScore <= 21)) || ((dScore > 21) && (userScore <= 21))) {
        return 1;
    } else if ((userScore == dScore) || ((userScore > 21) && (dScore > 21))) {
        return 2;
    } else {
        return 3;
    }
}
