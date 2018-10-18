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
/*       Assignment Number: 11                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty Four                           */
/*                                                            */
/*       File Name: ass11_chiyuchen.c                         */
/*                                                            */
/*       Date: 10/17/2018                                     */
/*                                                            */
/*       Objective: Read in numbers and store them in         */
/*                  a linked list. Reverse it, and            */
/*                  try to remove some nodes.                 */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0

typedef struct listnode {
    int data;
    struct listnode *next;
} LISTNODE;

typedef struct list {
    LISTNODE *head;
} LIST;

/* global declarations */

/* function prototypes */
void linsert(LIST *, int, FILE *);
int ldelete(LIST *, int, FILE *);
void lreverse(LIST *, FILE *);
int lempty(LIST *);
void ldump(LIST *, FILE *);

/* Verify if the linked list is empty. Return TRUE or FALSE */
int lempty(LIST *list)
{
    return (list->head == NULL);
}

/* Insert a new node into the given linked list. */
void linsert(LIST *list, int value, FILE *fout)
{
    LISTNODE *newnode, *prenode = NULL, *curnode = NULL;
    newnode = (LISTNODE *) malloc(sizeof(LISTNODE));

    if (newnode != NULL)
    {
        newnode->data = value;
        newnode->next = NULL;
        curnode = list->head;
        while (curnode != NULL)
        {
            prenode = curnode;
            curnode = curnode->next;
        }
        if (prenode == NULL)
        {
            list->head = newnode;
        }
        else
        {
            prenode->next = newnode;
        }
        fprintf(fout, "Create a new node for %d. ", value);
    }
    else
    {
        fprintf(fout, "%c not inserted. No memory is available\n", value);
        printf("%c not inserted. No memory is available\n", value);
    }
}

/* Remove a node from the given linked list. */
int ldelete(LIST *list, int value, FILE *fout)
{
    LISTNODE *prenode = NULL, *curnode = NULL;
    curnode = list->head;
    while (curnode != NULL && curnode->data != value)
    {
        prenode = curnode;
        curnode = curnode->next;
    }
    if (curnode != NULL)
    {
        if (prenode == NULL)
            list->head = curnode->next;
        else
            prenode->next = curnode->next;
        fprintf(fout, "Remove the node for %d. ", value);
        curnode->next = NULL;
        free(curnode);
        return (FALSE);
    }
    return (TRUE);
}

/* Reverse the given linked list. */
void lreverse(LIST *list, FILE *fout)
{
    LISTNODE *prenode = NULL, *curnode = NULL, *tmpnode = NULL;
    curnode = list->head;
    while (curnode != NULL)
    {
        tmpnode = curnode->next;
        curnode->next = prenode;
        prenode = curnode;
        curnode = tmpnode;
    }
    list->head = prenode;
}

/* Print out the given linked list. */
void ldump(LIST *list, FILE *fout)
{
    LISTNODE *node = list->head;
    while (node != NULL)
    {
        fprintf(fout, "%d --> ", node->data);
        node = node->next;
    }
    fprintf(fout, "NULL \n");
}

int main(int argc, char const *argv[])
{
    int i, value;
    char *startp, cmd;
    LISTNODE *newnode;
    LIST l;
    LIST *list = &l;

    // For file I/O
    FILE *fin, *fout;
    char *data = NULL;
    size_t len;
	ssize_t bytes;

    // Read in
    fout = fopen (argv[2],"w+");
    fin = fopen(argv[1], "r");
    bytes = getline(&data, &len, fin);
    data[bytes] = '\0';

    // Parse
    list->head = NULL;
    startp = data;
    for (i = 0; i < bytes; i++)
    {
        if (data[i] == ',')
        {
            data[i] = '\0';
            value = atoi(startp);
            fprintf(fout, "Read in %d. ", value);
            linsert(list, value, fout);
            fprintf(fout, "The updated linked list: ");
            ldump(list, fout);
        }
        else if (data[i] == ' ')
        {
            startp = data + i + 1;
        }
    }
    value = atoi(startp);
    fprintf(fout, "Read in %d. ", value);
    linsert(list, value, fout);
    fprintf(fout, "The updated linked list: ");
    ldump(list, fout);
    free(data);
	fclose(fin);
    fprintf(fout, "\n");

    // Reverse
    lreverse(list, fout);
    fprintf(fout, "The reversed linked list: ");
    ldump(list, fout);
    fprintf(fout, "\n");

    // Interaction
    while (TRUE)
    {
        printf("Next command [Q for quit, D for deletion]: ");
        scanf(" %c", &cmd);
        fprintf(fout, "Next command [Q for quit, D for deletion]: %c\n", cmd);
        if (cmd == 'Q')
        {
            printf("Good bye.\n");
            break;
        }
        if (cmd != 'D')
        {
            printf("Invalid command. Try again.\n");
            fprintf(fout, "Invalid command. Try again.\n");
            continue;
        }

        printf("The number to be deleted: ");
        scanf(" %d", &value);
        fprintf(fout, "The number to be deleted: %d\n", value);
        if (ldelete(list, value, fout) == 0)
        {
            fprintf(fout, "The updated linked list: ");
            ldump(list, fout);
            fprintf(fout, "\n");
        }
        else
        {
            fprintf(fout, "Not found.\n");
            printf("Not found.\n");
            fprintf(fout, "\n");
        }
    }

    fclose(fout);
    return 0;
}
