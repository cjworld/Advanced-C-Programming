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
/*       Assignment Number: 1                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter One and Chapter Two                   */
/*                                                            */
/*       File Name: ass1_chiyuchen.c                          */
/*                                                            */
/*       Date: 09/18/2018                                     */
/*                                                            */
/*       Objective: To transfer Celsius to Fahrenheit         */
/*                  and vise versa                            */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

#include<stdio.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    // users command
    char ans;
    int degree;
    do {
        printf("There are available commands:\n(C) From Celsius to Fahrenheit.\n(F) From Fahrenheit to Celsius.\n(Q) Quit.\nYour command(C/F/Q): ");
        scanf(" %c", &ans);
        if (ans == 'Q' || ans == 'q') {
            break;
        } else if (ans == 'C' || ans == 'c') {
            printf("Celsius degree? ");
            scanf("%d", &degree);
            printf("Celsius %d degree => Fahrenheit %d degree\n", degree, (degree*9/5)+32);
        } else if (ans == 'F' || ans == 'f') {
            printf("Fahrenheit degree? ");
            scanf("%d", &degree);
            printf("Fahrenheit %d degree => Celsius %d degree\n", degree, (degree-32)*5/9);
        } else {
            printf("Invalid command.\n");
        }
    } while (1);
    printf("Goodbye.\n");
    return 0;
}
