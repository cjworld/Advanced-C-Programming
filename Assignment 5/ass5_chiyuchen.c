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
/*       Assignment Number: 5                                 */
/*                                                            */
/*                                                            */
/*       Topic: Chapter Ten                                   */
/*                                                            */
/*       File Name: ass5_chiyuchen.c                          */
/*                                                            */
/*       Date: 09/19/2018                                     */
/*                                                            */
/*       Objective: Print out the variable, pointer,          */
/*                  and dereference of pointers               */
/*                                                            */
/*       Comments:                                            */
/*                                                            */
/**************************************************************/

/* preprocessor directives */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    int vint = 25;
    float vfloat = 3.142;
    double vdouble = 1.73217;
    char *vstr = "Hello, C Programmer";
    char vchar = 'x';

    int *vintp = &vint;
    float *vfloatp = &vfloat;
    double *vdoublep = &vdouble;
    char **vstrp = &vstr;
    char *vcharp = &vchar;

    int **vintpp = &vintp;

    printf("Int: %d, Address of Int: %p, Address of Int Pointer: %p, Dereference the Int Pointer: %d\n", vint, &vint, &vintp, *vintp); 
    printf("Float: %f, Address of Float: %p, Address of Float Pointer: %p, Dereference the Float Pointer: %f\n", vfloat, &vfloat, &vfloatp, *vfloatp);
    printf("Double: %lf, Address of Double: %p, Address of Double Pointer: %p, Dereference the Double Pointer: %lf\n", vdouble, &vdouble, &vdoublep, *vdoublep);
    printf("String: %s, Address of String: %p, Address of String Pointer: %p, Dereference the String Pointer: %s\n", vstr, &vstr, &vstrp, *vstrp);
    printf("Char: %c, Address of Char: %p, Address of Char Pointer: %p, Dereference the Char Pointer: %c\n", vchar, &vchar, &vcharp, *vcharp);

    printf("Dereference the Pointer of Int Pointer: %d\n", *(*vintpp));

    return 0;
}
