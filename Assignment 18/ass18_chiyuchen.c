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
/*       Assignment Number: 18                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty Five                           */
/*                                                            */
/*       File Name: ass18_chiyuchen.c                         */
/*                                                            */
/*       Date: 10/17/2018                                     */
/*                                                            */
/*       Objective: Long Integer                              */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#define CHUNK 100000
#define CHUNKSHIFT 5
#define TRUE 1
#define FALSE 0

typedef struct dldnode {
    int data;
    struct dldnode *higher;
    struct dldnode *lower;
} DLLNODE;

/* global declarations */

/* function prototypes */
DLLNODE *get_longint(char *, int);
void dll_insert_higher_than(DLLNODE *, DLLNODE *);
DLLNODE *dll_addition(DLLNODE *, DLLNODE *);
void dll_dump(DLLNODE *, FILE *);
DLLNODE *dll_new_node(int);
void dll_release(DLLNODE *);

DLLNODE *dll_new_node(int value)
{
    DLLNODE *node = (DLLNODE *) malloc(sizeof(DLLNODE));
    node->data = value;
    node->higher = node;
    node->lower = node;
    return node;
}

void dll_release(DLLNODE *tail)
{
    DLLNODE *node = tail->lower, *tmp;
    while (node != tail)
    {
        tmp = node;
        node = node->lower;
        free(tmp);
    }
    free(tail);
}

void dll_dump(DLLNODE *head, FILE *fout)
{
    DLLNODE *node, *tail;
    tail = head->higher;
    node = head;
    fprintf(fout, "%d", node->data);
    while (node->lower != head)
    {
        node = node->lower;
        fprintf(fout, " --> %05d", node->data);
    }
}

void dll_insert_higher_than(DLLNODE *lnode, DLLNODE *hnode)
{
    hnode->higher = lnode->higher;
    lnode->higher = hnode;
    hnode->higher->lower = hnode;
    hnode->lower = lnode;
}

DLLNODE *dll_addition(DLLNODE *heada, DLLNODE *headb)
{
    DLLNODE *nodea, *nodeb, *nodec, *newnode;
    int tmpa, tmpb, tmpc, carry = 0;

    nodea = heada->higher;
    nodeb = headb->higher;
    nodec = NULL;
    while (nodea || nodeb)
    {
        tmpa = nodea ? nodea->data : 0;
        tmpb = nodeb ? nodeb->data : 0;
        tmpc = tmpa + tmpb + carry;
        carry = tmpc / CHUNK;
        newnode = dll_new_node(tmpc % CHUNK);
        if (nodec)
            dll_insert_higher_than(nodec, newnode);
        nodec = newnode;
        if (nodea)
            nodea = (nodea == heada) ? NULL : nodea->higher;
        if (nodeb)
            nodeb = (nodeb == headb) ? NULL : nodeb->higher;
    }
    return nodec;
}

DLLNODE *get_longint(char *data, int bytes)
{
    int i, sidx, value;
    char *startp;
    DLLNODE *node = NULL, *newnode = NULL;
    for (i = bytes - 1; i >= 0; i = i - CHUNKSHIFT)
    {
        sidx = (i - CHUNKSHIFT >= 0) ? (i - CHUNKSHIFT) : 0;
        startp = data + sidx;
        value = atoi(startp);
        newnode = dll_new_node(value);
        if (node)
            dll_insert_higher_than(node, newnode);
        node = newnode;
        *startp = '\0';
    }
    return node;
}

int main(int argc, char const *argv[])
{
    int i, value, sidx;
    char *startp = NULL;
    DLLNODE *nodea, *nodeb, *noderet;

    // For file I/O
    FILE *fin, *fout;
    char *dataa = NULL, *datab = NULL;
    size_t len;
	ssize_t bytesa, bytesb;

    // Read in
    fout = fopen (argv[2],"w+");
    fin = fopen(argv[1], "r");
    bytesa = getline(&dataa, &len, fin);
    dataa[bytesa - 1] = '\0';
    bytesb = getline(&datab, &len, fin);
    datab[bytesb - 1] = '\0';

    // Parse
    fprintf(fout, "1st operand: ");
    nodea = get_longint(dataa, bytesa);
    dll_dump(nodea, fout);
    fprintf(fout, "\n");

    fprintf(fout, "2nd operand: ");
    nodeb = get_longint(datab, bytesb);
    dll_dump(nodeb, fout);
    fprintf(fout, "\n");

    free(dataa);
    free(datab);
	fclose(fin);

    fprintf(fout, "Result: ");
    noderet = dll_addition(nodea, nodeb);
    dll_dump(noderet, fout);
    fprintf(fout, "\n");

    dll_release(nodea);
    dll_release(nodeb);
    dll_release(noderet);
    fclose(fout);
    return 0;
}
