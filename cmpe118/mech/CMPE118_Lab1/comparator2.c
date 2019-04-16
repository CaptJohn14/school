#define Vcc 3.3
#define Vr  0



int main(){
     float Va1;
     float Va2;
     float R1 = 100 * 1000;
     float R2 = 100 * 1000;
     float R3 = 1000  * 1000;
     float R4 = 1000;
     float Vchange;
     float n;

     Va1 =(R2*Vcc)/(R2 + ((R1*R3)/(R1+R3)));
     // Va1 =1/((((R1*R3)/(R1+R3))/Vcc*R2)+(1/Vcc));
     n = R1/R3;
     Va2 = Va1/(n+1);
     printf("Va1: %.2f\n", Va1);
     printf("Va2: %.2f\n", Va2);
     printf("R1: %.3fk\n", R1/1000);
     printf("R2: %.3fk\n", R2/1000);
     printf("R3: %.3fk\n", R3/1000);
     printf("R4: %.3fk\n", R4/1000);
     // printf("hello\n");
}
