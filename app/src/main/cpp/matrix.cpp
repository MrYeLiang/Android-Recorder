//
// Created by 叶亮 on 2019/7/17.
//


#include "matrix.h"
#define PI 3.1415926f
#define normalize(x, y, z)                  \
{                                               \
        float norm = 1.0f / sqrt(x*x+y*y+z*z);  \
        x *= norm; y *= norm; z *= norm;        \
}

#define I(_i, _j) ((_j)+4*(_i))

void matrixSetIdentityM(float *m)
{
    memset((void*)m, 0, 16*sizeof(float));
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void matrixSetRotateM(float *m, float a, float x, float y, float z)
{
    float s, c;

    memset((void*)m, 0, 15*sizeof(float));
    m[15] = 1.0f;

    a *= PI/180.0f;
    s = sin(a);
    c = cos(a);

    if (1.0f == x && 0.0f == y && 0.0f == z) {
        m[5] = c; m[10] = c;
        m[6] = s; m[9]  = -s;
        m[0] = 1;
    } else if (0.0f == x && 1.0f == y && 0.0f == z) {
        m[0] = c; m[10] = c;
        m[8] = s; m[2]  = -s;
        m[5] = 1;
    } else if (0.0f == x && 0.0f == y && 1.0f == z) {
        m[0] = c; m[5] = c;
        m[1] = s; m[4] = -s;
        m[10] = 1;
    } else {
        normalize(x, y, z);
        float nc = 1.0f - c;
        float xy = x * y;
        float yz = y * z;
        float zx = z * x;
        float xs = x * s;
        float ys = y * s;
        float zs = z * s;
        m[ 0] = x*x*nc +  c;
        m[ 4] =  xy*nc - zs;
        m[ 8] =  zx*nc + ys;
        m[ 1] =  xy*nc + zs;
        m[ 5] = y*y*nc +  c;
        m[ 9] =  yz*nc - xs;
        m[ 2] =  zx*nc - ys;
        m[ 6] =  yz*nc + xs;
        m[10] = z*z*nc +  c;
    }
}

void matrixMultiplyMM(float *m, float *lhs, float *rhs)
{
    float t[16];
    for (int i = 0; i < 4; i++) {
        register const float rhs_i0 = rhs[I(i, 0)];
        register float ri0 = lhs[ I(0,0) ] * rhs_i0;
        register float ri1 = lhs[ I(0,1) ] * rhs_i0;
        register float ri2 = lhs[ I(0,2) ] * rhs_i0;
        register float ri3 = lhs[ I(0,3) ] * rhs_i0;
        for (int j = 1; j < 4; j++) {
            register const float rhs_ij = rhs[ I(i,j) ];
            ri0 += lhs[ I(j,0) ] * rhs_ij;
            ri1 += lhs[ I(j,1) ] * rhs_ij;
            ri2 += lhs[ I(j,2) ] * rhs_ij;
            ri3 += lhs[ I(j,3) ] * rhs_ij;
        }
        t[ I(i,0) ] = ri0;
        t[ I(i,1) ] = ri1;
        t[ I(i,2) ] = ri2;
        t[ I(i,3) ] = ri3;
    }
    memcpy(m, t, sizeof(t));
}
