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
/*       Assignment Number: 12                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty Five                           */
/*                                                            */
/*       File Name: ass12_chiyuchen.c                         */
/*                                                            */
/*       Date: 10/02/2018                                     */
/*                                                            */
/*       Objective: Implement all sorting methods             */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>

/* global declarations */

/* function prototypes */

void printarray(int array[], int length)
{
    int i, j, tmp;
    for (i = 0; i < length; i++)
    {
        printf("%d, ", array[i]);
    }
    printf("\n");
}

void swap(int *a, int *b)
{
    int tmp;
    tmp = *b;
    *b = *a;
    *a = tmp;
}

void bubblesort(int array[], int length)
{
    int i, j, tmp;
    for (i = 0; i < length - 1; i++)
    {
        for (j = 0; j < length - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                swap(&array[j], &array[j + 1]);
            }
        }
        printarray(array, length);
    }
}

void insertsort(int array[], int length)
{
    int i, j;
    for (i = 1; i < length; i++)
    {
        for (j = i; j > 0; j--)
        {
            if (array[j-1] > array[j])
            {
                swap(&array[j-1], &array[j]);
            }
        }
        printarray(array, length);
    }
}

void selectionsort(int array[], int length)
{
    int i, j, val, idx;
    for (i = 0; i < length - 1; i++)
    {
        val = array[i];
        for (j = i; j < length; j++)
        {
            if (val > array[j])
            {
                val = array[j];
                idx = j;
            }
        }
        swap(&array[i], &array[idx]);
        printarray(array, length);
    }
}


void quicksort(int array[], int length)
{
    int value = array[length - 1];
    int i = 0, j = length - 2;
    int idx = i;
    printf("Length: %d\n", length);
    printarray(array, length);

    if (length == 1)
    {
        return;
    }

    for (; i <= j; i++)
    {
        printf("array[%d]: %d\n", i, array[i]);
        if (array[i] > value)
        {
            for (; j > i; j--)
            {
                if (array[j] < value)
                {
                    printf("Swap %d to %d\n", array[i], array[j]);
                    swap(&array[i], &array[j]);
                    idx = i;
                    break;
                }
            }
        }
        else
        {
            idx = i;
        }
    }
    if (array[idx] < value)
    {
        idx++;
    }
    printarray(array, length);
    printf("idx: %d\n", idx);
    printarray(array, length);
    quicksort(&array[0], idx);
    swap(&array[idx], &array[length - 1]);
    getchar();
    quicksort(&array[idx + 1], (length - 1) - (idx + 1) + 1);
    getchar();
}



int main()
{
    int input[] = {49,17,56,85,12,97,33,71,19,62,38,84,51,29,77,65,44,99,4,47,67,41,23,88,73,8,100,25,91,58,59,22,15,35,95,60,20,7,50,10};
    printarray(input, 40);
    // bubblesort(input, 40);
    // insertsort(input, 40);
    // selectionsort(input, 40);
    quicksort(input, 40);
    printarray(input, 40);
    return 0;
}








// struct node {
//     int info;
//     struct node *next;
// };
// typedef struct node *NODEPTR;

// NODEPTR getnode(void)
// {
//     NODEPTR p;
//     p = (NODEPTR) malloc(sizeof(struct node));
//     return (p);
// }

// void freenode(NODEPTR p)
// {
//     free(p);
// }

// /* Place x in the tail of pstack */
// void insafter(NODEPTR p, int x)
// {
//     NODEPTR q;
//     if (p == NULL)
//     {
//         printf("void insertion \n");
//         exit(1);
//     }
//     q = getnode();
//     q->info = x;
//     q->next = p->next;
//     p->next = q;
//     return;
// }

// void delafter(NODEPTR p, int *px)
// {
//     NODEPTR q;
//     if ((p == NULL) || (p->next == NULL))
//     {
//         printf("void deletion \n");
//         exit(1);
//     }
//     q = p->next;
//     *px = q->info;
//     p->next = q->next;
//     freenode(q);
//     return;
// }

// void place(NODEPTR *plist, int x)
// {
//     NODEPTR p, q = NULL;
//     for (p = *plist; p != NULL && x > p->info; p = p->next)
//     {
//         q = p;
//     }
//     if (q == NULL)
//     {
//         push(plist, x);
//     }
//     else
//     {
//         insafter(q, x);
//     }
//     return;
// }

