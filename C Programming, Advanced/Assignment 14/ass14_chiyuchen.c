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
/*       Assignment Number: 14                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty Eight                          */
/*                                                            */
/*       File Name: ass14_chiyuchen.c                         */
/*                                                            */
/*       Date: 10/02/2018                                     */
/*                                                            */
/*       Objective: Binary Tree Traversal                     */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>

/* global declarations */

/* function prototypes */

#define NUM 5

typedef struct btreenode {
    int numtrees;
    struct btreenode *child[NUM];
    int keys[NUM - 1];
    struct btreenode *parent;
    int index;
} BTREE;
typedef BTREE *BTREEP;

BTREEP makebtree(int key)
{
    BTREEP btree = getbtree();
    btree->keys[0] = key;
    btree->numtrees++;
    return (btree);
}

BTREEP getbtree()
{
    BTREEP btree = (BTREEP) malloc(sizeof(BTREE));
    for (int i = 0; i < NUM - 1; i++)
    {
        btree->child[i] = NULL;
        btree->keys[i] = 0;
    }

    btree->child[i] = NULL;
    btree->parent = NULL;
    btree->index = 0;
    btree->numtrees = 0;
    printf("\n getbtree - A new BTree is created at: %p. \n", btree);
    return (btree);
}

void find()
{
    q = NULL;
    p = tree;
    while (p != NULL)
    {
        i = nodesearch(p, key);
        q = p;
        if (key == k(p, i))
        {
            found = TRUE;
            position = i;
            return (p);
        }
        p = p(child(i));
    }
    found = FALSE;
    position = i;
    return (q);
}

void insert()
{
    nd = s;
    pos = position;
    newnode = NULL;
    newrec = rec;
    newkey = key;
    f = parent(nd);
    while (f != NULL && numtrees(nd) == n)
    {
        split(nd, pos, newkey, newrec, newnode, nd2, midkey, midrec);
        newnode = nd2;
        pos = index(nd);
        nd = f;
        f = parent(nd);
        newkey = midkey;
        newrec = midrec;
    }
    if (numtrees(nd) < n)
    {
        insnode(nd, pos, newkey, newrecm newnode);
        return;
    }
    split(nd, pos, newkey, newrec, newnode, nd2, midkey, midrec);
    tree = maketree(midkey, midrec);
    child(tree, 0) = nd;
    child(tree, 1) = nd2;
}

void split()
{
    nd2 = getnode();
    if (pos > ndiv2)
    {
        copy(nd, ndiv2 + 1, n - 2, nd2);
        insnode(nd2, pos - ndiv2 - 1, newkey, newrec, newnode);
        numtrees(nd) = ndiv2 + 1;
        midkey = k(nd, ndiv2);
        midrec = r(nd, ndiv2);
        return;
    }

    if (pos == ndiv2)
    {
        copy(nd, ndiv2, n - 2, nd2);
        numtrees(nd) = ndiv2 + 1;
        child(nd2, 0) = newnode;
        midkey = newkey;
        midrec = newrec;
        return;
    }

    if (pos < ndiv2)
    {
        copy(nd, ndiv2 + 1, n - 2, nd2);
        numtrees(nd) = ndiv2;
        insnode(nd2, pos, newkey, newrec, newnode);
        midkey = k(nd, ndiv2 - 1);
        midrec = r(nd, ndiv2 - 1);
        child(nd2, 0) = child(nd, ndiv2 + 1);
        return;
    }
}

void insnode(nd, pos, newkey, newrec, newnode)
{
    for (i = numtrees(nd) - 1; i >= pos + 1; i--)
    {
        child(nd, i + 1) = child(nd, i);
        k(nd, i) = k(nd, i - 1);
        r(nd, i) = r(nd, i - 1);
    }
    child(nd, pos + 1) = newnode;
    k(nd, pos) = newkey;
    r(nd, pos) = newrec;
    numtrees(nd) += 1;
}

int main(int argc, char const *argv[])
{
    char cmd;

    // For file I/O
    FILE *fout;
    fout = fopen ("output.txt","w+");

    do
    {
        printf("Place your queen in a row [0-7, Q]:");
        scanf(" %c", &cmd);
        if (cmd != 'Q' || (cmd < '0' || cmd > '7'))
        {
            printf("Invalid. Please try again.\n");
        }
        else
        {
            if (cmd == 'Q')
                break;
            printf("Invalid. Please try again.\n");
        }
    }
    while (TRUE);

    fclose(fout);
   
    return 0;
}
