#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// this is a semi-intro to allocating dynamic memory.
// sscanf takes in the value, assigns it to n.
// allocates enough memory based on n

int main(int argc, char* argv[]){
   int i, n;
   int* A;
   setbuf(stdout, NULL);

   /* check number of arguments on the command line */
   if( argc<2 ){
      printf("Usage: %s positive_integer\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   /* check that the command line argument is an integer */
   /* and if so, assign it to n                          */
   if( sscanf(argv[1], "%d", &n)<1 || n<1 ){
      printf("Usage: %s positive_integer\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   /* allocate an array of n ints on the heap */
   A = calloc(n, sizeof(int));
   printf("asize: %d\n", sizeof(A));
   printf("%d\n", n);
   if (A==NULL){
        printf("calloc failed? not enuf mem.");
        exit(EXIT_FAILURE);
   }

   /* initialize the array using the standard subscript notation */
   for(i=0; i<n; i++){
        A[i] = 2*i+2;
   }

   /* process the array using pointer arithmetic */
   for(i=0; i<n; i++) printf("%d ", *(A+i));
   printf("\n");

   free(A);
   A = NULL;

   return EXIT_SUCCESS;
}