// NODEPTR search(NODEPTR list, int x)
// {
//     NODEPTR p;
//     for (p = list; p != NULL; p = p->next)
//     {
//         if (p->info == x)
//         {
//             return (p);
//         }
//     }
//     return (NULL);
// }

// void remvx(NODEPTR *plist, int x)
// {
//     NODEPTR p = *plist, q = NULL;
//     int y;
//     while (p != NULL)
//     {
//         if (p->info == x)
//         {
//             p = p->next;
//             if (q == NULL)
//             {
//                 freenode(*plist);
//                 *plist = p;
//             }
//             else
//             {
//                 delafter(q, &y);
//             }
//         }
//         else
//         {
//             q = p;
//             p = p->next;
//         }
//     }
//     return;
// }

// /* Place x in the tail of pstack */
// void push(NODEPTR *pstack, int x)
// {
//     NODEPTR p;
//     p = getnode();
//     p->info = x;
//     if (empty(pstack) == TRUE)
//     {
//         *pstack = p;
//     }
//     else
//     {
//         p->next = (*pstack)->next;
//     }
//     (*pstack)->next = p;
// }

// int empty(NODEPTR *pstack)
// {
//     return (*pstack == NULL ? TRUE : FALSE);
// }

// /* Remove x in the tail of pstack */
// int pop(NODEPTR *pstack)
// {
//     int x;
//     NODEPTR p;
//     if (empty(pstack) == TRUE)
//     {
//         printf("stack underflow \n");
//         exit(1);
//     }
//     p = (*pstack)->next;
//     x = p->info;
//     if (p == *pstack)
//     {
//         *pstack = NULL;
//     }
//     else
//     {
//         *pstack = p->next;
//     }
//     freenode(p);
//     return (x);
// }

// void insert(NODEPTR *pq, int x)
// {
//     NODEPTR p;
//     p = getnode();
//     p->info = x;
//     if (empty(pq) == TRUE)
//     {
//         *pq = p;
//     }
//     else
//     {
//         p->next = (*pq)->next;
//     }   
//     p->next = (*pq)->next;
//     *pq = p;
//     return;
// }

// int remove(NODEPTR *pq)
// {
//     int x;
//     NODEPTR p;
//     if (empty(pq) == TRUE)
//     {
//         printf("queue underflow \n");
//         exit(1);
//     }
//     p = (*pq)->next;
//     x = p->info;
//     if (p == *pq)
//     {
//         *pq = NULL;
//     }
//     else
//     {
//         *pq = p->next;
//     }
//     freenode(p);
//     return (x);
// }




















// struct dlnode {
//     int info;
//     struct node *next;
//     struct node *prev;
// };
// typedef struct dlnode *DLNDPTR, DLNODE;
// DLNDPTR dllheader;



// DLNDPTR makeDLNode(int dllistinfo)
// {
//     DLNDPTR pdln = getnode();
//     pdln->info = dllistinfo;
//     pdln->next = NULL;
//     pdln->prev = NULL;
//     return (pdln);
// }

// dllheader->next = dllheader;
// dllheader->prev = dllheader;

// void insertdlnatEnd(DLNDPTR dllheader, DLNDPTR pdln)
// {
//     pdln->prev = dllheader->prev;
//     dllheader->prev->next = pdln;
//     dllheader->prev = pdln;
//     pdln->next = dllheader;
//     return;
// }


// int deleteatpdln(DLNDPTR dllheader, DLNDPTR pdln)
// {
//     int pdlninfo;
//     if (pdln && dllheader)
//     {
//         pdln->info = pdln->info;
//         pdln->prev->next = pdln->next;
//         pdln->next->prev = pdln->prev;
//         pdln->prev = NULL;
//         pdln->next = NULL;
//         free(pdln);
//         return (pdlninfo);
//     }
//     return (-1);
// }









// int main(int argc, char const *argv[])
// {
//     char cmd;

//     // For file I/O
//     FILE *fout;
//     fout = fopen ("output.txt","w+");

//     do
//     {
//         printf("Place your queen in a row [0-7, Q]:");
//         scanf(" %c", &cmd);
//         if (cmd != 'Q' || (cmd < '0' || cmd > '7'))
//         {
//             printf("Invalid. Please try again.\n");
//         }
//         else
//         {
//             if (cmd == 'Q')
//                 break;
//             printf("Invalid. Please try again.\n");
//         }
//     }
//     while (TRUE);

//     fclose(fout);
   
//     return 0;
// }
