#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

char string[256];
char* mystring;
int numA, numD, numP, numW;

void extract_chars(char* s, char* a, char* d, char* p, char* w);

int main(int argc, char* argv[]){
     FILE* inputFile;
     FILE* outputFile;

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
     int line= 0;
     // strcat(alpha, "h E lo 124 ^()*4");
     while(fgets(string, 256, inputFile) != NULL){
          char* alpha = calloc(16, sizeof(int));
          char* digit = calloc(16, sizeof(int));
          char* punct = calloc(16, sizeof(int));
          char* white = calloc(16, sizeof(int));
          line = line + 1;
          fprintf(outputFile, "line %d contains:\n", line);
          extract_chars(string, alpha, digit, punct, white);
          if(numA>1){
               fprintf(outputFile, "%d alphabetic characters: %s\n", numA ,alpha);
          }
          else{
               fprintf(outputFile, "%d alphabetic character: %s\n", numA ,alpha);
          }
          if(numD>1){
               fprintf(outputFile, "%d numeric characters: %s\n", numD ,digit);
          }
          else{
               fprintf(outputFile, "%d numeric character: %s\n", numD ,digit);
          }
          if(numP>1){
               fprintf(outputFile, "%d punctuation characters: %s\n", numP ,punct);
          }
          else{
               fprintf(outputFile, "%d punctuation character: %s\n", numP ,punct);
          }
          if(numW>1){
               fprintf(outputFile, "%d whitespace characters: %s\n", numW ,white);
          }
          else{
               fprintf(outputFile, "%d whitespace character: %s\n", numW ,white);
          }
          free(alpha);
          free(digit);
          free(punct);
          free(white);
     }

     return EXIT_SUCCESS;

}
void extract_chars(char* s, char* a, char* d, char* p, char* w){
     numA = 0;
     numD = 0;
     numP = 0;
     numW = 0;
     // printf("%s",s);
     for(int i=0 ; i<strlen(s) ; i++){
          if(isspace(s[i])){
               *(w+numW) = s[i];
               numW++;
          }
          else if(isalpha(s[i])){
               *(a+numA) = s[i];
               numA++;
          }
          else if(isdigit(s[i])){
               *(d+numD) = s[i];
               numD++;
          }
          else if(ispunct(s[i])){
               *(p+numP) = s[i];
               numP++;
          }
     }
}
