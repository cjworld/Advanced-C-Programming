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
/*       Date: 10/27/2018                                     */
/*                                                            */
/*       Objective: Binary Tree                               */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXSIZE 100
#define EMPTYSTACK -1
#define TRUE 1
#define FALSE 0

typedef struct nodetype {
    int info;
    struct nodetype *left;
    struct nodetype *right;
} NODE;
typedef NODE * NODEPTR;

typedef struct intstack {
    int elements[MAXSIZE];
    int top;
} INTSTACK;

typedef struct nodestack {
    NODEPTR elements[MAXSIZE];
    int top;
} NODESTACK;


/* global declarations */

/* function prototypes */
void isdump_tailfirst(INTSTACK *, FILE *);
void isdump_headfirst(INTSTACK *, FILE *);
int nsempty(NODESTACK *);
int isempty(INTSTACK *);
NODEPTR nspop(NODESTACK *);
int ispop(INTSTACK *);
void nspush(NODESTACK *, NODE *);
void ispush(INTSTACK *, int);
NODEPTR get_treenode();
NODEPTR maketree(int);
void freenode(NODEPTR);
int insertnode(NODEPTR, int);
int deletenode(NODEPTR *, int);
void pretravrec(NODEPTR, FILE *);
void pretravwl(NODEPTR, FILE *);
void intravrec(NODEPTR, FILE *);
void intravwl(NODEPTR, FILE *);
void posttravrec(NODEPTR, FILE *);
void posttravwl(NODEPTR, FILE *);


/* Print out the all element from stack in order */
void isdump_tailfirst(INTSTACK *ps, FILE *fout)
{
    int ptr = 0;
    while (ptr < ps->top)
    {
        fprintf(fout, "%d ", ps->elements[ptr]);
        ptr++;
    }
}

/* Print out the all element from stack in reversed order */
void isdump_headfirst(INTSTACK *ps, FILE *fout)
{
    int ptr = ps->top - 1;
    while (ptr >= 0)
    {
        fprintf(fout, "%d ", ps->elements[ptr]);
        ptr--;
    }
}

/* Verify if the btnode stack is empty. Return TRUE or FALSE */
int nsempty(NODESTACK *ps)
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

/* Verify if the int stack is empty. Return TRUE or FALSE */
int isempty(INTSTACK *ps)
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

/* Remove the last element from btnode stack. Return the removed element. */
NODEPTR nspop(NODESTACK *ps)
{
    if (nsempty(ps))
    {
        return NULL;
    }
    return ps->elements[--(ps->top)];
}

/* Remove the last element from int stack. Return the removed element. */
int ispop(INTSTACK *ps)
{
    if (isempty(ps))
    {
        printf("%s", "stack underflow");
        exit(1);
    }
    return ps->elements[--(ps->top)];
}

/* Insert the element to btnode stack. */
void nspush(NODESTACK *ps, NODE *ptr)
{
    if (ps->top == MAXSIZE)
    {
        printf("%s", "stack overflow");
        exit(1);
    }
    else
    {
        ps->elements[ps->top] = ptr;
        (ps->top)++;
    }
    return;
}

/* Insert the element to int stack. */
void ispush(INTSTACK *ps, int val)
{
    if (ps->top == MAXSIZE)
    {
        printf("%s", "stack overflow");
        exit(1);
    }
    else
    {
        ps->elements[ps->top] = val;
        (ps->top)++;
    }
    return;
}

/* Create a new btnode */
NODEPTR get_treenode()
{
    NODEPTR p;
    p = (NODEPTR) malloc(sizeof(struct nodetype));
    return (p);
}

/* Create a new btree */
NODEPTR maketree(int x)
{
    NODEPTR p;
    p = get_treenode();
    p->info = x;
    p->left = NULL;
    p->right = NULL;
    return (p);
}

/* Free a btnode */
void freenode(NODEPTR p)
{
    free(p);
}

