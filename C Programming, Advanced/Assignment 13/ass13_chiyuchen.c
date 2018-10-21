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
/*       Assignment Number: 13                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Twenty Six                            */
/*                                                            */
/*       File Name: ass13_chiyuchen.c                         */
/*                                                            */
/*       Date: 10/02/2018                                     */
/*                                                            */
/*       Objective: Binary Tree                               */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>

struct tree_el {
    int val;
    struct tree_el *left;
    struct tree_el *right;
};
typedef struct tree_el node;


/* global declarations */

/* function prototypes */
void insert(node **, node *);


void insert(node **tree, node *item)
{
    if (!(*tree))
    {
        *tree = item;
        return;
    }
    if (item->val < (*tree)->val)
    {
        insert(&(*tree)->left, item);
    }
    else if (item->val > (*tree)->val)
    {
        insert(&(*tree)->right, item);
    }
}

void printout(node *tree)
{
    if (tree->left)
    {
        printout(tree->left);
    }
    printf("%d\n", tree->val);
    if (tree->right)
    {
        printout(tree->right);
    }
}

void main()
{
    node *curr, *root;
    int i, root = NULL;
    for (i = 1; i <= 10; i++)
    {
        curr = (node *) malloc(sizeof(node));
        curr->left = curr->right = NULL;
        curr->val = rand();
        insert(&root, curr);
    }
    printout(root);
}



struct nodetype {
    int info;
    struct nodetype *left;
    struct nodetype *right;
};
typedef struct nodetype *NODEPTR;
typedef struct nodetype NODE;

NODEPTR maketree(int);
void intrac(NODEPTR);
NODEPTR get_treenode();
void setbintree(NODEPTR, int);

NODEPTR ndptr;
NODE nd;
NODEPTR current_ptr;
NODE current_nd;

void main()
{
    NODEPTR btree = NULL;
    in number;
    printf("please enter the next node info:");
    scanf("%d", &number);
    btree = maketree(number);
    printf("please enter the next node info:");
    while (scanf("%d", &number) != EOF)
    {
        setbintree(btree, number);
        printf("please enter the next node info:\n");
    }
    printf("\n\n inordered binary tree node info:\n");
    intrav(btree);
    printf("\n end of inordered binary tree node search\n");
    return;
}

void intrav(NODEPTR tree)
{
    if (tree != NULL)
    {
        intrav(tree->left);
        printf("%d \n", tree->info);
        intrav(tree->right);
    }
    return;
}

NODEPTR get_treenode()
{
    NODEPTR p;
    p = (NODEPTR) malloc(sizeof(struct nodetype));
    return (p);
}

NODEPTR maketree(int x)
{
    NODEPTR p;
    p = get_treenode();
    p->info = x;
    p->left = NULL;
    p->right = NULL;
    return (p);
}

void setbintree(NODEPTR p, int x)
{
    NODEPTR s = p;
    while (s != NULL)
    {
        if (s->info == x)
        {
            printf("\n duplicate entry \n");
            return;
        }

        if (s->info > x)
        {
            if (s->left == NULL)
            {
                s->left = maketree(x);
                return;
            }
            else
            {
                s = s->left;
            }
        }
        else
        {
            if (s->right == NULL)
            {
                s->right = maketree(x);
                return;
            }
            else
            {
                s = s->right;
            }
        }
    }
    return;
}











    p = tree;
    q = NULL;

    while (p != NULL && k(p) != key)
    {
        q = p;
        p = (key < k(p)) ? left(p) : right(p);
    }
    if (p == NULL)
        return;

    if (left(p) == NULL)
    {
        rp = right(p);
    }
    else
    {
        if (right(p) == NULL)
        {
            rp = left(p);
        }
        else
        {
            f = p;
            rp = right(p);
            s = left(rp);
            while (s != NULL)
            {
                f = rp;
                rp = s;
                s = left(rp);
            }
            if (f != p)
            {
                right(rp) == right(p);
            }
            left(rp) = left(p);
        }

        if (q == NULL)
        {
            tree = rp;
        }
        else
        {
            p == left(q) ? left(q) = rp : right(q) = rp;
        }
        freenode(p);
    }
    return;

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
