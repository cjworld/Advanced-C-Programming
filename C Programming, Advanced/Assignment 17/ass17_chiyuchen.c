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
/*       Date: 10/26/2018                                     */
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
int prefix_eval(ELEMSTACK *, FILE *);
void prefix_convert(ELEMSTACK *, ELEMSTACK *, FILE *);

/* Return the top element in the stack */ 
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
            fprintf(fout, "%c ", ps->elements[ptr].data.opt);
        else
            fprintf(fout, "%d ", ps->elements[ptr].data.opd);
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

/* Check if the given op has lower precedence than prev op */
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

/* Evaluate a prefix stack */
int prefix_eval(ELEMSTACK *prefixsp, FILE *fout)
{
    int result;
    ELEMNODE *elemnode, *opnode1, *opnode2;
    ELEMSTACK optstack = {0};
    ELEMSTACK opdstack = {0};

    fprintf(fout, "Start to evaluate the prefix string: ");
    sdump(prefixsp, fout);
    fprintf(fout, "\n");
    while (!sempty(prefixsp))
    {
        elemnode = spop(prefixsp);
        if (elemnode->type == OPERATOR)
        {
            fprintf(fout, ">> Operator: %c.", elemnode->data.opt);
            opnode1 = spop(&opdstack);
            opnode2 = spop(&opdstack);
            fprintf(fout, " Pop operand %d and operand %d from opd stack", opnode1->data.opd, opnode2->data.opd);
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
            fprintf(fout, ", and push result %d to opd stack.\n", result);
        }
        else
        {
            spush(&opdstack, NULL, elemnode->data.opd);
            fprintf(fout, ">> Operand: %d. Push operand %d to opd stack.\n", elemnode->data.opd, elemnode->data.opd);
        }   
    }
    elemnode = spop(&opdstack);
    return elemnode->data.opd;
}

/* Convert a infix stack to a prefix stack */
void prefix_convert(ELEMSTACK *infixsp, ELEMSTACK *prefixsp, FILE *fout)
{
    ELEMNODE *elemnode, *prevelemnode;
    ELEMSTACK optstack = {0};
    ELEMSTACK tmpstack = {0};
    ELEMSTACK *tmpsp = &tmpstack;

    fprintf(fout, "Start to convert the infix string: ");
    sdump(infixsp, fout);
    fprintf(fout, "\n");

    while (!sempty(infixsp))
    {
        elemnode = spop(infixsp);
        if (elemnode->type == OPERATOR)
        {
            // If we encounter a (, we need to pop out all operator in between ( and ).
            fprintf(fout, "Operator: %c.\n", elemnode->data.opt);
            if (elemnode->data.opt == '(')
            {
                fprintf(fout, ">> Pop until ).\n");
                prevelemnode = stop(&optstack);
                if (prevelemnode)
                {
                    while (prevelemnode->data.opt != ')')
                    {
                        prevelemnode = spop(&optstack);
                        spush(tmpsp, &(prevelemnode->data.opt), 0);
                        fprintf(fout, ">>>> Pop operator %c from the optstack and push to the prefix stack.\n", prevelemnode->data.opt);
                        prevelemnode = stop(&optstack);
                    }
                    fprintf(fout, ">> Pop operator %c from the optstack.\n", prevelemnode->data.opt);
                    prevelemnode = spop(&optstack);
                    if (!sempty(infixsp))
                    {
                        elemnode = spop(infixsp);
                    }
                    else
                    {
                        elemnode = NULL;
                    }
                }
            }
            // Compare to the toppest operator in the stack, which is the operator followed.
            // If the folloing operator has higher precedence, process it first.
            if (elemnode)
            {
                prevelemnode = stop(&optstack);
                if (prevelemnode)
                {
                    if (has_lower_precedence_than(elemnode->data.opt, prevelemnode->data.opt) == TRUE)
                    {
                        fprintf(fout, ">> Pop the operators which has higher prcedence after the operand %c.\n", elemnode->data.opt);
                        while (has_lower_precedence_than(elemnode->data.opt, prevelemnode->data.opt) == TRUE)
                        {
                            prevelemnode = spop(&optstack);
                            spush(tmpsp, &(prevelemnode->data.opt), 0);
                            fprintf(fout, ">>>> Pop the operator %c from the optstack and push to the prefix stack.\n", prevelemnode->data.opt);
                            prevelemnode = stop(&optstack);
                        }
                    }
                }
                spush(&optstack, &(elemnode->data.opt), 0);
                fprintf(fout, ">> Push operator %c to the optstack.\n", elemnode->data.opt);
            }
        }
        else
        {
            fprintf(fout, "Operand: %d.\n", elemnode->data.opd);
            spush(tmpsp, NULL, elemnode->data.opd);
            fprintf(fout, ">> Push operand %d to the prefix stack.\n", elemnode->data.opd);
        }   
    }
    if (!sempty(&optstack))
    {
        fprintf(fout, "Pop all operands in the optstack: ");
        sdump(&optstack, fout);
        fprintf(fout, "\n");
        while (!sempty(&optstack))
        {
            prevelemnode = spop(&optstack);
            spush(tmpsp, &(prevelemnode->data.opt), 0);
            fprintf(fout, ">> Push operand %c to the prefix stack.\n", prevelemnode->data.opt);
        }
    }

    while (!sempty(tmpsp))
    {
        elemnode = spop(tmpsp);
        if (elemnode->type == OPERATOR)
        {
            spush(prefixsp, &(elemnode->data.opt), 0);
        }
        else
        {
            spush(prefixsp, NULL, elemnode->data.opd);
        }   
    }
}

int main(int argc, char const *argv[])
{
    int i, isnum, value;
    char op, *startp;
    int input[MAXSIZE];
    ELEMNODE *elemnode;
    ELEMSTACK infixstack = {0};
    ELEMSTACK prefixstack = {0};

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

    fprintf(fout, "\n");
    fprintf(fout, "Infix string: ");
    sdump(&infixstack, fout);
    fprintf(fout, "\n\n");

    prefix_convert(&infixstack, &prefixstack, fout);

    fprintf(fout, "\n");
    fprintf(fout, "Prefix string: ");
    sdump(&prefixstack, fout);
    fprintf(fout, "\n\n");

    value = prefix_eval(&prefixstack, fout);

    fprintf(fout, "\n");
    fprintf(fout, "The result of evaluation: %d.\n", value);

    // Cleanup
    fclose(fout);
    return 0;
}
