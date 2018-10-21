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
/*       Assignment Number: 7                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty Three                          */
/*                                                            */
/*       File Name: ass7_chiyuchen.c                          */
/*                                                            */
/*       Date: 10/13/2018                                     */
/*                                                            */
/*       Objective: Place queens on boards so that            */
/*                  they would not attack each others         */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BOARDSIZE 8
#define DIAGONAL (2 * BOARDSIZE - 1)
#define DOWNOFFSET 7
#define TRUE 1
#define FALSE 0

/* global declarations */
int queencol[BOARDSIZE]; // queen column
int colfree[BOARDSIZE]; // is column free
int upfree[DIAGONAL];   // up diagonal free
int downfree[DIAGONAL]; // down diagonal free
int queencount = -1;    // row queen is placed

/* function prototypes */
void writeBoard(FILE *);
void clearBoard();
int addQueen(FILE *);

/* Place the rest queens on the board. */
int addQueen(FILE *fout)
{
    int found = FALSE;
    int row = ++queencount;
    for (int col = 0; col < BOARDSIZE; col++)
    {
        // Check if the given column of the given row is legal to place a queen.
        if (colfree[col] && upfree[row + col] && downfree[row - col + DOWNOFFSET])
        {
            // Place the queen in the given column and the given row on the board.
            queencol[queencount] = col;
            colfree[col] = FALSE;
            upfree[row + col] = FALSE;
            downfree[row - col + DOWNOFFSET] = FALSE;

            // Since the queen has been place, check if any queen left to be placed.
            // If so, we need to continue place the next queen.
            // If not, we find a final result.
            if (queencount == BOARDSIZE - 1)
            {
                writeBoard(fout);
                found = TRUE;
            }
            else
            {
                found = addQueen(fout);
            }

            // Remove the queen in the given column and the given row from the board.
            queencol[queencount] = -1;
            colfree[col] = TRUE;
            upfree[row + col] = TRUE;
            downfree[row - col + DOWNOFFSET] = TRUE;

            // If any final result found, stop trying the next combination.
            if (found == TRUE)
                break;
        }
    }
    queencount--;
    return (found == TRUE);
}

/* Print out the current board. */
void writeBoard(FILE *fout)
{
    int col, row;
    for (row = 0; row < BOARDSIZE; row++)
    {
        for (col = 0; col < BOARDSIZE; col++)
        {
            // If the given column in the given row has been placed a queen. Print Q or U.else
            // Print U for the first row because users always place their queen on the first row.
            if (queencol[row] == col)
            {
                if (row == 0)
                    fprintf(fout, "U   ");
                else
                    fprintf(fout, "Q   ");
            }
            else
                fprintf(fout, "*   ");
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
}

/* Reset the current board. */
void clearBoard()
{
    for (int i = 0; i < BOARDSIZE; i++)
    {
        colfree[i] = TRUE;
        queencol[i] = -1;
    }

    for (int j = 0; j < DIAGONAL; j++)
    {
        upfree[j] = TRUE;
        downfree[j] = TRUE;
    }
}

int main(int argc, char const *argv[])
{
    char ans;
    int col, row;

    // File I/O
    FILE *fout;
    fout = fopen (argv[1], "w+");

    // Prompt user to give a location for the first queen.
    do
    {
        printf("Place your queen in a column [0-7, Q]:");
        fprintf(fout, "Place your queen in a column [0-7, Q]:");
        scanf(" %c", &ans);
        fprintf(fout, " %c\n\n", ans);
        if (ans == 'Q')
        {
            printf("See you!\n");
            fprintf(fout, "See you!\n");
            break;
        }

        col = atoi(&ans);
        if (col >= 0 && col <= 7)
        {
            // Reset the board.
            clearBoard();

            // Place the queen on the given column in the first row.
            row = ++queencount;
            queencol[queencount] = col;
            colfree[col] = FALSE;
            upfree[row + col] = FALSE;
            downfree[row - col + DOWNOFFSET] = FALSE;

            // Place the rest queens.
            addQueen(fout);

            // Remove the queen
            queencol[queencount] = -1;
            colfree[col] = TRUE;
            upfree[row + col] = TRUE;
            downfree[row - col + DOWNOFFSET] = TRUE;
            queencount--;
        }
        else
        {
            printf("Invalid. Please try again.\n");
            fprintf(fout, "Invalid. Please try again.\n\n");
        }
    }
    while (TRUE);

    // Cleanup
    fclose(fout);
    return 0;
}
