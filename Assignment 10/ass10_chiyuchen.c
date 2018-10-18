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
/*       Assignment Number: 10                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty Four                           */
/*                                                            */
/*       File Name: ass10_chiyuchen.c                         */
/*                                                            */
/*       Date: 10/13/2018                                     */
/*                                                            */
/*       Objective: Tower of Hanoi                            */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXSTACK 10
#define EMPTYSTACK -1
#define TRUE 1
#define FALSE 0

/* global declarations */
struct stack {
    char tag;
    int top;
    int stackelement[MAXSTACK];
};
struct stack isa, isb, isc;

/* function prototypes */
int empty(struct stack *);
int pop(struct stack *);
void push(struct stack *, int);
void move(struct stack *, struct stack *, struct stack *, int, FILE *, int printlast);
void print_toh(FILE *);

/* Verify if the stack is empty. Return TRUE or FALSE */
int empty(struct stack *ps)
{
    if (ps->top == 0)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/* Remove the last element from stack. Return the removed element. */
int pop(struct stack *ps)
{
    if (empty(ps))
    {
        printf("%s", "stack underflow");
        exit(1);
    }
    return (ps->stackelement[--(ps->top)]);
}

/* Insert the element to stack. */
void push(struct stack *ps, int x)
{
    if (ps->top == MAXSTACK)
    {
        printf("%s", "stack underflow");
        exit(1);
    }
    else
    {
        ps->stackelement[(ps->top)++] = x;
    }
    return;
}

// Move len disks from src stack to dst stack via aux stack
void move(struct stack *issrc, struct stack *isdst, struct stack *isaux, int len, FILE *fout, int lastmove)
{
    int val;

    // Edge condition.
    if (len < 1)
        return;

    // Move n-1 disks from src to aux, move a disk from src to dest, and then move n-1 disks from aux to dst.
    move(issrc, isaux, isdst, len - 1, fout, FALSE);
    val = pop(issrc);
    push(isdst, val);
    if (lastmove == TRUE)
        fprintf(fout, "Final move %d from %c to %c\n", val, issrc->tag, isdst->tag);
    else
        fprintf(fout, "Move %d from %c to %c\n", val, issrc->tag, isdst->tag);
    print_toh(fout);
    move(isaux, isdst, issrc, len - 1, fout, lastmove);
}

// print out the poles
void print_toh(FILE *fout)
{
    int i;

    fprintf(fout, "%c: ", isa.tag);
    for (i = 0; i < MAXSTACK; i++)
    {
        if (i < isa.top)
            fprintf(fout, "%2d ", isa.stackelement[i]);
        else
            fprintf(fout, "   ");
    }
    fprintf(fout, "\t");

    fprintf(fout, "%c: ", isb.tag);
    for (i = 0; i < MAXSTACK; i++)
    {
        if (i < isb.top)
            fprintf(fout, "%2d ", isb.stackelement[i]);
        else
            fprintf(fout, "   ");
    }
    fprintf(fout, "\t");

    fprintf(fout, "%c: ", isc.tag);
    for (i = 0; i < MAXSTACK; i++)
    {
        if (i < isc.top)
            fprintf(fout, "%2d ", isc.stackelement[i]);
        else
            fprintf(fout, "   ");
    }
    fprintf(fout, "\n");
}

int main(int argc, char const *argv[])
{
    int num, i;
    isa.tag = 'A';
    isa.top = 0;
    isb.tag = 'B';
    isb.top = 0;
    isc.tag = 'C';
    isc.top = 0;

    // For file I/O
    FILE *fout;
    fout = fopen (argv[1], "w+");

    printf("How many disks[0-26]:");
    scanf(" %d", &num);
    fprintf(fout, "User want %d disks.\n", num);
    if (num < 0 || num > 26)
    {
        printf("Invalid. Please try again.\n");
    }
    else
    {
        for (i = num; i > 0; i--)
        {
            push(&isa, i);
        }
        fprintf(fout, "Initial Poles\n");
        print_toh(fout);
        move(&isa, &isc, &isb, num, fout, TRUE);
    }

    // Cleanup
    fclose(fout);
    return 0;
}
