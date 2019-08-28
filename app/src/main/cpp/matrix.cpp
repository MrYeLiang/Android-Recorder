//
// Created by 叶亮 on 2019/7/17.
//


#include "matrix.h"

void matrixSetIdentityM(float *m)
{
    memset((void*)m, 0, 16*sizeof(float));
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

