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
/*       Assignment Number: 15                                */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Thirty                                */
/*                                                            */
/*       File Name: ass15_chiyuchen.c                         */
/*                                                            */
/*       Date: 11/12/2018                                     */
/*                                                            */
/*       Objective: Hash Table                                */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BUCKETSIZE 15
#define KEYSIZE 20
#define ARRAYSIZE 100

typedef struct hashrecord {
    char key[KEYSIZE];
    int record;
    struct hashrecord *next;
} hashrecord;

typedef struct hashtable {
    hashrecord *buckets[BUCKETSIZE];
} hashtable;

/* global declarations */

/* function prototypes */
void hashbucket_dump(hashrecord *, FILE *);
void hashtable_dump(hashtable *, FILE *);
int get_hash_bucket(char *);
void hashtable_add_entry(hashtable *, char *, int, FILE *);

/* Print out the given hash bucket. */
void hashbucket_dump(hashrecord *hrecordp, FILE *fout)
{
    while (hrecordp != NULL) {
        fprintf(fout, "%s(%d) -> ", hrecordp->key, hrecordp->record);
        hrecordp = hrecordp->next;
    }
    fprintf(fout, "NULL\n");
}

/* Print out the given hash table. */
void hashtable_dump(hashtable *htable, FILE *fout)
{
    int bucket_index = 0;
    hashrecord *hrecordp = NULL;
    for (bucket_index = 0; bucket_index < BUCKETSIZE; bucket_index++) {
        hrecordp = htable->buckets[bucket_index];
        fprintf(fout, "Bucket %d: ", bucket_index);
        hashbucket_dump(hrecordp, fout);
    }
}

/* Calculate the hash key for the given key. */
int get_hash_bucket(char *key)
{
    char *tmp = key;
    int total = 0;
    while (*tmp != '\0') {
        total += *tmp;
        tmp++;
    }
    // printf("The hashkey for %s is %d.\n", key, (total % BUCKETSIZE));
    return (total % BUCKETSIZE);
}

/* Add a key-value to the given hash table. */
void hashtable_add_entry(hashtable *htable, char *key, int record, FILE *fout)
{
    int bucket_idx = get_hash_bucket(key);
    hashrecord *hrecord = malloc(sizeof(hashrecord));
    strcpy(hrecord->key, key);
    hrecord->record = record;
    hrecord->next = htable->buckets[bucket_idx];
    htable->buckets[bucket_idx] = hrecord;

    fprintf(fout, "Add %s to bucket %d\n", key, bucket_idx);
    fprintf(fout, "\t The bucket %d after updated: ", bucket_idx);
    hashbucket_dump(htable->buckets[bucket_idx], fout);
}

int main(int argc, char const *argv[])
{
    // To store the input data
    char key[KEYSIZE] = {0};
    int record = 0;
    hashtable htable = {0};

    // For file I/O
    FILE *fin, *fout;

    // Read in
    fout = fopen(argv[2],"w+");
    fin = fopen(argv[1], "r");
    while (fscanf(fin, "%d. %s", &record, key) != EOF) {
        // printf("%s: %d.\n", key, record);
        hashtable_add_entry(&htable, key, record, fout);
    }
    fprintf(fout, "The final hash table: \n");
    hashtable_dump(&htable, fout);
	fclose(fin);
    fclose(fout);
    return 0;
}
