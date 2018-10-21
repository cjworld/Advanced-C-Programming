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
/*       Assignment Number: 2                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Three, Four, and Five                 */
/*                                                            */
/*       File Name: ass2_chiyuchen.c                          */
/*                                                            */
/*       Date: 09/18/2018                                     */
/*                                                            */
/*       Objective: To read in scores and summary it          */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    // users command
    char cmd;
    int total;
    int score;
    int count = 0;
    int max = 0;
    int min = 100;

    printf("First score: ");
    do {
        scanf(" %d", &score);
        count += 1;
        total += score;
        if (score > max) {
            max = score;
        }
        if (score < min) {
            min = score;
        }
        printf("The %dth score: %d\n", count, score);
        printf("More score (y/N)? ");
        scanf(" %c", &cmd);
        if (cmd != 'Y' && cmd != 'y') {
            break;
        }
        printf("Next score: ");
    } while (1);
    printf("Total: %d.\n", total);
    printf("Average: %d.\n", total/count);
    printf("Max: %d.\n", max);
    printf("Min: %d.\n", min);
    return 0;
}
