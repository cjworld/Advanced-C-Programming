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
/*       Assignment Number: 9                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty One                            */
/*                                                            */
/*       File Name: ass9_chiyuchen.c                          */
/*                                                            */
/*       Date: 10/17/2018                                     */
/*                                                            */
/*       Objective: Implement a queue, read in the            */
/*                  operations from input file, and           */
/*                  print out every step with the element     */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXQUEUE 26
#define TRUE 1
#define FALSE 0

/* global declarations */
typedef struct queue {
    int queueelement[MAXQUEUE];
    int front, rear;
}INTQUEUE;

/* function prototypes */
int qempty(INTQUEUE *);
void qdump(INTQUEUE *, FILE *);
int qremove(INTQUEUE *);
void qinsert(INTQUEUE *, int);

/* Verify if the queue is empty. Return TRUE or FALSE */
int qempty(INTQUEUE *pq)
{
    if (pq->front == pq->rear)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/* Print out the all element from queue in order */
void qdump(INTQUEUE *pq, FILE *fout)
{
    int ptr = pq->front;
    while (ptr != pq->rear)
    {
        if (ptr == MAXQUEUE - 1)
            ptr = 0;
        else
            ptr++;
        fprintf(fout, "%d ", pq->queueelement[ptr]);
    }
}

/* Remove the first element in queue. Return the removed element. */
int qremove(INTQUEUE *pq)
{
    if (qempty(pq))
    {
        printf("%s", "queue underflow");
        exit(1);
    }
    if (pq->front == MAXQUEUE - 1)
        pq->front = 0;
    else
        (pq->front)++;
    return (pq->queueelement[pq->front]);
}

/* Insert the element to the end of queue. */
void qinsert(INTQUEUE *pq, int x)
{
    if (pq->rear == MAXQUEUE - 1)
        pq->rear = 0;
    else
        (pq->rear)++;
    if (pq->rear == pq->front)
    {
        printf("%s", "queue underflow");
        exit(1);
    }
    else
    {
        pq->queueelement[pq->rear] = x;
    }
    return;
}

int main(int argc, char const *argv[])
{
    int i, num;
    char *startp, *tmp;
    INTQUEUE queue;

    // For file I/O
    FILE *fin, *fout;
	char *data = NULL;
    size_t len;
	ssize_t bytes;

    fout = fopen(argv[2],"w+");
    fin = fopen(argv[1], "r");
    bytes = getline(&data, &len, fin);
    data[bytes] = '\0';
    fprintf(fout, "Input: %s.\n", data);

    i = 0;
    startp = data;
    queue.rear = 0;
    queue.front = 0;
    while (i < bytes)
    {
        if (data[i] == 'D')
        {
            num = qremove(&queue);
            fprintf(fout, "Dequeue command. Pop data %d from queue. The updated queue: ", num);
            qdump(&queue, fout);
            fprintf(fout, "\n");
        }
        else if (data[i] == 'E')
        {
            i++;
            while (data[i] == ' ' && i < bytes)
            {
                i++;
            }
            if (i < bytes)
            {
                startp = &data[i];
                while (data[i] != ' ' && i < bytes)
                {
                    i++;
                }
                data[i] = '\0';
                num = atoi(startp);
                qinsert(&queue, num);
                fprintf(fout, "Enqueue command. Push data %d into queue. The updated queue: ", num);
                qdump(&queue, fout);
                fprintf(fout, "\n");
            }
        }
        i++;
    }

    // Cleanup
    free(data);
	fclose(fin);
    fclose(fout);
    return 0;
}