/* Insert a node with given value to a btree. */
int insertnode(NODEPTR p, int x)
{
    NODEPTR s = p;
    while (s != NULL)
    {
        if (s->info == x)
        {
            return 1;
        }

        if (s->info > x)
        {
            if (s->left == NULL)
            {
                s->left = maketree(x);
                return 0;
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
                return 0;
            }
            else
            {
                s = s->right;
            }
        }
    }
    return 1;
}

/* Delete a node from a btree. */
int deletenode(NODEPTR *tree, int key)
{
    NODEPTR p = *tree;
    NODEPTR q = NULL, rp = NULL, f = NULL, s = NULL;

    // Search the node
    while (p != NULL && p->info != key)
    {
        q = p;
        p = (key < p->info) ? p->left : p->right;
    }
    
    // If not found.
    if (p == NULL)
    {
        return 1;
    }
    
    // If found.
    if (p->left == NULL)
    {
        // Only one child, assign the replacing node is the child.
        rp = p->right;
    }
    else if (p->right == NULL)
    {
        // Only one child, assign the replacing node is the child.
        rp = p->left;
    }
    else
    {
        // Find the leftmost node in the right subtree.
        f = p;
        rp = p->right;
        s = rp->left;
        while (s != NULL)
        {
            f = rp;
            rp = s;
            s = rp->left;
        }

        // The right subtree of the replacing node has to connect to the left side of its parent.
        // The replacing node has to connect the right subtree of the deleted node.
        if (f != p)
        {
            f->left = rp->right;
            rp->right = p->right;
        }

        // The replacing node has to connect the left subtree of the deleted node.
        rp->left = p->left;
    }

    // Replace the deleted node with the replacing node
    if (q == NULL)
    {
        *tree = rp;
    }
    else
    {
        if (p == q->left)
            q->left = rp;
        else
            q->right = rp;
    }
    freenode(p);
    return 0;
}

/* Print out the tree in preorder by recursive function. */
void pretravrec(NODEPTR tree, FILE *fout)
{
    if (tree != NULL)
    {
        fprintf(fout, "%d ", tree->info);
        pretravrec(tree->left, fout);
        pretravrec(tree->right, fout);
    }
    return;
}

/* Print out the tree in preorder by while loop. */
void pretravwl(NODEPTR tree, FILE *fout)
{
    INTSTACK istack = {0};
    NODESTACK nstack = {0};
    NODEPTR p = tree;
    while (p != NULL)
    {
        // printf("push data %d to stack\n", p->info);
        ispush(&istack, p->info);
        if (p->right)
        {
            // printf("push pointer %p to stack\n", p->right);
            nspush(&nstack, p->right);
        }
        if (p->left)
        {
            // printf("push pointer %p to stack\n", p->left);
            nspush(&nstack, p->left);
        }
        p = nspop(&nstack);
        // printf("pop pointer %p from stack\n", p);
    }
    isdump_tailfirst(&istack, fout);
    return;
}

/* Print out the tree in inorder by recursive function. */
void intravrec(NODEPTR tree, FILE *fout)
{
    if (tree != NULL)
    {
        intravrec(tree->left, fout);
        fprintf(fout, "%d ", tree->info);
        intravrec(tree->right, fout);
    }
    return;
}

/* Print out the tree in inorder by while loop. */
void intravwl(NODEPTR tree, FILE *fout)
{
    int goleft = TRUE;
    INTSTACK istack = {0};
    NODESTACK nstack = {0};
    NODEPTR p = tree;
    while (p != NULL)
    {
        if (goleft)
        {
            while (p->left != NULL)
            {
                // printf("push node %d to stack\n", p->info);
                nspush(&nstack, p);
                p = p->left;
            }
        }
        // printf("push data %d to stack\n", p->info);
        ispush(&istack, p->info);
        if (p->right)
        {
            // printf("go to right pointer of node %d\n", p->info);
            p = p->right;
            goleft = TRUE;
        }
        else
        {
            p = nspop(&nstack);
            // if (p)
            //     printf("pop node %d from stack\n", p->info);
            goleft = FALSE;
        }
    }
    isdump_tailfirst(&istack, fout);
    return;
}

