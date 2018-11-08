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
/*       Date: 11/08/2018                                     */
/*                                                            */
/*       Objective: Implement all sorting methods             */
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

/* global declarations */

/* function prototypes */
void printarray(int [], int, FILE *);
void swap(int *, int *);
void bubble_sort(int [], int);
void insertion_sort(int [], int);
void selection_sort(int [], int);
void shell_sort(int [], int);

int partition(int [], int, int);
void quick_sort(int [], int, int, int);
void heapify(int [], int, int);
void heap_sort(int [], int);
void merge(int [], int, int, int);
void merge_sort(int [], int, int, int);

void duplicate(int [], int [], int);

/* Print out the array. */
void printarray(int array[], int length, FILE *fout)
{
    int i, j, tmp;
    for (i = 0; i < length; i++)
    {
        fprintf(fout, "%d, ", array[i]);
    }
    fprintf(fout, "\n");
}

/* Swap the values of two integer. */
void swap(int *a, int *b)
{
    int tmp;
    tmp = *b;
    *b = *a;
    *a = tmp;
}

/* sort an array of integers by bubble sort. */
void bubble_sort(int array[], int length)
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
        // printarray(array, length);
    }
}

/* sort an array of integers by insertion sort. */
void insertion_sort(int array[], int length)
{
    int i, j;
    for (i = 1; i < length; i++)
    {
        j = i;
        while (j > 0 && array[j - 1] > array[j])
        {
            swap(&array[j - 1], &array[j]);
            j--;
        }
        // printarray(array, length);
    }
}

/* sort an array of integers by selection sort. */
void selection_sort(int array[], int length)
{
    int i, j, min, min_idx;
    for (i = 0; i < length - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < length; j++)
        {
            if (array[min_idx] > array[j])
            {
                min_idx = j;
            }
        }
        swap(&array[i], &array[min_idx]);
        // printarray(array, length);
    }
}

/* sort an array of integers by shell sort. */
void shell_sort(int array[], int length)
{
    int i, j, gap;
    for (gap = length / 2; gap > 0; gap /= 2)
    {
        for (j = gap; j < length; j++)
        {
            for (i = j; i - gap >= 0 && array[i - gap] > array[i]; i -= gap)
            {
                swap(&array[i], &array[i - gap]);
            }
        }
        // printarray(array, length);
    }
}

/* Split an array into two subsets. */
int partition(int array[], int from, int to)
{
    int j;
    int pos = from;
    int pivot = array[to];
    for (j = from; j < to; j++)
    {
        if (array[j] <= pivot)
        {
            swap(&array[pos], &array[j]);
            pos++;
        }
    }
    swap(&array[pos], &array[to]);
    return pos;
}

/* sort an array of integers by quick sort. */
void quick_sort(int array[], int from, int to, int length)
{
    int pi;
    if (from < to)
    {
        pi = partition(array, from, to);
        // printarray(array + from, pi - 1 - from + 1);
        // printarray(array + pi + 1, to - pi - 1 + 1);
        quick_sort(array, from, pi - 1, length);
        quick_sort(array, pi + 1, to, length);
    }
}

/* Build up the heap tree in array. */
void heapify(int array[], int root, int length) 
{ 
    int largest = root; // Initialize largest as root 
    int left = 2 * root + 1; // left = 2*i + 1 
    int right = 2 * root + 2; // right = 2*i + 2 
  
    if (left < length && array[left] > array[largest]) 
        largest = left; 
  
    if (right < length && array[right] > array[largest]) 
        largest = right; 
  
    if (largest != root)
    { 
        swap(&array[root], &array[largest]); 
        heapify(array, largest, length); 
    } 
}

/* sort an array of integers by heap sort. */
void heap_sort(int array[], int length) 
{ 
    int i;
    for (i = length / 2 - 1; i >= 0; i--)
    {
        heapify(array, i, length); 
        // printarray(array, length);
    }
  
    for (i = length - 1; i >= 0; i--) 
    { 
        swap(&array[0], &array[i]);
        // printarray(array, length);
        heapify(array, 0, i);
    } 
} 

