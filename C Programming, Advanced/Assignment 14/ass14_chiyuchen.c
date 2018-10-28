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
/*       Date: 10/28/2018                                     */
/*                                                            */
/*       Objective: BTree Traversal                           */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0
#define NUM 5

typedef struct btreenode {
    int numtrees;
    struct btreenode * child[NUM];
    int keys[NUM - 1];
    struct btreenode * parent;
    int index;
} BTREE;
typedef BTREE * BTREEP;

/* global declarations */

/* function prototypes */
void dumptree(BTREEP, int, char *, FILE *);
BTREEP getbtree(void);
BTREEP makebtree(int);
int nodesearch(BTREEP, int);
int find(BTREEP, int, BTREEP *, int *);
void insert(int, BTREEP, int, BTREEP *, FILE *);
void copy(BTREEP, int, int, BTREEP);
void split(BTREEP, int, int, BTREEP, BTREEP *, int *, FILE *);
void insnode(BTREEP, int, int, BTREEP, FILE *);

void dumptree(BTREEP tree, int deep, char *prefix, FILE *fout)
{
    int i, j;
    for (i = 0; i < tree->numtrees; i++)
    {
        if (tree->child[i] != NULL)
            dumptree(tree->child[i], deep + 1, prefix, fout);
        fprintf(fout, "%s", prefix);
        for (j = 0; j < deep - 1; j++)
            fprintf(fout, "|   ");
        if (deep > 0)
            fprintf(fout, "|---");
        fprintf(fout, "%03d at %d at level %d\n", tree->keys[i], i, deep);
    }
    if (tree->child[i] != NULL)
        dumptree(tree->child[i], deep + 1, prefix, fout);
}

/* Create a new btree structure. */
BTREEP getbtree()
{
    int i;
    BTREEP btree = (BTREEP) malloc(sizeof(BTREE));
    for (i = 0; i < NUM - 1; i++)
    {
        btree->child[i] = NULL;
        btree->keys[i] = 0;
    }
    btree->child[i] = NULL;
    btree->parent = NULL;
    btree->index = 0;
    btree->numtrees = 0;
    return (btree);
}

/* Create a new tree. */
BTREEP makebtree(int key)
{
    BTREEP btree = getbtree();
    btree->keys[0] = key;
    btree->numtrees++;
    return (btree);
}

/* Find a key in the given node. */
int nodesearch(BTREEP nd, int key)
{
    int i;
    for (i = 0; i < nd->numtrees; i++)
    {
        if (nd->keys[i] == key)
            return i;

        if (nd->keys[i] > key)
            break;
    }
    return i;
}

/* Find a key in the given tree. */
int find(BTREEP tree, int key, BTREEP *nodep, int *positionp)
{
    int i;
    BTREEP q = NULL, p = tree;
    while (p != NULL)
    {
        i = nodesearch(p, key);
        q = p;
        if (i < p->numtrees && key == p->keys[i])
        {
            *positionp = i;
            *nodep = p;
            return TRUE;
        }
        p = p->child[i];
    }
    *positionp = i;
    *nodep = q;
    return FALSE;
}

/* Add a new key at the given position of the given node. */
void insert(int newkey, BTREEP nd, int newpos, BTREEP *treep, FILE *fout)
{
    // Try to add a key to the node. If the node is full after addition,
    // find out the middle key and add this key to the parents of the node.
    // If the parent node is full, keep doing this util there is no parent
    // node or there is a vacancy in the node.

    fprintf(fout, ">> Insert key %d to position %d of node starts with key %d \n", newkey, newpos, nd->keys[0]);

    int midkey;
    BTREEP f = nd->parent, midnd, newnode = NULL;
    while (f != NULL && nd->numtrees == NUM - 1)
    {
        // Add the new key to the node at the given position.
        split(nd, newpos, newkey, newnode, &midnd, &midkey, fout);

        // After adding, push up the middle key and add it to
        // the parent node.
        newnode = midnd;
        newkey = midkey;
        newpos = nd->index;
        nd = f;
        f = nd->parent;
    }

    if (nd->numtrees < NUM - 1)
    {
        // If there is a vacancy in the node.
        insnode(nd, newpos, newkey, newnode, fout);
        return;
    }
    else
    {
        // If there is no vacancy in the node and no parent node
        // exists, we need to create a new parent node in order
        // to place it.
        split(nd, newpos, newkey, newnode, &midnd, &midkey, fout);
        fprintf(fout, ">>>> Create a new root node for key %d\n", midkey);
        *treep = makebtree(midkey);
        (*treep)->child[0] = nd;
        (*treep)->child[1] = midnd;
        if (nd)
        {
            nd->parent = *treep;
            nd->index = 0;
        }
        if (midnd)
        {
            midnd->parent = *treep;
            midnd->index = 1;
        }
        return;
    }
}

/* Copy the keys and childs from node to node. */
void copy(BTREEP nd, int first, int last, BTREEP nd2)
{
    int i;
    for (i = first; i <= last; i++)
    {
        nd2->keys[i - first] = nd->keys[i];
        nd2->child[i - first] = nd->child[i];
        if (nd2->child[i - first])
        {
            nd2->child[i - first]->parent = nd2;
            nd2->child[i - first]->index = i - first;
        }
    }
    nd2->child[i - first] = nd->child[i];
    if (nd2->child[i - first])
    {
        nd2->child[i - first]->parent = nd2;
        nd2->child[i - first]->index = i - first;
    }
}

