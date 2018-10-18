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
/*       Assignment Number: 8                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty                                */
/*                                                            */
/*       File Name: ass8_chiyuchen.c                          */
/*                                                            */
/*       Date: 10/13/2018                                     */
/*                                                            */
/*       Objective: Read in a string, validate the scopes,    */
/*                  and put the result to output file         */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#define MAXSTACK 26
#define EMPTYSTACK -1
#define TRUE 1
#define FALSE 0

/* global declarations */
typedef struct stack {
    int top;
    char stackelement[MAXSTACK];
}CHSTACK;

/* function prototypes */
int empty(CHSTACK *);
char pop(CHSTACK *);
void push(CHSTACK *, char);
int validate(char *, int, FILE *);
void dump(CHSTACK *, FILE *);

/* Verify if the stack is empty. Return TRUE or FALSE */
int empty(CHSTACK *ps)
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
void dump(CHSTACK *ps, FILE *fout)
{
    int ptr = 0;
    while (ptr < ps->top)
    {
        fprintf(fout, "%c ", ps->stackelement[ptr]);
        ptr++;
    }
}

/* Remove the last element from stack. Return the removed element. */
char pop(CHSTACK *ps)
{
    if (empty(ps))
    {
        printf("%s", "stack underflow");
        exit(1);
    }
    return (ps->stackelement[--(ps->top)]);
}

/* Insert the element to stack. */
void push(CHSTACK *ps, char x)
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

/* Valid the scope of the given string. Return TRUE or FALSE. */
int validate(char *exp, int length, FILE *fout)
{
    int i;
    CHSTACK stack;
    char ch, ch_in_stack;
    for(i = 0; i < length; i++)
    {
        ch = exp[i];
        if (ch == '[' || ch == '{' || ch == '(' ) 
        {
            push(&stack, ch);
            fprintf(fout, "Opening scope %c. Push it to the scope stack. The updated scope stack: ", ch);
            dump(&stack, fout);
            fprintf(fout, "\n");
        }
        else if (ch == ']' || ch == '}' || ch == ')' ) 
        {
            fprintf(fout, "Closing scope %c. Pop a opening scope from the scope stack, and ", ch);

            if (empty(&stack) == TRUE)
            {
                fprintf(fout, "Not found.\n");
                return FALSE;
            }   

            ch_in_stack = pop(&stack);
            if (ch == ']') 
            {
                if (ch_in_stack != '[')
                {
                    fprintf(fout, "opening scope %c is not match.\n", ch_in_stack);
                    return FALSE;
                }
            }
            else if (ch == '}')
            {
                if (ch_in_stack != '{')
                {
                    fprintf(fout, "opening scope %c is not match.\n", ch_in_stack);
                    return FALSE;
                }
            }
            else if (ch == ')')
            {
                if (ch_in_stack != '(')
                {
                    fprintf(fout, "opening scope %c is not match.\n", ch_in_stack);
                    return FALSE;
                }
            }
            
            fprintf(fout, "opening scope %c is match. The updated scope stack: ", ch_in_stack);
            dump(&stack, fout);
            fprintf(fout, "\n");
        }
        else
        {
            fprintf(fout, "Ignore %c. Drop it.\n", ch);
        }
    }
    if (empty(&stack) == FALSE)
        return FALSE;

    return TRUE;
}

int main(int argc, char const *argv[])
{
    int i, ret;

    // For file I/O
    FILE *fin, *fout;
	char *line = NULL;
    size_t len = 0;
	ssize_t readbytes;
    fin = fopen("input.txt", "r");
    fout = fopen (argv[1],"w+");

    // Read line from file and parse it.
    while ((readbytes = getline(&line, &len, fin)) != -1) {
        fprintf(fout, "The string %s is %s.\n", line, validate(line, readbytes, fout) == TRUE ? "valid" : "invalid");
	}

    // Cleanup
    free(line);
	fclose(fin);
    fclose(fout);
    return 0;
}
