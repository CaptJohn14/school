/*==============================================================================
 * Name: Johnson Le
 * Date 3/9/2019
 * Desc: Main for lab 4.
 ==============================================================================
 *  * Libraries
 =============================================================================*/
#include "BOARD.h"
#include <stdio.h>
#include "Oled.h"
#include <math.h>
#include "matrix.h"
#include "MPU9250.h"

/*==============================================================================
 * Defines/Globals
 =============================================================================*/
#define MAIN
#define Part4
//#define test
#define DegToRad(angleInDegrees) (double)((angleInDegrees) * M_PI / 180.0)
#define RadToDeg(angleInRadians) (double)((angleInRadians) * 180.0 / M_PI)

char string[128];

typedef struct Angles {
    double roll, pitch, yaw;
} Angles;

//double m[3][3] = {
//    {1, -2, 1},
//    {2, 1, -3},
//    {-1, 3, 1}
//};

/*==============================================================================
 * PrintOled(char*s): Prints given string to led
 =============================================================================*/
void PrintOled(char* s) {
    sprintf(string, s);
    OledClear(0);
    OledDrawString(string);
    OledUpdate();
}

/*==============================================================================
 * Print 3x3 to Oled
 =============================================================================*/
void OledMat(double m[3][3]) {
    sprintf(string, "%.2f | %.2f | %.2f\n%.2f | %.2f | %.2f\n%.2f | %.2f | %.2f\n",
            m[0][0], m[0][1], m[0][2],
            m[1][0], m[1][1], m[1][2],
            m[2][0], m[2][1], m[2][2]);
    PrintOled(string);
}

/*==============================================================================
 * Print 3x3 to Serial
 =============================================================================*/
void PrintMat(double m[3][3]) {
    printf("=======================================================\r\n");
    printf("|| %.9f | %.9f | %.9f ||\r\n|| %.9f | %.9f | %.9f ||\r\n|| %.9f | %.9f | %.9f ||\r\n",
            m[0][0], m[0][1], m[0][2],
            m[1][0], m[1][1], m[1][2],
            m[2][0], m[2][1], m[2][2]);
    printf("========================================================\r\n");
}

/*==============================================================================
 * Print 3x1 to Serial
 =============================================================================*/
void PrintMat3by1(double m[3]) {
    printf("========================================================\r\n");
    printf("|| %.9f | %.9f | %.9f ||\r\n", m[0], m[1], m[2]);
    printf("=======================================================\r\n");
}

/*==============================================================================
 * wait(unsigned ms): just a waiting function. waits for ms time
 =============================================================================*/
void wait(unsigned ms) {
    unsigned int t_start = 0;
    t_start = TIMERS_GetMilliSeconds();

    while (TIMERS_GetMilliSeconds() - t_start < ms);
}

/*==============================================================================
 * Returns Angles in Degrees
 =============================================================================*/
Angles Get_Attitude(double m[3][3]) {
    Angles a;
    //    if (m[0][2] >= 0.9999) {
    //        a.pitch = -1 * asin(0.9999);
    //    } else {
    //        a.pitch = (-1 * asin(m[0][2]));
    //    }
    //    if (m[1][2] >= 0.9999) {
    //        a.roll = asin(0.9999) / (cos(a.pitch));
    //    } else {
    //        a.roll = asin(m[1][2] / cos(a.pitch));
    //    }
    //    if (m[0][1] >= 0.9999) {
    //        a.yaw = asin(0.9999) / (cos(a.pitch));
    //    } else {
    //        a.yaw = asin(m[0][1] / cos(a.pitch));
    //    }


    //    a.pitch = (-1 * asin(m[0][2]));
    //    a.roll = asin(m[1][2] / cos(a.pitch));
    //    a.yaw = asin(m[0][1] / cos(a.pitch));

    a.pitch = -1 * asin(m[0][2]);
    a.roll = atan(m[1][2] / m[2][2]);
    a.yaw = atan(m[0][1] / m[0][0]);

    a.pitch = RadToDeg(a.pitch);
    a.roll = RadToDeg(a.roll);
    a.yaw = RadToDeg(a.yaw);
    //    sprintf(string, "%.6lf\n%.6lf\n%.6lf", a.pitch, a.roll, a.yaw);
    //    PrintOled(string);
    return a;
}