/* Print out the tree in postorder by recursive function. */
void posttravrec(NODEPTR tree, FILE *fout)
{
    if (tree != NULL)
    {
        posttravrec(tree->left, fout);
        posttravrec(tree->right, fout);
        fprintf(fout, "%d ", tree->info);
    }
    return;
}

/* Print out the tree in postorder by while loop. */
void posttravwl(NODEPTR tree, FILE *fout)
{
    INTSTACK istack = {0};
    NODESTACK nstack = {0};
    NODEPTR p = tree;
    while (p != NULL)
    {
        // printf("push data %d to stack\n", p->info);
        ispush(&istack, p->info);
        if (p->left)
        {
            // printf("push pointer %p to stack\n", p->left);
            nspush(&nstack, p->left);
        }
        if (p->right)
        {
            // printf("push pointer %p to stack\n", p->right);
            nspush(&nstack, p->right);
        }
        p = nspop(&nstack);
        // printf("pop pointer %p from stack\n", p);
    }
    isdump_headfirst(&istack, fout);
    return;
}

int main(int argc, char const *argv[])
{
    NODEPTR tree = NULL;
    int i, isnum, value;
    char *startp;
    char input[10];

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
    fprintf(fout, "Input: %s\n", data);

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
                if (tree == NULL)
                    tree = maketree(atoi(startp));
                else
                {
                    if (insertnode(tree, atoi(startp)) == 1)
                    {
                        printf("duplicate entry: %d \n", atoi(startp));
                        fprintf(fout, "duplicate entry: %d \n", atoi(startp));
                    }
                }   
                isnum = FALSE;
            }
        }
    }
    if (isnum == TRUE)
    {
        if (tree == NULL)
            maketree(atoi(startp));
        else
        {
            if (insertnode(tree, atoi(startp)) == 1)
            {
                printf("duplicate entry: %d \n", atoi(startp));
                fprintf(fout, "duplicate entry: %d \n", atoi(startp));
            }
        }
        isnum = FALSE;
    }
    free(data);
	fclose(fin);

    // Preorder
    fprintf(fout, "Preorder traversal by recursive calls: ");
    pretravrec(tree, fout);
    fprintf(fout, "\n");

    fprintf(fout, "Preorder traversal by while loop: ");
    pretravwl(tree, fout);
    fprintf(fout, "\n");

    // Inorder
    fprintf(fout, "Inorder traversal by recursive calls: ");
    intravrec(tree, fout);
    fprintf(fout, "\n");

    fprintf(fout, "Inorder traversal by while loop: ");
    intravwl(tree, fout);
    fprintf(fout, "\n");

    // Postorder
    fprintf(fout, "Postorder traversal by recursive calls: ");
    posttravrec(tree, fout);
    fprintf(fout, "\n");

    fprintf(fout, "Postorder traversal by while loop: ");
    posttravwl(tree, fout);
    fprintf(fout, "\n");

    // Prompt user to delete node
    while (TRUE)
    {
        printf("Any node to delete: ");
        scanf("%s", input);
        fprintf(fout, "Any node to delete: %s\n", input);
        if (strcmp(input, "Q") == 0)
            break;
        
        value = atoi(input);
        if (deletenode(&tree, value) == 0)
        {
            fprintf(fout, "The node %d is deleted. The tree after deletion: ", value);
            intravrec(tree, fout);
            fprintf(fout, "\n");
        }
        else 
        {
            printf("The node %d is not found.\n", value);
            fprintf(fout, "The node %d is not found.\n", value);
        }
    }

    // Cleanup
    fclose(fout);
    return 0;
}


