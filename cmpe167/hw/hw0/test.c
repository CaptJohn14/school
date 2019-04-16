#include <stdio.h>
#define bool int
#define TRUE 1
#define FALSE 0

void num4();
void num5();
void num6();

int main(){
    num6();
}

//
// void num4(){
//     for(int i = 0; i<10; i++){
//         printf("%d\n", 1<<i);
//     }
// }
//
// #define BIT3HI 0x04 // 0000 0100
// void num5(){
//     char bitBang = 0b0;
//     char tester = bitBang & BIT3HI; // 0 = not set.
//     bitBang |= BIT3HI;              // sets
//     bitBang &= BIT3HI;              // clears
//     bitBang ^= BIT3HI;              // toggle
// }

void num6(){
    for (int i=1; i<=1000; i++){
        //if value is divisible by 4 and 9, then output is DEADBEEF
        if(i%4==0 && i%9==0){
            printf("DEADBEEF\n");
        }
        //if value is divisble by only 4, then output is DEAD
        else if(i%4==0){
            printf("DEAD\n");
        }
        //if value is divisible by only 9, then output is BEEF
        else if(i%9==0){
            printf("BEEF\n");
        }
        //if value is not divisible by 4 or 9, then output is the value
        else{
            printf("%d\n", i);
        }
    }
}