/*==============================================================================
 * returns magnitude of 3x1 
 =============================================================================*/
double Magn(double w[3]) {
    return sqrt(fabs(w[0] * w[0]) + fabs(w[1] * w[1]) + fabs(w[2] * w[2]));
}

double sinc(double x) {
    if (fabs(x) < 0.0001) {
        return 1.0;
    } else {
        return (sin(x) / x);
    }
}

/*==============================================================================
 * Returns RxCros 3x3
 =============================================================================*/
void rcross(double r[3], double result[3][3]) {
    result[0][0] = 0;
    result[0][1] = -r[2];
    result[0][2] = r[1];

    result[1][0] = r[2];
    result[1][1] = 0;
    result[1][2] = -r[0];

    result[2][0] = -r[1];
    result[2][1] = r[0];
    result[2][2] = 0;
}

/*==============================================================================
 * Rexp
 =============================================================================*/
void Rexp(double w[3], double result[3][3]) {
    double uh[3][3];
    MakeIdentity(uh);
    double norm = Magn(w);
    double rx[3][3];
    double rx2[3][3];
    rcross(w, rx);
    double s = sinc(norm / 2);
    double c = cos(norm / 2);

    MatrixMultiply(rx, rx, rx2);
    MatrixScalarMultiply((s * c), rx, rx);
    MatrixScalarMultiply((s * s / 2), rx2, rx2);
    MatrixAdd(uh, rx, uh);
    MatrixAdd(uh, rx2, result);
}

/*==============================================================================
 * IntegrateOpenLoop
 =============================================================================*/
void IntegrateOpenLoop(double Rminus[3][3], double gyros[3], double deltaT, double result[3][3]) {
    double temp1[3][3] = {0};
    double temp2[3][3] = {0};

    if (0) {
        double uh[3] = {gyros[0] * deltaT, gyros[1] * deltaT, gyros[2] * deltaT};
        Rexp(uh, temp1);
        MatrixMultiply(Rminus, temp1, result);
    } else {
        rcross(gyros, temp1);
        MatrixScalarMultiply(deltaT, temp1, temp2);
        MatrixMultiply(temp2, Rminus, temp1);
        MatrixAdd(temp1, Rminus, result);
    }
}
/*==============================================================================
 * IntegrateClosedLoop
 =============================================================================*/
#define Kp_a 0.2
#define Ki_a Kp_a/10
#define Kp_m 0.2 //0.3
#define Ki_m Kp_m/10

void IntegrateClosedLoop(double Rminus[3][3], double Bminus[3], double gyros[3],
        double mags[3], double accels[3], double magInertial[3],
        double accelInertial[3], double deltaT,
        double Rplus[3][3], double Bplus[3]) {

    double a_read[3], m_read[3], a_iref[3], m_iref[3], gyros_biased[3];
    int i = 0;
    for (i = 0; i < 3; i++) {
        a_read[i] = accels[i] / Magn(accels);
        m_read[i] = mags[i] / Magn(mags);
        a_iref[i] = accelInertial[i] / Magn(accelInertial);
        m_iref[i] = magInertial[i] / Magn(magInertial);

        gyros_biased[i] = gyros[i] - Bminus[i];
    }

    double a_cross[3][3], m_cross[3][3], Rminus_Transpose[3][3];
    rcross(a_read, a_cross);
    rcross(m_read, m_cross);
    MatrixTranspose(Rminus, Rminus_Transpose);

    double a_wmeas[3], m_wmeas[3], temp1[3], temp2[3];
    MatrixMult3to1(Rminus_Transpose, a_iref, temp1);
    MatrixMult3to1(Rminus_Transpose, m_iref, temp2);

    MatrixMult3to1(a_cross, temp1, a_wmeas);
    MatrixMult3to1(m_cross, temp2, m_wmeas);

    double gyros_feedbacked[3], bdot[3];
    for (i = 0; i < 3; i++) {
        gyros_feedbacked[i] = gyros_biased[i] + (Kp_a * a_wmeas[i]) + (Kp_m * m_wmeas[i]);
        // bdot[i] = -(Ki_a*a_wmeas[i]) - Ki_m*m_wmeas[i];
        // Bplus[i] = bdot[i]+Bminus[i];
        Bplus[i] = Bminus[i] - (Ki_a * a_wmeas[i]) - (Ki_m * m_wmeas[i]);
    }

    double temp[3][3];
    Rexp(gyros_feedbacked, temp);
    MatrixMultiply(Rminus, temp, Rplus);
}

