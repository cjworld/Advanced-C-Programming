/**************************************************************/
/*********************     prologue     ***********************/
/*                                                            */
/*       University of California Extension, Santa Cruz       */
/*                                                            */
/*       Advanced C Programming                               */
/*                                                            */
/*       Instructor: Rajainder A. Yeldandi                    */
/*                                                            */
/*       Author: Chi Yu, Chen                                 */
/*                                                            */
/*       Assignment Number: 3                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Six                                   */
/*                                                            */
/*       File Name: ass3_chiyuchen.c                          */
/*                                                            */
/*       Date: 09/19/2018                                     */
/*                                                            */
/*       Objective: To print the given input scores in        */
/*                  different order, and find out the         */
/*                  index of a given value                    */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SCORES_NUM 100

/* function prototypes */
int indexOfValue(int *, int, int);

/* Search the given value in the given array. 
   Return the index of the found value. Return -1 if not found */
int indexOfValue(int *arr, int length, int val)
{
    int i;
    for (i = 0; i < length; i++) {
        if (arr[i] == val) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    // users command
    char cmd[10];
    int score[MAX_SCORES_NUM], score_tmp;
    int length = 0, i, j;
    char scorec[10];
    int finding_score;

    printf("First score: ");
    do {
        scanf("%s", scorec);
        score[length++] = atoi(scorec);
        printf("More score (y/N)? ");
        scanf("%s", cmd);
        if (strcmp(cmd, "Y") != 0 && strcmp(cmd, "y") != 0) {
            break;
        }
        printf("Next score: ");
    } while (1);

    printf("Original order: ");
    for(i = 0; i < length; i++) {
        printf("%d", score[i]);
        if (i != length-1) {
            printf(",");
        }
    }
    printf("\n");

    printf("Reversed order: ");
    for(i = length-1; i >= 0; i--) {
        printf("%d", score[i]);
        if (i != 0) {
            printf(",");
        }
    }
    printf("\n");

    printf("Ascending order: ");
    for (i = 0; i < length; i++) {
        for (j = i + 1; j < length; j++) {
            if (score[i] > score[j]) 
            {
                score_tmp =  score[i];
                score[i] = score[j];
                score[j] = score_tmp;
            }
        }
    }
    for(i = 0; i < length; i++) {
        printf("%d", score[i]);
        if (i != length-1) {
            printf(",");
        }
    }
    printf("\n");

    printf("Find the given number from the array.\n");
    printf("First score: ");
    do {
        scanf("%s", scorec);
        finding_score = atoi(scorec);
        printf("Index of %d is %d\n", finding_score, indexOfValue(score, length, finding_score));

        printf("More score (y/N)? ");
        scanf("%s", cmd);
        if (strcmp(cmd, "Y") != 0 && strcmp(cmd, "y") != 0) {
            break;
        }
        printf("Next score: ");
    } while (1);

    printf("Goodbye\n");
    return 0;
}
