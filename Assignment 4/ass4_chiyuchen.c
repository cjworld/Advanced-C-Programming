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
/*       Assignment Number: 4                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Ten                                   */
/*                                                            */
/*       File Name: ass4_chiyuchen.c                          */
/*                                                            */
/*       Date: 09/19/2018                                     */
/*                                                            */
/*       Objective: Read in scores from files, show           */
/*                  scores, average, min, and max of          */
/*                  each student, and output to file          */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NUMOFSTUDENT 5

/* function prototypes */
int get_student_id(void);
void get_statistic(int *, int, int *, int *, int *);
void str2array(char *, int, int **, int *);

/* Generate a new student ID. Return the student ID. */
int get_student_id(void) 
{
    static int id = 1000;
    id++;
    return id;
}

/* Get the maximum, minimum, and average value. */
void get_statistic(int *arr, int len, int *avgp, int *maxp, int *minp) 
{
    int i;

    // Count the total scores, keep track of max and min
    int max = 0, min = 100, total = 0;
    for (i = 0; i < len; i++) {
        total += arr[i];
        if (arr[i] < min) {
            min = arr[i];
        }
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    // Return 
    *avgp = total/len;
    *maxp = max;
    *minp = min;
    return;
}

/* Split the given string into array of integers by comma. */
void str2array(char *line, int len, int **arrpp, int *arrlenp)
{
    int *arr = NULL;
    int tok_cnt = 1, i, j = 0, prev = 0;

    // Count how many scores
    for (i = 0; i < len; i++) {
        if (line[i] == ',') {
            tok_cnt++;
        }
    }
    
    // Malloc an array based on the number of tokens and store the values.
    arr = malloc(tok_cnt * sizeof(int));
    for (i = 0; i < len; i++) {
        if (line[i] == ',') {
            line[i] = '\0';
            arr[j++] = atoi(&line[prev]);
            prev = i + 1;
        }
    }
    arr[j++] = atoi(&line[prev]);
    prev = i + 1;

    // Return 
    *arrpp = arr;
    *arrlenp = tok_cnt;
    return;
}

int main(int argc, char const *argv[])
{
    // users command
    char cmd[10];
    int *scores, scores_len, scores_per_student, i, j, max, min, avg;

    // For file I/O
    FILE *fin, *fout;
	char *line = NULL;
    size_t len = 0;
	ssize_t sz;

    fin = fopen("input.txt", "r");
    sz = getline(&line, &len, fin);
    str2array(line, sz, &scores, &scores_len);
    scores_per_student = scores_len / NUMOFSTUDENT;
    free(line);
    fclose(fin);

    fout = fopen ("output.txt","w+");
    for (j = 0; j < NUMOFSTUDENT; j++)
    {
		fprintf(fout, "The %dth student: \n", get_student_id());
        get_statistic(scores + j * scores_per_student, scores_per_student, &avg, &max, &min);
        fprintf(fout, "\tAverge score: %d, Maximum: %d, Minimum: %d\n", avg, max, min);
		fprintf(fout, "\tScores: ");
        for(i = j * scores_per_student; i < (j + 1) * scores_per_student && i < scores_len; i++) {
            if (scores[i] > 95) {
                fprintf(fout, "A+");
            } else if (scores[i] == 95) {
                fprintf(fout, "A");
            } else if (scores[i] >= 90) {
                fprintf(fout, "A-");
            } else if (scores[i] >= 85) {
                fprintf(fout, "B+");
            } else if (scores[i] == 85) {
                fprintf(fout, "B");
            } else if (scores[i] >= 80) {
                fprintf(fout, "B-");
            } else if (scores[i] >= 75) {
                fprintf(fout, "C+");
            } else if (scores[i] == 75) {
                fprintf(fout, "C");
            } else if (scores[i] >= 70) {
                fprintf(fout, "C-");
            } else if (scores[i] >= 60) {
                fprintf(fout, "D");
            } else {
                fprintf(fout, "F");
            }
            if (i != (j + 1) * scores_per_student - 1 && i != scores_len - 1) {
                fprintf(fout, ", ");
            }
        }
        fprintf(fout, "\n");
	}
    get_statistic(scores, scores_len, &avg, &max, &min);
    fprintf(fout, "Overall averge score: %d, Maximum: %d, Minimum: %d\n", avg, max, min);
    fclose(fout);
   
    return 0;
}