/*==============================================================================
 * Get DCM from Trial
 =============================================================================*/
void DCMfromTrial(double mags[3], double accels[3], double magInertial[3], double accelInertial[3], double result[3][3]) {
    double a_read[3], m_read[3], a_iref[3], m_iref[3];
    double m_rcross[3][3], m_temp[3], m[3], M_rcross[3][3], M_temp[3], M[3];
    if (mags[0] == accels[0] && mags[1] == accels[1] && mags[2] == accels[2]) {
        return;
    }
    int i = 0;
    for (i = 0; i < 3; i++) {
        a_read[i] = accels[i] / Magn(accels);
        m_read[i] = mags[i] / Magn(mags);
        a_iref[i] = accelInertial[i] / Magn(accelInertial);
        m_iref[i] = magInertial[i] / Magn(magInertial);
    }

    rcross(m_read, m_rcross);
    MatrixMult3to1(m_rcross, a_read, m_temp);

    rcross(m_iref, M_rcross);
    MatrixMult3to1(M_rcross, a_iref, M_temp);

    for (i = 0; i < 3; i++) {
        m[i] = m_temp[i] / Magn(m_temp);
        M[i] = M_temp[i] / Magn(M_temp);
    }

    double A1[3][3], A1_rcross[3][3], A1_temp[3];
    rcross(m_iref, A1_rcross);
    MatrixMult3to1(A1_rcross, M, A1_temp);

    double A2[3][3], A2_rcross[3][3], A2_temp[3];
    rcross(m_read, A2_rcross);
    MatrixMult3to1(A2_rcross, m, A2_temp);

    for (i = 0; i < 3; i++) {
        A1[i][0] = m_iref[i];
        A1[i][1] = M[i];
        A1[i][2] = A1_temp[i];

        A2[i][0] = m_read[i];
        A2[i][1] = m[i];
        A2[i][2] = A2_temp[i];
    }

    double A[3][3], A2_transpose[3][3];
    MatrixTranspose(A2, A2_transpose);

    MatrixMultiply(A1, A2_transpose, A);
    MatrixTranspose(A, result);
}

void GrabXSeconds(unsigned int sec) {
    int i = 0;
    unsigned int t_start;
    unsigned int p = 100; // f = 1/pE-3
    unsigned int timeout = sec;

    PrintOled("Wait 2 seconds");
    wait(2000);

    t_start = TIMERS_GetMilliSeconds();
    for (i = 0; i < (timeout)*(1000 / p); i++) {
        sprintf(string, "Grabbing data %u(s)\ntimer: %d(m) %d(s)", timeout, (i / 60) * p / 1000, (i * p / 1000) % 60);
        PrintOled(string);
        printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
                MPU9250_ReadAccelX(), MPU9250_ReadAccelY(), MPU9250_ReadAccelZ(),
                MPU9250_ReadMagX(), MPU9250_ReadMagY(), MPU9250_ReadMagZ(),
                MPU9250_ReadGyroX(), MPU9250_ReadGyroY(), MPU9250_ReadGyroZ());
        wait(p);
    }
}