/* Insert the key by splitting the target node into two nodes. Return the right side node */
void split(BTREEP nd, int pos, int newkey, BTREEP newnode, BTREEP *midndp, int *midkeyp, FILE *fout)
{
    int ndiv2 = NUM / 2;
    *midndp = getbtree();
    (*midndp)->parent = nd->parent;

    fprintf(fout, ">>>> Split node starts with key %d. \n", nd->keys[0]);

    // If the position is in the second half, splie the node into
    // two nodes, and connect the new splitted node to the 
    // given newnode.
    if (pos > ndiv2)
    {
        copy(nd, ndiv2 + 1, NUM - 2, *midndp);
        (*midndp)->numtrees = NUM - ndiv2 - 2;
        *midkeyp = nd->keys[ndiv2];
        nd->numtrees = ndiv2;
        insnode(*midndp, pos - ndiv2 - 1, newkey, newnode, fout);
        fprintf(fout, ">>>> Key %d left to be insert\n", *midkeyp);
        return;
    }

    // If the position is in the middle, splie the node into
    // two nodes, and connect the new splitted node to the 
    // given newnode.
    if (pos == ndiv2)
    {
        copy(nd, ndiv2, NUM - 2, *midndp);
        (*midndp)->numtrees = NUM - ndiv2 - 1;
        *midkeyp = newkey;
        nd->numtrees = ndiv2;
        (*midndp)->child[0] = newnode;
        if (newnode)
        {
            newnode->parent = (*midndp);
            newnode->index = 0;
        }
        fprintf(fout, ">>>> Key %d left to be insert\n", *midkeyp);
        return;
    }

    if (pos < ndiv2)
    {
        copy(nd, ndiv2, NUM - 2, *midndp);
        (*midndp)->numtrees = NUM - ndiv2 - 1;
        *midkeyp = nd->keys[ndiv2 - 1];
        nd->numtrees = ndiv2 - 1;
        insnode(nd, pos, newkey, newnode, fout);
        fprintf(fout, ">>>> Key %d left to be insert\n", *midkeyp);
        return;
    }
}

/* Insert the given newkey along with the subtree to the given position */
void insnode(BTREEP nd, int pos, int newkey, BTREEP newnode, FILE *fout)
{
    int i;

    // Shift the elements from position #pos to #numtrees by 1
    for (i = nd->numtrees ; i >= pos + 1; i--)
    {
        fprintf(fout, ">>>> Shift key %d from pos %d to pos %d \n", nd->keys[i - 1], i - 1, i);
        nd->child[i + 1] = nd->child[i];
        if (nd->child[i + 1])
        {
            nd->child[i + 1]->parent = nd;
            nd->child[i + 1]->index = i + 1;
        }
        nd->keys[i] = nd->keys[i - 1];
    }

    // Place the new node at position #pos
    fprintf(fout, ">>>> Place key %d at pos %d: \n", newkey, pos);
    nd->child[pos + 1] = newnode;
    nd->keys[pos] = newkey;
    nd->numtrees += 1;
    if (nd->child[pos + 1])
    {
        nd->child[pos + 1]->parent = nd;
        nd->child[pos + 1]->index = pos + 1;
    }
}

int main(int argc, char const *argv[])
{
    BTREEP tree, nd;
    int i, isnum, newpos, value;
    char *startp;
    char input[10], kerker;

    // For file I/O
    FILE *fin, *fout;
    char *data = NULL;
    size_t len;
	ssize_t bytes;

    // Read in
    fout = fopen(argv[2],"w+");
    fin = fopen(argv[1], "r");
    bytes = getline(&data, &len, fin);
    data[bytes] = '\0';
    fprintf(fout, "Input: %s\n\n", data);

    // Parse the input and build a tree.
    isnum = FALSE;
    startp = NULL;
    for (i = 0; i < bytes; i++)
    {
        if (data[i] >= '0' && data[i] <= '9')
        {
            if (isnum == FALSE)
            {
                startp = &(data[i]);
                isnum = TRUE;
            }
        }
        else
        {
            if (isnum == TRUE)
            {
                data[i] = '\0';
                value = atoi(startp);
                if (tree == NULL)
                {
                    fprintf(fout, "Create a tree for key %d \n", value);
                    tree = makebtree(value);
                }
                else
                {
                    if (find(tree, value, &nd, &newpos) == TRUE)
                    {
                        printf("duplicate entry: %d \n", value);
                        fprintf(fout, "duplicate entry: %d \n", value);
                    }
                    else
                    {
                        fprintf(fout, "Insert key %d to the tree.\n", value);
                        insert(value, nd, newpos, &tree, fout);
                    }
                }
                fprintf(fout, "The final tree: \n");
                dumptree(tree, 0, "", fout);
                fprintf(fout, "\n\n");
                isnum = FALSE;
            }
        }
    }
    if (isnum == TRUE)
    {
        value = atoi(startp);
        if (tree == NULL)
        {
            fprintf(fout, "Create a tree for key %d \n", value);
            tree = makebtree(value);
        }
        else
        {
            if (find(tree, value, &nd, &newpos) == TRUE)
            {
                printf("duplicate entry: %d \n", value);
                fprintf(fout, "duplicate entry: %d \n", value);
            }
            else
            {
                fprintf(fout, "Insert key %d to the tree.\n", value);
                insert(value, nd, newpos, &tree, fout);
            }
        }
        fprintf(fout, "The final tree: \n");
        dumptree(tree, 0, "", fout);
        fprintf(fout, "\n\n");
        isnum = FALSE;
    }
    free(data);
	fclose(fin);

    fclose(fout);
    return 0;
}
