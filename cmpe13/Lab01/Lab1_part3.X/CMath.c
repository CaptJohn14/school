
#include "CMath.h"

float abs(float a); // abs = absolute value

float abs(float a) {
    if (a < 0) {
        a = -a;
    }
    return a;
}

float enorm(float px, float py, float qx, float qy) {
    float g, e, r, s, t;
    float dx = abs(qx - px);
    float dy = abs(qy - py);
    if (dx > dy) {
        g = dx;
        e = dy;
    } else {
        g = dy;
        e = dx;
    }
    float count;
    for (count = 0; count < 2; count++) {
        t = e / g;
        r = t*t;
        s = r / (4.0 + r);
        g = g + 2 * s * g;
        e = e * s;
    }
    return g;
}

float arctangent2(float x, float y) {
    float theta, r;
    float absy = abs(y); //absy = absolute value of y. |y|
    if (x > 0) {
        r = (x - absy) / (x + absy);
        theta = 0.1963 * r * r * r - 0.9817 * r + (3.14 / 4);
    } else {
        r = (x + absy) / (x - absy);
        theta = 0.1963 * r * r * r - 0.9817 * r + (3 / 4)*3.14;
    }
    if (absy < 0) {
        theta = -1 * theta;
    }
    return theta;
}

