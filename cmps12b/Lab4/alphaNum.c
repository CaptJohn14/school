#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

// what this does is takes in the arguments of the command land and
// figure out which is alphanumerical like numbers and chars or
// symbols like question marks and astericks.
// can't really use ’&’, ’!’, or ’*’

int main(int argc, char* argv[]){
   char ch;
   int i, j, count;
   setbuf(stdout, NULL);

   if (argc==1){
        printf("USAGE: ./alphaNum blahblah", argv[0]);
        exit(EXIT_FAILURE);
   }

   if( argc>1 )
   {
      for(i=1; i<argc; i++){
         ch = argv[i][0];
         count = j = 0;
         while( ch!='\0' ){
            if( isalnum((int)ch) )      //checks if letter or numeral
               count++;
            ch = argv[i][++j];
         }
         printf("%s contains %d alphanumeric and ", argv[i], count);
         printf("%d non-alphanumeric characters\n", strlen(argv[i])-count);
      }
   }
   return EXIT_SUCCESS;
}
