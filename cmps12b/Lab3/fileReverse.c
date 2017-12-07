/*
     Name: Johnson Le
     cruzID: jonle
     id number: 1512346
     File Name: fileReverse.c
     Description: takes tokens and reverses it. ie abc-> cba.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// char test[]="abcdefghijklmnopqrstuvwxyz";
char string[256];
void stringReverse(char* s);

int main(int argc, char* argv[]){
     // printf("normal: %s", test);
     // stringReverse(test);
     //declaring the files.
     FILE* inputFile;
     FILE* outputFile;
     //following code is provided code from assignment
     /* check command line for correct number of arguments */
     if( argc != 3 ){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
     }
     /* open input file for reading */
     inputFile = fopen(argv[1], "r");
     if( inputFile==NULL ){
        printf("Unable to read from file %s\n", argv[1]);
        exit(EXIT_FAILURE);
     }

     /* open output file for writing */
     outputFile = fopen(argv[2], "w");
     if( outputFile==NULL ){
        printf("Unable to write to file %s\n", argv[2]);
        exit(EXIT_FAILURE);
     }
     //following is changed a bit to reverse.
     while( fscanf(inputFile, " %s", string) != EOF ){//eof = end of file
          stringReverse(string);
          fprintf(outputFile, "%s\n", string);
          // printf("%s\n", string);
     }


     return(EXIT_SUCCESS);
}

void stringReverse(char* s){
     //because there is no return type,
     //have to work with pointers.

     char *first = s;                   //points to the first address in s
     char *last = first+strlen(s)-1;    //points to the last
     char temp;                         //will recycle

     while(last>first){                 //basically swapping
          temp = *first;                //temp=first char
          *first = *last;               //now first char replaced by last
          *last = temp;                 //last char is temp=first char
          first=first+1;
          last = last-1;
     }
     // printf("\nreverse: %s", s);
}