/*==============================================================================
 * Main
 =============================================================================*/
#ifdef MAIN

int main() {
    /*=================
     * Initialization
    =================*/
    BOARD_Init();
    OledInit();
    TIMERS_Init();

    printf("\r\nWelcome to Johnson's Lab4\r\n");
    if (!MPU9250_Init()) {
        printf("Initialization failed.\r\n");
        while (1);
    }

    /*=================
     * Part 4.
    =================*/
#ifdef Part4
    double R[3][3] = {0};
    double R2[3][3] = {0};

    double gyroInput[3] = {DegToRad(0), DegToRad(0), DegToRad(0)};

    int i = 0;
    unsigned int t_start;
    unsigned int p = 50; // f = 1/pE-3
    unsigned int timeout = 60;
    PrintOled("Wait 1 seconds");
    wait(1000);
    PrintOled("Getting Data.");
    t_start = TIMERS_GetMilliSeconds();

    MakeIdentity(R);
    for (i = 0; i < (timeout)*(1000 / p); i++) {
        gyroInput[0] = DegToRad(((double) (MPU9250_ReadGyroX()) + 479.74) / 131);
        gyroInput[1] = DegToRad(((double) (MPU9250_ReadGyroY()) - 255.16) / 131);
        gyroInput[2] = DegToRad(((double) (MPU9250_ReadGyroZ()) - 32) / 131);

        //        printf("%d | %d | %d\r\n", MPU9250_ReadGyroX(), MPU9250_ReadGyroY(), MPU9250_ReadGyroZ());
        //        printf("%.5f | %.5f | %.5f\r\n", gyroInput[0], gyroInput[1], gyroInput[2]);

        MatrixCopy(R, R2);
        IntegrateOpenLoop(R2, gyroInput, (double) ((double) p / 1000), R);

        Angles a;
        a = Get_Attitude(R);

        PrintMat(R);
        printf("%.9lf | %.9lf | %.9lf\r\n", a.roll, a.pitch, a.yaw);
        sprintf(string, "%us: %d(m) %d(s)\n%.6lf\n%.6lf\n%.6lf", timeout, (i / 60) * p / 1000, (i * p / 1000) % 60, a.roll, a.pitch, a.yaw);
        PrintOled(string);

        wait(p);
    }
#endif

    /*=================
     * Part 5.
    =================*/
#ifdef Part5
    //inits
    double gyroInput[3] = {0, 0, 0}; //const get
    double magRead[3] = {1, 0, 0}; //nothing
    double accRead[3] = {0, 0, -1}; //const get 

    double accInertial[3] = {0, 0, -1}; //
    double magInertial[3] = {1, 0, 0}; //

    double R[3][3], R_temp[3][3], biasEstimate_temp[3];
    double biasEstimate[3] = {0};
    Angles a;

    MakeIdentity(R);

    //wait 1 sec. set parameters for how long to run for
    int i = 0;
    unsigned int t_start;
    unsigned int p = 50; // f = 1/pE-3
    unsigned int timeout = 9999;
    PrintOled("Part 5: Wait 1 seconds");
    wait(1000);

    t_start = TIMERS_GetMilliSeconds();
    for (i = 0; i < (timeout)*(1000 / p); i++) {

        gyroInput[0] = DegToRad(((double) (MPU9250_ReadGyroX()) + 533.0408163) / 131);
        gyroInput[1] = DegToRad(((double) (MPU9250_ReadGyroY()) - 247.3469388) / 131);
        gyroInput[2] = DegToRad(((double) (MPU9250_ReadGyroZ()) - 84.89795918) / 131);

        accRead[0] = MPU9250_ReadAccelX();
        accRead[1] = MPU9250_ReadAccelY();
        accRead[2] = MPU9250_ReadAccelZ();

        MatrixCopy(R, R_temp);
        MatrixCopy3by1(biasEstimate, biasEstimate_temp);

        IntegrateClosedLoop(R_temp, biasEstimate_temp, gyroInput, magRead, accRead, magInertial, accInertial, (double) ((double) p / 1000), R, biasEstimate);

        a = Get_Attitude(R);

        PrintMat(R);
        PrintMat3by1(biasEstimate);
        printf("%.9lf | %.9lf | %.9lf\r\n", a.roll, a.pitch, a.yaw);

        sprintf(string, "%us: %d(m) %d(s)\n%.6lf\n%.6lf\n%.6lf", timeout, (i / 60) * p / 1000, (i * p / 1000) % 60, a.roll, (a.pitch - 1)*2, a.yaw);
        PrintOled(string);
        wait(p);
    }
#endif

    /*=================
     * Part 7
    =================*/
#ifdef Part7
    double Rmis[3][3] = {
        {0.186478450929338, -0.422420223573230, 0.887010113840500},
        {0.737726526434303, 0.656462543926206, 0.157532538280546},
        {-0.648833845860535, 0.624994466486125, 0.434046722516772}
    };
    double Atilda[3][3] = {
        {5.43227570280761e-05, 7.48583475796387e-08, 5.59962327843036e-08},
        {7.94426651940132e-08, 5.33340552131848e-05, -7.86394920968802e-07},
        {-4.70395353027608e-07, -7.37512580498740e-07, 5.73735756099586e-05}
    };
    double Btilda[3] = {-0.00379353168349567, 0.00161232204773737, 0.00738156283272772};

    double RmisTranspose[3][3];
    MatrixTranspose(Rmis, RmisTranspose);

    double gyroInput[3] = {0, 0, 0}; //const get
    double magRead[3] = {1, 0, 0}; //nothing
    double magTemp[3] = {0};
    double accRead[3] = {0, 0, -1}; //const get 
    double accTemp1[3] = {0};
    double accTemp2[3] = {0};

    double accInertial[3] = {0, 0, -1}; //
    double magInertial[3] = {1, 0, 0}; //

    double R[3][3], R_temp[3][3], biasEstimate_temp[3];
    double biasEstimate[3] = {0};
    Angles a;

    MakeIdentity(R);

    double LR_R[3][3], LR_R2[3][3];
    Angles LR_a;
    MakeIdentity(LR_R);

    //wait 1 sec. set parameters for how long to run for
    int i = 0;
    unsigned int t_start;
    unsigned int p = 50; // f = 1/pE-3
    unsigned int timeout = 9999;
    PrintOled("Part 7: Wait 1 seconds");
    wait(1000);

    t_start = TIMERS_GetMilliSeconds();
    for (i = 0; i < (timeout)*(1000 / p); i++) {
        gyroInput[0] = DegToRad(((double) (MPU9250_ReadGyroX()) + 533.0408163) / 131);
        gyroInput[1] = DegToRad(((double) (MPU9250_ReadGyroY()) - 247.3469388) / 131);
        gyroInput[2] = DegToRad(((double) (MPU9250_ReadGyroZ()) - 84.89795918) / 131);

        accRead[0] = (MPU9250_ReadAccelX()*1.0021) - 0.0275;
        accRead[1] = (MPU9250_ReadAccelY()*0.999) - 0.0061;
        accRead[2] = (MPU9250_ReadAccelZ()*0.99) - 0.0211;

        //        accTemp1[0] = (MPU9250_ReadAccelX());//*1.0021) - 0.0275;
        //        accTemp1[1] = (MPU9250_ReadAccelY());//*0.999) - 0.0061;
        //        accTemp1[2] = (MPU9250_ReadAccelZ());//*0.99) - 0.0211;
        //
        //        MatrixMult3to1(Atilda, accTemp1, accTemp2);
        //
        //        int j = 0;
        //        for (j = 0; j < 3; j++) {
        //            accRead[j] = accTemp2[j] + Btilda[j];
        //        }

        magTemp[0] = (MPU9250_ReadMagX() * 1.1517) - 13.5;
        magTemp[1] = (MPU9250_ReadMagY() * 1.0833) - 42.5;
        magTemp[2] = (MPU9250_ReadMagZ() * 1.1253) + 214.5;

        MatrixMult3to1(Rmis, magTemp, magRead);

        MatrixCopy(R, R_temp);
        MatrixCopy3by1(biasEstimate, biasEstimate_temp);

        IntegrateClosedLoop(R_temp, biasEstimate_temp, gyroInput, magInput, accRead, magInertial, accInertial, (double) ((double) p / 1000), R, biasEstimate);
        a = Get_Attitude(R);

        PrintMat(R);
        printf("%.9lf | %.9lf | %.9lf\r\n", a.roll, a.pitch, a.yaw);
        sprintf(string, "%us: %d(m) %d(s)\n%.6lf\n%.6lf\n%.6lf", timeout, (i / 60) * p / 1000, (i * p / 1000) % 60, a.roll, (a.pitch - 1)*2, a.yaw);
        PrintOled(string);
        wait(p);
    }

#endif

#ifdef Part8
    double Rmis[3][3] = {
        {0.186478450929338, -0.422420223573230, 0.887010113840500},
        {0.737726526434303, 0.656462543926206, 0.157532538280546},
        {-0.648833845860535, 0.624994466486125, 0.434046722516772}
    };
    double RmisTranspose[3][3];
    MatrixTranspose(Rmis, RmisTranspose);

    double aR[3], mR[3], aI[3], mI[3];

    double accRead[3] = {0};
    double magRead[3] = {0};
    double magTemp[3];

    double aInertial[3] = {0, 0, -1};
    double mInertial[3] = {1, 0, 0};

    double R[3][3], Ro[3][3], R_temp[3][3];

    MakeIdentity(Ro);
    Angles a;


    //    DCMfromTrial(magInput, accInput, magInertial, accInertial, R);
    //    PrintMat(R);

    //wait 1 sec. set parameters for how long to run for
    int i = 0;
    unsigned int t_start;
    unsigned int p = 50; // f = 1/pE-3
    unsigned int timeout = 9999;
    PrintOled("Part 8: Wait 1 seconds");
    wait(1000);

    t_start = TIMERS_GetMilliSeconds();
    for (i = 0; i < (timeout)*(1000 / p); i++) {

        accRead[0] = (MPU9250_ReadAccelX()*1.0021) - 0.0275;
        accRead[1] = (MPU9250_ReadAccelY()*0.999) - 0.0061;
        accRead[2] = (MPU9250_ReadAccelZ()*0.99) - 0.0211;
        //
        magTemp[0] = (MPU9250_ReadMagX() * 1.1517) - 13.5;
        magTemp[1] = (MPU9250_ReadMagY() * 1.0833) - 42.5;
        magTemp[2] = (MPU9250_ReadMagZ() * 1.1253) + 214.5;

        MatrixMult3to1(Rmis, magTemp, magRead);

        DCMfromTrial(magRead, accRead, mInertial, aInertial, R_temp);
        //        MatrixAdd(R, Ro, R_temp);
        MatrixCopy(R_temp, R);

        a = Get_Attitude(R);

        PrintMat(R);
        printf("%.9lf | %.9lf | %.9lf\r\n", a.roll, a.pitch, a.yaw);

        sprintf(string, "%us: %d(m) %d(s)\n%.6lf\n%.6lf\n%.6lf", timeout, (i / 60) * p / 1000, (i * p / 1000) % 60, a.roll, a.pitch, a.yaw);
        PrintOled(string);

        wait(p);
    }

#endif
    while (1);
}
#endif
