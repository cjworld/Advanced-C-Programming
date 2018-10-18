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
/*       Assignment Number: 16                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty One                            */
/*                                                            */
/*       File Name: ass16_chiyuchen.c                         */
/*                                                            */
/*       Date: 10/17/2018                                     */
/*                                                            */
/*       Objective: Shuffle pokers                            */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/
/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXCARDS 52
#define SUITSIZE 13
#define TRUE 1
#define FALSE 0

enum cardsuits{
    CLUB, 
    DIAMOND, 
    HEART, 
    SPADE
}; 

typedef struct card {
    char face[3];
    enum cardsuits suit;
} CARD;

typedef struct cardqueue {
    CARD cards[MAXCARDS + 1];
    int front, rear;
} CARDQUEUE;

typedef struct cardstack {
    CARD cards[MAXCARDS];
    int top;
} CARDSTACK;

/* global declarations */

/* function prototypes */
int sempty(CARDSTACK *);
CARD *spop(CARDSTACK *);
void spush(CARDSTACK *, CARD *);
void sdump(CARDSTACK *, FILE *);

int qempty(CARDQUEUE *);
CARD *qremove(CARDQUEUE *);
void qinsert(CARDQUEUE *, CARD *);
void qdump(CARDQUEUE *, FILE *);

void cshuffle(CARD *, CARDQUEUE *, FILE *);
void copen(CARDQUEUE *, CARD *, FILE *);
void cdump(CARD *, FILE *);