/* Merge the two subarrays for merge sort. */
void merge(int array[], int start, int mid, int end)
{
    int i, j, k;
    int n1 = mid - start + 1;
    int n2 = end - mid;
    int left[n1], right[n2];

    for (i = 0; i < n1; i++)
        left[i] = array[start + i];
    for (j = 0; j < n2; j++)
        right[j] = array[mid + 1 + j];

    i = 0;
    j = 0;
    k = start;

    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
        {
            array[k] = left[i];
            i++;
        }
        else
        {
            array[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        array[k] = left[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        array[k] = right[j];
        j++;
        k++;
    }
}

/* sort an array of integers by merge sort. */
void merge_sort(int array[], int start, int end, int length)
{
    int mid;
    if (start < end)
    {  
        mid = start + (end - start) / 2;
        // printarray(array + start, mid - start + 1);
        merge_sort(array, start, mid, length);
        // printarray(array + start, mid - start + 1);
        // printarray(array + mid + 1, end - mid - 1 + 1);
        merge_sort(array, mid + 1, end, length);
        // printarray(array + mid + 1, end - mid - 1 + 1);
        merge(array, start, mid, end);
    }
}

/* Copy an array of integers to another array of integers. */ 
void duplicate(int array[], int targetarray[], int length)
{
    int i = 0;
    for (i = 0; i < length; i++)
    {
        targetarray[i] = array[i];
    }
}


int main(int argc, char const *argv[])
{
    // To store the input data
    int cnt = 0, choice;
    int input[MAXSIZE] = {0};
    int test1[MAXSIZE], test2[MAXSIZE];

    // For file I/O
    int i, isnum;
    FILE *fin, *fout;
    char *data = NULL, *startp;
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
                input[cnt++] = atoi(startp);
                isnum = FALSE;
            }
        }
    }
    if (isnum == TRUE)
    {
        input[cnt++] = atoi(startp);
        isnum = FALSE;
    }
    free(data);
	fclose(fin);

    // Prompt user to select node
    duplicate(input, test1, cnt);
    duplicate(input, test2, cnt);

    while (1)
    {
        printf("The list of simple sorting methods:\n");
        printf("1) Bubble sort.\n");
        printf("2) Selection sort.\n");
        printf("3) Shell sort.\n");
        printf("4) Insertion sort.\n");
        fprintf(fout, "The list of simple sorting methods:\n");
        fprintf(fout, "1) Bubble sort.\n");
        fprintf(fout, "2) Selection sort.\n");
        fprintf(fout, "3) Shell sort.\n");
        fprintf(fout, "4) Insertion sort.\n");
        printf("Select a method to sort: ");
        scanf("%d", &choice);
        fprintf(fout, "Select a method to sort: %d\n", choice);
        if (1 <= choice && choice <= 4)
        {
            break;
        }
        else
        {
            printf("Invalid. Try again.\n");
            fprintf(fout, "Invalid. Try again.\n");
        }
    }
    fprintf(fout, "The array before sorting: ");
    printarray(test1, cnt, fout);
    if (choice == 1)
    {
        bubble_sort(test1, cnt);
    }
    else if (choice == 2)
    {
        selection_sort(test1, cnt);
    }
    else if (choice == 3)
    {
        shell_sort(test1, cnt);
    }
    else if (choice == 4)
    {
        insertion_sort(test1, cnt);
    }
    fprintf(fout, "The array after sorting: ");
    printarray(test1, cnt, fout);

    while (1)
    {
        printf("The list of advanced sorting methods:\n");
        printf("1) Quick sort.\n");
        printf("2) Heap sort.\n");
        printf("3) Merge sort.\n");
        fprintf(fout, "The list of advanced sorting methods:\n");
        fprintf(fout, "1) Quick sort.\n");
        fprintf(fout, "2) Heap sort.\n");
        fprintf(fout, "3) Merge sort.\n");
        printf("Select a method to sort: ");
        scanf("%d", &choice);
        fprintf(fout, "Select a method to sort: %d\n", choice);
        if (1 <= choice && choice <= 3)
        {
            break;
        }
        else
        {
            printf("Invalid. Try again.\n");
            fprintf(fout, "Invalid. Try again.\n");
        }
    }
    fprintf(fout, "The array before sorting: ");
    printarray(test2, cnt, fout);
    if (choice == 1)
    {
        quick_sort(test2, 0, cnt - 1, cnt);
    }
    else if (choice == 2)
    {
        heap_sort(test2, cnt);
    }   
    else if (choice == 3)
    {
        merge_sort(test2, 0, cnt - 1, cnt);
    }   
    fprintf(fout, "The array after sorting: ");
    printarray(test2, cnt, fout);

    // Cleanup
    fclose(fout);
    return 0;
}

