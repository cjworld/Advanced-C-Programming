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
/*       Assignment Number: 17                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty Two                            */
/*                                                            */
/*       File Name: ass17_chiyuchen.c                         */
/*                                                            */
/*       Date: 10/20/2018                                     */
/*                                                            */
/*       Objective: Infix to Prefix                           */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#define MAXSIZE 100
#define EMPTYSTACK -1
#define TRUE 1
#define FALSE 0

typedef enum elemtype {
    OPERAND,
    OPERATOR
} ELEMTYPE;

typedef union elemdata {
    int opd;
    char opt;
} ELEMDATA;

typedef struct elemnode {
    ELEMTYPE type;
    ELEMDATA data;
} ELEMNODE;

typedef struct elemstack {
    ELEMNODE elements[MAXSIZE];
    int top;
} ELEMSTACK;

/* global declarations */

/* function prototypes */
ELEMNODE *stop(ELEMSTACK *);
int sempty(ELEMSTACK *);
ELEMNODE *spop(ELEMSTACK *);
void spush(ELEMSTACK *, char *, int);
void sdump(ELEMSTACK *, FILE *);
int has_lower_precedence_than(char, char);

ELEMNODE *stop(ELEMSTACK *ps)
{
    if (ps->top == 0)
    {
        return NULL;
    }
    else
    {
        return &(ps->elements[ps->top - 1]);
    }
}

/* Verify if the stack is empty. Return TRUE or FALSE */
int sempty(ELEMSTACK *ps)
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

/* Print out the all element from stack in order */
void sdump(ELEMSTACK *ps, FILE *fout)
{
    int ptr = 0;
    while (ptr < ps->top)
    {
        if (ps->elements[ptr].type == OPERATOR)
            printf("%c ", ps->elements[ptr].data.opt);
        else
            printf("%d ", ps->elements[ptr].data.opd);
        ptr++;
    }
}

/* Remove the last element from stack. Return the removed element. */
ELEMNODE *spop(ELEMSTACK *ps)
{
    if (sempty(ps))
    {
        printf("%s", "stack underflow");
        exit(1);
    }
    return &(ps->elements[--(ps->top)]);
}

/* Insert the element to stack. */
void spush(ELEMSTACK *ps, char *opt, int opd)
{
    if (ps->top == MAXSIZE)
    {
        printf("%s", "stack overflow");
        exit(1);
    }
    else
    {
        if (opt)
        {
            ps->elements[ps->top].type = OPERATOR;
            ps->elements[ps->top].data.opt = *opt;
        }
        else
        {
            ps->elements[ps->top].type = OPERAND;
            ps->elements[ps->top].data.opd = opd;    
        }
        (ps->top)++;
    }
    return;
}

int has_lower_precedence_than(char op, char prev)
{
    if (prev == ')')
        return FALSE;

    switch (op)
    {
        case '^': if (prev == '^')
                      return FALSE;
                  break;
        case '+':
        case '-': if (prev == '^' || prev == '+' || prev == '-')
                      return FALSE;
                  break;
        case '*':
        case '/':
        case '%': 
        case ')': return FALSE;
                  break;
        default:  break;
    }
    return TRUE;
}

