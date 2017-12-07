#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>

#include "queue.h"

char string[256];

int main(int argc, char* argv[]){
     // char* string = calloc(16, sizeof(int));
     // test1();
     // printf("hi");
     FILE* inputFile;
     FILE* outputFile;

     Queue myQueue;
     queueInit(&myQueue);

     if( argc != 3 ){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
     }

     inputFile = fopen(argv[1], "r");
     if( inputFile==NULL ){
        printf("Unable to read from file %s\n", argv[1]);
        exit(EXIT_FAILURE);
     }

     outputFile = fopen(argv[2], "w");
     if( outputFile==NULL ){
        printf("Unable to write to file %s\n", argv[2]);
        exit(EXIT_FAILURE);
     }
     //
     // int line = 0;
     while(fgets(string, 256, inputFile) != NULL){
          // line++;
          char instruction = string[0];
          // char *num = &string[2];
          // printf("%d\n", atoi(num));
          // printf("%d\n", atoi(&string[2]));
          if(instruction == 'e'){
               enQueue(&myQueue, atoi(&string[2]));
               fprintf(outputFile, "enqueued %d", atoi(&string[2]));
          }
          else if(instruction == 'd'){
               if(myQueue.size==0){
                    fprintf(outputFile, "empty\n");
                    continue;
               }
               fprintf(outputFile, "%d", deQueue(&myQueue));
          }
          else if(instruction == 'p'){
               printQueue(&myQueue, outputFile);
          }
          fprintf(outputFile,"\n");
     }

     queueFree(&myQueue);
     fclose(inputFile);
     fclose(outputFile);
     // free(string);

     return EXIT_SUCCESS;
}