/* Verify if the stack is empty. Return TRUE or FALSE */
int sempty(CARDSTACK *cstack)
{
    if (cstack->top == 0)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/* Print out the all element from stack in order */
void sdump(CARDSTACK *cstack, FILE *fout)
{
    int ptr = 0;
    while (ptr < cstack->top)
    {
        fprintf(fout, "%s ", cstack->cards[ptr].face);
        ptr++;
    }
}

/* Remove the last element from stack. Return the removed element. */
CARD *spop(CARDSTACK *cstack)
{
    if (sempty(cstack))
    {
        printf("%s", "stack underflow");
        exit(1);
    }
    return (&(cstack->cards[--(cstack->top)]));
}

/* Insert the element to stack. */
void spush(CARDSTACK *cstack, CARD *card)
{
    if (cstack->top == MAXCARDS)
    {
        printf("%s", "stack overflow");
        exit(1);
    }
    else
    {
        strcpy(cstack->cards[cstack->top].face, card->face);
        cstack->cards[cstack->top].suit = card->suit;
        (cstack->top)++;
    }
    return;
}

/* Verify if the queue is empty. Return TRUE or FALSE */
int qempty(CARDQUEUE *cqueue)
{
    if (cqueue->front == cqueue->rear)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/* Print out the all element from queue in order */
void qdump(CARDQUEUE *cqueue, FILE *fout)
{
    int ptr = cqueue->front;
    while (ptr != cqueue->rear)
    {
        if (ptr == MAXCARDS - 1)
            ptr = 0;
        else
            ptr++;
        fprintf(fout, "%s ", cqueue->cards[ptr].face);
    }
}

/* Remove the first element in queue. Return the removed element. */
CARD *qremove(CARDQUEUE *cqueue)
{
    if (qempty(cqueue))
    {
        printf("%s", "queue underflow");
        exit(1);
    }
    if (cqueue->front == MAXCARDS - 1)
        cqueue->front = 0;
    else
        (cqueue->front)++;
    return (&(cqueue->cards[cqueue->front]));
}

/* Insert the element to the end of queue. */
void qinsert(CARDQUEUE *cqueue, CARD *card)
{
    if (cqueue->rear == MAXCARDS - 1)
        cqueue->rear = 0;
    else
        (cqueue->rear)++;
    if (cqueue->rear == cqueue->front)
    {
        printf("%s", "queue overflow");
        exit(1);
    }
    else
    {
        strcpy(cqueue->cards[cqueue->rear].face, card->face);
        cqueue->cards[cqueue->rear].suit = card->suit;
    }
    return;
}

/* Given an array of cards, shuffle it and store them into a queue. */
void cshuffle(CARD *carray, CARDQUEUE *cqueue, FILE *fout)
{
    CARDSTACK cs;
    CARDSTACK *cstack = &cs;
    CARD *card;
    int i;

    cstack->top = 0;

    for (i = SUITSIZE - 1; i >= 0; i--)
    {
        card = carray + i;

        // Push cards to the queue.
        qinsert(cqueue, card);
        fprintf(fout, "The card %s is inserted to the queue.\n", card->face);

        // Pop cards from the front and push them back to the rear of the queue.
        card = qremove(cqueue);
        qinsert(cqueue, card);
        fprintf(fout, "The card %s is removed and then inserted back to the queue.\n", card->face);
    }
    fprintf(fout, "The queue after 1st step: ");
    qdump(cqueue, fout);
    fprintf(fout, ".\n");

    // Pop cards from the queue and push them to the stack
    while (!qempty(cqueue))
    {
        card = qremove(cqueue);
        spush(cstack, card);
        fprintf(fout, "The card %s is removed from the queue and pushed to the stack.\n", card->face);
    }
    fprintf(fout, "The stack after 2nd step: ");
    sdump(cstack, fout);
    fprintf(fout, ".\n");

    // Push cards from the stack and push them to the queue.
    while (!sempty(cstack))
    {
        card = spop(cstack);
        qinsert(cqueue, card);
        fprintf(fout, "The card %s is popped from the stack and inserted to the queue.\n", card->face);
    }
    fprintf(fout, "The shuffled queue after 3rd step: ");
    qdump(cqueue, fout);
    fprintf(fout, ".\n");
}

/* Given a queue of shuffled cards, open the cards and store them in an array. */
void copen(CARDQUEUE *cqueue, CARD *carray, FILE *fout)
{
    CARD *card;
    while (!qempty(cqueue))
    {
        // Pop cards from the front and push them back to the end of the queue.
        card = qremove(cqueue);
        qinsert(cqueue, card);
        fprintf(fout, "The card %s is removed and inserted back to the queue.\n", card->face);

        // Move a card from the temperary deck to the shuffled deck
        card = qremove(cqueue);
        fprintf(fout, "The card %s is removed from the queue and opened.\n", card->face);

        strcpy(carray->face, card->face);
        carray->suit = card->suit;
        carray++;
    }
}

/* Print out the given array of cards. */
void cdump(CARD *carray, FILE *fout)
{
    int i;
    CARD *card;
    for (i = 0; i < SUITSIZE; i++)
    {
        card = carray + i;
        fprintf(fout, "%s", card->face);
        if (i < SUITSIZE - 1)
            fprintf(fout, ", ");
    }
}

/* Read in the cards from input file, shuffle it, open it, and print it out. */
int main(int argc, char const *argv[])
{
    int i, clen, val;
    char *startp, *endp;
    CARD carray[SUITSIZE];
    CARDQUEUE cq;
    CARDQUEUE *cqueue = &cq;

    // For file I/O
    FILE *fin, *fout;
	char *data = NULL;
    size_t len;
	ssize_t bytes;

    fout = fopen(argv[2], "w+");

    // Read in
    fin = fopen(argv[1], "r");
    bytes = getline(&data, &len, fin);
    data[bytes] = '\0';
    clen = 0;
    startp = data;
    for (i = 0; i < bytes; i++)
    {
        if (data[i] == ',')
        {
            data[i] = '\0';
            strcpy(carray[clen].face, startp);
            carray[clen].suit = SPADE;
            clen++;
        }
        else if (data[i] == ' ')
        {
            startp = data + i + 1;
        }
    }
    strcpy(carray[clen].face, startp);
    carray[clen].suit = SPADE;
    free(data);
	fclose(fin);
    fprintf(fout, "Readin cards: ");
    cdump(carray, fout);
    fprintf(fout, ".\n");

    // Shuffle
    cqueue->front = 0;
    cqueue->rear = 0;
    cshuffle(carray, cqueue, fout);
    copen(cqueue, carray, fout);

    // Display
    fprintf(fout, "Opened cards: ");
    cdump(carray, fout);
    fprintf(fout, ".\n");

    // Cleanup
    fclose(fout);
    return 0;
}