int main(int argc, char const *argv[])
{
    int i, isnum, result;
    char op, *startp;
    int input[MAXSIZE];
    ELEMNODE *elemnode, *prevelemnode, *opnode1, *opnode2;
    ELEMSTACK infixstack;
    ELEMSTACK reversedstack;
    ELEMSTACK prefixstack;
    ELEMSTACK optstack;
    ELEMSTACK opdstack;

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
    infixstack.top = 0;
    reversedstack.top = 0;
    prefixstack.top = 0;
    optstack.top = 0;
    opdstack.top = 0;
    isnum = FALSE;
    startp = NULL;
    for (i = 0; i < bytes; i++)
    {
        if (data[i] < '0' || data[i] > '9')
        {
            op = data[i];
            if (isnum)
            {
                data[i] = '\0';
                spush(&infixstack, NULL, atoi(startp));
                startp = NULL;
            }
            if (op != ' ')
            {
                spush(&infixstack, &op, 0);
            }
            isnum = FALSE;
        }
        else
        {
            if (!isnum)
                startp = data + i;
            isnum = TRUE;
        }
    }
    if (isnum)
    {
        spush(&infixstack, NULL, atoi(startp));
    }
    free(data);
	fclose(fin);

    sdump(&infixstack, fout);
    printf("\n");

    // while (!sempty(&infixstack))
    // {
    //     elemnode = spop(&infixstack);
    //     if (elemnode->type == OPERATOR)
    //     {
    //         spush(&reversedstack, &(elemnode->data.opt), 0);
    //     }
    //     else
    //     {
    //         spush(&reversedstack, NULL, elemnode->data.opd);
    //     }   
    // }
    // sdump(&reversedstack, fout);
    // printf("\n");

    while (!sempty(&infixstack))
    {
        elemnode = spop(&infixstack);
        if (elemnode->type == OPERATOR)
        {
            printf("OP: %c\n", elemnode->data.opt);
            if (elemnode->data.opt == '(')
            {
                prevelemnode = stop(&optstack);
                if (prevelemnode)
                {
                    printf(">>>> Pop OP %c until ).\n", prevelemnode->data.opt);
                    while (prevelemnode->data.opt != ')')
                    {
                        prevelemnode = spop(&optstack);
                        spush(&prefixstack, &(prevelemnode->data.opt), 0);
                        printf(">>>> Push OP %c to prefix stack.\n", prevelemnode->data.opt);
                        prevelemnode = stop(&optstack);
                        printf(">>>> Pop OP %c until ).\n", prevelemnode->data.opt);
                    }
                    prevelemnode = spop(&optstack);
                    if (!sempty(&infixstack))
                    {
                        elemnode = spop(&infixstack);
                        printf(">>>> Pop next OP %c.\n", elemnode->data.opt);
                    }
                    else
                    {
                        elemnode = NULL;
                    }
                }
            }
            if (elemnode)
            {
                prevelemnode = stop(&optstack);
                if (prevelemnode)
                {
                    printf(">>>> Compare OP %c with prev OP %c\n", elemnode->data.opt, prevelemnode->data.opt);
                    while (has_lower_precedence_than(elemnode->data.opt, prevelemnode->data.opt) == TRUE)
                    {
                        prevelemnode = spop(&optstack);
                        spush(&prefixstack, &(prevelemnode->data.opt), 0);
                        printf(">>>> Push prev OP %c to prefix stack.\n", prevelemnode->data.opt);
                        prevelemnode = stop(&optstack);
                        printf(">>>> Compare OP %c with prev OP %c.\n", elemnode->data.opt, prevelemnode->data.opt);
                    }
                }
                spush(&optstack, &(elemnode->data.opt), 0);
                printf(">> Push OP %c to optstack.\n", elemnode->data.opt);
                printf(">> prefixstack: ");
                sdump(&prefixstack, fout);
                printf("\n");
                printf(">> optstack: ");
                sdump(&optstack, fout);
                printf("\n");
            }
        }
        else
        {
            printf("OPERAND: %d\n", elemnode->data.opd);
            spush(&prefixstack, NULL, elemnode->data.opd);
            printf(">> Push OPERAND %d to prefix stack.\n", elemnode->data.opd);
            printf(">> prefixstack: ");
            sdump(&prefixstack, fout);
            printf("\n");
            printf(">> optstack: ");
            sdump(&optstack, fout);
            printf("\n");
        }   
    }
    while (!sempty(&optstack))
    {
        prevelemnode = spop(&optstack);
        spush(&prefixstack, &(prevelemnode->data.opt), 0);
        printf(">> Push prev OP %c to prefix stack.\n", prevelemnode->data.opt);
    }
    sdump(&prefixstack, fout);
    printf("\n");

    while (!sempty(&prefixstack))
    {
        elemnode = spop(&prefixstack);
        if (elemnode->type == OPERATOR)
        {
            spush(&reversedstack, &(elemnode->data.opt), 0);
        }
        else
        {
            spush(&reversedstack, NULL, elemnode->data.opd);
        }   
    }
    sdump(&reversedstack, fout);
    printf("\n");

    while (!sempty(&reversedstack))
    {
        elemnode = spop(&reversedstack);
        if (elemnode->type == OPERATOR)
        {
            printf("OP: %c\n", elemnode->data.opt);
            opnode1 = spop(&opdstack);
            opnode2 = spop(&opdstack);
            printf(">> Pop operand %d and operand %d from opd stack.\n", opnode1->data.opd, opnode2->data.opd);
            switch (elemnode->data.opt)
            {
                case '+': result = opnode1->data.opd + opnode2->data.opd; break;
                case '-': result = opnode1->data.opd - opnode2->data.opd; break;
                case '*': result = opnode1->data.opd * opnode2->data.opd; break;
                case '/': result = opnode1->data.opd / opnode2->data.opd; break;
                case '%': result = opnode1->data.opd % opnode2->data.opd; break;
                case '^': result = opnode1->data.opd ^ opnode2->data.opd; break;
                default:  result = 0;
            }
            spush(&opdstack, NULL, result);
            printf(">> Push result %d to opd stack.\n", result);
        }
        else
        {
            printf("OPERAND: %d\n", elemnode->data.opd);
            spush(&opdstack, NULL, elemnode->data.opd);
            printf(">> Push OPERAND %d to opd stack.\n", elemnode->data.opd);
        }   
    }
    elemnode = spop(&opdstack);
    printf("Result %d.\n", elemnode->data.opd);

    // Cleanup
    fclose(fout);
    return 0;
}
