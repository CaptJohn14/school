// #define Va1 2.9       //up
// #define Va2 2.0       //low

#define Vcc 3.3
#define Vr  0




int main(){
     float Va1 = 1.95;
     float Va2 = 1.35;
     float R3 = 470 * 1000;
     float R4 = 3900;


     float Vchange = Va1 - Va2;
     float n = Vchange / Va2;
     float R1 = n * R3;
     float R2 = ((R1*R3)/(R1+R3))/((Vcc/Va1)-1);
     printf("Va1: %.2f\n", Va1);
     printf("Va2: %.2f\n", Va2);
     printf("R1: %.3fk\n", R1/1000);
     printf("R2: %.3fk\n", R2/1000);
     printf("R3: %.3fk\n", R3/1000);
     printf("R4: %.3fk\n", R4/1000);

}
