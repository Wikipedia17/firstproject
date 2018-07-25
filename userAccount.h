/*
 * File:    userAccount.h
 * Project: Black Jack Game
 * Author:  Anna Stoneman
 * Version: 2.0 July 2018, added documentation
 *
 * Copyright (c) 2018 Anna Stoneman all rights reserved
 * Distribution without written permission of the author is prohibited
 */

/* inclues */
#include <stdio.h>

/* function prototypes */
int getSize(int balance);

/* checks if a username exists checks their bank account
 * if a username does not exist, creates a new one and inits bank
 */
int bankAccount(char* userName)
{
    /* inits vars */
    int money = 500;
    int i = 0;
    char checkName[128];
    int newUser = 0;
    /* opens userAccount.txt for reading */
    FILE *fp = fopen("userAccount.txt", "r");
    /* puts searcher at beginning of file */
    rewind(fp);
    /* while searcher has not reached end of file */
    while(!feof(fp)) {
        /* checks if username is there */
        fscanf(fp, "%s", checkName);
        /* if username is found, moves searcher to balance */
        if(strcmp(userName, checkName) == 0) {
            fscanf(fp, "%*[^0-9]");
            fscanf(fp, "%d", &money);
            break;
        }
    }
    /* if searcher has reached end of file, creates new username */
    if(feof(fp)){
        /* closes userAccount.txt */
        fclose(fp);
        printf("new user!\n");
        /* opens userAccount.txt for appending */
        FILE *fp = fopen("userAccount.txt", "a");
        /* prints new username and init balance to userAccount.txt*/
        fprintf(fp, "%s  ", userName);
        fprintf(fp, "500\n");
        newUser = 1;
    }
    /* closes userAccount.txt */
    fclose(fp);
    return money;
}

/* adds/takes money from user's balance */
int addMoney(char* userName, int addAmount, int sign)
{
    /* inits vars */
    int newAmount = 0;
    int currentAmount = 0;
    int location = 0;
    char checkName[128];
    int fLocation;
    /* opens userAccount.txt for reading */
    FILE *fp = fopen("userAccount.txt", "r");
    currentAmount = bankAccount(userName);
    /* if adding, add money to account */
    if (sign == 1) 
        newAmount = currentAmount+=addAmount;
    /* if subtracting, take money from account */
    else
        newAmount = currentAmount-=addAmount;
    /* find location of user's balance */
    while(!feof(fp)) {
        fscanf(fp, "%s", checkName);
        if(strcmp(userName, checkName) == 0) {
            fscanf(fp, "%*[^0-9]");
            fscanf(fp, "%*d");
            fLocation = ftell(fp);
            break;
        }
    }
    /* closes userAccount.txt */
    fclose(fp);
    /* opens userAccount.txt for reading and adding(?) */
    FILE *fd = fopen("userAccount.txt", "r+b");
    /* moves searcher to start of number */
    fLocation -= getDigits(currentAmount);
    fseek(fd, fLocation, SEEK_SET);
    /* if new digits are more than previous, moves cursor over */
    if (getDigits(newAmount) > getDigits(currentAmount))
        fprintf(fd, " ");
    /* prints new amount over old amount */
    fprintf(fd, "%d", newAmount);
    /* closes userAccount.txt */
    fclose(fd);
    /* returns new user balance */
    return newAmount;
}

/* finds the amount of digits in a number */
int getDigits(int balance)
{
    /* inits vars */
    int decimals = 1;
    int i = 0;
    /* keeps dividing number by 10 while greater than 1 */
    while ((balance/decimals) >=1) {
         decimals *= 10;
         /* increments each division to find number of decimals */
         i++;
    }
    /* returns number of decimals */
    return i;
}
