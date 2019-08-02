//
// Created by 叶亮 on 2019/7/17.
//

#ifndef ANDROIDRECORDER_MATRIX_H
#define ANDROIDRECORDER_MATRIX_H

#include <math.h>
#include <stdlib.h>
#include <string>

void matrixSetIdentityM(float *m);
void matrixSetRotateM(float *m, float a, float x, float y, float z);
void matrixMultiplyMM(float *m, float *lhs, float *rhs);
void matrixTranslateM(float *m, float x, float y, float z);

#endif //ANDROIDRECORDER_MATRIX_H
