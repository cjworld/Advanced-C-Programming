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
/*       Assignment Number: 6                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Ten                                   */
/*                                                            */
/*       File Name: ass6_chiyuchen.c                          */
/*                                                            */
/*       Date: 10/26/2018                                     */
/*                                                            */
/*       Objective: Play tic-tac-toe                          */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define PLAYERS 2
#define BOARDSIZE 4

/* global declarations */

/* function prototypes */
int is_gameover(int *board, int len);
void printboard(int *board, int len, FILE *fout);

// Return 1 if game is over. Otherwise, return 0.
int is_gameover(int *board, int len)
{
    int tmp = 0;

    // Check if any row meets the winning condition
    for (int i = 0; i < BOARDSIZE; i++)
    {
        tmp = 0;
        for (int j = 0; j < BOARDSIZE; j++)
        {
            tmp += *(board + i * len + j);
        }
        if (tmp == BOARDSIZE || tmp == -(BOARDSIZE))
            return 1;
    }

    // Check if any column meets the winning condition
    for (int j = 0; j < BOARDSIZE; j++)
    {
        tmp = 0;
        for (int i = 0; i < BOARDSIZE; i++)
        {
            tmp += *(board + i * len + j);
        }
        if (tmp == BOARDSIZE || tmp == -(BOARDSIZE))
            return 1;
    }

    // Check if the downward diagonal meets the winning condition
    tmp = 0;
    for (int i = 0; i < BOARDSIZE; i++)
    {
        tmp += *(board + i * len + i);
    }
    if (tmp == BOARDSIZE || tmp == -(BOARDSIZE))
        return 1;

    // Check if the upward diagonal meets the winning condition
    tmp = 0;
    for (int i = 0; i < BOARDSIZE; i++)
    {
        tmp += *(board + (BOARDSIZE - i - 1) * len + i);
    }
    if (tmp == BOARDSIZE || tmp == -(BOARDSIZE))
        return 1;

    return 0;
}

// Output the board.
void printboard(int *board, int len, FILE *fout)
{
    int i, j, value;
    fprintf(fout, "-----------------\n");
    for (i = 0; i < len; i++) {
        for (j = 0; j < len; j++) {
            value = *(board + i * len + j);
            if (value == 1) {
                fprintf(fout, "| X ");
            } else if (value == -1) {
                fprintf(fout, "| O ");
            } else {
                fprintf(fout, "| - ");
            }
        }
        fprintf(fout, "|\n");
        fprintf(fout, "-----------------\n");
    }
} 

int main(int argc, char const *argv[])
{
    int i, j, winner = 0, player = 1, row, col, leftsteps = BOARDSIZE * BOARDSIZE;
    int board[BOARDSIZE][BOARDSIZE] = {0};

    // For file I/O
    FILE *fout;
    fout = fopen (argv[1], "w+");

    // Print out the initial board.
    printboard(&board[0][0], BOARDSIZE, fout);

    // Game starts. Players make a move in turns.
    while (!winner && leftsteps > 0) {

        // Next player. Prompt the next player to move
        player = !player;
        printf("Player %d make a move (row, column):", player + 1);
        scanf("%d,%d", &row, &col);
        fprintf(fout, "Player %d make a move at %d, %d.\n", player + 1, row, col);

        // Check if the location is occupaied. If not, check if the player wins the game.
        if (board[row-1][col-1] != 0) {
            printf("Try another location.\n");
            continue;
        }

        // Check if the player wins the game.
        if (player == 0) {
            board[row-1][col-1] = 1;
            winner = is_gameover(&board[0][0], BOARDSIZE);
        } else {
            board[row-1][col-1] = -1;
            winner = is_gameover(&board[0][0], BOARDSIZE);
        }
        printboard(&board[0][0], BOARDSIZE, fout);

        // Decrease the steps by one.
        leftsteps--;
    }

    // Show the result, and print out the final board.
    if (leftsteps > 0) 
        fprintf(fout, "Player %d wins!\n", player+1);
    else
        fprintf(fout, "Its a draw.\n");
    printboard(&board[0][0], BOARDSIZE, fout);

    // Cleanup
    fclose(fout);
    return 0;
}
